//===- ScriptParser.cpp ---------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "LinkerScriptLexer.h"
#include "lld/Common/ErrorHandler.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;
using namespace lld;
using namespace lld::elf;

LinkerScriptLexer::LinkerScriptLexer(MemoryBufferRef MB, llvm::SourceMgr &SM,
                                     llvm::SMDiagnostic &Err)
    : MB(MB), ErrorInfo(Err), SM(SM) {
  curStringRef = MB.getBuffer();
}

/*
bool LinkerScriptLexer::Error(SMLoc ErrorLoc, const Twine &Msg) const {
  ErrorInfo = SM.GetMessage(ErrorLoc, llvm::SourceMgr::DK_Error, Msg);
  return true;
}

void LinkerScriptLexer::Warning(SMLoc WarningLoc, const Twine &Msg) const {
  SM.PrintMessage(WarningLoc, llvm::SourceMgr::DK_Warning, Msg);
}
*/

// bool LinkerScriptLexer::expect(ScriptToken token) { return token == tok1; }

inline LinkerScriptLexer::TokenInfo
LinkerScriptLexer::advanceTokenInfo(ScriptToken kind, size_t pos = 1) {
  // TODO: special case for kind == ScriptToken::Error
  llvm::StringRef valRef = curStringRef.substr(0, pos);
  curStringRef = curStringRef.substr(pos);
  return {kind, valRef};
}

LinkerScriptLexer::TokenInfo LinkerScriptLexer::getTokenInfo() {
  curStringRef = skipComments();

  // TODO: make sure the empty situation is not an error
  if (curStringRef.empty())
    return advanceTokenInfo(ScriptToken::Eof);
  const char c = curStringRef.front();
  if (std::isdigit(c))
    return getDigits();
  if (std::isalpha(c))
    return getCommandOrIdentify();
  return getSymbolToken();
}

llvm::StringRef LinkerScriptLexer::skipComments() {
  // this code now is copied from ScriptLexer.cpp
  // and modified so it can use SourceMgr
  while (true) {
    if (curStringRef.starts_with("/*")) {
      size_t e = curStringRef.find("*/", 2);
      if (e == llvm::StringRef::npos) {
        // TODO: Error("Unclosed comment in a linker script");
        return "";
      }
      curStringRef = curStringRef.substr(e + 2);
      continue;
    }
    if (curStringRef.starts_with("#")) {
      size_t e = curStringRef.find("\n", 1);
      if (e == StringRef::npos)
        e = curStringRef.size() - 1;
      curStringRef = curStringRef.substr(e + 1);
      continue;
    }

    size_t size = curStringRef.size();
    curStringRef = curStringRef.ltrim();
    if (curStringRef.size() == size)
      return curStringRef;
  }
}

