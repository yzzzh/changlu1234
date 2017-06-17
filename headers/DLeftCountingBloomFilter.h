//
// Created by YZH on 2017/5/11.
//

#ifndef __DLEFTCOUNTINGBLOOMFILTER_H__
#define __DLEFTCOUNTINGBLOOMFILTER_H__

#include <stdint.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include "constant.h"
using std::string;



struct Cell
{
    uint16_t fingerprint = 0;
    uint8_t counter = 0;
};
struct Bucket
{
    Cell cells[BUCKET_SIZE];
};
struct HashTable
{
    Bucket buckets[HASHTALBE_SIZE];
};

class DLeftCountingBloomFilter
{
public:
    DLeftCountingBloomFilter();
    void add(const string &str);
    void remove(const string &str);
    bool isContain(const string &str) const;
private:
    HashTable* hashtables[DLEFT_SUBTALBE_SIZE];
    uint32_t FNV32_1a_hash(const string &str) const;
    void permutation(uint32_t &hashvalue) const;
};

#endif // __DLEFTCOUNTINGBLOOMFILTER_H__
