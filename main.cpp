#include "lexical_analyzer.h"
#include "Grammar_Analyzer.h"

int main() {
//    testModules();
    cout<<"词法分析开始"<<endl;
    Lexical_analyzer lex;
    lex.lexical();
    cout<<"词法分析结束,输出symbol.txt,token.txt"<<endl;
    Grammar_Analyzer gra;
//    dbg(lex.sysTable);
    cout<<"语法分析开始"<<endl;
    gra.LR1(lex.sysTable);
    cout<<"语法分析结束,输出producer.txt"<<endl;
    return 0;
}
