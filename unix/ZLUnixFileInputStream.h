#ifndef __ZLUNIXFILEINPUTSTREAM_H__
#define __ZLUNIXFILEINPUTSTREAM_H__

#include <stdio.h>

#include "ZLInputStream.h"

class ZLUnixFileInputStream : public ZLInputStream {

public:
	ZLUnixFileInputStream(const std::string &name);
	~ZLUnixFileInputStream();
	bool open();
	std::size_t read(char *buffer, std::size_t maxSize);
	void close();

	void seek(int offset, bool absoluteOffset);
	std::size_t offset() const;
	std::size_t sizeOfOpened();

private:
	std::string myName;
	FILE *myFile;
	bool myNeedRepositionToStart;
};

#endif /* __ZLUNIXFILEINPUTSTREAM_H__ */
