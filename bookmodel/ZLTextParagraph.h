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

#ifndef __ZLTEXTPARAGRAPH_H__
#define __ZLTEXTPARAGRAPH_H__

#include <map>
#include <vector>
#include <string>

#include <shared_ptr.h>

#include <ZLHyperlinkType.h>
#include <ZLTextKind.h>
#include <ZLTextAlignmentType.h>
class ZLTextParagraphEntry {

public:
	enum Kind {
		TEXT_ENTRY = 1,
		IMAGE_ENTRY = 2,
		CONTROL_ENTRY = 3,
		HYPERLINK_CONTROL_ENTRY = 4,
		STYLE_CSS_ENTRY = 5,
		STYLE_OTHER_ENTRY = 6,
		STYLE_CLOSE_ENTRY = 7,
		FIXED_HSPACE_ENTRY = 8,
		RESET_BIDI_ENTRY = 9,
		AUDIO_ENTRY = 10,
		VIDEO_ENTRY = 11,
		EXTENSION_ENTRY = 12,
	};

protected:
	ZLTextParagraphEntry();

public:
	virtual ~ZLTextParagraphEntry();

private: // disable copying
	ZLTextParagraphEntry(const ZLTextParagraphEntry &entry);
	const ZLTextParagraphEntry &operator = (const ZLTextParagraphEntry &entry);
};

class ZLTextParagraph {

public:

	enum Kind {
		TEXT_PARAGRAPH = 0,
		TREE_PARAGRAPH = 1,
		EMPTY_LINE_PARAGRAPH = 2,
		BEFORE_SKIP_PARAGRAPH = 3,
		AFTER_SKIP_PARAGRAPH = 4,
		END_OF_SECTION_PARAGRAPH = 5,
		PSEUDO_END_OF_SECTION_PARAGRAPH = 6,
		END_OF_TEXT_PARAGRAPH = 7,
		ENCRYPTED_SECTION_PARAGRAPH = 8,
	};

protected:
	ZLTextParagraph();

public:
	virtual ~ZLTextParagraph();
	virtual Kind kind() const;
private:
	void addEntry(char *address);

private:
	std::size_t myEntryNumber;

friend class ZLTextModel;
friend class ZLTextPlainModel;
};

class ZLTextSpecialParagraph : public ZLTextParagraph {

private:
	ZLTextSpecialParagraph(Kind kind);

public:
	~ZLTextSpecialParagraph();
	Kind kind() const;

private:
	Kind myKind;

friend class ZLTextPlainModel;
};

inline ZLTextParagraphEntry::ZLTextParagraphEntry() {}
inline ZLTextParagraphEntry::~ZLTextParagraphEntry() {}

inline ZLTextParagraph::ZLTextParagraph() : myEntryNumber(0) {}
inline ZLTextParagraph::~ZLTextParagraph() {}
inline ZLTextParagraph::Kind ZLTextParagraph::kind() const { return TEXT_PARAGRAPH; }
inline void ZLTextParagraph::addEntry(char *address) { ++myEntryNumber; }

inline ZLTextSpecialParagraph::ZLTextSpecialParagraph(Kind kind) : myKind(kind) {}
inline ZLTextSpecialParagraph::~ZLTextSpecialParagraph() {}
inline ZLTextParagraph::Kind ZLTextSpecialParagraph::kind() const { return myKind; }

#endif /* __ZLTEXTPARAGRAPH_H__ */
