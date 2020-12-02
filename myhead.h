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
/**
 * 带属性的"token"
 * @param symbol 在ID，数字这些时，这个字段是token和constV
 * @param value  存放的真正的ID和数字
 * @param intVal
 * @param address
 * @param name
 * @param type
 * @param synthesis
 */
attributeTable::attributeTable(string symbol,
                   string value,
                   int intVal,
                   int address,
                   int name,
                   int type,
                   int synthesis) {
    this->symbol = std::move(symbol);
    this->value = std::move(value);
    this->address = address;
    this->name = name;
    this->type = type;
    this->synthesis = synthesis;
    this->intVal = intVal;
}

#endif //COMPILE_MYHEAD_H
