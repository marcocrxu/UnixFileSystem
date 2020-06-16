#pragma once
#include "AbstractFile.h"
#include "Dir.h"

class File :public AbstractFile {
public:
	const static int READ = 1;
	const static int WRITE = 2;

	/*读写标志*/
	int flag = 0;
	/*读写的偏移量*/
	int offset;

	Dir* father;
	//AbstractFile* father;
public:
	File(string);
	~File();
	
};