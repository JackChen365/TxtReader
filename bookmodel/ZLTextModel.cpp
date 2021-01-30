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

#include <cstdint>
#include <cstring>
#include <algorithm>

#include "ZLTextModel.h"
#include "ZLTextParagraph.h"
#include "util/ZLUnicodeUtil.h"

ZLTextModel::ZLTextModel(const std::string &id, const std::string &language, const std::size_t rowSize,
		const std::string &directoryName, const std::string &fileExtension) :
	myId(id),
	myLanguage(language),
	myAllocator(new ZLCachedMemoryAllocator(rowSize, directoryName, fileExtension)),
	myLastEntryStart(0){
}

ZLTextModel::~ZLTextModel() {
	for (std::vector<ZLTextParagraph*>::const_iterator it = myParagraphs.begin(); it != myParagraphs.end(); ++it) {
		delete *it;
	}
}

void ZLTextModel::addParagraphInternal(ZLTextParagraph *paragraph) {
	const std::size_t dataSize = myAllocator->blocksNumber();
	const std::size_t bytesOffset = myAllocator->currentBytesOffset();

	myStartEntryIndices.push_back((dataSize == 0) ? 0 : (dataSize - 1));
	myStartEntryOffsets.push_back(bytesOffset / 2); // offset in words for future use in Java
	myParagraphLengths.push_back(0);
	myTextSizes.push_back(myTextSizes.empty() ? 0 : myTextSizes.back());
	myParagraphKinds.push_back(paragraph->kind());

	myParagraphs.push_back(paragraph);
	myLastEntryStart = 0;
}

ZLTextPlainModel::ZLTextPlainModel(const std::string &id, const std::string &language, const std::size_t rowSize,
		const std::string &directoryName, const std::string &fileExtension) :
	ZLTextModel(id, language, rowSize, directoryName, fileExtension) {
}

void ZLTextPlainModel::createParagraph(ZLTextParagraph::Kind kind) {
	ZLTextParagraph *paragraph = (kind == ZLTextParagraph::TEXT_PARAGRAPH) ? new ZLTextParagraph() : new ZLTextSpecialParagraph(kind);
	addParagraphInternal(paragraph);
}

void ZLTextModel::addText(const std::string &text) {
	ZLUnicodeUtil::Ucs2String ucs2str;
	ZLUnicodeUtil::utf8ToUcs2(ucs2str, text);
	const std::size_t len = ucs2str.size();

	if (myLastEntryStart != 0 && *myLastEntryStart == ZLTextParagraphEntry::TEXT_ENTRY) {
		const std::size_t oldLen = ZLCachedMemoryAllocator::readUInt32(myLastEntryStart + 2);
		const std::size_t newLen = oldLen + len;
		myLastEntryStart = myAllocator->reallocateLast(myLastEntryStart, 2 * newLen + 6);
		ZLCachedMemoryAllocator::writeUInt32(myLastEntryStart + 2, newLen);
		std::memcpy(myLastEntryStart + 6 + oldLen, &ucs2str.front(), 2 * newLen);
	} else {
		myLastEntryStart = myAllocator->allocate(2 * len + 6);
		*myLastEntryStart = ZLTextParagraphEntry::TEXT_ENTRY;
		*(myLastEntryStart + 1) = 0;
		ZLCachedMemoryAllocator::writeUInt32(myLastEntryStart + 2, len);
		std::memcpy(myLastEntryStart + 6, &ucs2str.front(), 2 * len);
		myParagraphs.back()->addEntry(myLastEntryStart);
		++myParagraphLengths.back();
	}
	myTextSizes.back() += len;
}

