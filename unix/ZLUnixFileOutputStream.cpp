#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "ZLUnixFileOutputStream.h"

ZLUnixFileOutputStream::ZLUnixFileOutputStream(const std::string &name) : myName(name), myHasErrors(false), myFile(0) {
}

ZLUnixFileOutputStream::~ZLUnixFileOutputStream() {
	close();
}

bool ZLUnixFileOutputStream::open(bool append) {
	close();
	myTemporaryName = myName + ".XXXXXX" + '\0';
	mode_t currentMask = umask(S_IRWXO | S_IRWXG);
	int temporaryFileDescriptor = ::mkstemp(const_cast<char*>(myTemporaryName.data()));
	umask(currentMask);
	if (temporaryFileDescriptor == -1) {
		return false;
	}
    if(append) {
        myFile = fdopen(temporaryFileDescriptor, "a");
    } else {
        myFile = fdopen(temporaryFileDescriptor, "w+");
    }
	return myFile != 0;
}

void ZLUnixFileOutputStream::write(const char *data, std::size_t len) {
	if (::fwrite(data, 1, len, myFile) != len) {
		myHasErrors = true;
	}
}

void ZLUnixFileOutputStream::write(const std::string &str) {
	if (::fwrite(str.data(), 1, str.length(), myFile) != (std::size_t)str.length()) {
		myHasErrors = true;
	}
}

void ZLUnixFileOutputStream::close() {
	if (myFile != 0) {
		::fclose(myFile);
		myFile = 0;
		if (!myHasErrors) {
			rename(myTemporaryName.c_str(), myName.c_str());
		}
	}
}
