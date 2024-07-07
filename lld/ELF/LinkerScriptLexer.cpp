//===- ScriptParser.cpp ---------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "LinkerScriptLexer.h"
#include "lld/Common/ErrorHandler.h"
#include "llvm/ADT/Twine.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;
using namespace lld;
using namespace lld::elf;

LinkerScriptLexer::LinkerScriptLexer(MemoryBufferRef MB) {}

ScriptToken LinkerScriptLexer::getScriptToken() { return ScriptToken::ENTRY; }

unsigned LinkerScriptLexer::getNextChar() { return 0; }
