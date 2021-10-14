#include "query_processor/QueryProcessor.h"

QueryProcessor::QueryProcessor(PKB *pkb) {
    this->pkb = pkb;
    this->preprocessor = QueryPreprocessor();
    this->evaluator = QueryEvaluator(pkb);
}

void QueryProcessor::processQuery(string query, list<string> &results) {
    Query q;
    try {
        bool successful = preprocessor.preprocessQuery(query, q);
        if (!successful) {
            return;
        }
    } catch (ValidityError ve) {
        results.push_back("FALSE");
        return;
    }

    vector<string> e = evaluator.evaluateQuery(q);
    copy(e.begin(), e.end(), back_inserter(results));
}
