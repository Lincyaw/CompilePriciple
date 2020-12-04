//
// Created by llincyaw on 2020/11/12.
//

#ifndef COMPILE_LEXICAL_ANALYZER_H
#define COMPILE_LEXICAL_ANALYZER_H

#include "myhead.h"

using namespace std;

class systable {
public:
    explicit systable(string type = "-1", string address = "?") {
        this->address = address;
        this->type = type;
    }

    string type;
    string address;
};

class Lexical_analyzer {
public:
    // token串存放的数据结构
    vector<pair<int, attributeTable>> sysTable;
    // 符号表
    map<string, systable> sysMap;

    void read_program(const char *file, string &prog);

    void initMap();

    bool getNext(char &ch, string::size_type &pos, const string &program);

    pair<int, string> scanner(const string &program, string::size_type &pos);

    void lexical();

    map<string, int> charMap;
};


#endif //COMPILE_LEXICAL_ANALYZER_H
