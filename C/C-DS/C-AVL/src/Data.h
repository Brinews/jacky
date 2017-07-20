#ifndef DATA_H
#define DATA_H

#include <iostream>


class DoubleData {
private:
    double* data;

public:
    DoubleData();
    DoubleData(double a);
    ~DoubleData() = default;

    // accessors -------------------------------
    double getData() const ;
    double compare(DoubleData& other) const;
    friend std::ostream& operator<<(std::ostream& os, const DoubleData& dd);

    // mutators --------------------------------
    void setData(double _data);
};


#endif
