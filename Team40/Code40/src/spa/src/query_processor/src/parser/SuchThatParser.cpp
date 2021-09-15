#include "query_processor/parser/SuchThatParser.h"

SuchThatParser(DesignEntityTypeHelper *deHelper, ClauseTypeHelper *clsHelper) {
    this->deHelper = *deHelper;
    this->clsHelper = *clsHelper;
}


