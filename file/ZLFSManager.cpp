#include <ZLFileUtil.h>
#include "ZLFSManager.h"

ZLFSManager *ZLFSManager::ourInstance = 0;

void ZLFSManager::deleteInstance() {
	if (ourInstance != 0) {
		delete ourInstance;
		ourInstance = 0;
	}
}

std::string ZLFSManager::delimiter() const{
    return "/";
}

int ZLFSManager::findLastFileNameDelimiter(const std::string &path) const {
	int index = findArchiveFileNameDelimiter(path);
	if (index == -1) {
		index = path.rfind(ZLFSManager::delimiter());
	}
	return index;
}

void ZLFSManager::normalize(std::string &path) const {
	int index = findArchiveFileNameDelimiter(path);
	if (index == -1) {
		normalizeRealPath(path);
	} else {
		std::string realPath = path.substr(0, index);
		normalizeRealPath(realPath);
		path = realPath + ':' + ZLFileUtil::normalizeUnixPath(path.substr(index + 1));
	}
}
