#include "../headers/CountingBloomFilter.h"

CountingBloomFilter::CountingBloomFilter()
{
    cbf_counter_array = (uint8_t*)calloc(CBF_LENGTH, sizeof(uint8_t));
}

CountingBloomFilter::~CountingBloomFilter()
{
    free(cbf_counter_array);
}

void CountingBloomFilter::add(const string &str)
{
    if (isContain(str))
        return;
    uint32_t hashvalue = FNV32_1a_hash(str);
    for (int i = 0; i < HASH_FUN_AMOUNT; i++)
    {
        permutation(hashvalue);
        cbf_counter_array[hashvalue] += 1;
    }
}

void CountingBloomFilter::remove(const string &str)
{
    uint32_t hashvalue = FNV32_1a_hash(str);
    uint32_t positions[HASH_FUN_AMOUNT];
    for (int i = 0; i < HASH_FUN_AMOUNT; i++)
    {
        permutation(hashvalue);
        if (cbf_counter_array[hashvalue] == 0)
            return;
        positions[i] = hashvalue;
    }

    for (int i = 0; i < HASH_FUN_AMOUNT; i++)
    {
        cbf_counter_array[positions[i]] -= 1;
    }
}

bool CountingBloomFilter::isContain(const string &str)
{
    uint32_t hashvalue = FNV32_1a_hash(str);
    for (int i = 0; i < HASH_FUN_AMOUNT; i++)
    {
        permutation(hashvalue);
        if (cbf_counter_array[hashvalue] == 0)
            return false;
    }
    return true;
}


uint32_t CountingBloomFilter::FNV32_1a_hash(const string &str) const
{
    register uint32_t hash_result = FNV_OFFSET_BASIS;
    for (char ch : str)
    {
        hash_result ^= ch;
        hash_result *= FNV_PRIME;
    }
    return (uint32_t)(hash_result >> CBF_LENGTH_LOG2) ^ (hash_result & (CBF_LENGTH - 1));
}

void CountingBloomFilter::permutation(uint32_t &hashvalue) const
{
    hashvalue = (uint32_t)((PERMUTATION_ODDNUMBER * hashvalue) & (CBF_LENGTH - 1));
}
