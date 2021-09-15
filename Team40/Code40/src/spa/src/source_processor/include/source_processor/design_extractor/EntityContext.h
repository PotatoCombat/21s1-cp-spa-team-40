#include <vector>

using namespace std;

template <class T> class EntityContext {
private:
    vector<T *> stack;

public:
    EntityContext() = default;

    void push(T *entity) { stack.push_back(entity); };

    void pop(T *entity) {
        if (stack.empty()) {
            throw runtime_error(
                "Attempting to pop entity while context stack is empty.");
        }
        if (stack.back() != entity) {
            throw runtime_error("Attempting to pop entity while it is not at "
                                "the top of the context stack.");
        }
        return stack.pop_back();
    }

    // Creates and returns a copy of the stack
    vector<T *> getCopy() {
        const vector<T *> copy(stack);
        return copy;
    }

    void reset() { stack.clear(); }

    bool empty() { return stack.empty(); }
};