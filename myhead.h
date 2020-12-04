//
// Created by llincyaw on 2020/11/12.
//

#ifndef COMPILE_MYHEAD_H
#define COMPILE_MYHEAD_H

#include <iostream>
#include <map>
#include <string>
#include <typeinfo>
#include <fstream>
#include <utility>
#include <vector>
#include <sstream>
#include <stdlib.h>
//#include "dbg.h"
using namespace std;
#define IS_BLANK(x) x==' '?true:false
#define ID 85
#include <deque>

class attributeTable {
public:
    attributeTable()=default;
    attributeTable(string symbol,
                   string value,
                   int intVal = INT32_MIN,
                   int address = INT32_MIN,
                   int name = INT32_MIN,
                   int type = INT32_MIN,
                   int synthesis = INT32_MIN
                   );

    string symbol;
    string value;
    int address;
    int name;
    int type;
    int synthesis;
    int intVal;
};


#endif //COMPILE_MYHEAD_H
