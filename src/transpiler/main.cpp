#include <iostream>
#include <vector>
#include <string>

#include "lexer.h"

int main () {
	std::string line;
	std::string program;
	while (std::getline(std::cin, line)) {
		program += line + '\n';
	}

	std::vector<Token*> tokList = tokenise(program);
	for (int i = 0; i < tokList.size(); i++) {
		printToken(tokList[i]);
	}
}