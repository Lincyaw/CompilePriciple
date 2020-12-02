#include "lexical_analyzer.h"

void Lexical_analyzer::read_program(const char *file, string &prog) {
    cout << file << endl;
    ifstream fin(file);
    if (!fin) {
        cerr << file << " error!" << endl;
        exit(2);
    }
    prog.clear();
    string line;
    while (getline(fin, line)) {
        prog += line + '\n';
    }
}

void Lexical_analyzer::initMap() {
    // token
    this->charMap["auto"] = 1;
    this->charMap["break"] = 2;
    this->charMap["case"] = 3;
    this->charMap["char"] = 4;
    this->charMap["const"] = 5;
    this->charMap["continue"] = 6;
    this->charMap["default"] = 7;
    this->charMap["do"] = 8;
    this->charMap["double"] = 9;
    this->charMap["else"] = 10;
    this->charMap["enum"] = 11;
    this->charMap["extern"] = 12;
    this->charMap["float"] = 13;
    this->charMap["for"] = 14;
    this->charMap["goto"] = 15;
    this->charMap["if"] = 16;
    this->charMap["int"] = 17;
    this->charMap["long"] = 18;
    this->charMap["register"] = 19;
    this->charMap["return"] = 20;
    this->charMap["short"] = 21;
    this->charMap["signed"] = 22;
    this->charMap["sizeof"] = 23;
    this->charMap["static"] = 24;
    this->charMap["struct"] = 25;
    this->charMap["switch"] = 26;
    this->charMap["typedef"] = 27;
    this->charMap["union"] = 28;
    this->charMap["unsigned"] = 29;
    this->charMap["void"] = 30;
    this->charMap["volatile"] = 31;
    this->charMap["while"] = 32;
    this->charMap["-"] = 33;
    this->charMap["--"] = 34;
    this->charMap["-="] = 35;
    this->charMap["->"] = 36;
    this->charMap["!"] = 37;
    this->charMap["!="] = 38;
    this->charMap["%"] = 39;
    this->charMap["%="] = 40;
    this->charMap["&"] = 41;
    this->charMap["&&"] = 42;
    this->charMap["&="] = 43;
    this->charMap["("] = 44;
    this->charMap[")"] = 45;
    this->charMap["*"] = 46;
    this->charMap["*="] = 47;
    this->charMap[","] = 48;
    this->charMap["."] = 49;
    this->charMap["/"] = 50;
    this->charMap["/="] = 51;
    this->charMap[":"] = 52;
    this->charMap[";"] = 53;
    this->charMap["?"] = 54;
    this->charMap["["] = 55;
    this->charMap["]"] = 56;
    this->charMap["^"] = 57;
    this->charMap["^="] = 58;
    this->charMap["{"] = 59;
    this->charMap["|"] = 60;
    this->charMap["||"] = 61;
    this->charMap["|="] = 62;
    this->charMap["}"] = 63;
    this->charMap["~"] = 64;
    this->charMap["+"] = 65;
    this->charMap["++"] = 66;
    this->charMap["+="] = 67;
    this->charMap["<"] = 68;
    this->charMap["<<"] = 69;
    this->charMap["<<="] = 70;
    this->charMap["<="] = 71;
    this->charMap["="] = 72;
    this->charMap["=="] = 73;
    this->charMap[">"] = 74;
    this->charMap[">="] = 75;
    this->charMap[">>"] = 76;
    this->charMap[">>="] = 77;
    this->charMap["\""] = 78;
    this->charMap["\'"] = 79;
    this->charMap["#"] = 80;
    this->charMap["/*注释*/"] = 81;
    this->charMap["常数"] = 82;
    this->charMap["8进制"] = 83;
    this->charMap["16进制"] = 84;
    this->charMap["标识符 "] = ID;
}

bool Lexical_analyzer::getNext(char &ch, string::size_type &pos, const string &program) {
    pos++;
    if (pos >= program.size()) {
        return false;
    } else {
        ch = program[pos];
        return true;
    }
}

