#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <cstddef>
#include <utility>
#include <fstream>

using namespace std;

unsigned int testsFailed;

#include "error.h"
#include "symtable.h"
#include "compiler/lexer.h"
#include "compiler/parser.h"
#include "codegen/gen.cpp"

void lexerTest();
void parserTest();
void genTest();