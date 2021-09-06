#include "QueryProcessor.h"

QueryProcessor::QueryProcessor() {}

void QueryProcessor::processQuery(string query, list<string>& results) {
	Query q = preprocessor.preprocessQuery(query);
	vector<string> e = evaluator.evaluateQuery(q);
	copy(e.begin(), e.end(), back_inserter(results));
}
