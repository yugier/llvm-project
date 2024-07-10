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
  curPtr = curStringRef.begin();
}

bool LinkerScriptLexer::Error(SMLoc ErrorLoc, const Twine &Msg) const {
  ErrorInfo = SM.GetMessage(ErrorLoc, llvm::SourceMgr::DK_Error, Msg);
  return true;
}

void LinkerScriptLexer::Warning(SMLoc WarningLoc, const Twine &Msg) const {
  SM.PrintMessage(WarningLoc, llvm::SourceMgr::DK_Warning, Msg);
}

ScriptToken LinkerScriptLexer::getToken() {
  while (true) {
    curStringRef = skipComments();

    // Quoted token. Note that double-quote characters are parts of a token
    // because, in a glob match context, only unquoted tokens are interpreted as
    // glob patterns. Double-quoted tokens are literal patterns in that context.
    if (curStringRef.starts_with("\"")) {
      size_t e = curStringRef.find("\"", 1);
      if (e == StringRef::npos) {
        StringRef fileName = MB.getBufferIdentifier();
        Error(fileName + ": unclosed quote");
        return ScriptToken::Error;
      }
      llvm::StringRef quotedRef = curStringRef.take_front(e + 1);
      // TODO: need a function to tell if quotedRef is a keyword or not
      // and return ScriptToken
      curStringRef = curStringRef.substr(e + 1);
      continue;
    }

    // Unquoted token. This is more relaxed than tokens in C-like language,
    // so that you can write "file-name.cpp" as one bare token, for example.
    size_t pos = curStringRef.find_first_not_of(
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
        "0123456789_.$/\\~=+[]*?-!^:");
    // TODO: a new function for this for keeping the code clean
    if (pos == 0) {
      // single char

      const char *curChar = curStringRef.begin();
      switch (*curChar) {
      case EOF:
        return ScriptToken::Eof;
      case '(':
        return ScriptToken::BracektBegin;
      case ')':
        return ScriptToken::BracektEnd;
      case '{':
        return ScriptToken::CurlyBegin;
      case '}':
        return ScriptToken::CurlyEnd;
      case ';':
        return ScriptToken::Semicolon;
      case ',':
        return ScriptToken::Comma;
      case ':':
        return ScriptToken::Colon;
      case '*':
      case '/':
      case '+':
      case '-':
      case '<':
      case '>':
      case '&':
      case '^':
      case '|':
      default:
        // TODO
        break;
      }
    } else {
      // TODO: COMMAND / user defined symbol/file name match
    }

    return ScriptToken::Error;
  }
}

llvm::StringRef LinkerScriptLexer::skipComments() {
  // this code now is copied from ScriptLexer.cpp
  // and modified so it can use SourceMgr
  while (true) {
    if (curStringRef.starts_with("/*")) {
      size_t e = curStringRef.find("*/", 2);
      if (e == llvm::StringRef::npos) {
        Error("Unclosed comment in a linker script");
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

ScriptToken LinkerScriptLexer::getCommandOrSymbolName() {
  // TODO: use marco like .ll AsmLexer
  return ScriptToken::SymbolName;
}
