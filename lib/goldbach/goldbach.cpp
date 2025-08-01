#include "goldbach.hpp"

void fill_xvalues(num* storage)
{

    for (num idx = 1; idx <= COMET_LENGTH+1; idx++)
    {
        *storage = 2*idx;
        storage++;
    }
}

void find_primes_without_bootstrapping(const num from, const num to, num* &storage_head)
{
    num primes_found=0;

    for (num candidate = from; candidate < to; candidate++)
    {
        num limit = sqrt(candidate);
        bool is_prime=true;
        num num_lt_limit=2;
        
        while (num_lt_limit <= limit)
        {
            if (candidate%num_lt_limit==0)
            {
                is_prime=false;
                break;
            }
            num_lt_limit++;
        }

        if (is_prime)
        {
            
            *storage_head = candidate;
            storage_head++;
            primes_found++;
        }
    }
    printf("Found %d primes between %d and %d without bootstrapping\n", primes_found, from, to);
}

void find_primes(const num from, const num to, num* &storage_head, num* storage_start)
{
    if (storage_head < storage_start)
    {
        printf("Cannot find primes because function was called wrongly\n");
    }

    num n_candidates = to - from;
    num primes_found=0;

    if (storage_head == storage_start)
    {
        // no primes have been found yet, need to start from the beginning
        printf("storage_head == storage_start\n");
        find_primes_without_bootstrapping(from, to, storage_head);
        return;
    }

    if (*(storage_head-1) < sqrt(to))
    {
        printf("%d < %f\n", *(storage_head-1), sqrt(to));
        find_primes_without_bootstrapping(from, to, storage_head);
        return;
    }

    for (num candidate = from; candidate < to; candidate++)
    {
        num limit = sqrt(candidate);
        num* kp_loc = storage_start;
        num known_prime = *kp_loc;
        bool is_prime=true;
        
        while (known_prime <= limit)
        {
            if (candidate%known_prime==0)
            {
                is_prime=false;
                break;
            }
            kp_loc++;
            known_prime = *kp_loc;
        }

        if (is_prime)
        {
            
            *storage_head = candidate;
            storage_head++;
            primes_found++;
        }
    }
    printf("Found %d primes between %d and %d\n", primes_found, from, to);
}

num evalute_partition_function(const num xval, num* prime_storage_start, num* &start_hint)
{
    if (xval%2)
    {
        // refuse to eval on odd numbers
        printf("Function evalute_partition_function was called wrongly, returning 0\n");
        return 0; // lol nobel prize
    }

    num half = (xval >> 1);
    num result=0;
    num* p1 = prime_storage_start;
    num* p2_min_loc = start_hint;
    num* p2 = p2_min_loc;

    if (*start_hint > half)
    {
        printf("Got bad hint. Resetting...");
        p2_min_loc = prime_storage_start+1;
    }

    while (*p1 <= half)
    {
        while (*p2 < half)
        {
            start_hint = p2; // strictly less than half
            p2++;
            p2_min_loc = p2;
        }

        num sum = *p1 + *p2;
        
        while (sum <= xval) // once the sum has exceeded xval, no need to check
        {
            if (sum == xval)
            {
                if constexpr (verbose)
                {
                    printf("xval=%d, p1=%d, p2=%d\n", xval, *p1, *p2);
                }
                result++;
            }
            p2++;
            sum = *p1 + *p2;
        }

        p2=p2_min_loc;
        p1++;
    }

    return result;
}

void update_buckets(const num xval, num* prime_storage_start, num* &bucket_positions, num num_buckets)
{
    if (xval%2)
    {
        // refuse to eval on odd numbers
        printf("Function update_buckets was called wrongly\n");
    }

    for (num _bucket=0; _bucket < num_buckets; _bucket++)
    {
        num bucket_min_pos = _bucket*xval/num_buckets;
        while (prime_storage_start[bucket_positions[_bucket]] < bucket_min_pos)
        {
            (*(bucket_positions+_bucket))++;
        }
    }
}

num evalute_partition_function_recursive(const num xval, num* prime_storage_start, num* b2_positions, const num num_buckets)
{
    if (xval%2)
    {
        // refuse to eval on odd numbers
        printf("Function evalute_partition_function was called wrongly, returning 0\n");
        return 0;
    }

    num half = (xval >> 1);
    num result=0;
    num* p1 = prime_storage_start;

    while (*p1 <= half)
    {
        num p1_bucket = (*p1)*num_buckets/xval;
        num p2_bucket = num_buckets - 1 - p1_bucket;

        if (p2_bucket < 0)
        {
            printf("BUG! p2 bucket is negative for p1=%d, xval=%d\n", *p1, xval); 
        }

        num* p2 = prime_storage_start + *(b2_positions + p2_bucket);
        num sum = *p1 + *p2;

        while (sum <= xval)
        {
            // printf("xval=%d, sum=%d\n", xval, sum);
            if (sum==xval)
            {
                if constexpr (verbose)
                {
                    printf("xval=%d, p1=%d, p2=%d\n", xval, *p1, *p2);
                }

                result++;
            }
            p2++;
            sum = *p1 + *p2;
        }

        p1++;

    }
    return result;
}

void write_to_file(std::ofstream &record, num* const xpos, num* const ypos, const num entries)
{
    for (num idx=0; idx < entries - 1; idx++)
    {
        record << xpos[idx] << ", " << ypos[idx] << "\n";
    }

    record << xpos[entries-1] << ", " << ypos[entries-1] << std::endl;
    // endl will cause the stream to flush only once at the end of the writing process
}