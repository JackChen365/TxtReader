#include "ZLUnixFileInputStream.h"

ZLUnixFileInputStream::ZLUnixFileInputStream(const std::string &name) : myName(name), myNeedRepositionToStart(false) {
	myFile = 0;
}

ZLUnixFileInputStream::~ZLUnixFileInputStream() {
	close();
}

bool ZLUnixFileInputStream::open() {
	if (myFile == 0) {
		myFile = fopen(myName.c_str(), "rb");
	} else {
		//fseek(myFile, 0, SEEK_SET);
		myNeedRepositionToStart = true;
	}
	return myFile != 0;
}

std::size_t ZLUnixFileInputStream::read(char *buffer, std::size_t maxSize) {
	if (buffer != 0) {
		if (myNeedRepositionToStart) {
			fseek(myFile, 0, SEEK_SET);
			myNeedRepositionToStart = false;
		}
		return fread(buffer, 1, maxSize, myFile);
	} else {
		if (myNeedRepositionToStart) {
			fseek(myFile, maxSize, SEEK_SET);
			myNeedRepositionToStart = false;
			return ftell(myFile);
		} else {
			int pos = ftell(myFile);
			fseek(myFile, maxSize, SEEK_CUR);
			return ftell(myFile) - pos;
		}
	}
}

void ZLUnixFileInputStream::close() {
	if (myFile != 0) {
		fclose(myFile);
		myFile = 0;
	}
}

std::size_t ZLUnixFileInputStream::sizeOfOpened() {
	if (myFile == 0) {
		return 0;
	}
	long pos = ftell(myFile);
	fseek(myFile, 0, SEEK_END);
	long size = ftell(myFile);
	fseek(myFile, pos, SEEK_SET);
	return size;
}

void ZLUnixFileInputStream::seek(int offset, bool absoluteOffset) {
	if (myNeedRepositionToStart) {
		absoluteOffset = true;
		myNeedRepositionToStart = false;
	}
	fseek(myFile, offset, absoluteOffset ? SEEK_SET : SEEK_CUR);
}

std::size_t ZLUnixFileInputStream::offset() const {
	return myFile == 0 || myNeedRepositionToStart ? 0 : ftell(myFile);
}
