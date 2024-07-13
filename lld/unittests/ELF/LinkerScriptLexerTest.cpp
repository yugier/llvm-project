//===- lld/unittests/ELF/LinkerScriptLexerTest.cpp --------------*- C++ -*-===//
//
// This file is licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "lld/ELF/LinkerScriptLexer.h"
#include "llvm/ADT/SmallVector"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/MemoryBuff.h"
#include "llvm/Support/SourceMgr.h"

#include "gtest/gtest.h"

namespace lld {
namespace elf {

class LinkerScriptLexerTest : public testing::Test {
protected:
  llvm::SourceMgr SrcMgr;
  llvm::SMDiagnostic Err;
  std::unique_ptr<LinkerScriptLexer> Lexer;

  /*void setupCallToLinkScriptLexer(llvm::StringRef scriptStr) {
    std::unique_ptr<llvm::MemoryBuffer>
  Buffer(llvm::MemoryBuffer::getMemBuffer(scriptStr));
    SrcMgr.AddNewSourceBuffer(std::move(Buffer), SMLoc());
    EXPECT_EQ(Buffer, nullptr);

    Lexer.reset(LinkerScriptLexer())
  }

  void lexAndCheckTokens(llvm::StringRef scriptStr,
  llvm::SmallVector<ScriptToken> ExpectedTokens) {

    for(size_t I = 0; I < ExpectedTokens.size(); ++I) {
      EXPECTED_EQ();
    }
  } */
};
} // namespace elf
} // namespace lld
