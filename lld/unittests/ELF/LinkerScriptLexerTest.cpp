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
      {ScriptToken::LS_SECTIONS, ScriptToken::CurlyBegin,
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
      {ScriptToken::LS_SECTIONS,  ScriptToken::CurlyBegin,
       ScriptToken::Dot,          ScriptToken::Assign,
       ScriptToken::Hexdecimal,   ScriptToken::Semicolon,
       ScriptToken::Identifier,   ScriptToken::Colon,
       ScriptToken::CurlyBegin,   ScriptToken::Asterisk,
       ScriptToken::BracektBegin, ScriptToken::Identifier,
       ScriptToken::BracektEnd,   ScriptToken::CurlyEnd,
       ScriptToken::Dot,          ScriptToken::Assign,
       ScriptToken::Hexdecimal,   ScriptToken::Semicolon,
       ScriptToken::Identifier,   ScriptToken::Colon,
       ScriptToken::CurlyBegin,   ScriptToken::Asterisk,
       ScriptToken::BracektBegin, ScriptToken::Identifier,
       ScriptToken::BracektEnd,   ScriptToken::CurlyEnd,
       ScriptToken::Identifier,   ScriptToken::Colon,
       ScriptToken::CurlyBegin,   ScriptToken::Asterisk,
       ScriptToken::BracektBegin, ScriptToken::Identifier,
       ScriptToken::BracektEnd,   ScriptToken::CurlyEnd,
       ScriptToken::CurlyEnd});
  lexAndCheckTokens(ExpectedTokens);
}

TEST_F(LinkerScriptLexerTest, CheckPROVIDECommand) {
  llvm::StringRef testRef = "SECTIONS\n{.text :\n{\n*(.text)\n\t_etext = .;\n \
                             \t PROVIDE(etext = .);\n}\n}";
  setupCallToLinkScriptLexer(testRef);
  llvm::SmallVector<ScriptToken> ExpectedTokens({
      ScriptToken::LS_SECTIONS,  ScriptToken::CurlyBegin,
      ScriptToken::Identifier,   ScriptToken::Colon,
      ScriptToken::CurlyBegin,   ScriptToken::Asterisk,
      ScriptToken::BracektBegin, ScriptToken::Identifier,
      ScriptToken::BracektEnd,   ScriptToken::Identifier,
      ScriptToken::Assign,       ScriptToken::Dot,
      ScriptToken::Semicolon,    ScriptToken::LS_PROVIDE,
      ScriptToken::BracektBegin, ScriptToken::Identifier,
      ScriptToken::Assign,       ScriptToken::Dot,
      ScriptToken::BracektEnd,   ScriptToken::Semicolon,
      ScriptToken::CurlyEnd,     ScriptToken::CurlyEnd,
  });

  lexAndCheckTokens(ExpectedTokens);
}

TEST_F(LinkerScriptLexerTest, CheckINSERTandBEFORE) {
  llvm::StringRef testRef =
      "SECTIONS { .foo.data : { *(.foo.data) } } INSERT BEFORE .data;";
  setupCallToLinkScriptLexer(testRef);
  llvm::SmallVector<ScriptToken> ExpectedTokens(
      {ScriptToken::LS_SECTIONS, ScriptToken::CurlyBegin,
       ScriptToken::Identifier, ScriptToken::Colon, ScriptToken::CurlyBegin,
       ScriptToken::Asterisk, ScriptToken::BracektBegin,
       ScriptToken::Identifier, ScriptToken::BracektEnd, ScriptToken::CurlyEnd,
       ScriptToken::CurlyEnd, ScriptToken::LS_INSERT, ScriptToken::LS_BEFORE,
       ScriptToken::Identifier, ScriptToken::Semicolon});

  lexAndCheckTokens(ExpectedTokens);
}

TEST_F(LinkerScriptLexerTest, CheckALIGNandDecimal) {
  llvm::StringRef testRef = "SECTIONS {.foo : ALIGN(2M) { *(.foo) }}";
  setupCallToLinkScriptLexer(testRef);
  llvm::SmallVector<ScriptToken> ExpectedTokens(
      {ScriptToken::LS_SECTIONS, ScriptToken::CurlyBegin,
       ScriptToken::Identifier, ScriptToken::Colon, ScriptToken::LS_ALIGN,
       ScriptToken::BracektBegin, ScriptToken::Decimal_M,
       ScriptToken::BracektEnd, ScriptToken::CurlyBegin, ScriptToken::Asterisk,
       ScriptToken::BracektBegin, ScriptToken::Identifier,
       ScriptToken::BracektEnd, ScriptToken::CurlyEnd, ScriptToken::CurlyEnd});

  lexAndCheckTokens(ExpectedTokens);
}

