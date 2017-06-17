#ifndef __COUNTINGBLOOMFILTER_H__
#define __COUNTINGBLOOMFILTER_H__

#include <stdlib.h>
#include <string>
#include <iostream>
#include "constant.h"
using std::string;

class CountingBloomFilter
{
public:

    CountingBloomFilter();
    ~CountingBloomFilter();

    void add(const string &str);
    void remove(const string &str);
    bool isContain(const string &str);

protected:

private:
    uint8_t *cbf_counter_array;
    uint32_t FNV32_1a_hash(const string &str) const;
    void permutation(uint32_t &hashvalue) const;
};

#endif // __COUNTINGBLOOMFILTER_H__
