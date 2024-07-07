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
    case 'A' ... 'Z':
    case 'a' ... 'z':
      // TODO
      break;
    case '0' ... '9':
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
    }
  }
}

unsigned LinkerScriptLexer::getNextChar() {
  char curChar = *curPtr;
  switch (curChar) {
  case 0:
    if (curPtr != curStringRef.end()) {
      curPtr++;
      return 0;
    } else {
      return EOF;
    }
  default:
    curPtr++;
    return (unsigned char)curChar;
  }
}
