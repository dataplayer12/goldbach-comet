#pragma once

#include <stdio.h>
#include <math.h>
#include <vector>
#include <string>

#define COMET_LENGTH (1000000)
#define NUM_BUCKETS 8192
constexpr bool verbose=false;
// ToDo: add logging levels

typedef u_int32_t num;

void fill_xvalues(num* storage);

void find_primes_without_bootstrapping(const num from, const num to, num* &storage_head);

/*
Find all prime numbers from `from` to `to` and store them beginning from storage
*/
void find_primes(const num from, const num to, num* &storage_head, num* storage_start);

num evalute_partition_function(const num xval, num* prime_storage_start, num* &start_hint);

void update_buckets(const num xval, num* prime_storage_start, num* &bucket_positions, num num_buckets);

num evalute_partition_function_recursive(const num xval, num* prime_storage_start, num* b2_positions, const num num_buckets);