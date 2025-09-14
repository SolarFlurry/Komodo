#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "token.h"

enum IdType {
	Uninitialised,
	Global,
	Constant,
	Score,
	Function
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
	string identifier;
	IdType type;
	symtableEntry(string ident, IdType type) : identifier(ident), type(type) {}
	symtableEntry(string ident) : identifier(ident), type(Uninitialised) {}
};

vector<symtableEntry*> symtable;

void symtabAdd(symtableEntry* entry) {
	symtable.push_back(entry);
}

symtableEntry* symtabLookup(string identifier) {
	for (symtableEntry* entry : symtable) {
		if (entry->identifier == identifier) {
			return entry;
		}
	}
	return nullptr;
}

void printSymtable() {
	for (symtableEntry* entry : symtable) {
		cout << "\"" << entry->identifier << "\"\t" << idTypeToString(entry->type) << "\n";
	}
}

#endif // SYMTABLE_H