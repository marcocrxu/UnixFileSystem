#pragma once
#include <string>
#include "INode.h"
using namespace std;

class AbstractFile {
public:
	string name;
	bool isDir;
	INode *inode;
	string curPath;

	AbstractFile();
	~AbstractFile();
};