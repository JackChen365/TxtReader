#ifndef __ZLSTRINGUTIL_H__
#define __ZLSTRINGUTIL_H__

#include <vector>
#include <string>

class ZLStringUtil {

private:
	ZLStringUtil();

public:
	static bool stringStartsWith(const std::string &str, const std::string &start);
	static bool stringEndsWith(const std::string &str, const std::string &end);

	static void appendNumber(std::string &str, unsigned int n);
	static void append(std::string &str, const std::vector<std::string> &buffer);

	static std::string toLower(std::string &str);
	static std::string toUpper(std::string &str);
};

#endif /* __ZLSTRINGUTIL_H__ */
