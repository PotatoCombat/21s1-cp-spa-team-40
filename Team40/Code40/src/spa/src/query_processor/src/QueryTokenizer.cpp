#include "query_processor/QueryTokenizer.h"

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
        // remaining = "";
        // return "";
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

void QueryTokenizer::tokenizeDeclaration(string input,
                                         vector<DeclPair> &decls) {
    size_t sem_pos;
    size_t space_pos;
    string type;
    string rem = input;

    sem_pos = rem.find(SEMICOLON); // position of semicolon
    while (sem_pos != string::npos) {
        string sub = trim(rem.substr(0, sem_pos)); // e.g. assign x, y
        rem = trim(rem.substr(sem_pos + 1));

        space_pos = sub.find(WHITESPACE); // position of whitespace
        string type = sub.substr(0, space_pos);

        // check if first part is a design entity type

        deHelper.getType(type); // throws if not

        // remaining string should have either
        // comma, comma + whitespace, none
        string rest = trim(sub.substr(space_pos + 1));
        vector<string> declSyn;
        splitComma(rest, declSyn);
        for (auto x : declSyn) {
            decls.push_back(make_pair(type, x));
        }
        sem_pos = rem.find(SEMICOLON);
    }

    return;
}

void QueryTokenizer::tokenizeClause(string input, vector<ClsTuple> &clss,
                                    vector<PatTuple> &pats) {
    // tokenizes by whitespace
    size_t curr_pos = input.find_first_not_of(WHITESPACE_SET);
    int suchThatCounter = 0; // 1 for such, 2 for such that
    int isClause = 0;        // 1 for such that, 2 for pattern
    // 0 for before (, 1 for between ( and ,, 2 for between , and )
    int portionOfClause = 0;
    string token1;
    string token2;
    string token3;

    while (curr_pos != string::npos) {
        // find the next whitespace
        // chop the string between curr and next whitespace
        // check if the string is a something

        size_t cut_pos = input.find_first_of(WHITESPACE_SET, curr_pos);
        string token = input.substr(curr_pos, cut_pos - curr_pos);

        // check if it's a bracket
        // check if it's a comma
        if (isClause) { // handle tokenizing of X1(X2,X3) here
            if (portionOfClause == 0) {
                // size_t not_name_char_pos = token.find_first_not_of(NAME_SET);
                size_t not_name_char_pos = token.find(L_BRACKET);
                if (not_name_char_pos != string::npos) {
                    if (token[not_name_char_pos] == '(') {
                        token1 = token.substr(0, not_name_char_pos);
                        size_t size = not_name_char_pos + 1;
                        if (size < token.size()) { // something after the '('
                            // change the next position to search to after '('
                            cut_pos = cut_pos - (token.size() - size);
                        }
                    } else {
                        throw "error";
                    }
                } else {
                    token1 = token;
                }
                ++portionOfClause; // 1
            } else if (portionOfClause == 1) {
                // size_t not_name_char_pos = token.find_first_not_of(NAME_SET);
                size_t not_name_char_pos = token.find(COMMA);
                if (not_name_char_pos != string::npos) {
                    if (token[not_name_char_pos] == ',') {
                        token2 = token.substr(0, not_name_char_pos);
                        size_t size = not_name_char_pos + 1;
                        if (size < token.size()) { // something after the ','
                            // change the next position to search to after ','
                            cut_pos = cut_pos - (token.size() - size);
                        }
                    } else {
                        throw "error";
                    }
                } else {
                    token2 = token;
                }
                ++portionOfClause; // 2
            } else if (portionOfClause == 2) {
                // size_t not_name_char_pos = token.find_first_not_of(NAME_SET);
                size_t not_name_char_pos = token.find(R_BRACKET);
                if (not_name_char_pos != string::npos) {
                    if (token[not_name_char_pos] == ')') {
                        token3 = token.substr(0, not_name_char_pos);
                    } else {
                        throw "error";
                    }
                } else {
                    token3 = token;
                }
                portionOfClause = 0;
                if (isClause == 1) {
                    clss.push_back(make_tuple(token1, token2, token3));
                } else if (isClause == 2) {
                    pats.push_back(
                        make_pair(token1, vector<PatArg>{token2, token3}));
                } else {
                    throw "error"; // something is very wrong
                }
                isClause = 0;
            } else {
                throw "error"; // error in the token counting or something
            }
        } else { // handle tokenizing of such that / pattern keywords here
            if (token == KEYWORD_SUCH) {
                if (suchThatCounter == 0) {
                    ++suchThatCounter;
                } else {
                    throw "error"; // such is misplaced!
                }
            } else if (token == KEYWORD_THAT) {
                if (suchThatCounter == 1) {
                    ++suchThatCounter;
                    isClause = 1;
                } else {
                    throw "error"; // that is misplaced!
                }
            } else if (token == KEYWORD_PATTERN) {
                isClause = 2;
            } else {
                throw "error"; // error in the keyword counting or some
                               // irrevelant text
            }
        }

        // find the start position of the next token
        curr_pos = input.find_first_not_of(WHITESPACE_SET, cut_pos);
    }
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
        return input.substr(0, back_w + 1);
    }
    return "";
}

void QueryTokenizer::splitComma(string input, vector<string> &vec) {
    string rem = input;

    size_t split = rem.find(COMMA);
    while (split != string::npos) {
        string sub = trimR(rem.substr(0, split));
        vec.push_back(sub);
        rem = trimL(rem.substr(split + 1));
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

// void QueryTokenizer::splitBCBRel(string input, ClsTuple &tup) {
//    // check count
//    int l_c = count(input.begin(), input.end(), L_BRACKET);
//    int r_c = count(input.begin(), input.end(), R_BRACKET);
//    int c_c = count(input.begin(), input.end(), COMMA);
//
//    if (!(l_c == 1 && r_c == 1 && c_c == 1)) {
//        throw "error"; // unmatched ( , ) syntax
//    }
//
//    size_t l_b = input.find(L_BRACKET);
//    size_t r_b = input.find(R_BRACKET);
//    size_t c = input.find(COMMA);
//
//    if (!(l_b < c && c < r_b) || l_b == 0) {
//        throw "error"; // unmatched Keyword( , ) syntax
//    }
//
//    if (l_b + 1 == c || c + 1 == r_b) {
//        throw "error"; // no arguments
//    }
//
//    string key = trim(input.substr(0, l_b));
//    string a1 = trim(input.substr(l_b + 1, c - (l_b + 1)));
//    string a2 = trim(input.substr(c + 1, r_b - (c + 1)));
//    tup = make_tuple(key, a1, a2);
//
//    return;
//}
