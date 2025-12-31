#pragma once

#include "../utils/include.hpp"
#include "./ast.hpp"

struct Identifier;

typedef std::variant<Identifier> Type;