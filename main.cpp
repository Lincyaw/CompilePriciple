#include "lexical_analyzer.h"
#include "Grammar_Analyzer.h"
int main() {
    Lexical_analyzer t;
//    t.lexical();
    Grammar_Analyzer s;
    s.initGotoMap();
    s.initActionMap();

//    for (int i = 0; i < s.gotoMap.size(); i++) {
//        for (int j = 0; j < s.gotoMap[0].size(); ++j) {
//            cout << s.gotoMap[i][j] << "\t";
//        }
//        cout << endl;
//    }

    for (int i = 0; i < s.actionMap.size(); i++) {
        for (int j = 0; j < s.actionMap[0].size(); ++j) {
            cout<<"i = "<<i<<"  j = "<<j<<"  ";
            cout << s.actionMap[i][j] << endl;
        }
        cout << endl;
        cout << endl;
    }

    return 0;
}
