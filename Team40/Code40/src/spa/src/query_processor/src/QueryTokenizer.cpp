#include "QueryTokenizer.h"

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
        throw "Empty string"; // TODO: Test
    }
    return x;
}

DeclTuple QueryTokenizer::splitDecl(string input) {
    string whitespace = " ";
    size_t w_pos = input.find(whitespace);
    if (w_pos == string::npos) {
        throw "No whitespace";
    }
    return make_pair(input.substr(0, w_pos), input.substr(w_pos+1));
}

// split '(' _ ',' _ ')'
tuple<string, string, string> QueryTokenizer::splitBCB(string input) {
    string partial;
    string rel;
    string ref1;
    string ref2;
    
    size_t pos = input.find('(');
    if (pos == string::npos) {
        throw "Invalid clause";
    }
    rel = trimString(input.substr(0, pos));
    partial = input.substr(pos + 1);

    pos = partial.find(',');
    if (pos == string::npos) {
        throw "Invalid clause";
    }
    ref1 = trimString(partial.substr(0, pos));
    partial = partial.substr(pos + 1);

    pos = partial.find(')');
    if (pos == string::npos) {
        throw "Invalid clause";
    }
    ref2 = trimString(partial.substr(0, pos));

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
        throw "Empty declaration";  // TODO: Test
    }
    string selectClause = queryString.substr(split + 1);
    try {
        selectClause = trimString(selectClause);
    } catch (const char *msg) {
        throw "Empty select clause";  // TODO: Test
    }
    return make_pair(declaration, selectClause);
}

// return <designentity, syn>
vector<DeclTuple> QueryTokenizer::tokenizeDeclaration(string declaration) {
    // split by ;
    // TODO: handle ,
    vector<DeclTuple> decl;
    string::iterator it;
    string::iterator itB;
    it = declaration.begin();
    itB = declaration.begin();
    // int found = 0;
    for (; it != declaration.end(); ++it) {
        if (*it == ';') {
            decl.push_back(splitDecl(trimString(string(itB, it))));
            itB = it + 1;
            // found = 1;
        }
    }
    return decl;
}

// takes in all non declaration input
string QueryTokenizer::tokenizeReturnEntity(string clause) {
    string re;
    string partial;

    size_t selectKeyword = clause.find("Select"); // length 6
    // TODO: Test truth for the following lines
    if (selectKeyword != string::npos) {
        partial = trimString(clause.substr(selectKeyword + 6));
    }

    size_t firstWhitespace = partial.find(" ");
    if (firstWhitespace != string::npos) {
        re = trimString(partial.substr(0, firstWhitespace));
    }
    return re;
}

vector<RelTuple> QueryTokenizer::tokenizeSuchThatClause(string clause) {
    string result;
    size_t suchThatPos = clause.find("such that"); // length 9
    
    if (suchThatPos == string::npos) {
        return vector<RelTuple>();
    }

    result = trimString(clause.substr(suchThatPos + 9));

    size_t patternPos = clause.find("pattern");    // length 7

    if (patternPos != string::npos) {
        if (patternPos > suchThatPos) {
            result = trimString(result.substr(0, patternPos));
        }
    }

    RelTuple tup = splitBCB(result);

    return vector<RelTuple>{tup};
}

//vector<PatTuple> QueryTokenizer::tokenizePatternClause(string clause) {
//    string result;
//    size_t patternPos = clause.find("pattern");    // length 7
//
//    if (patternPos == string::npos) {
//        return vector<PatTuple>();
//    }
//
//    result = trimString(clause.substr(patternPos + 9));
//
//    size_t suchThatPos = clause.find("such that"); // length 9
//
//    if (suchThatPos != string::npos) {
//        if (suchThatPos > patternPos) {
//            result = trimString(result.substr(0, suchThatPos));
//        }
//    }
//
//    tuple<string, string, string> strings = splitBCB(result);
//    PatTuple tup = strings;
//
//    return vector<PatTuple>{tup};
//}
