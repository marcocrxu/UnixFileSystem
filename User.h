#pragma once
#include <string>
#include "INode.h"
#include "FileManager.h"
#include "Dir.h"
using namespace std;

#define ROOT_DIR_NAME "/"

class User 
{
public:
	string username;
	FileManager* fileManager;
	//��ǰλ�ã���ǰ����Ŀ¼��Ӧ��AbstractFile��
	Dir* curPos;
	Dir* parentPos;
	//��ǰ·��
	string curDirPath;

	User(string username);
	~User();

	void Ls();
	void pwd();
	void mkdir(string name);
	void cd(string dirname);
	void fcreat(string filename);
	void fopen(string filename, int flag);
	void fformat();
	void fread(int fd, int off, int len);
	void fwrite(int fd, int dff, const char* c, int len);
	void fdelete(string filename);
	void fclose(int fd);
};
