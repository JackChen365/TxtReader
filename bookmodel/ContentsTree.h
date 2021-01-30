//
// Created by cz on 2021/1/30.
//

#ifndef FILE_CONTENTSTREE_H
#define FILE_CONTENTSTREE_H

#include <shared_ptr.h>
#include <iostream>
#include <vector>

class ContentsTree {
public:
    ContentsTree();
    ContentsTree(ContentsTree &parent, int reference);
    void addText(const std::string &buffer);

    const std::string &text() const;
    int reference() const;
    const std::vector<shared_ptr<ContentsTree> > &children() const;

private:
    std::string myText;
    const int myReference;
    std::vector<shared_ptr<ContentsTree> > myChildren;
};

inline ContentsTree::ContentsTree() : myReference(-1) {}
inline ContentsTree::ContentsTree(ContentsTree &parent, int reference) : myReference(reference) {
    parent.myChildren.push_back(this);
}
inline void ContentsTree::addText(const std::string &buffer) {
    myText += buffer;
}

inline const std::string &ContentsTree::text() const { return myText; }
inline int ContentsTree::reference() const { return myReference; }
inline const std::vector<shared_ptr<ContentsTree> > &ContentsTree::children() const { return myChildren; }


#endif //FILE_CONTENTSTREE_H
