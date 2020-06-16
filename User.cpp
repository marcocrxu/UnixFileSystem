#include "User.h"



User::User(string name) {
	this->username = name;
	this->curDirPath = ROOT_DIR_NAME;
	fileManager = new FileManager();
	//fileManager->user = this;
	this->curPos = fileManager->root;
	this->parentPos = NULL;
}

User::~User()
{
}

void User::Ls() {
	for (int i = 0; i < curPos->child.size(); i++) {
		cout << curPos->child.at(i)->name << endl;
	}
}

void User::pwd() {
	cout << curDirPath << endl;
}

void User::mkdir(string name) {
	if (name == "")
		return;
	/*先检查有没有同名*/
	for (int i = 0; i < this->curPos->child.size(); i++) {
		if (this->curPos->child.at(i)->name == name) {
			cout << "Current Dir include same name file:" << name << endl;
			return;
		}
	}
	int inodeNumber = this->fileManager->inodeTable.AllocINode();
	if (inodeNumber == -1) {
		cout << "out of INODE!" << endl;
	}
	Dir* newDir = new Dir(name);
	newDir->inode = &this->fileManager->inodeTable.allINode[inodeNumber];
	newDir->father = this->curPos;
	//this->fileManager->inodeTable.allINode[inodeNumber].abstractFather = newDir;
	/*把创建好的Dir指针放到当前父目录的child里面*/
	this->curPos->child.push_back(newDir);
}

void User::cd(string dirname) {
	if (dirname[0] != '/') {
		if (dirname[dirname.length() - 1] == '/') {
			dirname = dirname.substr(0, dirname.length() - 1);
		}
		/*相对路径*/
		int pos = 0;
		while (pos != -1) {
			pos = dirname.find('/');
			
			string curJump = dirname.substr(0, pos);
			dirname = dirname.substr(pos + 1, dirname.length());
			if (curJump == "..") {
				curPos = curPos->father;
				this->fileManager->curPos = curPos;
				curDirPath = curDirPath.substr(0, curDirPath.find_last_of('/'));
				curDirPath = curDirPath.substr(0, curDirPath.find_last_of('/') + 1);
				if (curDirPath == "") {
					curDirPath = "/";
				}
			}
			else if (curJump == ".") {
				continue;
			}
			else {
				/*查找当前目录下有没有这个文件夹*/
				bool tag = false;
				for (int i = 0; i < curPos->child.size(); i++) {
					//如果存在这个文件夹，并且试一个文件夹
					if (curPos->child.at(i)->name == curJump && curPos->child.at(i)->isDir) {
						curPos = (Dir*)curPos->child.at(i);
						this->fileManager->curPos = curPos;
						curDirPath = curDirPath.append(curPos->name).append("/");
						tag = true;
						break;
					}
				}
				if (!tag) {
					cout << "Directory Not Exist" << endl;
					return;
				}

			}

			if (pos == -1) {
				break;
			}

		}
	}
	else {
		if (dirname[dirname.length() - 1] == '/'&&dirname.length() != 1) {
			dirname = dirname.substr(0, dirname.length() - 1);
		}
		if (dirname[dirname.length() - 1] == '/'&&dirname.length() == 1) {
			curPos = this->fileManager->root;
			curDirPath = "/";
			return;
		}
		/*绝对路径*/
		Dir* temp = this->fileManager->root;
		string tempDirName = dirname;
		//string tempCurDirPath = "/";
		dirname = dirname.substr(1, dirname.length() - 1);
		int pos = 0;
		while (pos != -1) {
			pos = dirname.find_first_of('/');
			string path = dirname.substr(0, pos == -1 ? dirname.length() : pos);
			dirname = dirname.substr(pos + 1, dirname.length());
			bool find = false;
			for (int i = 0; i < temp->child.size(); i++) {
				if (temp->child.at(i)->name == path && temp->child.at(i)->isDir) {
					temp = (Dir*)temp->child.at(i);
					find = true;
					break;
				}
			}
			if (!find) {
				cout << "No Dir Named " << tempDirName << endl;
				return;
			}
		}
		this->curPos = temp;
		curDirPath = tempDirName;
	}
}

void User::fcreat(string filename) {
	/*查询当前目录下面有没有同名文件*/
	bool SameNameFileExist = this->curPos->HaveSameName(filename);
	if (SameNameFileExist) {
		cout << "There's already a file/dir named " << filename << " exist!" << endl;
	}
	bool createFile = fileManager->fcreat(filename);
	if (!createFile) {
		cout << "create File " << filename << " failed" << endl;
	}
}

void User::fopen(string filename, int flag) {
	int No = this->fileManager->fopen(filename, flag);
	if (No == -1) {
		cout << "No File named " << filename << endl;
	}
	else {
		cout << "File Fd is " << No << endl;
	}
}

void User::fformat() {
	this->fileManager->fformat();
	cout << "format successfully" << endl;
}

void User::fread(int fd, int off, int len) {
	this->fileManager->fread(fd, off, len);
}

void User::fwrite(int fd, int off, const char * c, int len) {
	bool flag = this->fileManager->fwrite(fd, off, c, len);
	if (!flag) {
		cout << "write failed" << endl;
	}
	else {
		cout << "write Success:" << c << endl;
	}
}

void User::fdelete(string filename) {
	/*删除文件*/
	if (!fileManager->fdelete(filename)) {
		cout << "delete File " << filename << " failed" << endl;
	}
	else {
		cout << "delete File " << filename << " success" << endl;
	}
}

void User::fclose(int fd) {
	this->fileManager->inodeTable.allINode[fd].IOFlag = 0;
}
