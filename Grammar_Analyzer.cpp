//
// Created by llincyaw on 2020/11/12.
//

#include "Grammar_Analyzer.h"
#include <utility>

void Grammar_Analyzer::initActionMap() {
    ifstream fp("../input.txt"); //定义声明一个ifstream对象，指定文件路径
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
    ifstream fp("../testinput.txt"); //定义声明一个ifstream对象，指定文件路径
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
    symbolStack.emplace_back("#");
    Input.emplace_back(-1, "#");

    pointer = 0;
    // 栈顶符号
    int S = stateStack.back();
    // 输入的字符串的第一个字符
    pair<int, string> a = Input[pointer];
    while (a.second != "#") {
        // TODO: 如果a.fisrt 类型是标识符，则要把action.second变成语法分析表里有的.常数同理
        auto action = parseState(actionMap[S][index[a.second]]);
        if (action.first == "s") {
            stateStack.push_back(atoi(action.second.c_str()));
            symbolStack.emplace_back(action.first);
//            pointer++;
        } else if (action.first == "r") {
            auto producer = parseProducer(action.second);
            for (int i = 0; i < producer.second.size(); i++) { // 2*|beta|.size()
                symbolStack.pop_back();
                stateStack.pop_back();
            }
            cout << "symbolStack.size = " << symbolStack.size() << ",  stateStack.size = " << stateStack.size() << endl;
            S = stateStack.back();
            symbolStack.push_back(producer.first); // 左部产生式只有一个字符
            stateStack.push_back(gotoMap[S][index[producer.first]]);
            dbg(producer);
        } else if (action.first == "a") {
            return;
        } else {
            exit(1);
        }
        pointer++;
        a = Input[pointer];
    }

}

pair<string, string> Grammar_Analyzer::parseState(const string &action) {

    if (action.size() <= 2) {
        cout << "should not use this state." << endl;
        exit(1);
    }
    string token;
    pair<string, string> ans;
    for (auto i:action) {
//        dbg(token);
        if (i == ' ') {
            if (token == "reduce") {
                ans.first = "r";
                token = "";
            } else if (token == "shift") {
                ans.first ="s";
            } else if (token == "accept") {
                ans.first = "a";
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


void Grammar_Analyzer::initIndex() {
    ifstream fp("../index.txt"); //定义声明一个ifstream对象，指定文件路径
    string line;
    int count = 0;
    while (getline(fp, line)) { //循环读取每行数据
        string token;
        for (auto i:line) {
            if (i == '\t') {
                index[token] = count;
                count++;
                token = "";
                continue;
            }
            token += i;
        }
    }
    dbg(index);
}

pair<string, string> Grammar_Analyzer::parseProducer(const string &input) {
    string token;
    pair<string, string> ans;
    for (int i = 0; i < input.size(); i++) {
        if (input[i] == '-' && input[i + 1] == '>') {
            ans.first = token;
            token="";
            i++;
        } else {
            token += input[i];
        }
    }
    ans.second=token;
    return ans;
}

void testModules() {
    testParseState();
    testParseProducer();
}

void testParseState() {
    Grammar_Analyzer test;
    dbg(test.parseState("shift 53"));
    dbg(test.parseState("shift 14"));
    dbg(test.parseState("reduce A -> B"));
    dbg(test.parseState("reduce A -> C"));
    dbg(test.parseState("reduce           B        -> token"));

}

void testParseProducer() {
    Grammar_Analyzer test;
    dbg(test.parseProducer("A->B"));
    dbg(test.parseProducer("A->Base"));
    dbg(test.parseProducer("A->token"));
    dbg(test.parseProducer("A->youarefool"));
}


