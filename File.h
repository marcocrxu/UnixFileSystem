#pragma once
#include "AbstractFile.h"
#include "Dir.h"

class File :public AbstractFile {
public:
	const static int READ = 1;
	const static int WRITE = 2;

	/*��д��־*/
	int flag = 0;
	/*��д��ƫ����*/
	int offset;

	Dir* father;
	//AbstractFile* father;
public:
	File(string);
	~File();
	
};