#ifndef __ZLFILE_H__
#define __ZLFILE_H__

#include <string>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>

#include "shared_ptr.h"
#include <ZLFileInfo.h>

class ZLInputStream;
class ZLOutputStream;

class ZLFile {
public:
	static std::string replaceIllegalCharacters(const std::string &fileName, char replaceWith);

private:
	ZLFile();

public:
	explicit ZLFile(const std::string &path, const std::string &mimeType = std::string());
	~ZLFile();

	bool isExists() const;
    bool isDirectory() const;
    bool isFile() const;
	std::size_t getFileSize() const;
	std::size_t getLastModified() const;

	bool remove() const;
	bool canRemove() const;

	const std::string &path() const;
	const std::string &name(bool hideExtension=false) const;
	const std::string &extension() const;

    std::string delimiter() const;
    void fileList(std::vector<shared_ptr<ZLFile>> &names,bool includeSymlinks);
    shared_ptr<ZLFile> createDirectory(bool createUnExisting=true) const;

	std::string physicalFilePath() const;
	std::string resolvedPath() const;

	shared_ptr<ZLInputStream> inputStream() const;
	shared_ptr<ZLOutputStream> outputStream() const;

	bool operator == (const ZLFile &other) const;
	bool operator != (const ZLFile &other) const;
	bool operator < (const ZLFile &other) const;

private:
	void fillInfo() const;
    ZLFile *createPlainDirectory(const std::string &path) const;
    ZLFile *createNewDirectory(const std::string &path) const;

private:
	std::string myPath;
	std::string myNameWithExtension;
	std::string myNameWithoutExtension;
	std::string myExtension;
	mutable std::string myMimeType;
	mutable bool myMimeTypeIsUpToDate;
	mutable ZLFileInfo myInfo;
	mutable bool myInfoIsFilled;
};

inline ZLFile::~ZLFile() {}

inline const std::string &ZLFile::path() const { return myPath; }
inline const std::string &ZLFile::name(bool hideExtension) const { return hideExtension ? myNameWithoutExtension : myNameWithExtension; }
inline const std::string &ZLFile::extension() const { return myExtension; }

inline bool ZLFile::operator == (const ZLFile &other) const { return myPath == other.myPath; }
inline bool ZLFile::operator != (const ZLFile &other) const { return myPath != other.myPath; }
inline bool ZLFile::operator < (const ZLFile &other) const { return myPath < other.myPath; }

#endif /* __ZLFILE_H__ */
