#include "File.h"

File::File(string name) {
	this->father = NULL;
	this->isDir = false;
	this->name = name;
}

File::~File() {

}
