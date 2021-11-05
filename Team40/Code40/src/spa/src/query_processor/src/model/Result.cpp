#include "query_processor/model/Result.h"

void Result::setValid(bool isValid) {
	this->isValid = isValid;
}

void Result::setResultList1(Reference *reference,
                            map<VALUE, VALUE_SET> resultList) {
    reference1 = reference;
    resultList1 = resultList;
}

void Result::setResultList2(Reference *reference,
                            map<VALUE, VALUE_SET> resultList) {
	reference2 = reference;
	resultList2 = resultList;
}

bool Result::isResultValid() {
	return isValid;
}

Reference* Result::getReference1() {
	return reference1;
}

Reference* Result::getReference2() {
	return reference2;
}

bool Result::hasResultList1() { return getReference1() != NULL; }

bool Result::hasResultList2() { return getReference2() != NULL; }

map<VALUE, VALUE_SET> Result::getResultList1() {
	return resultList1;
}

map<VALUE, VALUE_SET> Result::getResultList2() {
	return resultList2;
}

bool Result::equals(Result &other) {
    if (this->isResultValid() != other.isResultValid()) {
        return false;
    }

    if (this->hasResultList1() != other.hasResultList1()) {
        return false;
    }

    if (this->hasResultList2() != other.hasResultList2()) {
        return false;
    }

    if (this->hasResultList1()) {
        if (!this->getReference1()->equals(*other.getReference1())) {
            return false;
        }

        map<VALUE, VALUE_SET> v1 = this->getResultList1();
        map<VALUE, VALUE_SET> v2 = other.getResultList1();
        bool correctVec = v1.size() == v2.size();

        if (!correctVec) {
            return false;
        }

        for (auto valueToPointer : v1) {
            string value = valueToPointer.first;
            if (v2.find(value) == v2.end()) {
                return false;
            }

            VALUE_SET p1 = valueToPointer.second;
            VALUE_SET p2 = v2[value];
            if (p1 != p2) {
                return false;
            }
        }
    }

    if (this->hasResultList2()) {
        if (!this->getReference2()->equals(*other.getReference2())) {
            return false;
        }

        map<VALUE, VALUE_SET> v1 = this->getResultList2();
        map<VALUE, VALUE_SET> v2 = other.getResultList2();
        bool correctVec = v1.size() == v2.size();

        if (!correctVec) {
            return false;
        }

        for (auto valueToPointer : v1) {
            string value = valueToPointer.first;
            if (v2.find(value) == v2.end()) {
                return false;
            }

            VALUE_SET p1 = valueToPointer.second;
            VALUE_SET p2 = v2[value];
            if (p1 != p2) {
                return false;
            }
        }
    }

    return true;
}
