#include "source_processor/Line.h"

Line::Line(int index, vector<string> content) {
    this->index = index;
    this->content = content;
}
int Line::getIndex() { return index; };
vector<string> Line::getContent() { return content; };
