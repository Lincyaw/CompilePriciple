#include "lexical_analyzer.h"
#include "Grammar_Analyzer.h"

int main() {
//    testModules();
    cout<<"Lexical analysis begin"<<endl;
    Lexical_analyzer lex;
    lex.lexical();
    cout<<"Lexical analysis ends. Output: symbol.txt,token.txt "<<endl;
    Grammar_Analyzer gra;
//    dbg(lex.sysTable);
    cout<<"Grammar analysis begin"<<endl;
    gra.LR1(lex.sysTable);
    cout<<"Grammar analysis ends. Output: producer.txt "<<endl;
    return 0;
}
