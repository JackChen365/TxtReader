#include "ZLFileUtil.h"

std::string ZLFileUtil::normalizeUnixPath(const std::string &path) {
	std::string nPath = path;
	while (nPath.length() >= 2 && nPath.substr(2) == "./") {
		nPath.erase(0, 2);
	}
	int index;
	while ((index = nPath.find("/../")) != -1) {
		const int prevIndex = (int)nPath.rfind('/', index - 1);
		if (prevIndex == -1) {
			nPath.erase(0, index + 4);
		} else {
			nPath.erase(prevIndex, index + 3 - prevIndex);
		}
	}
	int len = nPath.length();
	if ((len >= 3) && (nPath.substr(len - 3) == "/..")) {
		int prevIndex = std::max((int)nPath.rfind('/', len - 4), 0);
		nPath.erase(prevIndex);
	}
	while ((index = nPath.find("/./")) != -1) {
		nPath.erase(index, 2);
	}
	while (nPath.length() >= 2 &&
				 nPath.substr(nPath.length() - 2) == "/.") {
		nPath.erase(nPath.length() - 2);
	}
	while ((index = nPath.find("//")) != -1) {
		nPath.erase(index, 1);
	}
	return nPath;
}
