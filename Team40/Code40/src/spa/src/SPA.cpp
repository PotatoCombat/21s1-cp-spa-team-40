#include "spa/SPA.h"

#include <utility>

using namespace std;

SPA::SPA() {
    this->pkb = PKB();
    this->sourceProcessor = SourceProcessor(&pkb);
    this->queryProcessor = QueryProcessor(&pkb);
}

void SPA::processSource(string filename) {
    sourceProcessor.processSource(move(filename));
}

void SPA::processQuery(string query, list<string> &results) {
    queryProcessor.processQuery(move(query), results);
}
