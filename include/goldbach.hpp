#pragma once

#include <stdio.h>
#include <math.h>
#include <vector>

#define COMET_LENGTH (25*1000)
typedef u_int32_t num;

void fill_xvalues(num* storage);

void find_primes_without_bootstrapping(const num from, const num to, num* &storage_head);

/*
Find all prime numbers from `from` to `to` and store them beginning from storage
*/
void find_primes(const num from, const num to, num* &storage_head, num* storage_start);

num evalute_partition_function(const num xval, num* prime_storage_start);