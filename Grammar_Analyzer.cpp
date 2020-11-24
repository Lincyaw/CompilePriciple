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
    symbolStack.push_back('#');
    input = std::move(Input);
    Input.push_back('#');

    pointer = 0;
    // 栈顶符号
    int S = stateStack.back();
    // 输入的字符串的第一个字符
    auto a = input[pointer];
    while (a != '#') {
        auto action = parseState(actionMap[S][a]);
        if (action.first == 's') {
            stateStack.push_back(action.second);
            symbolStack.push_back(action.first);
            pointer++;
        } else if (action.first == 'r') {
            for (int i = 0; i < producer[action.second].second.size(); i++) { // 2*|beta|.size()
                symbolStack.pop_back();
                stateStack.pop_back();
            }
            cout << "symbolStack.size = " << symbolStack.size() << ",  stateStack.size = " << stateStack.size() << endl;
            S = stateStack.back();
            char t = producer[action.second].first[0];
            symbolStack.push_back(t); // 左部产生式只有一个字符
            stateStack.push_back(gotoMap[S][(int) t]);
            cout << producer[action.second].first << "->" << producer[action.second].second << endl;
        } else if (action.first == 'a') {
            return;
        } else {
            exit(1);
        }
        pointer++;
        a = input[pointer];
    }

}

pair<char, int> Grammar_Analyzer::parseState(const string &action) {
    if (action.empty() || action.size() > 2) {
        exit(1);
    }
    if (action.size() == 1) {
        return {'e', action[0]};
    }
    return {action[0], action[1]};
}

void Grammar_Analyzer::initProducer() {
    //TODO
    // 产生式的初始化
}


