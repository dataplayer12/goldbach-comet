#include "goldbach.hpp"
#include <chrono>
#include <sciplot/sciplot.hpp>

int main(int argc, char* argv[])
{
    // std::array<num, COMET_LENGTH> xvalues;
    // std::array<num, COMET_LENGTH> yvalues;
    std::vector<num> xvalues;
    xvalues.resize(COMET_LENGTH);

    std::vector<num> yvalues;
    yvalues.resize(COMET_LENGTH);

    // std::vector<num> scratch;
    // scratch.resize(COMET_LENGTH/2); // half should be really enough
    std::array<num, COMET_LENGTH/2> scratch;

    std::array<num, NUM_BUCKETS> buckets{{0}};

    num* buckets_start = &buckets[0];
    num* storage_head = &scratch[0];

    num multiplicative_factor = COMET_LENGTH/100;

    fill_xvalues(&xvalues[0]);

    for (int idx=0; idx<100; idx++)
    {
        find_primes(xvalues[idx*multiplicative_factor], xvalues[(idx+1)*multiplicative_factor], storage_head, &scratch[0]);
    }

    num* hint = &scratch[0]; // initially we start guessing p2 at the first prime

    auto start = std::chrono::system_clock::now();

    for (num idx=0; idx < COMET_LENGTH; idx++)
    {
        update_buckets(xvalues[idx], &scratch[0], buckets_start, NUM_BUCKETS);

        if constexpr (verbose)
        {
            printf("xval=%d", xvalues[idx]);
            for (int aidx=0; aidx<NUM_BUCKETS; aidx++)
            {
                printf(", %d", buckets[aidx]);
            }
            printf("\n");
        }

        yvalues[idx] = evalute_partition_function_recursive(
            xvalues[idx], &scratch[0], buckets_start, NUM_BUCKETS);

        if (idx%1000==0)
        {
            printf("idx=%d, G(%d)=%d\n", idx, xvalues[idx], yvalues[idx]);
        }
    }
    auto end = std::chrono::system_clock::now();
    printf("Time taken = %lld ns\n", (end-start).count());
    sciplot::Plot2D plot;
    plot.xlabel("x");
    plot.ylabel("G(x)");

    plot.legend()
        .atOutsideBottom()
        .displayHorizontal()
        .displayExpandWidthBy(2);
    
    plot.drawDots(xvalues, yvalues).label("Goldbach partition function, G(x)");
    sciplot::Figure fig = {{plot}};
    fig.title("Goldbach partition function");
    sciplot::Canvas canvas = {{fig}};
    canvas.save("Goldbach.pdf");
}