#ifndef __ZLFILEUTIL_H__
#define __ZLFILEUTIL_H__

#include <string>

class ZLFileUtil {

public:
	static std::string normalizeUnixPath(const std::string &path);

private:
	ZLFileUtil();
};

#endif /* __ZLFILEUTIL_H__ */
