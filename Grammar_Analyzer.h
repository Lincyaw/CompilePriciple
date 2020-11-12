//
// Created by llincyaw on 2020/11/12.
//

#ifndef COMPILE_GRAMMAR_ANALYZER_H
#define COMPILE_GRAMMAR_ANALYZER_H

#include "myhead.h"

class Grammar_Analyzer {
public:
    //输入的句子
    string input;
    //句子指针
    int pointer;
    //状态栈
    vector<int> stateStack;
    //符号栈
    vector<char> symbolStack;
    //产生式
    vector<pair<string,string>> producer;
    //动作表
    vector<vector<string>> actionMap;
    //转移表
    vector<vector<int>> gotoMap;  // 只有状态转移
    void LR1(string Input);
    //解析动作，例如s1,解析为（s，1）
    static pair<char,int> parseState(const string& action);

    void initGotoMap();

    void initActionMap();

    void initProducer();
};


#endif //COMPILE_GRAMMAR_ANALYZER_H
