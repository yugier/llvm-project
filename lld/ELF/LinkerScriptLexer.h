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
  struct TokenInfo {
    ScriptToken kind;
    llvm::StringRef val;
  };

  // LLVM SourceMgr and SMDiagnostic cannot be used now since
  // ctx CommonLinkerContext has ownership of all MemoryBuffer
  // by using SmallVector<MemoryBuffer>> memoryBuffers in ELF/Config.h
  /*llvm::SMLoc getLoc() const {
    return llvm::SMLoc::getFromPointer(curStringRef.begin());
  }

  bool Error(llvm::SMLoc ErrorLoc, const llvm::Twine &Msg) const;
  bool Error(const llvm::Twine &Msg) const { return Error(getLoc(), Msg); }
  void Warning(llvm::SMLoc WarningLoc, const llvm::Twine &Msg) const;
  void Warning(const Twine &Msg) const { return Warning(getLoc(), Msg); }*/

  bool expect(ScriptToken token); // check if tok1 matches argument token
  bool inExpression = false;

  // TODO: rewrite next(), peek(), and peek2() since TokenInfo change

private:
  llvm::SMDiagnostic &ErrorInfo;
  llvm::SourceMgr &SM;
  llvm::MemoryBufferRef MB;
  llvm::StringRef curStringRef;

  // ScriptToken tok1;
  // ScriptToken tok2;
  // size_t tok1Pos = 0;
  // size_t tok2Pos = 0;

  llvm::StringRef skipComments();
  TokenInfo advanceTokenInfo(ScriptToken kind, size_t pos);
  TokenInfo getTokenInfo();
  TokenInfo getSymbolToken();
  TokenInfo getQuotedToken();
  TokenInfo getDigits();
  TokenInfo getCommandOrIdentify();
};
} // namespace lld::elf

#endif // LLD_ELF_LINKER_SCRIPT_LEXER_H