TEST_F(LinkerScriptLexerTest, CheckAbsoluteExprTest) {
  llvm::StringRef testRef = "SECTIONS { \
  .text : { \
    bar1 = ALIGNOF(.text); \
    bar2 = CONSTANT (MAXPAGESIZE); \
    bar3 = SIZEOF (.text); \
    bar4 = SIZEOF_HEADERS; \
    bar5 = 0x42; \
    bar6 = foo + 1; \
    *(.text) \
  } \
}";
  setupCallToLinkScriptLexer(testRef);
  llvm::SmallVector<ScriptToken> ExpectedTokens({ScriptToken::LS_SECTIONS,
                                                 ScriptToken::CurlyBegin,
                                                 ScriptToken::Identifier,
                                                 ScriptToken::Colon,
                                                 ScriptToken::CurlyBegin,
                                                 ScriptToken::Identifier,
                                                 ScriptToken::Assign,
                                                 ScriptToken::LS_ALIGNOF,
                                                 ScriptToken::BracektBegin,
                                                 ScriptToken::Identifier,
                                                 ScriptToken::BracektEnd,
                                                 ScriptToken::Semicolon,

                                                 ScriptToken::Identifier,
                                                 ScriptToken::Assign,
                                                 ScriptToken::LS_CONSTANT,
                                                 ScriptToken::BracektBegin,
                                                 ScriptToken::LS_MAXPAGESIZE,
                                                 ScriptToken::BracektEnd,
                                                 ScriptToken::Semicolon,

                                                 ScriptToken::Identifier,
                                                 ScriptToken::Assign,
                                                 ScriptToken::LS_SIZEOF,
                                                 ScriptToken::BracektBegin,
                                                 ScriptToken::Identifier,
                                                 ScriptToken::BracektEnd,
                                                 ScriptToken::Semicolon,

                                                 ScriptToken::Identifier,
                                                 ScriptToken::Assign,
                                                 ScriptToken::LS_SIZEOF_HEADERS,
                                                 ScriptToken::Semicolon,

                                                 ScriptToken::Identifier,
                                                 ScriptToken::Assign,
                                                 ScriptToken::Hexdecimal,
                                                 ScriptToken::Semicolon,

                                                 ScriptToken::Identifier,
                                                 ScriptToken::Assign,
                                                 ScriptToken::Identifier,
                                                 ScriptToken::Plus,
                                                 ScriptToken::Decimal,
                                                 ScriptToken::Semicolon,

                                                 ScriptToken::Asterisk,
                                                 ScriptToken::BracektBegin,
                                                 ScriptToken::Identifier,
                                                 ScriptToken::BracektEnd,
                                                 ScriptToken::CurlyEnd,
                                                 ScriptToken::CurlyEnd});

  lexAndCheckTokens(ExpectedTokens);
}

TEST_F(LinkerScriptLexerTest, checkAddrZeroTest) {
  llvm::StringRef testRef = "SECTIONS {\
  foo = ADDR(.text) - ABSOLUTE(ADDR(.text));\
};";
  setupCallToLinkScriptLexer(testRef);
  llvm::SmallVector<ScriptToken> ExpectedTokens(
      {ScriptToken::LS_SECTIONS, ScriptToken::CurlyBegin,
       ScriptToken::Identifier, ScriptToken::Assign, ScriptToken::LS_ADDR,
       ScriptToken::BracektBegin, ScriptToken::Identifier,
       ScriptToken::BracektEnd, ScriptToken::Minus, ScriptToken::LS_ABSOLUTE,
       ScriptToken::BracektBegin, ScriptToken::LS_ADDR,
       ScriptToken::BracektBegin, ScriptToken::Identifier,
       ScriptToken::BracektEnd, ScriptToken::BracektEnd, ScriptToken::Semicolon,
       ScriptToken::CurlyEnd});

  lexAndCheckTokens(ExpectedTokens);
}

} // namespace elf
} // namespace lld
