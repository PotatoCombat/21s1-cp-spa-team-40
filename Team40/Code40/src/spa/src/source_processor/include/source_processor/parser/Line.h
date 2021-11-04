#pragma once

#include <string>
#include <vector>

using namespace std;

class Line {
private:
    int index;
    vector<string> content;

public:
    Line(int index, vector<string> content);
    int getIndex();
    vector<string> getContent();
};