pair<int, string> Lexical_analyzer::scanner(const string &program, string::size_type &pos) {
    pair<int, string> ret;
    string token;
    int idx = 0;
    char ch = program[pos];
    while (IS_BLANK(ch)) {
        pos++;
        ch = program[pos];
    }
    // 判断标示符、关键字
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_') {
        while ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_') {
            token += ch;
            if (!getNext(ch, pos, program)) // 获取下一个字符，并且判断有没有到文末
            {
                break;
            }
        }
        // 这里先看做都是其他标示符
        idx = this->charMap.size();

        // 验证是否是关键字
        map<string, int>::const_iterator cit = this->charMap.find(token);
        if (cit != this->charMap.end()) {
            idx = cit->second;
        }
    } else if ((ch >= '0' && ch <= '9') || ch == '.') // 识别常数
    {
        if (ch == '0') { //8进制或16进制
            token += ch;
            getNext(ch, pos, program);
            if (ch == 'x' || ch == 'X') {   // 16进制
                token += ch;
                getNext(ch, pos, program);
                while (ch >= '0' && ch <= '9' || ch >= 'A' && ch <= 'F' || ch == '.') {
                    token += ch;
                    if (!getNext(ch, pos, program)) {
                        break;
                    }
                }
                idx = (int) this->charMap.size() - 1;
            } else {
                while (ch >= '0' && ch <= '7' || ch == '.') {  // 8进制
                    token += ch;
                    if (!getNext(ch, pos, program)) {
                        break;
                    }
                }
                idx = (int) this->charMap.size() - 2;
            }
        } else {
            while (ch >= '0' && ch <= '9' || ch == '.') {
                token += ch;
                if (!getNext(ch, pos, program)) {
                    break;
                }
            }
            idx = (int) this->charMap.size() - 3;  // 常数位于编码表达倒数第二位
        }
        int dot_num = 0;
        for (string::size_type i = 0; i != token.size(); i++) {
            if (token[i] == '.') {
                dot_num++;
            }
        }
        if (dot_num > 1) {  // error
            idx = -1;
        }
    } else {
        map<string, int>::const_iterator cit;
        switch (ch) {
            case '-': // - 操作符
                token += ch;
                if (getNext(ch, pos, program)) {
                    if (ch == '-' || ch == '=' || ch == '>') // -- 操作符
                    {
                        token += ch;
                        getNext(ch, pos, program);
                    }
                }
                cit = this->charMap.find(token);
                if (cit != this->charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '!': // ! 操作符
                token += ch;
                if (getNext(ch, pos, program)) {
                    if (ch == '=') {
                        token += ch;
                        getNext(ch, pos, program);
                    }
                }
                cit = this->charMap.find(token);
                if (cit != this->charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '%': // % 操作符
                token += ch;
                if (getNext(ch, pos, program)) {
                    if (ch == '=') {
                        token += ch;
                        getNext(ch, pos, program);
                    }
                }
                cit = this->charMap.find(token);
                if (cit != this->charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '*':
                token += ch;
                if (getNext(ch, pos, program)) {
                    if (ch == '=') {
                        token += ch;
                        getNext(ch, pos, program);
                    }
                }
                cit = this->charMap.find(token);
                if (cit != this->charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '^':
                token += ch;
                if (getNext(ch, pos, program)) {
                    if (ch == '=') {
                        token += ch;
                        getNext(ch, pos, program);
                    }
                }
                cit = this->charMap.find(token);
                if (cit != this->charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '=':
                token += ch;
                if (getNext(ch, pos, program)) {
                    if (ch == '=') {
                        token += ch;
                        getNext(ch, pos, program);
                    }
                }
                cit = this->charMap.find(token);
                if (cit != this->charMap.end()) {
                    idx = cit->second;
                }
                break;

            case '/':
                token += ch;
                if (getNext(ch, pos, program)) {
                    if (ch == '=') {
                        token += ch;
                        getNext(ch, pos, program);
                    }
                }
                cit = this->charMap.find(token);
                if (cit != this->charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '&':
                token += ch;
                if (getNext(ch, pos, program)) {
                    if (ch == '&' || ch == '=') {
                        token += ch;
                        getNext(ch, pos, program);
                    }
                }
                cit = this->charMap.find(token);
                if (cit != this->charMap.end()) {
                    idx = cit->second;
                }
                break;

            case '|':
                token += ch;
                if (getNext(ch, pos, program)) {
                    if (ch == '|' || ch == '=') {
                        token += ch;
                        getNext(ch, pos, program);
                    }
                }
                cit = this->charMap.find(token);
                if (cit != this->charMap.end()) {
                    idx = cit->second;
                }
                break;

            case '+':
                token += ch;
                if (getNext(ch, pos, program)) {
                    if (ch == '+' || ch == '=') {
                        token += ch;
                        getNext(ch, pos, program);
                    }
                }
                cit = this->charMap.find(token);
                if (cit != this->charMap.end()) {
                    idx = cit->second;
                }
                break;

            case '<':
                token += ch;
                if (getNext(ch, pos, program)) {
                    if (ch == '<') {
                        token += ch;
                        if (getNext(ch, pos, program)) {
                            if (ch == '=') {
                                token += ch;
                                getNext(ch, pos, program);
                            }
                        }
                    } else if (ch == '=') {
                        token += ch;
                        getNext(ch, pos, program);
                    }
                }
                cit = this->charMap.find(token);
                if (cit != this->charMap.end()) {
                    idx = cit->second;
                }
                break;

            case '>':
                token += ch;
                if (getNext(ch, pos, program)) {
                    if (ch == '>') {
                        token += ch;
                        if (getNext(ch, pos, program)) {
                            if (ch == '=') {
                                token += ch;
                                getNext(ch, pos, program);
                            }
                        }
                    } else if (ch == '=') {
                        token += ch;
                        getNext(ch, pos, program);
                    }
                }
                cit = this->charMap.find(token);
                if (cit != this->charMap.end()) {
                    idx = cit->second;
                }
                break;

            case '(':
                token += ch;
                cit = this->charMap.find(token);
                getNext(ch, pos, program);
                if (cit != this->charMap.end()) {
                    idx = cit->second;
                }
                break;
            case ')':
                token += ch;
                cit = this->charMap.find(token);
                getNext(ch, pos, program);
                if (cit != this->charMap.end()) {
                    idx = cit->second;
                }
                break;
            case ',':
                token += ch;
                cit = this->charMap.find(token);
                getNext(ch, pos, program);
                if (cit != this->charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '.':
                token += ch;
                cit = this->charMap.find(token);
                getNext(ch, pos, program);
                if (cit != this->charMap.end()) {
                    idx = cit->second;
                }
                break;
            case ':':
                token += ch;
                cit = this->charMap.find(token);
                getNext(ch, pos, program);
                if (cit != this->charMap.end()) {
                    idx = cit->second;
                }
                break;
            case ';':
                token += ch;
                cit = this->charMap.find(token);
                getNext(ch, pos, program);
                if (cit != this->charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '?':
                token += ch;
                cit = this->charMap.find(token);
                getNext(ch, pos, program);
                if (cit != this->charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '[':
                token += ch;
                cit = this->charMap.find(token);
                getNext(ch, pos, program);
                if (cit != this->charMap.end()) {
                    idx = cit->second;
                }
                break;
            case ']':
                token += ch;
                cit = this->charMap.find(token);
                getNext(ch, pos, program);
                if (cit != this->charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '{':
                token += ch;
                cit = this->charMap.find(token);
                getNext(ch, pos, program);

                if (cit != this->charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '}':
                token += ch;
                cit = this->charMap.find(token);
                getNext(ch, pos, program);
                if (cit != this->charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '~':
                token += ch;
                cit = this->charMap.find(token);
                getNext(ch, pos, program);
                if (cit != this->charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '\\':    // 转移符
                token += ch;
                if (getNext(ch, pos, program)) {
                    token += ch;
                    getNext(ch, pos, program);
                }
                cit = this->charMap.find(token);
                if (cit != this->charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '"':
                token += ch;
                getNext(ch, pos, program);
                cit = this->charMap.find(token);
                if (cit != this->charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '\'':
                token += ch;
                getNext(ch, pos, program);
                cit = this->charMap.find(token);
                if (cit != this->charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '#':
                token += ch;
                getNext(ch, pos, program);
                cit = this->charMap.find(token);
                if (cit != this->charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '\n':
                token += "nextLine";
                pos++;
                ch = program[pos];
                idx = -2;
                break;

            default:
                token += "Error: Not defined";
                pos++;
                ch = program[pos];
                idx = -1;
                break;
        }
    }
    ret.first = idx;
    ret.second = token;
    return ret;
}

pair<int, attributeTable> transfer(pair<int, string> tu) {
    pair<int, attributeTable> temp;
    temp.first = tu.first;
    temp.second.symbol = tu.second;
    return temp;
}

void Lexical_analyzer::lexical() {
    string program;
    ofstream tokenOut("../token.txt");
    ofstream sysOut("../symbol.txt");

    read_program("../program.txt", program);
    string::size_type pos = 0;
    int row = 1;

    initMap();
    int no = 0;
    do {
        auto tu = scanner(program, pos);
        switch (tu.first) {
            case -1:   // error;
                no++;
                cout << no << ": ";
                cout << "Error in row" << row << "!" << '<' << tu.second << "," << tu.first << '>' << endl;
                sysTable.push_back(transfer(tu));
                break;
            case -2:   // go next line;
                row++;
                break;
            default:
                no++;
//                cout << no << ": ";
                tokenOut << '(' << tu.first << "," << tu.second << ')' << endl;
                if (tu.first == ID) {
                    sysMap[tu.second] = systable();
                }
                sysTable.push_back(transfer(tu));
                break;
        }
    } while (pos < program.size());
    tokenOut.close();
    for (auto i:sysMap) {
        sysOut << "(" << i.first << ", " << i.second.type << ", " << i.second.address << ")" << endl;
    }
    sysOut.close();
}
