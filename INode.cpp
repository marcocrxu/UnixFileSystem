#include "INode.h"


SecondAddrMap::SecondAddrMap() {

}

SecondAddrMap::~SecondAddrMap() {

}

AddrMap::AddrMap() {

}

AddrMap::~AddrMap() {

}

Addr::Addr() {
	phyBlkNo = NOT_ALLOC;
}

Addr::~Addr() {

}

INode::INode() {
	//this->abstractFather = NULL;
	isAllocated = false;
	size = 0;
	No = 0;
	buff = NULL;
}

////INode::INode(AbstractFile * file) {
//	this->abstractFather = file;
//	isAllocated = false;
//	size = 0;
//}

INode::~INode()
{
}

void INode::setSize(int size) {
	this->size = size;
}

bool INode::write(int off, int len, const char * writeBuf) {
	int phyBlkNo = 0;
	int offset = 0;
	if ((this->IOFlag & INode::WRITEFlag) == 0) {
		cout << "You Are Not Permitted To Write This File" << endl;
		return false;
	}
	/*每一个物理块设置1KB内存大小*/
	int BlockNo = off / 1024;
	phyBlkNo = getPhyBlkNo(BlockNo);
	offset = off % 1024;
	if (offset + len <= 1024) {
		buff->write(phyBlkNo, offset, len, writeBuf);
	}
	else {
		/*分段写入*/
		buff->write(phyBlkNo, offset, 1024 - offset, writeBuf);
		char* temp = (char*)writeBuf + 1024 - offset;
		len = len - (1024 - offset);
		while (len > 1024) {
			BlockNo++;
			phyBlkNo = buff->Alloc();
			buff->write(phyBlkNo, 0, 1024, temp);
			temp += 1024;
			len -= 1024;
		}
		BlockNo++;
		phyBlkNo = buff->Alloc();
		buff->write(phyBlkNo, 0, len, temp);
	}
	return true;
}

void INode::read(int off, int len) {
	if ((this->IOFlag & INode::READFlag) == 0) {
		cout << "You Are Not Permitted To Read This File" << endl;
		return;
	}
	if (len > 1024) {
		cout << "Can Not Read More Than 1024 bytes One Time" << endl;
		return;
	}
	int BlkNo = off / 1024;
	int offset = off % 1024;
	bool flag = HaveThatBlk(BlkNo);
	if (!flag) {
		cout << "That Block is Not writted yet" << endl;
		return;
	}
	int phyBlkNo = getPhyBlkNo(BlkNo);
	if (len + offset < 1024) {
		buff->read(phyBlkNo, offset, len);
	}
	else {
		buff->read(phyBlkNo, offset, 1024 - offset);
		len = len - (1024 - offset);
		while (len > 1024) {
			flag = HaveThatBlk(++BlkNo);
			if (!flag) {
				cout << "That Block is Not writted yet" << endl;
				return;
			}
			phyBlkNo = getPhyBlkNo(BlkNo);
			buff->read(phyBlkNo, 0, 1024);
			len -= 1024;
		}
		flag = HaveThatBlk(++BlkNo);
		if (!flag) {
			cout << "That Block is Not writted yet" << endl;
			return;
		}
		phyBlkNo = getPhyBlkNo(BlkNo);
		buff->read(phyBlkNo, 0, len);
		cout << endl;
	}
}

