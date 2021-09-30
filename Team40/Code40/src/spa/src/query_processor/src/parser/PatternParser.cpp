#include "query_processor/parser/PatternParser.h"

PatternParser::PatternParser() {
	this->ident = "";
	this->args.clear();
}

void PatternParser::initReferences(vector<Reference*>& declList) {
	this->declList = declList;
}

void PatternParser::clear() {
	this->declList.clear();
	this->ident = "";
	this->args.clear();
}

PatternClause* PatternParser::parse(PatTuple patTuple) {
	this->ident = patTuple.first;
	this->args = patTuple.second;

}
