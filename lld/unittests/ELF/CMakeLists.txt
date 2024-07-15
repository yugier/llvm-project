# Test usage of LLD Linker Script Lexer
set(LLVM_LINK_COMPONENTS
  Support
)

add_lld_unittests(LLDELFLexerTests
  LinkerScriptLexerTest.cpp
)

target_link_libraries(LLDELFLexerTests
  PRIVATE
  lldELF
  lldCommon
  LLVMTestingSupport
)
