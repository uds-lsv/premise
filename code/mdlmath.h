#ifndef MDLMATH_H
#define MDLMATH_H

#include <assert.h>
#include <math.h>
#include <cmath>
#include <vector>
#include <unordered_map>

namespace MDLMath
{

    // static array that contains log factorials lazily evaluated
    static std::vector<double> logFacStorage (1,0);

    // return the log2 of n!
    inline double logFact(const uint32_t n)
    {
#pragma omp critical
{
        // evaluate everything up to requested value
        for (uint32_t i = logFacStorage.size(); i <= n; ++i)
        {
            logFacStorage.push_back(logFacStorage[i - 1] + log2(static_cast<double>(i)));
        }
}
        return logFacStorage[n];
    }

    // return log2 of binomial coefficient n choose k
    inline double logBinom(const uint32_t n, const uint32_t k)
    {
        assert(n >= k);
        return logFact(n) - logFact(n-k) - logFact(k);
    }


    // Returns the theoretical universal code length for positive integer x
    inline double univCodeIntegerL(double x)
    {
        double cl = log2(2.865064);
        x = log2(x);
        while (x > 0)
        {
            // count up bits
            cl += x;
            x = log2(x);
        }
        return cl;
    }

    // Yields the log2 of the weak number composition to split n into k partitions
    inline double logWeakNumComp(const uint32_t n, const uint32_t k)
    {
        return logFact(n+k-1) - logFact(n) - logFact(k-1);
    }

    inline double logStrongNumComp(const uint32_t n, const uint32_t k)
    {
        return logFact(n-1) - logFact(n-k-1) - logFact(k-1);
    }

    // holds computed parametric complexities
    // mapping from n to log of parametric complexity
    static std::unordered_map<uint32_t, double> pComplexities;

    // Compute the (log of) parametric complexity for binomial NML
    // Sublinear time version of [T. Mononen and P. Myllymaeki 2008]
    inline double paramCompApprox(uint32_t n) // TODO This uses the parametric complexity with parameter n; the version from codestats uses n and L. Difference?
    {
        if (n == 0)
            return 0;
        double res = 0;
#pragma omp critical
{
        auto pCompIt = pComplexities.find(n);
        if (pCompIt == pComplexities.end())
        {
            // use float precision
            constexpr uint32_t d = 32;
            double sum = 1;
            double b = 1;
            // Use equation 38 to bound the log term
            const uint32_t bound = ceil(2.0 + sqrt(2*n*d*log2(10)));
            for (uint32_t k = 1; k <= bound; ++k)
            {
                b = (double)(n-k+1)/(double)n * b;
                sum += b;
            }
            const double logPComp = log2(sum);
            pComplexities[n] = logPComp;
            res = logPComp;

        } else {
            res = pCompIt->second;
        }
}
        return res;
    }

    // one-sided Fisher's Exact Test, equivalent to numpy's setting "lower"
    double fishersExactTest(uint32_t a, uint32_t b, uint32_t c, uint32_t d);

} // end namespace MDLMath


#endif