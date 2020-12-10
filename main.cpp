#include "lexical_analyzer.h"
#include "Grammar_Analyzer.h"


int main() {
//    testModules();
    cout << "Do you want to start a Lexical analysis? [y/n]" << endl;
    string in;
    cin >> in;
    if (in != "y") {
        return -1;
    }
    cout << "Lexical analysis begin" << endl;
    Lexical_analyzer lex;
    lex.lexical();
    cout << "Lexical analysis ends. Output: symbol.txt,token.txt " << endl;
    Grammar_Analyzer gra;
//    dbg(lex.sysTable);
    cout << "Do you want to start a Grammar analysis? [y/n]" << endl;
    cin >> in;
    if (in != "y") {
        return -1;
    }

    gra.LR1(lex.sysTable);
    ofstream symbolTable("../symbolTable.txt");
    auto iter = gra.sysMap.begin();
    while (iter != gra.sysMap.end()) {
        if (!gra.notEnd(iter->second.symbol, gra.NoEndIndex)) {
            symbolTable << "symbol:\t" << iter->second.symbol << endl;
            symbolTable << "value:\t" << iter->second.value << endl;
            symbolTable << "intValue:" << iter->second.intVal << endl;
            symbolTable << "type:\t" << iter->second.type << endl;
            symbolTable << endl;
        }
        iter++;
    }
    symbolTable.close();
    cout << "Grammar analysis, Semantic analysis and intermediate code generation ends. Output: producer.txt, midCode.txt, symbolTable.txt" << endl;
    cout << "Do you want to generate target code? [y/n]" << endl;
    cin >> in;
    if (in != "y") {
        return -1;
    }
    gra.toAsm();
    cout << "Success! Output: asmCode.txt" << endl;
    return 0;
}
