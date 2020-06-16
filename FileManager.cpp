#include "FileManager.h"

FileManager::FileManager() {
	//FileManager类初始化的时候，需要创建root节点
	root = new Dir("/");
	int rootNum = inodeTable.AllocINode();
	if (rootNum == -1) {
		cout << "Some Thing Happened In FileManager Initializer!" << endl;
		exit(0);
	}
	INode *rootINode = &inodeTable.allINode[rootNum];
	root->inode = rootINode;
	this->curPos = root;
	//inodeTable.allINode[rootNum].abstractFather = root;

	initialize = false;

	buff = new Buff();

	for (int i = 0; i < inodeTable.MAX_INODE_NUM; i++) {
		inodeTable.allINode[i].buff = buff;
	}
}

FileManager::~FileManager() {

}

bool FileManager::check() {
	//if (this->user == nullptr)
	//	return false;
	return true;
}

void FileManager::ls() {
	if (!check()) {
		cout << "ls failed due to reasons below:" << endl;
		cout << "Some Error Happened In FileManager Class, Please report this to developer!" << endl;
		return;
	}
	
}

void FileManager::mkdir(string dirname) {
	if (!check()) {
		cout << "mkdir failed due to reasons below:" << endl;
		cout << "Some Error Happened In FileManager Class, Please report this to developer!" << endl;
		return;
	}
	//检查当前目录下面是不是有同名的文件
	bool find = this->curPos->HaveSameName(dirname);
	if (find) {
		cout << "mkdir failed due to reasons below:" << endl;
		cout << "Current Dir has a File/Dir has the same name with the Dir name you want to create!" << endl;
		return;
	}
	Dir* dir = new Dir(dirname);
	dir->father = curPos;
	int alloc = inodeTable.AllocINode();
	if (alloc < 0) {
		cout << "No more Space To Allocate!" << endl;
		exit(0);
	}
	//inodeTable.allINode[alloc].abstractFather = dir;
	dir->inode = &inodeTable.allINode[alloc];
	this->curPos->child.push_back(dir);
	cout << "mkdir " << dirname << " success" << endl;
}

void FileManager::fformat() {
	if (!check())
		cout << "FileManager init failed!" << endl;
	this->buff->fformat();
}

bool FileManager::fcreat(string filename)
{
	File* newFile = new File(filename);
	int inodeNumber = this->inodeTable.AllocINode();
	if (inodeNumber == -1) {
		return false;
	}
	newFile->inode = &this->inodeTable.allINode[inodeNumber];
	newFile->isDir = false;
	this->curPos->child.push_back(newFile);
	newFile->father = this->curPos;
	cout << "create file " << filename << " success!" << endl;
	return true;
}

int FileManager::fopen(string filename, int flag) {
	for (int i = 0; i < curPos->child.size(); i++) {
		if (curPos->child.at(i)->name == filename && !(curPos->child.at(i)->isDir)) {
			curPos->child.at(i)->inode->IOFlag = flag;
			return curPos->child.at(i)->inode->No;
		}
	}
	return -1;
}

bool FileManager::fclose(int fd)
{
	return false;
}

void FileManager::fread(int fd, int offset, int len) {
	this->inodeTable.allINode[fd].read(offset, len);
}

bool FileManager::fwrite(int fd, int offset, const char * writeBuf, int len) {
	return this->inodeTable.allINode[fd].write(offset, len, writeBuf);
}

bool FileManager::fdelete(string filename) {
	/*查询是不是有这个文件*/
	for (vector<AbstractFile*>::iterator it = curPos->child.begin(); it != curPos->child.end();) {
		if ((*it)->name == filename && !(*it)->isDir) {
			(*it)->inode->erase();
			it = curPos->child.erase(it);
			//File* temp = (File*)(*it);
			//delete temp;
			return true;
		}
	}
	return false;
}



INodeTable::INodeTable() {
	for (int i = 0; i < MAX_INODE_NUM; i++) {
		allINode[i].No = i;
	}
}

INodeTable::~INodeTable() {

}

/**
*返回
*/
int INodeTable::AllocINode() {
	//找到第一个还没有被分配的INode
	for (int i = 0; i < INodeTable::MAX_INODE_NUM; i++) {
		if (!allINode[i].isAllocated) {
			allINode[i].isAllocated = true;
			return i;
		}
	}
	return -1;
	
}

//void FileManager::fformat() {
//	if (initialize) {
//
//	}
//	else {
//		/*创建文件*/
//
//	}
//}
