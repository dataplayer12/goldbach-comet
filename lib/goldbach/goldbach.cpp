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

num evalute_partition_function(const num xval, num* prime_storage_start)
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
    num* p2_min_loc = prime_storage_start + 1;
    num* p2 = p2_min_loc;

    while (*p1 <= half)
    {
        while (*p2 < half)
        {
            p2++;
            p2_min_loc = p2;
        }

        while (*p2 < xval)
        {
            if (*p1 + *p2 == xval)
            {
                result++;
            }
            p2++;
        }

        p2=p2_min_loc;
        p1++;
    }

    return result;
}