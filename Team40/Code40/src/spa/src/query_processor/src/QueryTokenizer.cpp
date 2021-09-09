#include "QueryTokenizer.h"

pair<string, string> QueryTokenizer::separateDeclaration(string input) {
    size_t split = input.rfind(SEMICOLON);

    if (split == string::npos) {
        throw "error"; // no declaration
    }

    string firstHalf = trim(input.substr(0, split + 1));
    string secondHalf = trim(input.substr(split + 1));

    return make_pair(firstHalf, secondHalf);
}

string QueryTokenizer::tokenizeReturn(string input, string &remaining) {
    size_t first_w = findFirstWhitespace(input);
    string sub;
    string rem;

    if (first_w == string::npos) {
        //remaining = "";
        //return "";
        throw "error"; // no return
    }

    sub = input.substr(0, first_w);
    rem = trimL(input.substr(first_w + 1));

    if (sub != KEYWORD_SELECT) {
        throw "error"; // no return identifier
    }

    size_t next_w = findFirstWhitespace(rem);

    if (next_w == string::npos) {
        remaining = "";
        return rem;
    }

    sub = rem.substr(0, next_w);
    rem = trimL(rem.substr(next_w + 1));
    remaining = rem;
    return sub;
}

void QueryTokenizer::tokenizeDeclaration(string input, vector<DeclPair> &decls) {
    //vector<DeclPair> decls;
    size_t sem_pos;
    size_t space_pos;
    string type;

    sem_pos = input.find(SEMICOLON); // position of semicolon
    while (sem_pos != string::npos) {
        string sub = input.substr(0, sem_pos); // e.g. assign x, y
        space_pos = sub.find(WHITESPACE);      // position of whitespace
        string type = sub.substr(0, space_pos);

        // check if first part is a design entity type
        deHelper.getType(type); // throws if not

        // remaining string should have either
        // comma, comma + whitespace, none
        string rem = trim(sub.substr(space_pos + 1));
        vector<string> declSyn;
        splitComma(rem, declSyn);
        for (auto x : declSyn) {
            decls.push_back(make_pair(type, x));
        }
    }

    return;
}

void QueryTokenizer::tokenizeClause(string input, vector<RelTuple>& rels, vector<PatTuple>& pats) {
    size_t first_b = input.find(R_BRACKET); // find the first close bracket

    if (first_b == string::npos) {
        throw "error"; // no close bracket
    }
    
    string sub = input.substr(0, first_b + 1);
    string rem = trimL(input.substr(first_b + 1));

    RelTuple relT;
    splitBCBRel(sub, relT);

    PatTuple patT;
    //splitBCBRel(sub, patT);

    rels.push_back(relT);
    //pats.push_back(patT);
    return;
}

// helper functions
string QueryTokenizer::trim(string input) {
    size_t front_w = input.find_first_not_of(WHITESPACE_SET);
    size_t back_w = input.find_last_not_of(WHITESPACE_SET);
    if (back_w != string::npos && front_w != string::npos) {
        return input.substr(front_w, back_w - front_w + 1);
    }
    // throw "Empty string";
    return "";
}

string QueryTokenizer::trimL(string input) {
    size_t front_w = input.find_first_not_of(WHITESPACE_SET);
    if (front_w != string::npos) {
        return input.substr(front_w);
    }
    return "";
}

string QueryTokenizer::trimR(string input) {
    size_t back_w = input.find_last_not_of(WHITESPACE_SET);
    if (back_w != string::npos) {
        return input.substr(back_w);
    }
    return "";
}

void QueryTokenizer::splitComma(string input, vector<string> &vec) {
    string rem = input;

    size_t split = rem.find(COMMA);
    while (split != string::npos) {
        string sub = trimR(rem.substr(0, split));
        vec.push_back(sub);
        rem = trimL(input.substr(split + 1));
        split = rem.find(COMMA);
    }
    if (rem.size() > 0) {
        vec.push_back(rem);
    }

    return;
}

size_t QueryTokenizer::findFirstWhitespace(string input) {
    size_t first_w = input.find_first_of(WHITESPACE_SET);
    return first_w;
}

void QueryTokenizer::splitBCBRel(string input, RelTuple tup) {
    // check count
    int l_c = count(input.begin(), input.end(), L_BRACKET);
    int r_c = count(input.begin(), input.end(), R_BRACKET);
    int c_c = count(input.begin(), input.end(), COMMA);

    if (!(l_c == 1 && r_c == 1 && c_c == 1)) {
        throw "error"; // unmatched ( , ) syntax
    }

    size_t l_b = input.find(L_BRACKET);
    size_t r_b = input.find(R_BRACKET);
    size_t c = input.find(COMMA);

    if (!(l_b < c || c < r_b) || l_b == 0) {
        throw "error"; // unmatched Keyword( , ) syntax
    }

    string key = trim(input.substr(0, l_b));
    string a1 = trim(input.substr(l_b + 1, c));
    string a2 = trim(input.substr(c + 1));

    tup = make_tuple(key, a1, a2);
    
    return;
}
