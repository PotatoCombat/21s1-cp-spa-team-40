#include "QueryTokenizer.h"

pair<string, string> QueryTokenizer::splitIntoParts(string queryString) {
    int split = queryString.rfind(SEMICOLON);
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

tuple<vector<DeclTuple>, string, vector<RelTuple>, vector<PatTuple>> QueryTokenizer::tokenize(string input) {
    size_t split;
    vector<string> declarations;
    vector<string> relations;
    vector<string> patterns;
    string ret;
    string substring;
    string rest;
    split = input.rfind(SEMICOLON); // position of semicolon
    if (split == string::npos) {
        throw "No declaration";
    }
    
    substring = trimString(input.substr(0, split + 1)); // end after semicolon
    rest = trimString(input.substr(split + 1)); // start at the char after semicolon

    // settle declarations
    do {
        split = substring.rfind(SEMICOLON);
        if (split == string::npos) {
            declarations.push_back(substring); // push back existing substring
        } else { // add declaration to vector and set substring to remaining string
            declarations.push_back(trimString(substring.substr(split + 1)));
            substring = trimString(substring.substr(0, split + 1));
        }
    } while (split != string::npos);

    if (declarations.size() == 0) {
        throw "No declaration";
    }

    vector<DeclTuple> declTup = tokenizeDeclaration(declarations);

    // settle clauses

    split = rest.find(KEYWORD_SELECT);
    if (split != 0) { // there's something wrong if the first word/char is not Select
        throw "Missing Select / Select in wrong place";
    }

    rest = rest.substr(KEYWORD_SELECT.size()); // throws error if theres no string input
    split = rest.find_first_of(WHITESPACE_SET);
    if (split != 0) {
        throw "Not Select keyword";
    }

    rest = trimLString(rest);

    split = rest.find(WHITESPACE); // position of whitespace
    if (split == string::npos) {
        ret = rest;
        return make_tuple(declarations, ret, vector<RelTuple>(), vector<PatTuple>());
    }
    ret = rest.substr(0, split);
    rest = trimLString(rest.substr(split + 1));
    
    size_t st_pos;
    size_t pt_pos;
    string stString;
    string ptString;

    st_pos = rest.find(KEYWORD_SUCH_THAT);
    pt_pos = rest.find(KEYWORD_PATTERN);
    //while (st_pos != string::npos || pt_pos != string::npos) {
    if (st_pos != string::npos) {
        stString = rest.substr(KEYWORD_SUCH_THAT.size());
        st_pos = stString.find_first_of(WHITESPACE_SET);
        if (st_pos != 0) {
            throw "Not such that keyword";
        }
    }

    if (pt_pos != string::npos) {
        ptString = rest.substr(KEYWORD_PATTERN.size());
        pt_pos = ptString.find_first_of(WHITESPACE_SET);
        if (pt_pos != 0) {
            throw "Not pattern keyword";
        }
    }

    if (st_pos != string::npos && pt_pos != string::npos) {
        if (st_pos < pt_pos) {
            stString = trimString(stString.substr(pt_pos));
        } else {
            ptString = trimString(ptString.substr(st_pos));
        }
    }
    //}

    return make_tuple(declarations, ret, splitBCB(stString), vector<PatTuple>());
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
    RelTuple tup = splitBCB(clause);

    return vector<RelTuple>{tup};
}

//vector<PatTuple> QueryTokenizer::tokenizePatternClause(string clause) {
//    PatTuple tup = splitBCB(result);
//
//    return vector<PatTuple>{tup};
//}


string QueryTokenizer::trimString(string input) {
    size_t front_w = input.find_first_not_of(WHITESPACE_SET);
    size_t back_w = input.find_last_not_of(WHITESPACE_SET);
    string x;
    if (back_w != string::npos && front_w != string::npos) {
        x = input.substr(front_w, back_w - front_w + 1);
    }
    else {
        throw "Empty string"; // TODO: Test
    }
    return x;
}

string QueryTokenizer::trimLString(string input) {
    size_t front_w = input.find_first_not_of(WHITESPACE_SET);
    if (front_w == string::npos) {
        throw "Empty string"; // TODO: Test
    }
    return input.substr(front_w);
}

string QueryTokenizer::trimRString(string input) {
    size_t back_w = input.find_last_not_of(WHITESPACE_SET);
    if (back_w == string::npos) {
        throw "Empty string"; // TODO: Test
    }
    return input.substr(0, back_w + 1);;
}


DeclTuple QueryTokenizer::splitDecl(string input) {
    string whitespace = " ";
    size_t w_pos = input.find(whitespace);
    if (w_pos == string::npos) {
        throw "No whitespace";
    }
    return make_pair(input.substr(0, w_pos), input.substr(w_pos + 1));
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

