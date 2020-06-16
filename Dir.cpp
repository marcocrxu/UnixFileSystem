#include "Dir.h"

Dir::Dir(string name) {
	this->father = NULL;
	this->isDir = true;
	this->name = name;
}

bool Dir::HaveSameName(string name) {
	for (int i = 0; i < child.size(); i++) {
		if (child.at(i)->name == name) {
			return true;
		}
	}
	return false;
}

AbstractFile * Dir::findFile(string name) {


	return NULL;
}
