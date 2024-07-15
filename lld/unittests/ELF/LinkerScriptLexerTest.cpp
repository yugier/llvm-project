//===- lld/unittests/ELF/LinkerScriptLexerTest.cpp --------------*- C++ -*-===//
//
// This file is licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "../ELF/LinkerScriptLexer.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/MemoryBufferRef.h"

#include "gtest/gtest.h"

namespace lld {
namespace elf {

class LinkerScriptLexerTest : public testing::Test {
protected:
  std::unique_ptr<LinkerScriptLexer> Lexer;
  std::unique_ptr<MemoryBuffer> Buffer;

  void setupCallToLinkScriptLexer(llvm::StringRef scriptStr) {
    Buffer = llvm::MemoryBuffer::getMemBuffer(scriptStr);
    Lexer = std::make_unique<LinkerScriptLexer>(
        LinkerScriptLexer(Buffer->getMemBufferRef()));
  }

  void lexAndCheckTokens(llvm::SmallVector<ScriptToken> ExpectedTokens) {
    for (const auto &expected : ExpectedTokens) {
      Lexer->advanceLexer();
      EXPECT_EQ(Lexer->getTokenKind(), expected);
    }
  }
};

TEST_F(LinkerScriptLexerTest, CheckEntry) {
  llvm::StringRef testRef = "      ENTRY";
  setupCallToLinkScriptLexer(testRef);
  llvm::SmallVector<ScriptToken> ExpectedTokens({ScriptToken::LS_ENTRY});
  lexAndCheckTokens(ExpectedTokens);
}
} // namespace elf
} // namespace lld
