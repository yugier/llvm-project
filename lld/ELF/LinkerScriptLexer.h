//===- LinkerScriptLexer.h --------------------------------------------*- C++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLD_ELF_LINKER_SCRIPT_Lexer_H
#define LLD_ELF_LINKER_SCRIPT_Lexer_H

#include "ScriptTokenizer.h"
#include "lld/Common/LLVM.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/Twine.h"
#include "llvm/Support/MemoryBufferRef.h"
#include "llvm/Support/SMLoc.h"
#include "llvm/Support/SourceMgr.h"

namespace lld::elf {
class SMDiagnostic;
class SourceMgr;

class LinkerScriptLexer {
public:
  explicit LinkerScriptLexer(MemoryBufferRef MB, llvm::SourceMgr &SM,
                             llvm::SMDiagnostic &Err);
  llvm::SMLoc getLoc() const { return llvm::SMLoc::getFromPointer(tokStart); }
  bool Error(llvm::SMLoc ErrorLoc, const llvm::Twine &Msg) const;
  bool Error(const llvm::Twine &Msg) const { return Error(getLoc(), Msg); }
  void Warning(llvm::SMLoc WarningLoc, const llvm::Twine &Msg) const;
  void Warning(const Twine &Msg) const { return Warning(getLoc(), Msg); }

private:
  llvm::SMDiagnostic &ErrorInfo;
  llvm::SourceMgr &SM;
  const char *curPtr;
  llvm::StringRef curStringRef;

  const char *tokStart;
  size_t pos = 0;
  unsigned getNextChar();

  void skipComment();
  ScriptToken getToken();
  ScriptToken getIdentify();
  ScriptToken getArithmeticOrAssignment();
  ScriptToken getCommandOrSymbolName();
};
} // namespace lld::elf

#endif // LLD_ELF_LINKER_SCRIPT_Lexer_H
