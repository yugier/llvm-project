//===- ScriptLexer.h --------------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLD_ELF_SCRIPT_TOKENIZER_H
#define LLD_ELF_SCRIPT_TOKENIZER_H

namespace lld::elf {
enum CommandTokens {
  // Entry point
  ENTRY, // ENTRY

  // Commands Files
  INCLUDE,
  INPUT,
  GROUP,
  OUTPUT,
  SEARCH_DIR,
  STARTUP,

  INSERT, // TODO
  AFTER,  // TODO

  // Commands for object file formats
  OUTPUT_FORMAT,
  TARGET,

  // Other linker script commands
  ASSERT,
  EXTERN,
  // FORCE_COMMON_ALLOCATION
  // INHIBIT_COMMON_ALLOCATION
  // NOCROSSREFS
  OUTPUT_ARCH

      // Assignment
      PROVIDE,
  HIDDEN,
  PROVIDE_HIDDEN,

  // Section Command
  SECTIONS,
  // Input Section
  EXCLUDE_FILE,
  KEEP,
  INPUT_SECTION_FLAGS,

  // Read section
  OVERLAY,
  NOLOAD,
  COPY,
  INFO

      // Output Section
      OUTPUT,

  OVERWRITE_SECTIONS // TODO

      ALIGN, // TODO
  SUBALIGN,
  ONLY_IF_RO,
  ONLY_IF_RW,
  FILL, // TODO
  SORT, //

  // Builtin Functions
  ABSOLUTE,
  ADDR,
  ALIGN,
  // BLOCK, // synonym for ALIGN for compatibility with older linker script
  DATA_SEGMENT_ALIGN,
  DATA_SEGMENT_END,
  DEFINED,
  LOADADDR,
  LOG2CEIL, // TODO
  MAX,
  MIN,
  ORIGIN,        // TODO
  SEGMENT_START, // TODO
  // NEXT, // This function is closely related to ALIGN(exp); unless you use the
  // MEMORY command to define discontinuous memory for the output file, the two
  // functions are equivalent.
  SIZEOF,
  SIZEOF_HEADERS

      // PHDRS Command
      FILEHDR,
  PHDRS,
  AT,
  FLAGS,

  // Version Command
  VERSION,

  REGION_ALIAS   // TODO
      AS_NEEDED, // TODO
  CONSTRUCTORS,  // TODO: readsort?
}

enum labelTokens {
  Local,  // local
  Global, // global
  Extern, // extern
}

enum TargetType {
  ELF,
  Binary,
  Error
}

enum SymbolTokens {
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
  Greater       // >
      Less,     // <
  Minus,        // -
  Plus,         // +
  Bitwise,      // &
  Not,          // ^
  VerticalBar,  // |
  PercentSign,  // %
  // TODO:: operator
}

enum AssignmentSymbols {
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
}
} // namespace lld::elf

#endif // LLD_ELF_SCRIPT_TOKENIZER_H
