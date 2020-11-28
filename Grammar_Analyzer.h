//
// Created by llincyaw on 2020/11/12.
//

#ifndef COMPILE_GRAMMAR_ANALYZER_H
#define COMPILE_GRAMMAR_ANALYZER_H

#include "myhead.h"

class Grammar_Analyzer {
public:
    Grammar_Analyzer();
    map<string, int> index;
    map<string, int> NoEndIndex;
//    //输入的句子
//    string input;
    //句子指针
    int pointer;
    //状态栈
    vector<int> stateStack;
    //符号栈
    vector<string> symbolStack;

    //动作表
    vector<vector<string>> actionMap;
    //转移表
    vector<vector<int>> gotoMap;  // 只有状态转移
    void LR1(vector<pair<int, string>> Input);

    //解析动作，例如s1,解析为（s，1）
    static pair<string, string> parseState(const string &action);

    void initGotoMap();

    void initActionMap();

    pair<string, vector<string>> parseProducer(const string &input);

    void initIndex();
    void initNotEndIndex();
    void translate(pair<string, vector<string>>);
    // 将自定义的变量和常数转换为对应的名称
    string switcher(pair<int, string> input);
};

class attributeTable{
public:
    int value;
    int address;
    int name;
    int type;
    int synthesis;
    int inherit;
};
void testModules();

void testParseState();

void testParseProducer();

void testInitIndex();

void trim();
#endif //COMPILE_GRAMMAR_ANALYZER_H
