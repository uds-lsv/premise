#include "mdlmath.h"
#include <iostream>

double MDLMath::fishersExactTest(uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    // compute cummulative probability for this setting and more extreme settings (in one direction)
    double p = 0;

    // part of the computation are invariant
    // as counts are shifted from a to b and d to c
    const double invariant = MDLMath::logFact(a+b)+MDLMath::logFact(c+d)+MDLMath::logFact(a+c)+MDLMath::logFact(b+d)-MDLMath::logFact(a+b+c+d);
    std::cout.precision(10);

    while(true) {
        p += exp2(invariant-MDLMath::logFact(a)-MDLMath::logFact(b)-MDLMath::logFact(c)-MDLMath::logFact(d));
        if(a == 0 || d == 0)
            break;
        // change so that the column and row sums are the same
        a--; d--;
        b++; c++;
    };

    return p;
}