void INode::erase() {
	/*首先释放所有的addr*/
	for (int i = 0; i < HugeFileBlkNum; i++) {
		for (int j = 0; j < BLKSize; j++) {
			for (int k = 0; k < BLKSize; k++) {
				if (this->HugeFile[i].secondMap[j].firstMap[k].phyBlkNo != Addr::NOT_ALLOC) {
					int phyBlkNo = this->HugeFile[i].secondMap[j].firstMap[k].phyBlkNo;
					this->HugeFile[i].secondMap[j].firstMap[k].phyBlkNo = Addr::NOT_ALLOC;
					buff->clear(phyBlkNo);
				}
			}
		}
	}

	for (int i = 0; i < BigFileBlkNum; i++) {
		for (int j = 0; j < BLKSize; j++) {
			if (this->BigFile[i].firstMap[j].phyBlkNo != Addr::NOT_ALLOC) {
				int phyBlkNo = this->BigFile[i].firstMap[j].phyBlkNo;
				this->BigFile[i].firstMap[j].phyBlkNo = Addr::NOT_ALLOC;
				buff->clear(phyBlkNo);
			}
		}
	}

	for (int i = 0; i < SmallFileBlkNum; i++) {
		if (SmallFile[i].phyBlkNo != Addr::NOT_ALLOC) {
			int phyBlkNo = this->SmallFile[i].phyBlkNo;
			this->SmallFile[i].phyBlkNo = Addr::NOT_ALLOC;
			buff->clear(phyBlkNo);
		}
	}

	this->size = 0;
	this->isAllocated = false;
	this->IOFlag = 0;

}

int INode::getPhyBlkNo(int BlockNo)
{
	int phyBlkNo;
	if (BlockNo < 6) {
		phyBlkNo = this->SmallFile[BlockNo].phyBlkNo;
		if (phyBlkNo == Addr::NOT_ALLOC) {
			int x = buff->Alloc();
			this->SmallFile[BlockNo].phyBlkNo = x;
			if (x == -1) {
				cout << "Do Not Have Enough Space" << endl;
				exit(0);
			}
		}
		phyBlkNo = this->SmallFile[BlockNo].phyBlkNo;
	}
	else if (BlockNo < 128 * 2 + 6) {
		int i = (BlockNo - 6) / 128;
		phyBlkNo = this->BigFile[i].firstMap[(BlockNo - 6) % 128].phyBlkNo;
		if (phyBlkNo == Addr::NOT_ALLOC) {
			int x = buff->Alloc();
			this->BigFile[i].firstMap[(BlockNo - 6) % 128].phyBlkNo = x;
			if (x == -1) {
				cout << "Do Not Have Enough Space" << endl;
				exit(0);
			}
		}
		phyBlkNo = this->BigFile[i].firstMap[(BlockNo - 6) % 128].phyBlkNo;
	}
	else {
		int i = (BlockNo - 6 - 2 * 128) / (128 * 128);
		int j = (BlockNo - 6 - 2 * 128) % (128 * 128);
		phyBlkNo = this->HugeFile[i].secondMap[j].firstMap[j % 128].phyBlkNo;
		if (phyBlkNo == Addr::NOT_ALLOC) {
			int x = buff->Alloc();
			this->HugeFile[i].secondMap[j].firstMap[j % 128].phyBlkNo = x;
			if (x == -1) {
				cout << "Do Not Have Enough Space" << endl;
				exit(0);
			}
		}
		phyBlkNo = this->HugeFile[i].secondMap[j].firstMap[j % 128].phyBlkNo;
	}
	return phyBlkNo;
}

bool INode::HaveThatBlk(int BlockNo) {
	int phyBlkNo;
	if (BlockNo < 6) {
		phyBlkNo = this->SmallFile[BlockNo].phyBlkNo;
		if (phyBlkNo == Addr::NOT_ALLOC) {
			return false;
		}
	}
	else if (BlockNo < 128 * 2 + 6) {
		int i = (BlockNo - 6) / 128;
		phyBlkNo = this->BigFile[i].firstMap[(BlockNo - 6) % 128].phyBlkNo;
		if (phyBlkNo == Addr::NOT_ALLOC) {
			return false;
		}
	}
	else {
		int i = (BlockNo - 6 - 2 * 128) / (128 * 128);
		int j = (BlockNo - 6 - 2 * 128) % (128 * 128);
		phyBlkNo = this->HugeFile[i].secondMap[j].firstMap[j % 128].phyBlkNo;
		if (phyBlkNo == Addr::NOT_ALLOC) {
			return false;
		}
	}
	return true;
}
