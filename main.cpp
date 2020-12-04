#include "lexical_analyzer.h"
#include "Grammar_Analyzer.h"


int main() {
//    testModules();
    cout << "Lexical analysis begin" << endl;
    Lexical_analyzer lex;
    lex.lexical();
    cout << "Lexical analysis ends. Output: symbol.txt,token.txt " << endl;
    Grammar_Analyzer gra;
//    dbg(lex.sysTable);
    cout << "Grammar analysis begin" << endl;
    gra.LR1(lex.sysTable);
    cout << "Grammar analysis ends. Output: producer.txt " << endl;

    for (int i = 0; i < gra.midCodeOut.size(); i++) {
        // 如果当前式子的长度为2,并且其左侧不是终结符，那么他必定可以在一个式子的右边出现，那么可以将当前式子的右侧替换过去。
        if (gra.midCodeOut[i].size() == 2 && notEnd(gra.midCodeOut[i][0], gra.NoEndIndex)) {
            for (int j = i + 1; j < gra.midCodeOut.size(); j++) {
                int flag = 0;
                for (int k = 1; k < gra.midCodeOut[j].size(); k++) {
                    if (gra.midCodeOut[j][k] == gra.midCodeOut[i][0]) {
                        gra.midCodeOut[j][k] =gra.midCodeOut[i][1];
                        gra.midCodeOut.erase(gra.midCodeOut.cbegin() + i);
                        i--;
                        flag = 1;
                        break;
                    }
                }
                if (flag) {
                    break;
                }
            }
        } else if (gra.midCodeOut[i].size() == 2) {
            // 如果当前左部是终结符，就去他前面去找能不能换的。
            for (int j = i - 1; j > 0; j--) {
                if (gra.midCodeOut[j][0] == gra.midCodeOut[i][1] && notEnd(gra.midCodeOut[j][0], gra.NoEndIndex)) {
                    gra.midCodeOut[i][1] = gra.midCodeOut[j][1];
                    int k = 2;
                    while(k < gra.midCodeOut[j].size()){
                        gra.midCodeOut[i].push_back(gra.midCodeOut[j][k]);
                        k++;
                    }
                    gra.midCodeOut.erase(gra.midCodeOut.cbegin() + j);
                    break;
                }
            }


        }
    }
    for (auto i:gra.midCodeOut) {
        for (auto j:i) {
            cout << j << " ";
        }
        cout << endl;
    }
    return 0;
}
