# Test usage of LLD Linker Script Lexer

add_lld_unittests(LLDELFLexerTests
  LinkerScriptLexerTest.cpp
)

target_link_libraries(LLDELFLexerTests
  PRIVATE
  lldELF
  lldCommon
)
