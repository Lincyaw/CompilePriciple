#include "lexical_analyzer.h"
#include "Grammar_Analyzer.h"

int main() {
//    testModules();
    Lexical_analyzer lex;
    lex.lexical();
    Grammar_Analyzer gra;
    gra.initGotoMap();
    gra.initActionMap();
    gra.initIndex();
    dbg(lex.sysTable);
    gra.LR1(lex.sysTable);

    return 0;
}
