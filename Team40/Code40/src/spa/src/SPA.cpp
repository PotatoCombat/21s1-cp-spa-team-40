#include "SPA.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

void SPA::processSource(string filename) {
    fstream file;
    file.open(filename, ios::in);
    if (file.is_open()) {
        string input;
        int stmtNum;
        vector<vector<string>> program = {};
        while (getline(file, input)) {
            vector<string> inputLine = parser.parseLine(input);
            program.push_back(inputLine);
            // if (parser.isProc(inputLine)) {
            //     stmtNum = 0;
            // } else if (parser.isStmt(inputLine)) {
            //     stmtNum++;
            // }
            // cout << input << "\n";
        }
        file.close();
    } else {
        // cout << "No such file";
    }
    file.close();
}

int main() { // For testing on Fatin's machine only
    SPA spa;
    spa.processSource("C:\\Users\\Admin\\source\\repos\\21s1-cp-spa-team-"
                      "40\\Team40\\Tests40\\Sample_source.txt");
}
