//===- ScriptParser.cpp ---------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "LinkerScriptLexer.h"
#include "lld/Common/ErrorHandler.h"
#include "llvm/ADT/Twine.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;
using namespace lld;
using namespace lld::elf;

LinkerScriptLexer::LinkerScriptLexer(MemoryBufferRef MB) {
  curStringRef = MB.getBuffer();
}

ScriptToken LinkerScriptLexer::getToken() {
  while (true) {
    int curChar = getNextChar();

    switch (curChar) {
    case ' ':
    case '\n':
    case '\t':
    case '\r':
      continue; // ignore whitespace
      // TODO
      break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      // TODO
      break;
    case '{':
      return ScriptToken::CurlyBegin;
    case '}':
      return ScriptToken::CurlyEnd;
    case '?':
      return ScriptToken::QuestionMark;
    case '(':
      return ScriptToken::BracektBegin;
    case ')':
      return ScriptToken::BracektEnd;
    case '=':
      return ScriptToken::Assign;
    case '+':
      return ScriptToken::Plus;
    default:
      // default for [A-Z][a-z]
      break;
    }
  }
}

unsigned LinkerScriptLexer::getNextChar() {
  char curChar = curStringRef[pos];
  switch (curChar) {
  case 0:
    if (pos < curStringRef.size()) {
      pos++;
      return 0;
    } else {
      return EOF;
    }
  default:
    pos++;
    return (unsigned char)curChar;
  }
}

ScriptToken LinkerScriptLexer::getCommandOrSymbolName() {
  return ScriptToken::SymbolName;
}
