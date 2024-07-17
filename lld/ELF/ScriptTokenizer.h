//===- ScriptLexer.h --------------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the nums for LinkerScript lexer
//
//===----------------------------------------------------------------------===//

#ifndef LLD_ELF_SCRIPT_TOKENIZER_H
#define LLD_ELF_SCRIPT_TOKENIZER_H

namespace lld {
namespace elf {
enum class ScriptToken {
  // LS stands for LinkerScript
  LS_ENTRY,

  // Commands Files
  LS_INCLUDE,
  LS_INPUT,
  LS_GROUP,
  LS_MEMORY,
  LS_OUTPUT,
  LS_SEARCH_DIR,
  LS_STARTUP,

  LS_INSERT,
  LS_AFTER,

  // Commands for object file formats
  LS_OUTPUT_FORMAT,
  LS_TARGET,

  // Other linker script commands
  LS_ASSERT,
  LS_CONSTANT,
  LS_EXTERN,
  // FORCE_COMMON_ALLOCATION
  // INHIBIT_COMMON_ALLOCATION
  // NOCROSSREFS
  LS_OUTPUT_ARCH,

  // Assignment
  LS_PROVIDE,
  LS_HIDDEN,
  LS_PROVIDE_HIDDEN,

  LS_SECTIONS,
  LS_BEFORE,

  // Input Section
  LS_EXCLUDE_FILE,
  LS_KEEP,
  LS_INPUT_SECTION_FLAGS,

  // Read section
  LS_OVERLAY,
  LS_NOLOAD,
  LS_COPY,
  LS_INFO,

  // Output Section
  LS_OVERWRITE_SECTIONS,
  LS_SUBALIGN,
  LS_ONLY_IF_RO,
  LS_ONLY_IF_RW,
  LS_FILL,
  LS_SORT,

  // Builtin Functions
  LS_ABSOLUTE,
  LS_ADDR,
  LS_ALIGN,
  LS_ALIGNOF,
  // BLOCK, // synonym for ALIGN for compatibility with older linker script
  LS_DATA_SEGMENT_ALIGN,
  LS_DATA_SEGMENT_END,
  LS_DEFINED,
  LS_LENGTH,
  LS_LOADADDR,

  LS_LOG2CEIL,
  LS_MAX,
  LS_MIN,
  LS_ORIGIN,
  LS_SEGMENT_START,
  // NEXT, // This function is closely related to ALIGN(exp); unless you use the
  // MEMORY command to define discontinuous memory for the output file, the two
  // functions are equivalent.
  LS_SIZEOF,
  LS_SIZEOF_HEADERS,

  // PHDRS Command
  LS_FILEHDR,
  LS_PHDRS,
  LS_AT,
  LS_FLAGS,

  // Version Command
  LS_VERSION,

  LS_REGION_ALIAS,
  LS_AS_NEEDED,
  LS_CONSTRUCTORS,

  // Symbolic Constants
  LS_MAXPAGESIZE,
  LS_COMMONPAGESIZE,

  LS_BYTE,
  LS_SHORT,
  LS_LONG,
  LS_QUAD,

  LS_Local,  // local
  LS_Global, // global
  LS_Extern, // extern

  LS_ELF,
  LS_Binary,
  Error,
  Eof,

  Identifier,
  Hexdecimal,   // 0x
  Hexdecimal_H, // end with H/h
  Decimal,
  Decimal_K, // end with K/k
  Decimal_M, // end with M/m

  // Symbol tokens
  CurlyBegin,   // {
  CurlyEnd,     // }
  BracektBegin, // (
  BracektEnd,   // )
  Comma,        // ,
  Semicolon,    // ;
  Colon,        // :
  Asterisk,     // *
  QuestionMark, // ?
  Backslash,    // "\"
  Slash,        // /
  Greater,      // >
  Less,         // <
  Minus,        // -
  Plus,         // +
  Bitwise,      // &
  Xor,          // ^
  Or,           // |
  PercentSign,  // %
  Underscore,   // _
  Dot,          // .
  Quote, // Quoted token. Note that double-quote characters are parts of a token
  // because, in a glob match context, only unquoted tokens are interpreted as
  // glob patterns. Double-quoted tokens are literal patterns in that context.

  // Assignmemnt
  Assign,           // =
  PlusAssign,       // +=
  MinusAssign,      // -=
  MulAssign,        // *=
  DivAssign,        // /=
  LeftShiftAssign,  // <<=
  RightShiftAssign, // >>=
  AndAssign,        // &=
  OrAssign,         // |=
  XorAssign,        // ^=

  // operator token
  NotEqual,     // !=
  Equal,        // ==
  GreaterEqual, // >=
  LessEqual,    // <=
  LeftShift,    // <<
  RightShift,   // >>
  AndGate,      // &&
  OrGate,       // ||
};
} // namespace elf
} // namespace lld

#endif // LLD_ELF_SCRIPT_TOKENIZER_H
