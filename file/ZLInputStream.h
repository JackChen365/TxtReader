#ifndef __ZLINPUTSTREAM_H__
#define __ZLINPUTSTREAM_H__

#include <string>

#include <shared_ptr.h>

class ZLInputStream {

protected:
	ZLInputStream();

public:
	virtual ~ZLInputStream();
	virtual bool open() = 0;
	virtual std::size_t read(char *buffer, std::size_t maxSize) = 0;
	virtual void close() = 0;

	virtual void seek(int offset, bool absoluteOffset) = 0;
	virtual std::size_t offset() const = 0;
	virtual std::size_t sizeOfOpened() = 0;

private:
	// disable copying
	ZLInputStream(const ZLInputStream&);
	const ZLInputStream &operator = (const ZLInputStream&);
};


inline ZLInputStream::ZLInputStream() {}
inline ZLInputStream::~ZLInputStream() {}

#endif /* __ZLINPUTSTREAM_H__ */