LinkerScriptLexer::TokenInfo LinkerScriptLexer::getSymbolToken() {
  const char c = curStringRef.front();
  // TODO: single char token needs to substr(1)
  switch (c) {
  case EOF:
    return advanceTokenInfo(ScriptToken::Eof);
  case '"':
    return getQuotedToken();
  case '(':
    return advanceTokenInfo(ScriptToken::BracektBegin);
  case ')':
    return advanceTokenInfo(ScriptToken::BracektEnd);
  case '{':
    return advanceTokenInfo(ScriptToken::CurlyBegin);
  case '}':
    return advanceTokenInfo(ScriptToken::CurlyEnd);
  case ';':
    return advanceTokenInfo(ScriptToken::Semicolon);
  case ',':
    return advanceTokenInfo(ScriptToken::Comma);
  case '<':
    if (curStringRef.size() > 2 && curStringRef[1] == '<' &&
        curStringRef[2] == '=') {
      return advanceTokenInfo(ScriptToken::RightShiftAssign, 3);
    }
    if (curStringRef.size() > 1) {
      if (curStringRef[1] == '=') {
        return advanceTokenInfo(ScriptToken::LessEqual, 2);
      } else if (curStringRef[1] == '<') {
        return advanceTokenInfo(ScriptToken::LeftShift, 2);
      }
    }
    return advanceTokenInfo(ScriptToken::Less);
  case '>':
    if (curStringRef.size() > 2 && curStringRef[1] == '>' &&
        curStringRef[2] == '=') {
      return advanceTokenInfo(ScriptToken::LeftShiftAssign, 3);
    }
    if (curStringRef.size() > 1) {
      if (curStringRef[1] == '=') {
        return advanceTokenInfo(ScriptToken::GreaterEqual, 2);
      } else if (curStringRef[1] == '>') {
        return advanceTokenInfo(ScriptToken::RightShift, 2);
      }
    }
    return advanceTokenInfo(ScriptToken::Greater);
  case '&':
    if (curStringRef.size() > 1) {
      if (curStringRef[1] == '=') {
        return advanceTokenInfo(ScriptToken::AndAssign, 2);
      } else if (curStringRef[1] == '&') {
        return advanceTokenInfo(ScriptToken::AndGate, 2);
      }
    }
    return advanceTokenInfo(ScriptToken::Bitwise);
  case '^':
    if (curStringRef.size() > 1) {
      if (curStringRef[1] == '=') {
        return advanceTokenInfo(ScriptToken::AndAssign, 2);
      }
    }
    return advanceTokenInfo(ScriptToken::Xor);
  case '|':
    if (curStringRef.size() > 1) {
      if (curStringRef[1] == '=') {
        return advanceTokenInfo(ScriptToken::OrAssign, 2);
      } else if (curStringRef[1] == '|') {
        return advanceTokenInfo(ScriptToken::OrGate, 2);
      }
    }
    return advanceTokenInfo(ScriptToken::Or);
  default:
    return advanceTokenInfo(ScriptToken::Error);
  }
}

LinkerScriptLexer::TokenInfo LinkerScriptLexer::getQuotedToken() {
  // Quoted token. Note that double-quote characters are parts of a token
  // because, in a glob match context, only unquoted tokens are interpreted as
  // glob patterns. Double-quoted tokens are literal patterns in that context.
  size_t e = curStringRef.find("\"", 1);
  if (e == StringRef::npos) {
    StringRef fileName = MB.getBufferIdentifier();
    // TODO: Error(fileName + ": unclosed quote");
    return advanceTokenInfo(ScriptToken::Error, e);
  }
  return advanceTokenInfo(ScriptToken::Quote, e + 1);
}

LinkerScriptLexer::TokenInfo LinkerScriptLexer::getDigits() {
  size_t pos = curStringRef.find_first_not_of("0123456789XxHhKkMm");
  if (curStringRef.starts_with_insensitive("0x")) {
    return advanceTokenInfo(ScriptToken::Hexdecimal, pos);
  }
  const char c = curStringRef[pos];
  switch (c) {
  case 'H':
  case 'h':
    return advanceTokenInfo(ScriptToken::Hexdecimal_H, pos);
  case 'K':
  case 'k':
    return advanceTokenInfo(ScriptToken::Decimal_K, pos);
  case 'M':
  case 'm':
    return advanceTokenInfo(ScriptToken::Decimal_M, pos);
  default:
    return advanceTokenInfo(ScriptToken::Decimal, pos);
  };
}

LinkerScriptLexer::TokenInfo LinkerScriptLexer::getCommandOrIdentify() {
  // Unquoted token. This is more relaxed than tokens in C-like language,
  // so that you can write "file-name.cpp" as one bare token, for example.
  size_t pos = curStringRef.find_first_not_of(
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
      "0123456789_.$/\\~=+[]*?-!^:");

  // Quoted strings are literal strings, so we don't want to split it.
  if (inExpression && !curStringRef.starts_with("\"")) {
    StringRef ops = "!~*/+-<>?^:="; // List of operators
    size_t e = curStringRef.find_first_of(ops);
    if (e != StringRef::npos && e != 0) {
      curStringRef = curStringRef.substr(e);
      return advanceTokenInfo(ScriptToken::Identify, e);
    }
  }
  return advanceTokenInfo(ScriptToken::Identify, pos);
}
