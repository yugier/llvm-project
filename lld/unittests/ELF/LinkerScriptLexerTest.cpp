//===- lld/unittests/ELF/LinkerScriptLexerTest.cpp --------------*- C++ -*-===//
//
// This file is licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "../ELF/LinkerScriptLexer.h"
#include "llvm/ADT/SmallVector"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/MemoryBuff.h"

#include "gtest/gtest.h"

namespace lld {
namespace elf {

class LinkerScriptLexerTest : public testing::Test {
protected:
  std::unique_ptr<LinkerScriptLexer> Lexer;
  std::unique_ptr<MemoryBuffer> Buffer;

  void setupCallToLinkScriptLexer(llvm::StringRef scriptStr) {
    Buffer.reset(llvm::MemoryBuffer::getMemBuffer(scriptStr));
    Lexer.reset(LinkerScriptLexer(Buffer->getMemBufferRef()))
  }

  void lexAndCheckTokens(llvm::SmallVector<ScriptToken> ExpectedTokens) {
    for (const auto &expected : ExpectedTokens) {
      Lexer->advanceLexer();
      EXPECTED_EQ(Lexer->getTokenKind, expected);
    }
  }
};

TEST(LinkerScriptLexerTest, CheckEntry) {
  llvm::StringRef testRef = "      ENTRY";
  setupCallToLinkScriptLexer(testRef);
  llvm::SmallVector<ScriptToken> ExpectedTokens({ScriptToken::ENTRY});
  lexAndCheckTokens(ExpectedTokens);
}
} // namespace elf
} // namespace lld
