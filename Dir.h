#pragma once
#include "AbstractFile.h"
#include <vector>

class Dir :public AbstractFile {
public:
	Dir* father;
	vector<AbstractFile*> child;
public:
	Dir(string);

	bool HaveSameName(string name);

	AbstractFile* findFile(string name);
};