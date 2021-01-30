#ifndef __ZLOUTPUTSTREAM_H__
#define __ZLOUTPUTSTREAM_H__

#include <string>

class ZLOutputStream {

protected:
	ZLOutputStream();

public:
	virtual ~ZLOutputStream();
	virtual bool open(bool append=true) = 0;
	virtual void write(const char *data, std::size_t len) = 0;
	virtual void write(const std::string &str) = 0;
	virtual void close() = 0;

private:
	ZLOutputStream(const ZLOutputStream&);
	const ZLOutputStream &operator = (const ZLOutputStream&);
};

inline ZLOutputStream::ZLOutputStream() {}
inline ZLOutputStream::~ZLOutputStream() {}

#endif /* __ZLOUTPUTSTREAM_H__ */
