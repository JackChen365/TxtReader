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


#include <ZLTextModel.h>
#include "BookReader.h"
#include <shared_ptr.h>


BookReader::BookReader(shared_ptr<ZLFile> &file, const std::string &title, const std::string &encoding,
                       const std::string &language,const std::string &cacheDir):file(file),title(title),encoding(encoding),language(language) {
    myCurrentTextModel = new ZLTextPlainModel(std::string(),language,131072,cacheDir,"ncache");
    myInsideTitle = false;
    mySectionContainsRegularContents = false;
}

BookReader::~BookReader() {
}

bool BookReader::paragraphIsOpen() const {
	if (myCurrentTextModel.isNull()) {
		return false;
	}
	for (std::list<shared_ptr<ZLTextModel> >::const_iterator it = myModelsWithOpenParagraphs.begin(); it != myModelsWithOpenParagraphs.end(); ++it) {
		if (*it == myCurrentTextModel) {
			return true;
		}
	}
	return false;
}

void BookReader::addContentsData(const std::string &data) {
    if (!data.empty() && !myContentsTreeStack.empty()) {
        myContentsTreeStack.top()->addText(data);
    }
}

void BookReader::beginContentsParagraph(int referenceNumber) {
    if (referenceNumber == -1) {
        referenceNumber = myCurrentTextModel->paragraphsNumber();
    }
    shared_ptr<ContentsTree> parent =
            myContentsTreeStack.empty() ? new ContentsTree() : myContentsTreeStack.top();
    if (parent->text().empty()) {
        parent->addText("...");
    }
    new ContentsTree(*parent, referenceNumber);
    const std::vector<shared_ptr<ContentsTree> > &children = parent->children();
    myContentsTreeStack.push(children[children.size() - 1]);
    myContentsParagraphExists = true;
}

void BookReader::endContentsParagraph() {
    if (!myContentsTreeStack.empty()) {
        shared_ptr<ContentsTree> tree = myContentsTreeStack.top();
        if (tree->text().empty()) {
            tree->addText("...");
        }
        myContentsTreeStack.pop();
    }
    myContentsParagraphExists = false;
}

void BookReader::pushKind(FBTextKind kind) {
	myKindStack.push_back(kind);
}

bool BookReader::popKind() {
	if (!myKindStack.empty()) {
		myKindStack.pop_back();
		return true;
	}
	return false;
}

bool BookReader::isKindStackEmpty() const {
	return myKindStack.empty();
}

void BookReader::beginParagraph(ZLTextParagraph::Kind kind) {
	endParagraph();
	if (myCurrentTextModel != 0) {
		((ZLTextPlainModel&)*myCurrentTextModel).createParagraph(kind);
		for (std::vector<FBTextKind>::const_iterator it = myKindStack.begin(); it != myKindStack.end(); ++it) {
			myCurrentTextModel->addControl(*it, true);
		}
		if (!myHyperlinkReference.empty()) {
			myCurrentTextModel->addHyperlinkControl(myHyperlinkKind, myHyperlinkType, myHyperlinkReference);
		}
		myModelsWithOpenParagraphs.push_back(myCurrentTextModel);
	}
}

void BookReader::endParagraph() {
	if (paragraphIsOpen()) {
		flushTextBufferToParagraph();
		myModelsWithOpenParagraphs.remove(myCurrentTextModel);
	}
}

void BookReader::addData(const std::string &data) {
	if (!data.empty() && paragraphIsOpen()) {
		if (!myInsideTitle) {
			mySectionContainsRegularContents = true;
		}
		myBuffer.push_back(data);
	}
}

void BookReader::flushTextBufferToParagraph() {
	myCurrentTextModel->addText(myBuffer);
	myBuffer.clear();
}


void BookReader::insertEndParagraph(ZLTextParagraph::Kind kind) {
	if (myCurrentTextModel != 0 && mySectionContainsRegularContents) {
		std::size_t size = myCurrentTextModel->paragraphsNumber();
		if (size > 0 && ((*myCurrentTextModel)[(std::size_t)-1])->kind() != kind) {
			endParagraph();
			((ZLTextPlainModel&)*myCurrentTextModel).createParagraph(kind);
			mySectionContainsRegularContents = false;
		}
	}
}

void BookReader::insertEndOfSectionParagraph() {
	insertEndParagraph(ZLTextParagraph::END_OF_SECTION_PARAGRAPH);
}

void BookReader::insertPseudoEndOfSectionParagraph() {
	insertEndParagraph(ZLTextParagraph::PSEUDO_END_OF_SECTION_PARAGRAPH);
}

void BookReader::insertEndOfTextParagraph() {
	insertEndParagraph(ZLTextParagraph::END_OF_TEXT_PARAGRAPH);
}

void BookReader::insertEncryptedSectionParagraph() {
	beginParagraph(ZLTextParagraph::ENCRYPTED_SECTION_PARAGRAPH);
	endParagraph();
	beginParagraph(ZLTextParagraph::END_OF_SECTION_PARAGRAPH);
	endParagraph();
}

void BookReader::reset() {
	myKindStack.clear();
}
