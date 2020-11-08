#include <iostream>
#include <map>
#include <string>
#include <typeinfo>
#include <fstream>
#include <vector>

using namespace std;

#define IS_BLANK(x) x==' '?true:false
#define ID 83
class SYSTABLE {
public:
    SYSTABLE(string type="-1", string address="?"){
        this->address = address;
        this->type = type;
    }
    string type;
    string address;
};

map<string, int> charMap;

void read_program(const char *file, string &prog) {
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

void initMap() {
    // token
    charMap["auto"] = 1;
    charMap["break"] = 2;
    charMap["case"] = 3;
    charMap["char"] = 4;
    charMap["const"] = 5;
    charMap["continue"] = 6;
    charMap["default"] = 7;
    charMap["do"] = 8;
    charMap["double"] = 9;
    charMap["else"] = 10;
    charMap["enum"] = 11;
    charMap["extern"] = 12;
    charMap["float"] = 13;
    charMap["for"] = 14;
    charMap["goto"] = 15;
    charMap["if"] = 16;
    charMap["int"] = 17;
    charMap["long"] = 18;
    charMap["register"] = 19;
    charMap["return"] = 20;
    charMap["short"] = 21;
    charMap["signed"] = 22;
    charMap["sizeof"] = 23;
    charMap["static"] = 24;
    charMap["struct"] = 25;
    charMap["switch"] = 26;
    charMap["typedef"] = 27;
    charMap["union"] = 28;
    charMap["unsigned"] = 29;
    charMap["void"] = 30;
    charMap["volatile"] = 31;
    charMap["while"] = 32;
    charMap["-"] = 33;
    charMap["--"] = 34;
    charMap["-="] = 35;
    charMap["->"] = 36;
    charMap["!"] = 37;
    charMap["!="] = 38;
    charMap["%"] = 39;
    charMap["%="] = 40;
    charMap["&"] = 41;
    charMap["&&"] = 42;
    charMap["&="] = 43;
    charMap["("] = 44;
    charMap[")"] = 45;
    charMap["*"] = 46;
    charMap["*="] = 47;
    charMap[","] = 48;
    charMap["."] = 49;
    charMap["/"] = 50;
    charMap["/="] = 51;
    charMap[":"] = 52;
    charMap[";"] = 53;
    charMap["?"] = 54;
    charMap["["] = 55;
    charMap["]="] = 56;
    charMap["^"] = 57;
    charMap["^="] = 58;
    charMap["{"] = 59;
    charMap["|"] = 60;
    charMap["||"] = 61;
    charMap["|="] = 62;
    charMap["}"] = 63;
    charMap["~"] = 64;
    charMap["+"] = 65;
    charMap["++"] = 66;
    charMap["+="] = 67;
    charMap["<"] = 68;
    charMap["<<"] = 69;
    charMap["<<="] = 70;
    charMap["<="] = 71;
    charMap["="] = 72;
    charMap["=="] = 73;
    charMap[">"] = 74;
    charMap[">="] = 75;
    charMap[">>"] = 76;
    charMap[">>="] = 77;
    charMap["\""] = 78;
    charMap["\'"] = 79;
    charMap["#"] = 80;
    charMap["/*注释*/"] = 81;
    charMap["常数"] = 82;
    charMap["标识符 "] = ID;
}

bool getNext(char &ch, string::size_type &pos, const string &program) {
    pos++;
    if (pos >= program.size()) {
        return false;
    } else {
        ch = program[pos];
        return true;
    }
}

pair<int, string> scanner(const string &program, string::size_type &pos) {
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
        idx = charMap.size();

        // 验证是否是关键字
        map<string, int>::const_iterator cit = charMap.find(token);
        if (cit != charMap.end()) {
            idx = cit->second;
        }
    } else if ((ch >= '0' && ch <= '9') || ch == '.') // 识别常数
    {
        while (ch >= '0' && ch <= '9' || ch == '.') {
            token += ch;
            if (!getNext(ch, pos, program)) {
                break;
            }
        }
        idx = (int) charMap.size() - 1;  // 常数位于编码表达倒数第二位
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
                cit = charMap.find(token);
                if (cit != charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '!': // ! 操作符
                token += ch;
                if (getNext(ch, pos, program)) {
                    if (ch == '=')
                    {
                        token += ch;
                        getNext(ch, pos, program);
                    }
                }
                cit = charMap.find(token);
                if (cit != charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '%': // % 操作符
                token += ch;
                if (getNext(ch, pos, program)) {
                    if (ch == '=')
                    {
                        token += ch;
                        getNext(ch, pos, program);
                    }
                }
                cit = charMap.find(token);
                if (cit != charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '*':
                token += ch;
                if (getNext(ch, pos, program)) {
                    if (ch == '=')
                    {
                        token += ch;
                        getNext(ch, pos, program);
                    }
                }
                cit = charMap.find(token);
                if (cit != charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '^':
                token += ch;
                if (getNext(ch, pos, program)) {
                    if (ch == '=')
                    {
                        token += ch;
                        getNext(ch, pos, program);
                    }
                }
                cit = charMap.find(token);
                if (cit != charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '=':
                token += ch;
                if (getNext(ch, pos, program)) {
                    if (ch == '=')
                    {
                        token += ch;
                        getNext(ch, pos, program);
                    }
                }
                cit = charMap.find(token);
                if (cit != charMap.end()) {
                    idx = cit->second;
                }
                break;

            case '/':
                token += ch;
                if (getNext(ch, pos, program)) {
                    if (ch == '=')
                    {
                        token += ch;
                        getNext(ch, pos, program);
                    }
                }
                cit = charMap.find(token);
                if (cit != charMap.end()) {
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
                cit = charMap.find(token);
                if (cit != charMap.end()) {
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
                cit = charMap.find(token);
                if (cit != charMap.end()) {
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
                cit = charMap.find(token);
                if (cit != charMap.end()) {
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
                cit = charMap.find(token);
                if (cit != charMap.end()) {
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
                cit = charMap.find(token);
                if (cit != charMap.end()) {
                    idx = cit->second;
                }
                break;

            case '(':
                token += ch;
                cit = charMap.find(token);
                getNext(ch, pos, program);
                if (cit != charMap.end()) {
                    idx = cit->second;
                }
                break;
            case ')':
                token += ch;
                cit = charMap.find(token);
                getNext(ch, pos, program);
                if (cit != charMap.end()) {
                    idx = cit->second;
                }
                break;
            case ',':
                token += ch;
                cit = charMap.find(token);
                getNext(ch, pos, program);
                if (cit != charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '.':
                token += ch;
                cit = charMap.find(token);
                getNext(ch, pos, program);
                if (cit != charMap.end()) {
                    idx = cit->second;
                }
                break;
            case ':':
                token += ch;
                cit = charMap.find(token);
                getNext(ch, pos, program);
                if (cit != charMap.end()) {
                    idx = cit->second;
                }
                break;
            case ';':
                token += ch;
                cit = charMap.find(token);
                getNext(ch, pos, program);
                if (cit != charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '?':
                token += ch;
                cit = charMap.find(token);
                getNext(ch, pos, program);
                if (cit != charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '[':
                token += ch;
                cit = charMap.find(token);
                getNext(ch, pos, program);
                if (cit != charMap.end()) {
                    idx = cit->second;
                }
                break;
            case ']':
                token += ch;
                cit = charMap.find(token);
                getNext(ch, pos, program);
                if (cit != charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '{':
                token += ch;
                cit = charMap.find(token);
                getNext(ch, pos, program);

                if (cit != charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '}':
                token += ch;
                cit = charMap.find(token);
                getNext(ch, pos, program);
                if (cit != charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '~':
                token += ch;
                cit = charMap.find(token);
                getNext(ch, pos, program);
                if (cit != charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '\\':    // 转移符
                token += ch;
                if (getNext(ch, pos, program)) {
                    token += ch;
                    getNext(ch, pos, program);
                }
                cit = charMap.find(token);
                if (cit != charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '"':
                token += ch;
                getNext(ch, pos, program);
                cit = charMap.find(token);
                if (cit != charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '\'':
                token += ch;
                getNext(ch, pos, program);
                cit = charMap.find(token);
                if (cit != charMap.end()) {
                    idx = cit->second;
                }
                break;
            case '#':
                token += ch;
                getNext(ch, pos, program);
                cit = charMap.find(token);
                if (cit != charMap.end()) {
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


int main() {
    string program;
    ofstream tokenOut("../token.txt");
    ofstream sysOut("../symbol.txt");

    read_program("../program.txt", program);
    string::size_type pos = 0;
    int row = 1;
    vector<pair<int, string>> sysTable;
    map<string,SYSTABLE> sysMap;
    initMap();
    int no = 0;
    SYSTABLE g("fuck");
//    cout<<g.name<<" "<<g.type<<" "<<g.address<<endl;
    do {
        auto tu = scanner(program, pos);
        switch (tu.first) {
            case -1:   // error;
                no++;
                cout << no << ": ";
                cout << "Error in row" << row << "!" << '<' << tu.second << "," << tu.first << '>' << endl;
                sysTable.push_back(tu);
                break;
            case -2:   // go next line;
                row++;
                break;
            default:
                no++;
//                cout << no << ": ";
                tokenOut << '(' << tu.first << "," << tu.second << ')' << endl;
                if(tu.first==ID){
                    sysMap[tu.second] = SYSTABLE();
                }
                sysTable.push_back(tu);
                break;
        }
    } while (pos < program.size());
    tokenOut.close();
    for(auto i:sysMap){
        sysOut<<"("<<i.first<<", "<<i.second.type<<", "<<i.second.address<<")"<<endl;
    }
    sysOut.close();
    return 0;
}
