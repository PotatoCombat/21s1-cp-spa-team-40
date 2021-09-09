#pragma once

#include <map>

#include "Reference.h"

template <typename T> struct map_init_helper {
    T &m;
    map_init_helper(T &m) : m(m) {}
    map_init_helper &operator()(typename T::key_type const &key,
                                typename T::mapped_type const &value) {
        m[key] = value;
        return *this;
    }
};

template <typename T> map_init_helper<T> map_init(T &t) {
    return map_init_helper<T>(t);
}

// take in string and return DesignEntityType

DesignEntityType stringToDeType(string de) {
    DesignEntityType type;
    if (de == "stmt") {
        type = DesignEntityType::STMT;
    } else if (designEntity == "assign") {
        type = DesignEntityType::ASSIGN;
    } else if (designEntity == "variable") {
        type = DesignEntityType::VARIABLE;
    } else if (designEntity == "constant") {
        type = DesignEntityType::CONSTANT;
    } else if (designEntity == "procedure") {
        type = DesignEntityType::PROCEDURE;
    } else if (designEntity == "read") {
        type = DesignEntityType::READ;
    } else if (designEntity == "print") {
        type = DesignEntityType::PRINT;
    } else if (designEntity == "while") {
        type = DesignEntityType::WHILE;
    } else if (designEntity == "if") {
        type = DesignEntityType::IF;
    } else if (designEntity == "call") {
        type = DesignEntityType::CALL;
    } else {
        throw "error";
    }
    return type;
}

//#pragma once
//#pragma warning( push )
//#pragma warning( disable : 26812 )
//
//#include <string>

// using namespace std;
//
// class DesignEntityType {
// private:
//    uint8_t val;
//
// public:
//    enum Value : uint8_t {
//        STMT,
//        ASSIGN,
//        VARIABLE,
//        CONSTANT,
//        PROCEDURE,
//        READ,
//        PRINT,
//        WHILE,
//        IF,
//        CALL
//    };
//
//    DesignEntityType() = default;
//    DesignEntityType(Value val) : val(val) {}
//
//    bool operator==(DesignEntityType a) const { return val == a.val; }
//    bool operator!=(DesignEntityType a) const { return val != a.val; }
//
//    string toString() {
//        switch (val) {
//        case DesignEntityType::STMT:
//            return "stmt";
//        case DesignEntityType::ASSIGN:
//            return "assign";
//        case DesignEntityType::VARIABLE:
//            return "variable";
//        case DesignEntityType::CONSTANT:
//            return "constant";
//        case DesignEntityType::PROCEDURE:
//            return "procedure";
//        case DesignEntityType::READ:
//            return "read";
//        case DesignEntityType::PRINT:
//            return "print";
//        case DesignEntityType::WHILE:
//            return "while";
//        case DesignEntityType::IF:
//            return "if";
//        case DesignEntityType::CALL:
//            return "call";
//        default:
//            return "invalid type";
//        }
//    }
//};

//#pragma warning(pop)
