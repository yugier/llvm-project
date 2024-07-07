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
enum ScriptToken {
  ENTRY,

  // Commands Files
  INCLUDE,
  INPUT,
  GROUP,
  OUTPUT,
  SEARCH_DIR,
  STARTUP,

  INSERT,
  AFTER,

  // Commands for object file formats
  OUTPUT_FORMAT,
  TARGET,

  // Other linker script commands
  ASSERT,
  EXTERN,
  // FORCE_COMMON_ALLOCATION
  // INHIBIT_COMMON_ALLOCATION
  // NOCROSSREFS
  OUTPUT_ARCH,

  // Assignment
  PROVIDE,
  HIDDEN,
  PROVIDE_HIDDEN,

  SECTIONS,

  // Input Section
  EXCLUDE_FILE,
  KEEP,
  INPUT_SECTION_FLAGS,

  // Read section
  OVERLAY,
  NOLOAD,
  COPY,
  INFO,

  // Output Section
  OVERWRITE_SECTIONS,
  SUBALIGN,
  ONLY_IF_RO,
  ONLY_IF_RW,
  FILL,
  SORT,

  // Builtin Functions
  ABSOLUTE,
  ADDR,
  ALIGN,
  // BLOCK, // synonym for ALIGN for compatibility with older linker script
  DATA_SEGMENT_ALIGN,
  DATA_SEGMENT_END,
  DEFINED,
  LOADADDR,

  LOG2CEIL,
  MAX,
  MIN,
  ORIGIN,
  SEGMENT_START,
  // NEXT, // This function is closely related to ALIGN(exp); unless you use the
  // MEMORY command to define discontinuous memory for the output file, the two
  // functions are equivalent.
  SIZEOF,
  SIZEOF_HEADERS,

  // PHDRS Command
  FILEHDR,
  PHDRS,
  AT,
  FLAGS,

  // Version Command
  VERSION,

  REGION_ALIAS,
  AS_NEEDED,
  CONSTRUCTORS,

  // Symbolic Constants
  MAXPAGESIZE,
  COMMONPAGESIZE,

  Local,  // local
  Global, // global
  Extern, // extern

  ELF,
  Binary,
  Error,

  CurlyBegin,   // {
  CurlyEnd,     // }
  BracektBegin, // (
  BracektEnd,   // )
  Comma,        // ,
  Semicolon,    // ;
  Colon,        // :
  Asterisk,     // *
  QuestionMark, // ?
  Bacckslash,   // "\"
  Slash,        // /
  Greater,      // >
  Less,         // <
  Minus,        // -
  Plus,         // +
  Bitwise,      // &
  Not,          // ^
  VerticalBar,  // |
  PercentSign,  // %

  // Assignmemnt
  Assign,           // =
  PlusAssign,       // +=
  MinussAssign,     // -=
  MulAssign,        // *=
  DivAssign,        // /=
  LeftShiftAssign,  // <<=
  RightShiftAssign, // >>=
  BitWiseAssign,    // &=
  BarAssign         // |=
};
} // namespace elf
} // namespace lld

#endif // LLD_ELF_SCRIPT_TOKENIZER_H
