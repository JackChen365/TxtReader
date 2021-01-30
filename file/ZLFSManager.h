#ifndef __ZLFSMANAGER_H__
#define __ZLFSMANAGER_H__

#include <string>
#include <map>

#include <ZLFileInfo.h>
#include <ZLFile.h>

class ZLInputStream;
class ZLOutputStream;

class ZLFSManager {

public:
	static void deleteInstance();
	static ZLFSManager &Instance();
protected:
	static ZLFSManager *ourInstance;

protected:
	ZLFSManager();
	virtual ~ZLFSManager();

public:
	void normalize(std::string &path) const;
	virtual std::string resolveSymlink(const std::string &path) const = 0;

protected:
	virtual void normalizeRealPath(std::string &path) const = 0;
	virtual ZLInputStream *createPlainInputStream(const std::string &path) const = 0;
	virtual ZLOutputStream *createOutputStream(const std::string &path) const = 0;

	virtual ZLFileInfo fileInfo(const std::string &path) const = 0;

    virtual ZLFile *createNewDirectory(const std::string &path) const = 0;
    virtual ZLFile *createPlainDirectory(const std::string &path) const = 0;
    virtual void fileList(std::vector<shared_ptr<ZLFile>> &names,std::string &path, bool includeSymlinks) = 0;

	virtual bool removeFile(const std::string &path) const = 0;

	virtual int findArchiveFileNameDelimiter(const std::string &path) const = 0;
	int findLastFileNameDelimiter(const std::string &path) const;
    std::string delimiter() const;
	virtual std::string parentPath(const std::string &path) const = 0;

	virtual bool canRemoveFile(const std::string &path) const = 0;

    friend class ZLFile;
};

inline ZLFSManager &ZLFSManager::Instance() { return *ourInstance; }
inline ZLFSManager::ZLFSManager() {}
inline ZLFSManager::~ZLFSManager() {}

#endif /* __ZLFSMANAGER_H__ */
