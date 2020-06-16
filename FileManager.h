#pragma once
#include "SuperBlock.h"
#include "INode.h"
#include "Dir.h"
#include "File.h"
#include "Buff.h"

class INodeTable {
public:
	const static int MAX_INODE_NUM = 100;
	INode allINode[MAX_INODE_NUM];

	INodeTable();
	~INodeTable();

	int AllocINode();

};

class FileManager {
public:
	//User*		user;
	SuperBlock	superBlock;
	INodeTable	inodeTable;
	Dir*		root;
	Dir*		curPos;
	Buff*		buff;
	bool		initialize;
public:
	FileManager();
	~FileManager();

	bool check();

	void ls();
	void mkdir(string dirname);
	void fformat();
	bool fcreat(string filename);
	int fopen(string filename, int flag);
	bool fclose(int fd);
	void fread(int fd, int offset, int len);
	bool fwrite(int fd, int off, const char* writeBuf, int len);
	bool fdelete(string filename);
};


