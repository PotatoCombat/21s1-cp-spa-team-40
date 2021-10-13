#pragma once

#include <algorithm>
#include <string>

#include "query_processor/exception/ValidityError.h"
#include "query_processor/model/Reference.h"

namespace ParserUtil {
	ReferenceType checkRefType(std::string val);
	bool isWildcard(std::string val);
	bool isQuote(std::string val);
	bool isInteger(std::string val);
	bool isQuoted(std::string val);
	bool isValidName(std::string val);
	bool isAttrRef(std::string val);
	pair<std::string, std::string> splitAttrRef(std::string val);
};