void ZLTextModel::addText(const std::vector<std::string> &text) {
	if (text.size() == 0) {
		return;
	}
	std::size_t fullLength = 0;
	for (std::vector<std::string>::const_iterator it = text.begin(); it != text.end(); ++it) {
		fullLength += ZLUnicodeUtil::utf8Length(*it);
	}

	ZLUnicodeUtil::Ucs2String ucs2str;
	if (myLastEntryStart != 0 && *myLastEntryStart == ZLTextParagraphEntry::TEXT_ENTRY) {
		const std::size_t oldLen = ZLCachedMemoryAllocator::readUInt32(myLastEntryStart + 2);
		const std::size_t newLen = oldLen + fullLength;
		myLastEntryStart = myAllocator->reallocateLast(myLastEntryStart, 2 * newLen + 6);
		ZLCachedMemoryAllocator::writeUInt32(myLastEntryStart + 2, newLen);
		std::size_t offset = 6 + oldLen;
		for (std::vector<std::string>::const_iterator it = text.begin(); it != text.end(); ++it) {
			ZLUnicodeUtil::utf8ToUcs2(ucs2str, *it);
			const std::size_t len = 2 * ucs2str.size();
			std::memcpy(myLastEntryStart + offset, &ucs2str.front(), len);
			offset += len;
			ucs2str.clear();
		}
	} else {
		myLastEntryStart = myAllocator->allocate(2 * fullLength + 6);
		*myLastEntryStart = ZLTextParagraphEntry::TEXT_ENTRY;
		*(myLastEntryStart + 1) = 0;
		ZLCachedMemoryAllocator::writeUInt32(myLastEntryStart + 2, fullLength);
		std::size_t offset = 6;
		for (std::vector<std::string>::const_iterator it = text.begin(); it != text.end(); ++it) {
			ZLUnicodeUtil::utf8ToUcs2(ucs2str, *it);
			const std::size_t len = 2 * ucs2str.size();
			std::memcpy(myLastEntryStart + offset, &ucs2str.front(), len);
			offset += len;
			ucs2str.clear();
		}
		myParagraphs.back()->addEntry(myLastEntryStart);
		++myParagraphLengths.back();
	}
	myTextSizes.back() += fullLength;
}

void ZLTextModel::addFixedHSpace(unsigned char length) {
	myLastEntryStart = myAllocator->allocate(4);
	*myLastEntryStart = ZLTextParagraphEntry::FIXED_HSPACE_ENTRY;
	*(myLastEntryStart + 1) = 0;
	*(myLastEntryStart + 2) = length;
	*(myLastEntryStart + 3) = 0;
	myParagraphs.back()->addEntry(myLastEntryStart);
	++myParagraphLengths.back();
}

void ZLTextModel::addControl(ZLTextKind textKind, bool isStart) {
	myLastEntryStart = myAllocator->allocate(4);
	*myLastEntryStart = ZLTextParagraphEntry::CONTROL_ENTRY;
	*(myLastEntryStart + 1) = 0;
	*(myLastEntryStart + 2) = textKind;
	*(myLastEntryStart + 3) = isStart ? 1 : 0;
	myParagraphs.back()->addEntry(myLastEntryStart);
	++myParagraphLengths.back();
}

//static int EntryCount = 0;
//static int EntryLen = 0;

void ZLTextModel::addHyperlinkControl(ZLTextKind textKind, ZLHyperlinkType hyperlinkType, const std::string &label) {
	ZLUnicodeUtil::Ucs2String ucs2label;
	ZLUnicodeUtil::utf8ToUcs2(ucs2label, label);

	const std::size_t len = ucs2label.size() * 2;

	myLastEntryStart = myAllocator->allocate(len + 6);
	*myLastEntryStart = ZLTextParagraphEntry::HYPERLINK_CONTROL_ENTRY;
	*(myLastEntryStart + 1) = 0;
	*(myLastEntryStart + 2) = textKind;
	*(myLastEntryStart + 3) = hyperlinkType;
	ZLCachedMemoryAllocator::writeUInt16(myLastEntryStart + 4, ucs2label.size());
	std::memcpy(myLastEntryStart + 6, &ucs2label.front(), len);
	myParagraphs.back()->addEntry(myLastEntryStart);
	++myParagraphLengths.back();
}

void ZLTextModel::addBidiReset() {
	myLastEntryStart = myAllocator->allocate(2);
	*myLastEntryStart = ZLTextParagraphEntry::RESET_BIDI_ENTRY;
	*(myLastEntryStart + 1) = 0;
	myParagraphs.back()->addEntry(myLastEntryStart);
	++myParagraphLengths.back();
}

void ZLTextModel::flush() {
	myAllocator->flush();
}
