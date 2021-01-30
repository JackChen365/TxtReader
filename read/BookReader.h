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

#ifndef __BOOKREADER_H__
#define __BOOKREADER_H__

#include <vector>
#include <list>
#include <map>
#include <stack>
#include <string>

#include <ZLFile.h>
#include <ZLTextParagraph.h>
#include <ContentsTree.h>
#include "FBHyperlinkType.h"
#include "FBTextKind.h"

class ZLTextModel;
class ZLInputStream;
class ZLCachedMemoryAllocator;

class BookReader {

public:
	BookReader(shared_ptr<ZLFile> &file,const std::string &title,
            const std::string &encoding,const std::string &language,const std::string &cacheDir);
	virtual ~BookReader();

	void insertEndOfSectionParagraph();
	void insertPseudoEndOfSectionParagraph();
	void insertEndOfTextParagraph();
	void insertEncryptedSectionParagraph();

	void pushKind(FBTextKind kind);
	bool popKind();
	bool isKindStackEmpty() const;

	void beginParagraph(ZLTextParagraph::Kind kind = ZLTextParagraph::TEXT_PARAGRAPH);
	void endParagraph();
	bool paragraphIsOpen() const;

    void addContentsData(const std::string &data);
    void beginContentsParagraph(int referenceNumber = -1);
    void endContentsParagraph();

	bool contentsParagraphIsOpen() const;

	shared_ptr<ZLTextModel> getTextModel();

	void addData(const std::string &data);

	void enterTitle() { myInsideTitle = true; }
	void exitTitle() { myInsideTitle = false; }

	void reset();

private:
	void insertEndParagraph(ZLTextParagraph::Kind kind);
	void flushTextBufferToParagraph();

private:
    shared_ptr<ZLFile> &file;
    const std::string &encoding;
    const std::string &language;
    const std::string &title;

	shared_ptr<ZLTextModel> myCurrentTextModel;
	std::list<shared_ptr<ZLTextModel> > myModelsWithOpenParagraphs;

    std::stack<shared_ptr<ContentsTree> > myContentsTreeStack;

	std::vector<FBTextKind> myKindStack;
	bool myContentsParagraphExists;

	bool mySectionContainsRegularContents;
	bool myInsideTitle;

	std::vector<std::string> myBuffer;

	std::string myHyperlinkReference;
	FBHyperlinkType myHyperlinkType;
	FBTextKind myHyperlinkKind;

	shared_ptr<ZLCachedMemoryAllocator> myFootnotesAllocator;
};

inline bool BookReader::contentsParagraphIsOpen() const {
	return myContentsParagraphExists;
}

inline shared_ptr<ZLTextModel> BookReader::getTextModel() {
    return myCurrentTextModel;
}

#endif /* __BOOKREADER_H__ */
