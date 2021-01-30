/*
 * Copyright (C) 2004-2015 FBReader.ORG Limited <contact@fbreader.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#ifndef __ZLTEXTMODEL_H__
#define __ZLTEXTMODEL_H__

#include <algorithm>
#include <string>
#include <vector>
#include <map>

#include <ZLHyperlinkType.h>
#include <ZLTextParagraph.h>
#include <ZLTextKind.h>
#include <ZLCachedMemoryAllocator.h>

class ZLTextModel {

protected:
	ZLTextModel(const std::string &id, const std::string &language, const std::size_t rowSize,
		const std::string &directoryName, const std::string &fileExtension);

public:
	virtual ~ZLTextModel();

	const std::string &id() const;
	const std::string &language() const;
	//bool isRtl() const;

	std::size_t paragraphsNumber() const;
	ZLTextParagraph *operator [] (std::size_t index);
	const ZLTextParagraph *operator [] (std::size_t index) const;

	void addControl(ZLTextKind textKind, bool isStart);
	void addHyperlinkControl(ZLTextKind textKind, ZLHyperlinkType hyperlinkType, const std::string &label);
	void addText(const std::string &text);
	void addText(const std::vector<std::string> &text);
	void addFixedHSpace(unsigned char length);
	void addBidiReset();

	void flush();

	const ZLCachedMemoryAllocator &allocator() const;

	const std::vector<uint32_t> &startEntryIndices() const;
	const std::vector<uint32_t> &startEntryOffsets() const;
	const std::vector<uint32_t> &paragraphLengths() const;
	const std::vector<uint32_t> &textSizes() const;
	const std::vector<int8_t> &paragraphKinds() const;

protected:
	void addParagraphInternal(ZLTextParagraph *paragraph);

private:
	const std::string myId;
	const std::string myLanguage;
	std::vector<ZLTextParagraph*> myParagraphs;
	//mutable std::vector<ZLTextMark> myMarks;
	mutable shared_ptr<ZLCachedMemoryAllocator> myAllocator;

	char *myLastEntryStart;

	std::vector<uint32_t> myStartEntryIndices;
	std::vector<uint32_t> myStartEntryOffsets;
	std::vector<uint32_t> myParagraphLengths;
	std::vector<uint32_t> myTextSizes;
	std::vector<int8_t> myParagraphKinds;
private:
	ZLTextModel(const ZLTextModel&);
	const ZLTextModel &operator = (const ZLTextModel&);
};

class ZLTextPlainModel : public ZLTextModel {

public:
	ZLTextPlainModel(const std::string &id, const std::string &language, const std::size_t rowSize,
			const std::string &directoryName, const std::string &fileExtension);
	void createParagraph(ZLTextParagraph::Kind kind);
};

inline const std::string &ZLTextModel::id() const { return myId; }
inline const std::string &ZLTextModel::language() const { return myLanguage; }
inline std::size_t ZLTextModel::paragraphsNumber() const { return myParagraphs.size(); }
//inline const std::vector<ZLTextMark> &ZLTextModel::marks() const { return myMarks; }
//inline void ZLTextModel::removeAllMarks() { myMarks.clear(); }
inline const ZLCachedMemoryAllocator &ZLTextModel::allocator() const { return *myAllocator; }
inline const std::vector<uint32_t> &ZLTextModel::startEntryIndices() const { return myStartEntryIndices; }
inline const std::vector<uint32_t> &ZLTextModel::startEntryOffsets() const { return myStartEntryOffsets; }
inline const std::vector<uint32_t> &ZLTextModel::paragraphLengths() const { return myParagraphLengths; };
inline const std::vector<uint32_t> &ZLTextModel::textSizes() const { return myTextSizes; };
inline const std::vector<int8_t> &ZLTextModel::paragraphKinds() const { return myParagraphKinds; };

inline ZLTextParagraph *ZLTextModel::operator [] (std::size_t index) {
	return myParagraphs[std::min(myParagraphs.size() - 1, index)];
}

inline const ZLTextParagraph *ZLTextModel::operator [] (std::size_t index) const {
	return myParagraphs[std::min(myParagraphs.size() - 1, index)];
}

#endif /* __ZLTEXTMODEL_H__ */
