#include "Line.h"
using namespace std;

Line::Line(int index, vector<string> content) {
    this->index = index;
    this->content = content;
}
int Line::getIndex() { return index; };
vector<string> Line::getContent() { return content; };
