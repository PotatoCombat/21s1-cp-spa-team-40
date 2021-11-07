#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "query_processor/exception/ValidityError.h"

#include "query_processor/model/DesignEntityType.h"
#include "query_processor/model/Reference.h"

namespace ParserUtil {
	ReferenceType parseRefType(std::string val);
	bool isWildcard(std::string val);
	bool isQuote(std::string val);
	bool isInteger(std::string val);
	bool isQuoted(std::string val);
	bool isValidName(std::string val);
	bool isAttrRef(std::string val);
	std::pair<std::string, std::string> splitAttrRef(std::string val);
	ReferenceAttribute parseValidAttr(DesignEntityType deType, std::string val);
	Reference *getReferenceFromList(std::vector<Reference *> &list, std::string syn);
};
