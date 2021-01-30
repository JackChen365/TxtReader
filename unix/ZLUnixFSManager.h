#ifndef __ZLUNIXFSMANAGER_H__
#define __ZLUNIXFSMANAGER_H__

#include "ZLFSManager.h"

class ZLUnixFSManager : public ZLFSManager {
public:
    static void createInstance();
protected:
	void normalizeRealPath(std::string &path) const;

protected:
	std::string resolveSymlink(const std::string &path) const;
	ZLFile *createNewDirectory(const std::string &path) const;
	ZLFile *createPlainDirectory(const std::string &path) const;
	ZLInputStream *createPlainInputStream(const std::string &path) const;
	ZLOutputStream *createOutputStream(const std::string &path) const;
	bool removeFile(const std::string &path) const;

	ZLFileInfo fileInfo(const std::string &path) const;
    void fileList(std::vector<shared_ptr<ZLFile>> &names,std::string &path, bool includeSymlinks);

	int findArchiveFileNameDelimiter(const std::string &path) const;
	shared_ptr<ZLFile> rootDirectory() const;
	const std::string &rootDirectoryPath() const;
	std::string parentPath(const std::string &path) const;

	bool canRemoveFile(const std::string &path) const;
};

inline void ZLUnixFSManager::createInstance() {
    ourInstance = new ZLUnixFSManager();
}

#endif /* __ZLUNIXFSMANAGER_H__ */
