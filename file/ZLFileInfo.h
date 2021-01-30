#ifndef __ZLFILEINFO_H__
#define __ZLFILEINFO_H__

struct ZLFileInfo {
	bool Exists;
	bool IsDirectory;
	std::size_t Size;
	std::size_t MTime;

	ZLFileInfo();
};

inline ZLFileInfo::ZLFileInfo() : Exists(false), IsDirectory(false), Size(0) {
}

#endif /* __ZLFILEINFO_H__ */
