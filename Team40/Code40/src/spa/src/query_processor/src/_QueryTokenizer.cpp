#include "query_processor/QueryTokenizer.h"

pair<string, string> QueryTokenizer::separateDeclaration(string input) {
    size_t split = input.rfind(SEMICOLON);

    if (split == string::npos) {
        throw SyntaxError("no declaration");
    }

    string firstHalf = trim(input.substr(0, split + 1));
    string secondHalf = trim(input.substr(split + 1));

    return make_pair(firstHalf, secondHalf);
}

string QueryTokenizer::tokenizeReturn(string input, string& remaining) {
    size_t first_w = findFirstWhitespace(input);
    string sub;
    string rem;

    if (first_w == string::npos) {
        // remaining = "";
        // return "";
        throw SyntaxError("no return entity");
    }

    sub = input.substr(0, first_w);
    rem = trimL(input.substr(first_w + 1));

    if (sub != KEYWORD_SELECT) {
        throw SyntaxError("no select clause");
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
    vector<DeclPair>& decls) {
    size_t curr_pos;
    size_t space_pos;
    string type;
    string rem = input; // remaining string

    curr_pos = rem.find(SEMICOLON); // position of semicolon
    while (curr_pos != string::npos) {
        string sub = trim(rem.substr(0, curr_pos)); // e.g. assign x, y
        rem = trim(rem.substr(curr_pos + 1));

        space_pos = sub.find_first_of(WHITESPACE_SET); // position of whitespace
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
        curr_pos = rem.find(SEMICOLON);
    }

    return;
}

void QueryTokenizer::tokenizeClause(string input, vector<ClsTuple>& clss,
    vector<PatTuple>& pats) {
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
                size_t not_name_char_pos = input.find(L_BRACKET, curr_pos);
                if (not_name_char_pos != string::npos) {
                    size_t size = not_name_char_pos + 1 - curr_pos;
                    if (size > token.size()) { // space between token and '('
                        token1 = token;
                    }
                    else if (size <= token.size()) { // something after '('
                        token1 = token.substr(0, size - 1);
                    }
                    cut_pos = not_name_char_pos + 1;
                }
                else {
                    throw SyntaxError("invalid syntax");
                }
                ++portionOfClause; // 1
            }
            else if (portionOfClause == 1) {
                size_t not_name_char_pos = input.find(COMMA, curr_pos);
                if (not_name_char_pos != string::npos) {
                    size_t size = not_name_char_pos + 1 - curr_pos;
                    if (size > token.size()) { // space between token and ','
                        token2 = trimR(input.substr(curr_pos, size - 1));
                    }
                    else if (size <= token.size()) { // something after ','
                        token2 = token.substr(0, size - 1);
                    }
                    cut_pos = not_name_char_pos + 1;
                }
                else {
                    throw SyntaxError("invalid syntax");
                }
                ++portionOfClause; // 2
            }
            else if (portionOfClause == 2) {
                size_t not_name_char_pos = input.find(R_BRACKET, curr_pos);
                if (not_name_char_pos != string::npos) {
                    size_t size = not_name_char_pos + 1 - curr_pos;
                    if (size > token.size()) { // space between token and ')'
                        token3 = trimR(input.substr(curr_pos, size - 1));
                    }
                    else if (size <= token.size()) { // something after ')'
                        token3 = token.substr(0, size - 1);
                    }
                    cut_pos = not_name_char_pos + 1;
                }
                else {
                    throw SyntaxError("invalid syntax");
                }
                portionOfClause = 0;
                if (isClause == 1) {
                    token2 = removeWhitespaceWithinQuotes(token2);
                    token3 = removeWhitespaceWithinQuotes(token3);
                    clss.push_back(make_tuple(token1, token2, token3));
                }
                else if (isClause == 2) {
                    token2 = removeWhitespaceWithinQuotes(token2);
                    token3 = extractPatternString(token3);
                    pats.push_back(make_tuple(token1, token2, token3));
                }
                else {
                    throw SyntaxError("something is very wrong");
                }
                isClause = 0;
            }
            else {
                throw "error"; // error in the token counting or something
            }
        }
        else { // handle tokenizing of such that / pattern keywords here
            if (token == KEYWORD_SUCH) {
                if (suchThatCounter == 0) {
                    ++suchThatCounter;
                }
                else {
                    // such is misplaced!
                    throw SyntaxError("invalid syntax");
                }
            }
            else if (token == KEYWORD_THAT) {
                if (suchThatCounter == 1) {
                    ++suchThatCounter;
                    isClause = 1;
                }
                else {
                    // that is misplaced!
                    throw SyntaxError("invalid syntax");
                }
            }
            else if (token == KEYWORD_PATTERN) {
                isClause = 2;
            }
            else {
                // error in the keyword counting or some
                // irrevelant text
                throw SyntaxError("invalid syntax");
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

void QueryTokenizer::splitComma(string input, vector<string>& vec) {
    string rem = input;

    size_t split = rem.find(COMMA);
    while (split != string::npos) {
        string sub = trimR(rem.substr(0, split));
        if (!isValidName(sub)) {
            throw SyntaxError("invalid syntax");
        }
        vec.push_back(sub);
        rem = trimL(rem.substr(split + 1));
        split = rem.find(COMMA);
    }
    if (rem.size() > 0) {
        if (!isValidName(rem)) {
            throw SyntaxError("invalid syntax");
        }
        vec.push_back(rem);
    }

    return;
}

size_t QueryTokenizer::findFirstWhitespace(string input) {
    return input.find_first_of(WHITESPACE_SET);
}

string QueryTokenizer::removeWhitespaceWithinQuotes(string input) {
    if (count(input.begin(), input.end(), QUOTE) == 2) {
        if (input[0] == QUOTE && input[input.size() - 1] == QUOTE) {
            string trimmed = trim(input.substr(1, input.size() - 2));
            return "\"" + trimmed + "\"";
        }
        else {
            throw SyntaxError("misplaced quotes");
        }
    }

    return input;
}

string QueryTokenizer::extractPatternString(string input) {
    if (count(input.begin(), input.end(), QUOTE) == 2) {
        size_t n_underscore = count(input.begin(), input.end(), UNDERSCORE);
        if (n_underscore == 2) {
            string underscored_r = trim(input.substr(1, input.size() - 2));
            return trim(underscored_r.substr(1, underscored_r.size() - 2));
        }
        else if (n_underscore == 1) {
            throw SyntaxError("invalid syntax");
        }
        return trim(input.substr(1, input.size() - 2));
    }
    return input;
}

bool QueryTokenizer::isValidName(string name) {
    if (isalpha(name[0])) {
        auto it = find_if_not(begin(name), end(name), isalnum);
        return it == name.end();
    }
    return false;
}
