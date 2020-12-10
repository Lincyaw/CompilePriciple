//
// Created by llincyaw on 2020/11/12.
//

#ifndef COMPILE_GRAMMAR_ANALYZER_H
#define COMPILE_GRAMMAR_ANALYZER_H

#include "myhead.h"
//#define MY_INT 0
//#define MY_CHAR 1
//#define MY_VOID 2
enum week {MY_INT, MY_CHAR, MY_VOID};
class Grammar_Analyzer {
public:
    Grammar_Analyzer();
    deque<int> regs;
    map<string, int> index;
    map<string, int> NoEndIndex;
    map<string,string> valueToReg;
    vector<vector<string>> midCodeOut;
    //句子指针
    int pointer{};
    //状态栈
    vector<int> stateStack;
    //符号栈
    vector<attributeTable> symbolStack;
    //动作表
    vector<vector<string>> actionMap;
    //转移表
    vector<vector<int>> gotoMap;  // 只有状态转移
    void LR1(vector<pair<int, attributeTable>> Input);
    //解析动作，例如s1,解析为（s，1）
    static pair<string, string> parseState(const string &action);
    void initGotoMap();
    map<string, attributeTable> sysMap;
    void initActionMap();
    static pair<string, vector<string>> parseProducer(const string &input);
    void initIndex();
    void initNotEndIndex();
    bool translate(deque<attributeTable> &producer);
    // 将自定义的变量和常数转换为对应的名称
    static string switcher(pair<int, attributeTable> &input);
    void clearUselessProducer();
    static bool notEnd(const string& input,map<string, int> NoEndIndex);


    // 翻译成汇编
    void toAsm();
    string getReg();
};
void printAttrbuteTable(const attributeTable& table);
void testParseState();
void printProducerWithAttr(deque<attributeTable> producerWithAttr);
void testModules();
void testParseProducer();
void testInitIndex();
#endif //COMPILE_GRAMMAR_ANALYZER_H
