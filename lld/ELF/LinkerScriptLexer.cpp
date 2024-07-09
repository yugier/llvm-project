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
    skipComments();
  }
}

llvm::StringRef LinkerScriptLexer::skipComments() {
  // this code now is copied from ScriptLexer.cpp
  // and modified so it can use SourceMgr
  while (true) {
    if (curStringRef.starts_with("/*")) {
      size_t e = curStringRef.find("*/", 2);
      if (e == llvm::StringRef::npos) {
        // TODO: set up error message
      }
      curStringRef = curStringRef.substr(e + 2);
      continue;
    }
    if (curStringRef.starts_with("#")) {
      size_t e = curStringRef.find("\n", 1);
    }
  }
}

ScriptToken LinkerScriptLexer::getCommandOrSymbolName() {

  return ScriptToken::SymbolName;
}
