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
#include "llvm/Support/raw_ostream.h"

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
    bool outputinfo = true;
    for (const auto &expected : ExpectedTokens) {
      Lexer->advanceLexer();
      EXPECT_EQ(Lexer->getTokenKind(), expected);
      if (outputinfo) {
        llvm::errs() << Lexer->getTokenStringRef() << " "
                     << static_cast<unsigned>(Lexer->getTokenKind()) << " "
                     << static_cast<unsigned>(expected) << "\n";
      }
    }
  }
};

TEST_F(LinkerScriptLexerTest, CheckEntry) {
  llvm::StringRef testRef = "      ENTRY";
  setupCallToLinkScriptLexer(testRef);
  llvm::SmallVector<ScriptToken> ExpectedTokens({ScriptToken::LS_ENTRY});
  lexAndCheckTokens(ExpectedTokens);
}

TEST_F(LinkerScriptLexerTest, CheckEntryLabel) {
  llvm::StringRef testRef = "ENTRY(_label)";
  setupCallToLinkScriptLexer(testRef);
  llvm::SmallVector<ScriptToken> ExpectedTokens(
      {ScriptToken::LS_ENTRY, ScriptToken::BracektBegin,
       ScriptToken::Identifier, ScriptToken::BracektEnd});
  lexAndCheckTokens(ExpectedTokens);
}

TEST_F(LinkerScriptLexerTest, CheckSECTIONSandALIGN) {
  llvm::StringRef testRef = "SECTIONS { \
  .super_aligned : ALIGN(16) { /* ... */  }}";

  setupCallToLinkScriptLexer(testRef);
  llvm::SmallVector<ScriptToken> ExpectedTokens(
      {ScriptToken::LS_SECTIONS, ScriptToken::CurlyBegin, ScriptToken::Dot,
       ScriptToken::Identifier, ScriptToken::Colon, ScriptToken::LS_ALIGN,
       ScriptToken::BracektBegin, ScriptToken::Decimal, ScriptToken::BracektEnd,
       ScriptToken::CurlyBegin, ScriptToken::CurlyEnd, ScriptToken::CurlyEnd});
  lexAndCheckTokens(ExpectedTokens);
}

TEST_F(LinkerScriptLexerTest, CheckHex) {
  llvm::StringRef testRef =
      "SECTIONS{ \n . = 0x10000;\n .text : { *(.text) }\n \
                           . = 0x8000000;\n .data : { *(.data) }\n .bss : { *(.bss) }}";
  setupCallToLinkScriptLexer(testRef);
  llvm::SmallVector<ScriptToken> ExpectedTokens(
      {ScriptToken::LS_SECTIONS, ScriptToken::CurlyBegin,
       ScriptToken::Dot,         ScriptToken::Assign,
       ScriptToken::Hexdecimal,  ScriptToken::Semicolon,
       ScriptToken::Dot,         ScriptToken::Identifier,
       ScriptToken::Colon,       ScriptToken::CurlyBegin,
       ScriptToken::Asterisk,    ScriptToken::BracektBegin,
       ScriptToken::Dot,         ScriptToken::Identifier,
       ScriptToken::BracektEnd,  ScriptToken::CurlyEnd,
       ScriptToken::Dot,         ScriptToken::Assign,
       ScriptToken::Hexdecimal,  ScriptToken::Semicolon,
       ScriptToken::Dot,         ScriptToken::Identifier,
       ScriptToken::Colon,       ScriptToken::CurlyBegin,
       ScriptToken::Asterisk,    ScriptToken::BracektBegin,
       ScriptToken::Dot,         ScriptToken::Identifier,
       ScriptToken::BracektEnd,  ScriptToken::CurlyEnd,
       ScriptToken::Dot,         ScriptToken::Identifier,
       ScriptToken::Colon,       ScriptToken::CurlyBegin,
       ScriptToken::Asterisk,    ScriptToken::BracektBegin,
       ScriptToken::Dot,         ScriptToken::Identifier,
       ScriptToken::BracektEnd,  ScriptToken::CurlyEnd,
       ScriptToken::CurlyEnd});
  lexAndCheckTokens(ExpectedTokens);
}

} // namespace elf
} // namespace lld
