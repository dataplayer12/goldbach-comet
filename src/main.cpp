#include "goldbach.hpp"
#include <chrono>
#include <sciplot/sciplot.hpp>

int main(int argc, char* argv[])
{
    std::array<num, COMET_LENGTH> xvalues;
    std::array<num, COMET_LENGTH> yvalues;

    std::array<num, COMET_LENGTH/2> scratch;
    // std::vector<num> scratch;
    // scratch.resize(COMET_LENGTH/2); // half should be really enough
    num* storage_head = &scratch[0];
    num multiplicative_factor = COMET_LENGTH/100;

    fill_xvalues(&xvalues[0]);

    for (int idx=0; idx<100; idx++)
    {
        find_primes(xvalues[idx*multiplicative_factor], xvalues[(idx+1)*multiplicative_factor], storage_head, &scratch[0]);
    }

    for (num idx=0; idx < COMET_LENGTH; idx++)
    {
        yvalues[idx] = evalute_partition_function(xvalues[idx], &scratch[0]);

        if (idx%1000==0)
        {
            printf("idx=%d, G(%d)=%d\n", idx, xvalues[idx], yvalues[idx]);
        }
    }
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