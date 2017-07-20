#include "Data.h"

// constructors / destructor
DoubleData::DoubleData():
	data {nullptr} {};
DoubleData::DoubleData(double a){
	data = new double(a);
}

// accessors -------------------------------
double DoubleData::getData() const {
	if (this && this->data){
		return *this->data;
	} else {
		return 0;
	}
}

// returns positive number if *this->data is greater than *other.data
double DoubleData::compare(DoubleData& other) const {
	// avoids segfaults by calling getData()
	return (getData() - other.getData());
}

std::ostream& operator<<(std::ostream& os, const DoubleData& dd) {
	if (dd.data){
		os << *(dd.data);
	} else {
		os << "";
	}
	return os;
}

// mutators --------------------------------
void DoubleData::setData(double _data) {
	if (data){
		*data = _data;
	} else {
		data = new double(_data);
	}
}
