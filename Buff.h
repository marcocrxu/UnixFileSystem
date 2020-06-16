#pragma once
#include <iostream>
#include <fstream>
using namespace std;
#define BUFFSIZE	1<<20
#define SIZE		128
#define DISKNAME	"./myDisk.txt"
#define BLKSIZE		131072
class Buff {
public:
	fstream stream;
	bool UsedFlag[BLKSIZE];
public:
	Buff();
	~Buff();

	int Alloc();
	void fformat();
	bool check();
	void clear(int phyBlkNo);
	void read(int phyBlkNo, int off, int len);
	bool write(int phyBlkNo, int off, int len, const char* writeBuf);
};
