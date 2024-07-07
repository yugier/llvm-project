//===- LinkerScriptLexer.h --------------------------------------------*- C++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLD_ELF_LINKER_SCRIPT_Lexer_H
#define LLD_ELF_LINKER_SCRIPT_Lexer_H

#include "ScriptTokenizer.h"
#include "lld/Common/LLVM.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/MemoryBufferRef.h"

namespace lld::elf {

class LinkerScriptLexer {
public:
  explicit LinkerScriptLexer(MemoryBufferRef MB);

private:
  const char *curPtr;
  llvm::StringRef curStringRef;
  size_t pos = 0;
  unsigned getNextChar();

  ScriptToken getToken();
};
} // namespace lld::elf

#endif // LLD_ELF_LINKER_SCRIPT_Lexer_H
