#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include "Buff.h"
//#include "AbstractFile.h"
using namespace std;
#define BLKSize 128
class Addr {
public:
	const static int NOT_ALLOC = -1;
	int phyBlkNo;
	Addr();
	~Addr();
};

class AddrMap {
public:
	Addr firstMap[BLKSize];
	AddrMap();
	~AddrMap();
};

class SecondAddrMap {
public:
	AddrMap secondMap[BLKSize];
	SecondAddrMap();
	~SecondAddrMap();
};

class INode {
public:
	//const static int BLKSize = 128;
	const static int SmallFileBlkNum =	6;
	const static int BigFileBlkNum	=	2;
	const static int HugeFileBlkNum =	2;

	const static int WRITEFlag	= 1;
	const static int READFlag	= 2;
	//ÿһ��INode����һ���ļ�/Ŀ¼��ӵ��
	//����ͨ������ļ��鿴��ʲô���ԡ���Ŀ¼/�ļ�
	//AbstractFile* abstractFather;
	/*����*/
	Addr SmallFile[SmallFileBlkNum];
	AddrMap BigFile[BigFileBlkNum];
	SecondAddrMap HugeFile[HugeFileBlkNum];

	//�ļ���С
	int size;
	bool isAllocated;
	int No;
	int IOFlag;
	Buff* buff;

	INode();
	//INode(AbstractFile* file);
	~INode();

	void setSize(int);
	bool write(int off, int len, const char* writeBuf);
	void read(int off, int len);
	void erase();
	int getPhyBlkNo(int BlkNo);
	bool HaveThatBlk(int BlkNo);
};





