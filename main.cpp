#include "lexical_analyzer.h"
#include "Grammar_Analyzer.h"

int main() {
    Lexical_analyzer t;
//    t.lexical();
    Grammar_Analyzer s;
    s.initGotoMap();
    for (int i = 0; i < s.gotoMap.size(); i++) {
        for (int j = 0; j < s.gotoMap[0].size(); ++j) {
            cout << s.gotoMap[i][j] << "\t";
        }
        cout << endl;
    }
    return 0;
}
