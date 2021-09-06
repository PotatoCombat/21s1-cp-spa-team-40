#include "QueryTokenizer.h"
#include <iostream>
// #include <cstring> // am i able to use c libraries? :(

string QueryTokenizer::trimString(string input) {
    // char* split = strrchr(*(this->query), delimiter);
    string whitespace = " \n\t\r";
    // return first position non-whitespace
    size_t front_w = input.find_first_not_of(whitespace);
    // return last position non-whitespace
    size_t back_w = input.find_last_not_of(whitespace);
    string x;
    if (back_w != string::npos && front_w != string::npos) {
        x = input.substr(front_w, back_w - front_w + 1);
    } else {
        throw "Empty string";
    }
    return x;
}

pair<string, string> QueryTokenizer::splitDecl(string input) {
    string whitespace = " ";
    size_t w_pos = input.find(whitespace);
    if (w_pos == string::npos) {
        throw "No whitespace";
    }
    return make_pair(input.substr(0, w_pos), input.substr(w_pos+1));
}

// split '(' _ ',' _ ')'
tuple<string, string, string> QueryTokenizer::splitBCB(string input) {
    size_t f_bracket = input.find('(');
    size_t b_bracket = input.find(')');
    size_t comma = input.find(',');

    if (f_bracket == string::npos || 
        b_bracket == string::npos ||
        comma == string::npos) {
        throw "Invalid clause";
    }

    string rel = trimString(input.substr(0, f_bracket));
    string ref1 = trimString(input.substr(f_bracket + 1, comma));
    string ref2 = trimString(input.substr(comma + 1, b_bracket - 1));
    cout << rel << endl;
    cout << ref1 << endl;
    cout << ref2 << endl;
    return make_tuple(rel, ref1, ref2);
}

pair<string, string> QueryTokenizer::splitIntoParts(string queryString) {
    char delimiter = ';';
    string whitespace = " \n\t\r";
    int split = queryString.rfind(delimiter);
    string declaration = queryString.substr(0, split + 1);
    try {
        declaration = trimString(declaration);
    } catch (const char *msg) {
        throw "Empty declaration";
    }
    string selectClause = queryString.substr(split + 1);
    try {
        selectClause = trimString(selectClause);
    } catch (const char *msg) {
        throw "Empty select clause";
    }
    return make_pair(declaration, selectClause);
}

vector<pair<string, string>> QueryTokenizer::tokenizeDeclaration(string declaration) {
    // split by ;
    vector<pair<string, string>> decl;
    string::iterator it;
    string::iterator itB;
    it = declaration.begin();
    itB = declaration.begin();
    int found = 0;
    for (; it != declaration.end(); ++it) {
        if (*it == ';') {
            decl.push_back(splitDecl(trimString(string(itB, it))));
            itB = it + 1;
            found = 1;
        }
    }
    return decl;
}

// takes in all non declaration input
string QueryTokenizer::tokenizeReturnEntity(string clause) {
    string re;
    string partial;

    size_t selectKeyword = clause.find("Select"); // length 6
    if (selectKeyword != string::npos) {
        partial = trimString(clause.substr(selectKeyword + 6));
    }

    size_t firstWhitespace = partial.find(" ");
    if (firstWhitespace != string::npos) {
        re = trimString(partial.substr(0, firstWhitespace));
    }
    return re;
}

vector<tuple<string, string, string>> QueryTokenizer::tokenizeSuchThatClause(string clause) {
    string result;
    size_t suchThatPos = clause.find("such that"); // length 9
    
    if (suchThatPos == string::npos) {
        return vector<tuple<string, string, string>>();
    }

    result = trimString(clause.substr(suchThatPos + 9));

    size_t patternPos = clause.find("pattern");    // length 7

    if (patternPos != string::npos) {
        if (patternPos > suchThatPos) {
            result = trimString(result.substr(0, patternPos));
        }
    }

    tuple<string, string, string> tup = splitBCB(result);

    return vector<tuple<string, string, string>>{tup};
}

vector<tuple<string, string, string>> QueryTokenizer::tokenizePatternClause(string clause) {
    string result;
    size_t patternPos = clause.find("pattern");    // length 7

    if (patternPos == string::npos) {
        return vector<tuple<string, string, string>>();
    }

    result = trimString(clause.substr(patternPos + 9));

    size_t suchThatPos = clause.find("such that"); // length 9

    if (suchThatPos != string::npos) {
        if (suchThatPos > patternPos) {
            result = trimString(result.substr(0, suchThatPos));
        }
    }

    tuple<string, string, string> tup = splitBCB(result);

    return vector<tuple<string, string, string>>{tup};
}
