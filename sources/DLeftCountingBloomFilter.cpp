//
// Created by YZH on 2017/5/11.
//

#include "../headers/DLeftCountingBloomFilter.h"

DLeftCountingBloomFilter::DLeftCountingBloomFilter()
{
	// initialization
	for (int i = 0; i < DLEFT_SUBTALBE_SIZE; i++)
		hashtables[i] = (HashTable*)calloc(1, sizeof(HashTable));
}

// add element
void DLeftCountingBloomFilter::add(const string &str)
{
	if (isContain(str))
		return;
	uint32_t hashvalue = FNV32_1a_hash(str);
	uint16_t indexs[DLEFT_SUBTALBE_SIZE] = { 0, };
	uint16_t fingerprints[DLEFT_SUBTALBE_SIZE] = { 0, };
	uint8_t bucket_cell_load[DLEFT_SUBTALBE_SIZE] = { 0, };

	// calc d index and fingerprint  // count bucket load
	for (int i = 0; i < DLEFT_SUBTALBE_SIZE; i++)
	{
		permutation(hashvalue);
		indexs[i] = (uint16_t)(hashvalue >> (HASHVALUE_MAX_LOG2 - HASHTALBE_SIZE_LOG2));
		fingerprints[i] = (uint16_t)(hashvalue & (FINGERPRINT_MAX - 1));

		for (int j = 0; j < BUCKET_SIZE; j++)
			if (hashtables[i]->buckets[indexs[i]].cells[j].counter != 0)
				bucket_cell_load[i]++;
	}

	// choose sub hashtable which bucket is smallest
	int choose_subtable = -1;
	uint8_t temp = BUCKET_SIZE;
	for (int i = 0; i < DLEFT_SUBTALBE_SIZE; i++)
	{
		if (bucket_cell_load[i] < temp)
		{
			temp = bucket_cell_load[i];
			choose_subtable = i;
		}
	}
	// what about out of space ?
	if (choose_subtable == -1)
	{
		std::cout << "bucket out of space ?" << std::endl;
		return;
	}

	// choose cell in bucket which is has same fingerprint or not use yet
	int choose_cell = -1;
	for (int i = 0; i < BUCKET_SIZE; i++)
	{
		if (hashtables[choose_subtable]->buckets[indexs[choose_subtable]].cells[i].fingerprint == fingerprints[choose_subtable])
			choose_cell = i;
		else if ((choose_cell == -1) && (hashtables[choose_subtable]->buckets[indexs[choose_subtable]].cells[i].fingerprint == 0))
			choose_cell = i;
	}

	// add element already exist in cell
	if (hashtables[choose_subtable]->buckets[indexs[choose_subtable]].cells[choose_cell].fingerprint == fingerprints[choose_subtable])
		hashtables[choose_subtable]->buckets[indexs[choose_subtable]].cells[choose_cell].counter++;
	// add element new in cell
	else
	{
		hashtables[choose_subtable]->buckets[indexs[choose_subtable]].cells[choose_cell].fingerprint = fingerprints[choose_subtable];
		hashtables[choose_subtable]->buckets[indexs[choose_subtable]].cells[choose_cell].counter++;
	}
}

void DLeftCountingBloomFilter::remove(const string &str)
{
	if (!isContain(str))
		return;
	uint32_t hashvalue = FNV32_1a_hash(str);
	uint16_t index = 0;
	uint16_t fingerprint = 0;

	for (int i = 0; i < DLEFT_SUBTALBE_SIZE; i++)
	{
		permutation(hashvalue);
		index = (uint16_t)(hashvalue >> (HASHVALUE_MAX_LOG2 - HASHTALBE_SIZE_LOG2));
		fingerprint = (uint16_t)(hashvalue & (FINGERPRINT_MAX - 1));
		for (int j = 0; j < BUCKET_SIZE; j++)
		{
			if (hashtables[i]->buckets[index].cells[j].fingerprint == fingerprint)
				hashtables[i]->buckets[index].cells[j].counter--;
			if (hashtables[i]->buckets[index].cells[j].counter == 0)
				hashtables[i]->buckets[index].cells[j].fingerprint = 0;
		}
	}
}

bool DLeftCountingBloomFilter::isContain(const string &str) const
{
	uint32_t hashvalue = FNV32_1a_hash(str);
	uint16_t index = 0;
	uint16_t fingerprint = 0;

	for (int i = 0; i < DLEFT_SUBTALBE_SIZE; i++)
	{
		permutation(hashvalue);
		index = (uint16_t)(hashvalue >> (HASHVALUE_MAX_LOG2 - HASHTALBE_SIZE_LOG2));
		fingerprint = (uint16_t)(hashvalue & (FINGERPRINT_MAX - 1));
		for (int j = 0; j < BUCKET_SIZE; j++)
		{
			if (hashtables[i]->buckets[index].cells[j].fingerprint == fingerprint)
				return true;
		}
	}
	return false;
}

uint32_t DLeftCountingBloomFilter::FNV32_1a_hash(const string &str) const
{
	register uint32_t hash_result = FNV_OFFSET_BASIS;
	for (char ch : str)
	{
		hash_result ^= ch;
		hash_result *= FNV_PRIME;
	}
	return hash_result;
}

void DLeftCountingBloomFilter::permutation(uint32_t &hashvalue) const
{
	hashvalue = (uint32_t)((PERMUTATION_ODDNUMBER * hashvalue) & (((uint64_t)1 << 32) - 1));
}
