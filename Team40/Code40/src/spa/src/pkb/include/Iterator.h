#pragma once

using namespace std;

template<typename T>
class Iterator {
public:
    Iterator(vector<T> items);

    template<typename Predicate>
    Iterator<T> filter(Predicate pred);

    vector<T> asVector();
private:
    vector<T> items;
};

template<typename T>
Iterator<T>::Iterator(vector<T> items) {
    this->items = items;
}

template<typename T>
template<typename Predicate>
Iterator<T> Iterator<T>::filter(Predicate pred) {
    vector<T> out;
    copy_if(items.begin(), items.end(), back_inserter(out), pred);
    items = out;
    return *this;
}

template<typename T>
vector<T> Iterator<T>::asVector() {
    return items;
}
