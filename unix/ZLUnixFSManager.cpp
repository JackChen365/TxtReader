#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <set>

#include "ZLFSManager.h"
#include "ZLUnixFSManager.h"
#include "ZLUnixFileInputStream.h"
#include "ZLUnixFileOutputStream.h"

static std::string getPwdDir() {
	char *pwd = getenv("PWD");
	return (pwd != 0) ? pwd : "";
}

static std::string getHomeDir() {
	char *home = getenv("HOME");
	return (home != 0) ? home : "";
}

ZLFileInfo ZLUnixFSManager::fileInfo(const std::string &path) const {
	ZLFileInfo info;
	struct stat fileStat;
	info.Exists = stat(path.c_str(), &fileStat) == 0;
	if (info.Exists) {
		info.Size = fileStat.st_size;
		info.MTime = fileStat.st_mtime;
		info.IsDirectory = S_ISDIR(fileStat.st_mode);
	}
	return info;
}

void ZLUnixFSManager::fileList(std::vector<shared_ptr<ZLFile>> &names,std::string &path, bool includeSymlinks){
    std::string realPath(path);
    normalizeRealPath(realPath);
    printf("realPath:%s" , realPath.c_str());
    DIR *dir = opendir(realPath.c_str());
    if (dir != 0) {
        const std::string namePrefix = realPath + delimiter();
        const dirent *file;
        struct stat fileInfo;
        std::string shortName;
        while ((file = readdir(dir)) != 0) {
            shortName = file->d_name;
            if ((shortName == ".") || (shortName == "..")) {
                continue;
            }
            const std::string realPath = namePrefix + shortName;
            if (includeSymlinks) {
                stat(realPath.c_str(), &fileInfo);
            } else {
                lstat(realPath.c_str(), &fileInfo);
            }
            if (S_ISREG(fileInfo.st_mode)) {
                names.push_back(new ZLFile(shortName));
            }
        }
        closedir(dir);
    }
}

std::string ZLUnixFSManager::resolveSymlink(const std::string &path) const {
	std::set<std::string> names;
	std::string current = path;
	for (int i = 0; i < 256; ++i) {
		names.insert(current);

		std::string buffer(2048, '\0');
		int len = readlink(current.c_str(), (char*)buffer.data(), 2048);
		if ((len == 2048) || (len <= 0)) {
			return current;
		}
		buffer.erase(len);
		if (buffer[0] != '/') {
			buffer = parentPath(current) + '/' + buffer;
		}
		normalizeRealPath(buffer);
		if (names.find(buffer) != names.end()) {
			return buffer;
		}
		current = buffer;
	}
	return "";
}

void ZLUnixFSManager::normalizeRealPath(std::string &path) const {
	static std::string HomeDir = getHomeDir();
	static std::string PwdDir = getPwdDir();

	if (path.empty()) {
		path = PwdDir;
	} else if (path[0] == '~') {
		if ((path.length() == 1) || (path[1] == '/')) {
			path = HomeDir + path.substr(1);
		}
	} else if (path[0] != '/') {
		path = PwdDir + '/' + path;
	}
	int last = path.length() - 1;
	while ((last > 0) && (path[last] == '/')) {
		--last;
	}
	if (last < (int)path.length() - 1) {
		path = path.substr(0, last + 1);
	}

	int index;
	while ((index = path.find("/../")) != -1) {
		int prevIndex = std::max((int)path.rfind('/', index - 1), 0);
		path.erase(prevIndex, index + 3 - prevIndex);
	}
	int len = path.length();
	if ((len >= 3) && (path.substr(len - 3) == "/..")) {
		int prevIndex = std::max((int)path.rfind('/', len - 4), 0);
		path.erase(prevIndex);
	}
	while ((index = path.find("/./")) != -1) {
		path.erase(index, 2);
	}
	while (path.length() >= 2 &&
				 path.substr(path.length() - 2) == "/.") {
		path.erase(path.length() - 2);
	}
	while ((index = path.find("//")) != -1) {
		path.erase(index, 1);
	}
}

ZLFile *ZLUnixFSManager::createNewDirectory(const std::string &path) const {
	std::vector<std::string> subpaths;
	std::string current = path;

	while (current.length() > 1) {
		struct stat fileStat;
		if (stat(current.c_str(), &fileStat) == 0) {
			if (!S_ISDIR(fileStat.st_mode)) {
				return 0;
			}
			break;
		} else {
			subpaths.push_back(current);
			int index = current.rfind('/');
			if (index == -1) {
				return 0;
			}
			current.erase(index);
		}
	}

	for (int i = subpaths.size() - 1; i >= 0; --i) {
		if (mkdir(subpaths[i].c_str(), 0x1FF) != 0) {
			return 0;
		}
	}
	return createPlainDirectory(path);
}

ZLFile *ZLUnixFSManager::createPlainDirectory(const std::string &path) const {
	return new ZLFile(path);
}

ZLInputStream *ZLUnixFSManager::createPlainInputStream(const std::string &path) const {
	return new ZLUnixFileInputStream(path);
}

ZLOutputStream *ZLUnixFSManager::createOutputStream(const std::string &path) const {
	return new ZLUnixFileOutputStream(path);
}

bool ZLUnixFSManager::removeFile(const std::string &path) const {
	return unlink(path.c_str()) == 0;
}

int ZLUnixFSManager::findArchiveFileNameDelimiter(const std::string &path) const {
	return path.rfind(':');
}

static const std::string RootPath = "/";

shared_ptr<ZLFile> ZLUnixFSManager::rootDirectory() const {
	return createPlainDirectory(RootPath);
}

const std::string &ZLUnixFSManager::rootDirectoryPath() const {
	return RootPath;
}

std::string ZLUnixFSManager::parentPath(const std::string &path) const {
	if (path == RootPath) {
		return path;
	}
	int index = findLastFileNameDelimiter(path);
	return (index <= 0) ? RootPath : path.substr(0, index);
}

bool ZLUnixFSManager::canRemoveFile(const std::string &path) const {
	return access(parentPath(path).c_str(), W_OK) == 0;
}

