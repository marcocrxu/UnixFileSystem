#include "Buff.h"

Buff::Buff() {
	this->stream.open(DISKNAME, fstream::out);
	if (this->stream.bad()) {
		cout << "Open File Failed" << endl;
	}
	stream << "test" << endl;
	this->stream.close();
	this->stream.open(DISKNAME, ios::in | ios::out);
	memset(UsedFlag, 0, BLKSIZE);
}

Buff::~Buff() {

}

int Buff::Alloc() {
	for (int i = 0; i < BLKSIZE; i++) {
		if (!UsedFlag[i]) {
			UsedFlag[i] = true;
			return i;
		}
	}
	return -1;
}

void Buff::fformat() {
	if (!check()) 
		return;
	stream.seekp(0);
	char *ZEROBuff=new char[BUFFSIZE];
	memset(ZEROBuff, 0, BUFFSIZE);
	for (int i = 0; i < SIZE; i++) {
		stream.write(ZEROBuff, BUFFSIZE);
	}
	stream.close();
	stream.open(DISKNAME, ios::in | ios::out);
	check();
}

bool Buff::check() {
	if (this->stream.bad()) {
		cout << "CanNot Open File " << DISKNAME << "!" << endl;
		return false;
	}
	return true;
}

void Buff::clear(int phyBlkNo) {
	/*向其中写入空白*/
	if (!check()) {
		return;
	}
	stream.seekp(phyBlkNo * 1024);
	char clearChar[1024];
	memset(clearChar, 0, 1024);
	stream.write(clearChar, 1024);
	this->UsedFlag[phyBlkNo] = false;
}

void Buff::read(int phyBlkNo, int off, int len) {
	if (!check())
		return;
	stream.seekg(phyBlkNo * 1024 + off, ios::beg);
	char *readBuf = new char[len + 1];
	readBuf[len] = '\0';
	stream.read(readBuf, len);
	cout << readBuf;
	return;
}

bool Buff::write(int phyBlkNo, int off, int len, const char * writeBuf) {
	if (!check())
		return false;
	if (writeBuf == NULL || writeBuf == nullptr)
		return false;
	stream.seekp(phyBlkNo * 1024 + off, ios::beg);
	if (len + off <= 1024) {
		stream.write(writeBuf, len);
		stream.close();
		stream.open(DISKNAME, ios::in | ios::out);
		return true;
	}
	else 
		return false;
}


