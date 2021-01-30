#include <cstring>

#include "ZLFile.h"
#include "ZLOutputStream.h"
#include "ZLFSManager.h"
#include <ZLStringUtil.h>
ZLFile::ZLFile() : myMimeTypeIsUpToDate(true), myInfoIsFilled(true) {
}

ZLFile::ZLFile(const std::string &path, const std::string &mimeType) : myPath(path), myMimeType(mimeType), myMimeTypeIsUpToDate(!mimeType.empty()), myInfoIsFilled(false) {
	ZLFSManager::Instance().normalize(myPath);
	{
		std::size_t index = ZLFSManager::Instance().findLastFileNameDelimiter(myPath);
		if (index < myPath.length() - 1) {
			myNameWithExtension = myPath.substr(index + 1);
		} else {
			myNameWithExtension = myPath;
		}
	}
	myNameWithoutExtension = myNameWithExtension;

    std::string lowerCaseName = ZLStringUtil::toLower(myNameWithoutExtension);
	int index = myNameWithoutExtension.rfind('.');
	if (index > 0) {
        std::string extension = myNameWithoutExtension.substr(index + 1);
		myExtension = ZLStringUtil::toLower(extension);
		myNameWithoutExtension = myNameWithoutExtension.substr(0, index);
	}
}

shared_ptr<ZLInputStream> ZLFile::inputStream() const {
	shared_ptr<ZLInputStream> stream;

	int index = ZLFSManager::Instance().findArchiveFileNameDelimiter(myPath);
	if (index == -1) {
		if (isDirectory()) {
			return 0;
		}
		stream = ZLFSManager::Instance().createPlainInputStream(myPath);
	} else {
		const std::string baseName = myPath.substr(0, index);
		const ZLFile baseFile(baseName);
		shared_ptr<ZLInputStream> base = baseFile.inputStream();
		if (!base.isNull()) {
            if (isDirectory()) {
                return 0;
            }
            stream = ZLFSManager::Instance().createPlainInputStream(myPath);
		}
	}
	return stream;
}

shared_ptr<ZLOutputStream> ZLFile::outputStream() const {
	if (ZLFSManager::Instance().findArchiveFileNameDelimiter(myPath) != -1) {
		return 0;
	}
	return ZLFSManager::Instance().createOutputStream(myPath);
}

shared_ptr<ZLFile> ZLFile::createDirectory(bool createUnExisting) const {
	if (isExists()) {
		if (isDirectory()) {
			return createPlainDirectory(myPath);
		}
	} else if (createUnExisting) {
		myInfoIsFilled = false;
		return createNewDirectory(myPath);
	}
	return 0;
}

void ZLFile::fillInfo() const {
	myInfoIsFilled = true;
    myInfo = ZLFSManager::Instance().fileInfo(myPath);
}

bool ZLFile::remove() const {
	if (ZLFSManager::Instance().removeFile(myPath)) {
		myInfoIsFilled = false;
		return true;
	} else {
		return false;
	}
}

std::string ZLFile::delimiter() const{
    return ZLFSManager::Instance().delimiter();
}

void ZLFile::fileList(std::vector<shared_ptr<ZLFile>> &names,bool includeSymlinks) {

    ZLFSManager::Instance().fileList(names,myPath,includeSymlinks);
}

std::string ZLFile::physicalFilePath() const {
	std::string path = myPath;
	int index;
	const ZLFSManager &manager = ZLFSManager::Instance();
	while ((index = manager.findArchiveFileNameDelimiter(path)) != -1) {
		path = path.substr(0, index);
	}
	return path;
}

std::string ZLFile::resolvedPath() const {
	std::string physical = physicalFilePath();
	std::string postfix = myPath.substr(physical.length());
	return ZLFSManager::Instance().resolveSymlink(physical) + postfix;
}

bool ZLFile::isExists() const {
	if (!myInfoIsFilled) {
		fillInfo();
	}
	return myInfo.Exists;
}

std::size_t ZLFile::getFileSize() const {
	if (!myInfoIsFilled) {
		fillInfo();
	}
	return myInfo.Size;
}

std::size_t ZLFile::getLastModified() const {
	if (!myInfoIsFilled) {
		fillInfo();
	}
	return myInfo.MTime;
}

bool ZLFile::isDirectory() const {
	if (!myInfoIsFilled) {
		fillInfo();
	}
	return myInfo.IsDirectory;
}

bool ZLFile::isFile() const {
    if (!myInfoIsFilled) {
        fillInfo();
    }
    return !myInfo.IsDirectory;
}

bool ZLFile::canRemove() const {
	return ZLFSManager::Instance().canRemoveFile(path());
}

std::string ZLFile::replaceIllegalCharacters(const std::string &fileName, char replaceWith) {
	static const char charsToReplace[] = ":;<|>+\\/\"*?";
	const std::size_t len = fileName.length();
	char *data = new char[len];
	std::memcpy(data, fileName.data(), len);
	char *end = data + len;
	for (char *ptr = data; ptr != end; ++ptr) {
		if (strchr(charsToReplace, *ptr) != 0) {
			*ptr = replaceWith;
		}
	}
	std::string result(data, len);
	delete[] data;
	return result;
}

ZLFile * ZLFile::createPlainDirectory(const std::string &path) const{
    return ZLFSManager::Instance().createPlainDirectory(path);
}

ZLFile * ZLFile::createNewDirectory(const std::string &path) const{

}