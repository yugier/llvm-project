//===- LinkerScriptLexer.h --------------------------------------------*- C++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLD_ELF_LINKER_SCRIPT_LEXER_H
#define LLD_ELF_LINKER_SCRIPT_LEXER_H

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
  llvm::SMLoc getLoc() const {
    return llvm::SMLoc::getFromPointer(curStringRef.begin());
  }
  bool Error(llvm::SMLoc ErrorLoc, const llvm::Twine &Msg) const;
  bool Error(const llvm::Twine &Msg) const { return Error(getLoc(), Msg); }
  void Warning(llvm::SMLoc WarningLoc, const llvm::Twine &Msg) const;
  void Warning(const Twine &Msg) const { return Warning(getLoc(), Msg); }

  ScriptToken next();  // update tok1 and tok2
  ScriptToken peek();  // return tok1
  ScriptToken peek2(); // return tok2

  const std::string &getTok1Val() const { return tok1Val; }
  const std::string &getTok2Val() const { return tok2Val; }

  bool expect(ScriptToken token); // check if tok1 matches argument token
  bool inExpression = false;

private:
  llvm::SMDiagnostic &ErrorInfo;
  llvm::SourceMgr &SM;

  const char *curPtr;
  llvm::MemoryBufferRef MB;

  llvm::StringRef curStringRef;
  ScriptToken tok1;
  ScriptToken tok2;
  size_t tok1Pos = 0;
  size_t tok2Pos = 0;
  std::string tok1Val;
  std::string tok2Val;

  const char *tokStart;

  llvm::StringRef skipComments();
  ScriptToken getToken();
  ScriptToken getCommandOrIdentify(size_t pos);
};
} // namespace lld::elf

#endif // LLD_ELF_LINKER_SCRIPT_LEXER_H
