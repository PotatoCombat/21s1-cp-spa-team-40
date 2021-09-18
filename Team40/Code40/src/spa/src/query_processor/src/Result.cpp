#include "query_processor/Result.h"

void Result::setValid(bool isValid) {
	this->isValid = isValid;
}

void Result::setResultList1(Reference* reference, vector<ValueToPointersMap> resultList) {
	reference1 = reference;
	resultList1 = resultList;
}

void Result::setResultList2(Reference* reference, vector<ValueToPointersMap> resultList) {
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

vector<ValueToPointersMap> Result::getResultList1() {
	return resultList1;
}

vector<ValueToPointersMap> Result::getResultList2() {
	return resultList2;
}

bool Result::equals(Result& other) {
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

		vector<ValueToPointersMap> v1 = this->getResultList1();
		vector<ValueToPointersMap> v2 = other.getResultList1();
        bool correctVec = v1.size() == v2.size();

		if (!correctVec) {
            return false;
		}

        for (int i = 0; i < v1.size(); i++) {
            if (!(v1[i].equals(v2[i]))) {
                return false;
			}
		}	
	}

	if (this->hasResultList2()) {
		if (!this->getReference2()->equals(*other.getReference2())) {
            return false;
		}

		vector<ValueToPointersMap> v1 = this->getResultList2();
		vector<ValueToPointersMap> v2 = other.getResultList2();
        bool correctVec = v1.size() == v2.size();

		if (!correctVec) {
            return false;
		}

        for (int i = 0; i < v1.size(); i++) {
            if (!(v1[i].equals(v2[i]))) {
                return false;
			}
		}
	}

    return true;
}
