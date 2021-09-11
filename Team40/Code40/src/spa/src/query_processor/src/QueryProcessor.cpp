#include "query_processor/QueryProcessor.h"

void QueryProcessor::processQuery(string query, list<string>& results) {
	Query q;
	preprocessor.preprocessQuery(query, q);
	vector<string> e = evaluator.evaluateQuery(q);
	copy(e.begin(), e.end(), back_inserter(results));
}
