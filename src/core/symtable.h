#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "token.h"

enum IdType {
	Uninitialised,
	Global,
	Constant,
	Score,
	Function,
	Argument
};

string idTypeToString(IdType type) {
	switch (type) {
		case Uninitialised: return "Uninitialised";
		case Global: return "Global";
		case Constant: return "Constant";
		case Score: return "Score";
		case Function: return "Function";
		default: return "Unknown";
	}
}

struct symtableEntry {
	IdType varType;
	TokenType type;
	string genName;

	symtableEntry(IdType type) : varType(type) {}
};

unordered_map<string, symtableEntry*> symtable;

void symtabAdd(string id, symtableEntry* entry) {
	symtable.insert({id, entry});
}

symtableEntry* symtabLookup(string id) {
	symtableEntry* result;
	try {
		result = symtable.at(id);
	} catch (out_of_range e) {
		return nullptr;
	}
	return result;
}

bool varExists (string id) {
	return symtabLookup(id) != nullptr; 
}
/*
void newScope() {
	symtable.push_back(unordered_map<string, symtableEntry*>());
}

void deleteScope () {
	for (auto i : symtable.back()) {
		delete i.second;
	}
	symtable.pop_back();
}
*/
#endif // SYMTABLE_H