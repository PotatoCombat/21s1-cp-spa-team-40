#include "SPA.h"
#include "SourceProcessor/Line.cpp"
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
        vector<Line> programLines = {};
        while (getline(file, input)) {
            vector<string> inputLine = parser.parseLine(input);
            if (parser.isProc(inputLine)) {
                stmtNum = 0;
            } else {
                stmtNum++;
            }
            programLines.push_back(Line(stmtNum, inputLine));
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
