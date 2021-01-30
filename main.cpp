//
// Created by cz on 1/8/21.
//
#include <iostream>
#include <TxtBookReader.h>
#include <ZLStringUtil.h>
#include <ZLFile.h>
#include <ZLInputStream.h>
#include <ZLOutputStream.h>
#include "unix/ZLUnixFSManager.h"
#include "shared_ptr.h"

static std::string getPwdDir() {
    char *pwd = getenv("PWD");
    return (pwd != 0) ? pwd : "";
}

static std::string getHomeDir() {
    char *home = getenv("HOME");
    return (home != 0) ? home : "";
}

void normalizeRealPath(std::string &path) {
    static std::string HomeDir = getHomeDir();
    static std::string PwdDir = getPwdDir();

    if (path.empty()) {
        path = PwdDir;
    } else if (path[0] == '~') {
        if ((path.length() == 1) || (path[1] == '/')) {
            path = HomeDir + path.substr(1);
        }
    } else if (path[0] != '/') {
        path = PwdDir + '/' + path;
    }
    int last = path.length() - 1;
    while ((last > 0) && (path[last] == '/')) {
        --last;
    }
    if (last < (int)path.length() - 1) {
        path = path.substr(0, last + 1);
    }

    int index;
    while ((index = path.find("/../")) != -1) {
        int prevIndex = std::max((int)path.rfind('/', index - 1), 0);
        path.erase(prevIndex, index + 3 - prevIndex);
    }
    int len = path.length();
    if ((len >= 3) && (path.substr(len - 3) == "/..")) {
        int prevIndex = std::max((int)path.rfind('/', len - 4), 0);
        path.erase(prevIndex);
    }
    while ((index = path.find("/./")) != -1) {
        path.erase(index, 2);
    }
    while (path.length() >= 2 &&
           path.substr(path.length() - 2) == "/.") {
        path.erase(path.length() - 2);
    }
    while ((index = path.find("//")) != -1) {
        path.erase(index, 1);
    }
}

/*
 * Test string utils
 */
void stringTest(){
    std::string str("aBcc");
    std::cout<<ZLStringUtil::toUpper(str)<<std::endl;
    std::cout<<ZLStringUtil::toLower(str)<<std::endl;
    ZLStringUtil::appendNumber(str,123);
    std::cout<<str<<std::endl;

    std::cout<<str.rfind("c")<<std::endl;
    std::cout<<str.find("c")<<std::endl;
}

void fileTest(){
    //ZLFile
    ZLUnixFSManager::createInstance();
    ZLFile file("../conf/note.txt");
    shared_ptr<ZLInputStream> inputStream=file.inputStream();
    char* buffer=new char[10];
    if(inputStream->open()){
        while(0 != inputStream->read(buffer,30)) {
//            std::cout<<buffer;
        }
//        std::cout<<std::endl;
    }
    inputStream->close();

    //File output stream
    ZLFile newFile("../conf/new.txt");
    shared_ptr<ZLOutputStream> outputStream=newFile.outputStream();
    if(outputStream->open()){
        outputStream->write("Hello world xxx");
        std::cout<<"write"<<std::endl;
    }
    outputStream->close();
    //File recursive
    std::string path("../../.");
    normalizeRealPath(path);
    ZLFile folder("../../.");
    std::vector<shared_ptr<ZLFile>> names;
    folder.fileList(names,false);
    for(auto iter=names.begin();iter!=names.end();iter++){
        std::cout<<(*iter)->name()<<std::endl;
    }
    //File info
    std::cout<<"Existed:"<<file.isExists()<<std::endl;
    std::cout<<"FileSize:"<<file.getFileSize()<<std::endl;
    std::cout<<"LastModified:"<<file.getLastModified()<<std::endl;
    std::cout<<"isDirectory:"<<file.isDirectory()<<std::endl;
    std::cout<<"isFile:"<<file.isFile()<<std::endl;
}

int main(){
    //stringTest();
    //fileTest();
    ZLUnixFSManager::createInstance();
    shared_ptr<ZLFile> file=new ZLFile("../conf/little_prince.txt");
    PlainTextFormat textFormat(*file);

    PlainTextFormatDetector formatDetector;
    shared_ptr<ZLInputStream> inputStream=file->inputStream();
    formatDetector.detect(*inputStream,textFormat);
    if(textFormat.initialized()){
        std::cout<<"initialized"<<std::endl;
    }
    TxtBookReader bookReader(file,"小王子","utf-8","cn","../conf",textFormat);
    bookReader.readDocument(*inputStream);
    shared_ptr<ZLTextModel> textModel=bookReader.getTextModel();
    std::cout<<"start read book"<<std::endl;
    return 0;
}