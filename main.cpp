#include "lexical_analyzer.h"
#include "Grammar_Analyzer.h"

int main() {
//    testModules();
    Lexical_analyzer lex;
    lex.lexical();
    Grammar_Analyzer gra;
    dbg(lex.sysTable);
//    for(auto i:gra.index){
//        cout<<i.first<<"  "<<i.second<<endl;
//    }
    gra.LR1(lex.sysTable);
    cout<<"finished"<<endl;

    return 0;
}
