//
// Created by llincyaw on 2020/11/12.
//

#include "Grammar_Analyzer.h"
#include <utility>

void Grammar_Analyzer::initActionMap() {
    ifstream fp("../actionMap.txt"); //定义声明一个ifstream对象，指定文件路径
    string line;
    while (getline(fp, line)) { //循环读取每行数据
        vector<string> temp;
        string token;
        for (char i : line) {
            if (i == '\t') {
                temp.push_back(token);
                token = "";
                continue;
            }
            token += i;
        }
        temp.push_back(token);
        actionMap.push_back(temp);
    }
}

void Grammar_Analyzer::initGotoMap() {
    ifstream fp("../gotoMap.txt"); //定义声明一个ifstream对象，指定文件路径
    string line;
    while (getline(fp, line)) { //循环读取每行数据
        vector<int> temp;
        string token;
        for (char i : line) {
            if (i == '\t') {
                temp.push_back(atoi(token.c_str()));
                token = "";
                continue;
            }
            token += i;
        }
        temp.push_back(atoi(token.c_str()));
        gotoMap.push_back(temp);
    }
}

// LR1分析文法
void Grammar_Analyzer::LR1(vector<pair<int, string>> Input) {
    // 初始化状态栈和符号栈,以及输入的句子
    stateStack.push_back(0);
    symbolStack.emplace_back("$");
    Input.emplace_back(-5, "$");
    ofstream producerOut("../producer.txt");
    pointer = 0;
    // 栈顶符号
    int S;
    // 输入的字符串的第一个字符
    pair<int, string> a = Input[pointer];

//    dbg(actionMap, gotoMap);
    while (!symbolStack.empty()) {
//        dbg("round------------------------------------------------------");
//        dbg(a);
        S = stateStack.back();
        switch (a.first) {
            case ID:
                a.second = "token";
                break;
            case 82:
                a.second = "constV";
                break;
            case 83:
                a.second = "constV";
                break;
        }
        // TODO: 如果a.fisrt 类型是标识符，则要把action.second变成语法分析表里有的.常数同理

//        dbg(symbolStack, stateStack);
//        dbg(a);
//        dbg(S);
//        dbg(index[a.second]);

        // ? token 被跳过了
        auto LookAction = actionMap[S][index[a.second]];
//        dbg(LookAction);
        auto action = parseState(actionMap[S][index[a.second]]);


        if (action.first == "s") {
//            if(a.second=="$")
//                return;
            stateStack.emplace_back(atoi(action.second.c_str()));
            symbolStack.emplace_back(a.second);
            pointer++;
            a = Input[pointer];
        } else if (action.first == "r") {
            auto producer = parseProducer(action.second);

            for (int i = 0; i < producer.second.size(); i++) {
                symbolStack.pop_back();
                stateStack.pop_back();
            }

            auto S1 = stateStack.back();
            symbolStack.push_back(producer.first); // 左部产生式只有一个字符
            stateStack.push_back(gotoMap[S1][NoEndIndex[producer.first]]);

            producerOut<<producer.first<<" -> ";
            for(int i = 0;i<producer.second.size();i++){
                producerOut<<producer.second[i]<<" ";
            }
            producerOut<<endl;

//            dbg(S1);
//            dbg(producer.first);
//            dbg(producer.second);
//            dbg(NoEndIndex[producer.first]);
        } else if (LookAction=="accept") {
            return;
        } else {
            exit(2);
        }
//        dbg(symbolStack, stateStack);

    }

}

// 需求： 产生式保留空格，否则不知道pop多少下
pair<string, string> Grammar_Analyzer::parseState(const string &action) {

    if (action.size() <= 2) {
        cout << "should not use this state." << endl;
        exit(2);
    }
    string token;
    pair<string, string> ans;
    int flag = 0;
    for (auto i:action) {
//        dbg(token);
        if (i == ' ' && !flag) {
            if (token == "reduce") {
                ans.first = "r";
                token = "";
                flag = 1;
            } else if (token == "shift") {
                ans.first = "s";
                flag = 1;

            } else if (token == "accept") {
                ans.first = "a";
                flag = 1;

            }
            if (ans.first != "r")
                token = "";
            continue;
        }
        token += i;
    }
//    if (ans.first == 's') {

    ans.second = token;
//    }
    return ans;
}

void trim(string &s) {
    if (s.empty()) {
        return;
    }
    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_first_not_of(" " + 1));
}

pair<string, vector<string>> Grammar_Analyzer::parseProducer(const string &input) {
    string token;
    pair<string, vector<string>> ans;
    for (int i = 0; i < input.size(); i++) {
        if (input[i] == ' ') {
            if (token != "")
                ans.second.emplace_back(token);
            token = "";
            continue;
        }
        if (input[i] == '-' && input[i + 1] == '>') {
            ans.first = token;
            token = "";
            i++;
        } else {
            token += input[i];
        }
    }
    if (token != "")
        ans.second.emplace_back(token);
    ans.first = ans.second[0];
    ans.second.erase(ans.second.begin());
    return ans;
}


void Grammar_Analyzer::initIndex() {
    ifstream fp("../actionMapIndex.txt"); //定义声明一个ifstream对象，指定文件路径
    string line;
    int count = 0;
    while (getline(fp, line)) { //循环读取每行数据
        string token;
        for (auto i:line) {
            if (i == '\t') {
                index[token] = count;
//                cout << token << endl;
                count++;
                token = "";
                continue;
            }
            token += i;

        }
        index[token] = count;
    }
//    dbg(index);
}


void Grammar_Analyzer::initNotEndIndex() {
    ifstream fp("../gotoMapIndex.txt"); //定义声明一个ifstream对象，指定文件路径
    string line;
    int count = 0;
    while (getline(fp, line)) { //循环读取每行数据
        string token;
        for (auto i:line) {
            if (i == '\t') {
                NoEndIndex[token] = count;
                count++;
                token = "";
                continue;
            }
            token += i;
        }
        NoEndIndex[token] = count;
    }
}

Grammar_Analyzer::Grammar_Analyzer() {
    initIndex();
    initActionMap();
    initGotoMap();
    initNotEndIndex();
}

void testModules() {
    testParseState();
//    testParseProducer();
//    testInitIndex();

}

void testInitIndex() {
    Grammar_Analyzer test;
//    dbg(test.index);
}

void testParseState() {
    Grammar_Analyzer test;
    auto a = test.parseState("shift 53");
    auto b = test.parseState("  shift 14");
    auto c = test.parseState("reduce A -> B");
    auto d = test.parseState("reduce A -> C to b ");
//    dbg(a);
//    dbg(b);
//    dbg(c);
//    dbg(d);
//    dbg(test.parseProducer(a.second).first);
//    dbg(test.parseProducer(b.second).first);
//    dbg(test.parseProducer(c.second).first);
//    dbg(test.parseProducer(d.second).first);
//    dbg(test.parseProducer(a.second).second);
//    dbg(test.parseProducer(b.second).second);
//    dbg(test.parseProducer(c.second).second);
//    dbg(test.parseProducer(d.second).second);

}

void testParseProducer() {
    Grammar_Analyzer test;
//    dbg(test.parseProducer("A->B"));
//    dbg(test.parseProducer("A->Base"));
//    dbg(test.parseProducer("A->token"));
//    dbg(test.parseProducer("A->you are fool"));
}


