#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "token.h"

bool namespaceExists(string);

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

unordered_map<string, unordered_map<string, symtableEntry*>> symtable;

void symtabAdd(string id, symtableEntry* entry) {
	symtable.at(nameSpaces.back()).insert({id, entry});
}

symtableEntry* symtabLookup(string id) {
	symtableEntry* result;
	if (!namespaceExists(nameSpaces.back())) return nullptr;
	try {
		result = symtable.at(nameSpaces.back()).at(id);
	} catch (out_of_range e) {
		return nullptr;
	}
	return result;
}

symtableEntry* symtabLookup(string id, string ns) {
	symtableEntry* result;
	if (!namespaceExists(ns)) return nullptr;
	try {
		result = symtable.at(ns).at(id);
	} catch (out_of_range e) {
		return nullptr;
	}
	return result;
}


void printSymtab() {
	for (auto scope : symtable) {
		cout << scope.first << ":\n";
		for (auto entry : scope.second) {
			cout << entry.first << "\t" << idTypeToString(entry.second->varType) << "\n";
		}
	}
}

bool varExists (string id) {
	return symtable.at(nameSpaces.back()).count(id) > 0;
}

bool namespaceExists (string ns) {
	return symtable.count(ns) > 0;
}

void newNamespace(string ns) {
	symtable.insert({ns, unordered_map<string, symtableEntry*>()});
	nameSpaces.push_back(ns);
}
/*
void deleteScope () {
	for (auto i : symtable.back()) {
		delete i.second;
	}
	symtable.pop_back();
}
*/
#endif // SYMTABLE_H