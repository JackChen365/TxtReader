#ifndef __ZLUNIXFILEOUTPUTSTREAM_H__
#define __ZLUNIXFILEOUTPUTSTREAM_H__

#include <stdio.h>

#include "ZLOutputStream.h"

class ZLUnixFileOutputStream : public ZLOutputStream {

public:
	ZLUnixFileOutputStream(const std::string &name);
	~ZLUnixFileOutputStream();
	bool open(bool append = true);
	void write(const char *data, std::size_t len);
	void write(const std::string &str);
	void close();

private:
	std::string myName;
	std::string myTemporaryName;
	bool myHasErrors;
	FILE *myFile;
};

#endif /* __ZLUNIXFILEOUTPUTSTREAM_H__ */
