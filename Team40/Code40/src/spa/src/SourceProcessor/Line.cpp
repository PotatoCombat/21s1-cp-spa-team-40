#include <string>
#include <vector>
using namespace std;

class Line {
private:
    int index;
    vector<string> content;

public:
    Line(int index, vector<string> content) {
        this->index = index;
        this->content = content;
    }
    int getIndex() { return index; };
    vector<string> getContent() { return content; };
};
