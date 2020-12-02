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
void Grammar_Analyzer::LR1(vector<pair<int, attributeTable>> Input) {
    // 初始化状态栈和符号栈,以及输入的句子
    stateStack.push_back(0);
    symbolStack.emplace_back("$","$");
    pair<int, attributeTable> temp1;
    temp1.first  = -5;
    temp1.second = attributeTable("$","$");
    Input.emplace_back(temp1);
    ofstream producerOut("../producer.txt");
    pointer = 0;
    // 栈顶符号
    int S;
    // 输入的字符串的第一个字符
    pair<int, attributeTable> a = Input[pointer];

//    dbg(actionMap, gotoMap);
//TODO: 在这里遇到数字或者id之后，就将他们的值存到下面的TODO里说的数据结构里
    while (!symbolStack.empty()) {
//        dbg("round------------------------------------------------------");
//        dbg(a);
        S = stateStack.back();
//        dbg(symbolStack, stateStack);
//        dbg(a);
//        dbg(S);
//        dbg(index[a.second]);

        auto LookAction = actionMap[S][index[switcher(a.second)]];
//        dbg(LookAction);
        auto action = parseState(actionMap[S][index[switcher(a)]]);


        if (action.first == "s") {
//            if(a.second=="$")
//                return;
            stateStack.emplace_back(atoi(action.second.c_str()));
            symbolStack.emplace_back(switcher(a), a.second);
            pointer++;
            a = Input[pointer];
        } else if (action.first == "r") {
            auto producer = parseProducer(action.second);

            // 存放产生式左部
            producerWithAttr.emplace_back(producer.first,producer.first);
            for (int i = 0; i < producer.second.size(); i++) {
                // TODO:要在出栈的时候处理一下数据
                // 如： 将弹出的数据们赋值给producer.second里的字符串们。
                // 那可能要把producer的数据结构改一改

                // 存放产生式右部
                producerWithAttr.emplace_back(symbolStack.back());
                symbolStack.pop_back();
                stateStack.pop_back();
            }
            // TODO： 在这里处理产生式的赋值！！！！ 套个函数 加个case！！！
            auto S1 = stateStack.back();

            translate(producerWithAttr);
            symbolStack.emplace_back(producerWithAttr[0]); // 左部产生式只有一个字符
            stateStack.push_back(gotoMap[S1][NoEndIndex[producer.first]]);

            // TODO: 在这里进行生成中间代码，producer.first是产生式头部，producer.second 是一个vector，存放着产生式尾部。
            // 目前想法：使用map，为产生式中的每个符号都创建一个属性表
            // 数据结构大致如下：
            // map<string, MYSTRUCT> t;
            // typedef struct MYSTRUCT{
            //      int val;
            //      auto type;
            //      auto syn;
            //      auto inh;
            //}
            producerOut << producer.first << " -> ";
            for (int i = 0; i < producer.second.size(); i++) {
                producerOut << producer.second[i] << " ";

            }
            producerOut << endl;

//            translate(producer);


//            dbg(S1);
//            dbg(producer.first);
//            dbg(producer.second);
//            dbg(NoEndIndex[producer.first]);
        } else if (LookAction == "accept") {
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

void Grammar_Analyzer::translate(vector<attributeTable> &pro) {
    if (pro[0].symbol == "E'") {

    } else if (pro[0].symbol == "S'") {
        if (pro[1].symbol == "type") {
            if (pro[2].symbol == "token") {
                if (pro.size() == 3) {
                    // S' -> type token;
                    pro[2].type = pro[1].type;
                } else if (pro[3].symbol == "=") {
                    // S' -> type token = S;
                    pro[2].type = pro[1].type;
                    pro[2].value = pro[3].value;
                    pro[2].intVal = pro[3].intVal;
                } else if (pro[3].symbol == "[") {
                    // S' -> type token [ constV ];
                    exit(-1);
                } else if (pro[3].symbol == "(") {
                    // S' -> type token ( list ) { C };
                    exit(-1);
                } else {
                    cout << "error" << endl;
                    exit(-1);
                }
            } else {
                cout << "error" << endl;
                exit(-1);
            }

        } else if (pro[1].symbol == "token") {
            switch (pro.size()) {
                case 5: //S' -> token ( list )
                    exit(-1);
                    break;
                case 4: // S' -> token = S;
                    pro[1].value = pro[2].value;
                    pro[1].intVal = pro[2].intVal;
                    break;
                case 7: // S' -> token [ constV ] = S;
                    exit(-1);
                    break;
                default:
                    cout << "error" << endl;
                    exit(-1);
                    break;
            }
        } else if (pro[1].symbol == " ") {
            // S' ->  ;

        } else if (pro[1].symbol == "for") {
            // S' ->  for ( S' ; S , A) { C };
            exit(-1);
        } else {
            cout << "error" << endl;
            exit(-1);
        }
    } else if (pro[0].symbol == "S") {
        switch (pro.size()) {
            case 2:
                // S -> A;
                pro[0].intVal = pro[1].intVal;
                pro[0].value = pro[1].value;
                pro[0].type = pro[1].type;
                break;
            case 4:
                if (pro[2].symbol == "+") {
                    // S -> S + A;
                    pro[0].intVal = pro[0].intVal + pro[1].intVal;
                } else if (pro[2].symbol == "-") {
                    // S -> S - A;
                    pro[0].intVal = pro[0].intVal - pro[1].intVal;
                } else if (pro[2].symbol == "compOp") {
                    // S -> S compOp A;
                    if (pro[2].value == ">") {
                        pro[0].intVal = pro[1].intVal + pro[3].intVal;
                    } else if (pro[2].value == "<") {
                        pro[0].intVal = pro[1].intVal < pro[3].intVal;
                    } else if (pro[2].value == "<=") {
                        pro[0].intVal = pro[1].intVal <= pro[3].intVal;
                    } else if (pro[2].value == ">=") {
                        pro[0].intVal = pro[1].intVal >= pro[3].intVal;
                    } else if (pro[2].value == "==") {
                        pro[0].intVal = pro[1].intVal == pro[3].intVal;
                    } else {
                        cout << "error" << endl;
                        exit(-1);
                    }
                } else {
                    cout << "error" << endl;
                    exit(-1);
                }
                break;
            case 5:
                // S -> token ( list );
                exit(-1);
                break;
            default:
                cout << "error" << endl;
                exit(-1);
        }
    } else if (pro[0].symbol == "A") {
        switch (pro.size()) {
            case 4:
                if (pro[2].symbol == "*") {
                    // A -> A * B
                    pro[0].intVal = pro[1].intVal * pro[3].intVal;
                } else if (pro[2].symbol == "/") {
                    // A -> A / B
                    pro[0].intVal = pro[1].intVal / pro[3].intVal;
                } else {
                    cout << "error" << endl;
                    exit(-1);
                }
                break;
            case 3:
                if (pro[2].symbol == "++") {
                    // A -> A ++;
                    pro[0].intVal = pro[1].intVal + 1;
                    pro[1].intVal++;
                } else if (pro[2].symbol == "--") {
                    // A -> A --;
                    pro[0].intVal = pro[1].intVal - 1;
                    pro[1].intVal--;
                } else {
                    cout << "error" << endl;
                    exit(-1);
                }
                break;
            case 2:
                // A -> B;
                pro[0].intVal = pro[1].intVal;
                pro[0].value = pro[1].value;
                pro[0].type = pro[1].type;
                break;
            default:
                cout << "error" << endl;
                exit(-1);
        }
    } else if (pro[0].symbol == "B") {
        switch (pro.size()) {
            case 4:
                // B -> ( S )
                pro[0].intVal = pro[2].intVal;
                pro[0].value = pro[2].value;
                pro[0].type = pro[2].type;
                break;
            case 2:
                if (pro[1].symbol == "token") {
                    // B -> token;
                    pro[0].intVal = pro[1].intVal;
                    pro[0].value = pro[1].value;
                    pro[0].type = pro[1].type;
                } else if (pro[1].symbol == "constV") {
                    // B -> constV;
                    pro[0].intVal = pro[1].intVal;
                    pro[0].value = pro[1].value;
                    pro[0].type = pro[1].type;
                } else {
                    cout << "error" << endl;
                    exit(-1);
                }
                break;
        }
    } else if (pro[0].symbol == "list") {
        switch (pro.size()) {
            case 2:
                if (pro[1].symbol == "token") {
                    // list -> token
                    pro[0].intVal = pro[1].intVal;
                    pro[0].value = pro[1].value;
                    pro[0].type = pro[1].type;
                } else if (pro[1].symbol == " ") {
                    // list -> " "
                } else {
                    cout << "error" << endl;
                    exit(-1);
                }
                break;
            case 4:
                // list -> token , list;
                exit(1);
                break;
            default:
                cout << "error" << endl;
                exit(-1);
        }
    } else if (pro[0].symbol == "type") {
        if (pro[1].symbol == "int") {
            pro[1].type = MY_INT;
            pro[0].type = MY_INT;
        } else if (pro[1].symbol == "char") {
            pro[1].type = MY_CHAR;
            pro[0].type = MY_CHAR;
        } else if (pro[1].symbol == "void") {
            pro[1].type = MY_VOID;
            pro[0].type = MY_VOID;
        } else {
            cout << "error" << endl;
            exit(-1);
        }
    } else if (pro[0].symbol == "compOp") {
        if (pro[1].symbol == ">") {
            pro[0].value = ">";
        } else if (pro[1].symbol == "<") {
            pro[0].value = "<";

        } else if (pro[1].symbol == "<=") {
            pro[0].value = "<=";

        } else if (pro[1].symbol == ">=") {
            pro[0].value = ">=";

        } else if (pro[1].symbol == "==") {
            pro[0].value = "==";

        } else {
            cout << "error" << endl;
            exit(-1);
        }
    } else if (pro[0].symbol == "C") {
        if (pro[1].symbol == "S'") {
            // C -> S'
            pro[0].intVal = pro[1].intVal;
            pro[0].value = pro[1].value;
            pro[0].type = pro[1].type;
        } else if (pro[1].symbol == "C") {
            //C -> C # S'
        } else {
            cout << "error" << endl;
            exit(-1);
        }
    }
}

string Grammar_Analyzer::switcher(const pair<int, string> &input) {
    switch (input.first) {
        case ID:
            return "token";
        case 82:
            return "constV";
        case 83:
            return "constV";
        default:
            return input.second;
    }
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


