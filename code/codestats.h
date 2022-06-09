#ifndef CODESTATS_H
#define CODESTATS_H

#include <math.h>
#include <cmath>
#include <vector>
#include <unordered_map>



namespace MDLAlgo
{

namespace detail
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
        // if (k > n)
        // {
        //     std::cerr << "K > N for binomial! n=" << n << "   k=" << k << "\n\n";
        // }
        return logFact(n) - logFact(n-k) - logFact(k);
    }

    inline double logMultinom(const uint32_t n, const std::vector<uint32_t>& chSizes)
    {
        double res = logFact(n);
        for (const uint32_t chSize : chSizes)
        {
            res -= logFact(chSize);
        }
        return res;
    }


    // Returns the theoretical universal code length for integer x
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
    // mapping from n and L to log of parametric complexity
    struct ParamKey
    {
        uint32_t n;
        uint32_t L;

        bool operator==(const ParamKey& other) const
        {
            return (n==other.n) && (L==other.L);
        }
    };
    template <class T>
    inline void hash_combine(std::size_t & s, const T & v)
    {
        std::hash<T> h;
        s^= h(v) + 0x9e3779b9 + (s<< 6) + (s>> 2);
    }

    template <class T>
    class MyHash;

    template<>
    struct MyHash<ParamKey>
    {
        std::size_t operator()(ParamKey const& PK) const
        {
            std::size_t res = 0;
            hash_combine(res, PK.n);
            hash_combine(res, PK.L);
            return res;
        }
    };
    static std::unordered_map<ParamKey, double, MyHash<ParamKey> > pComplexities;

    // Compute the (log of) parametric complexity for binomial NML
    // Sublinear time version of [T. Mononen and P. Myllym\"aki 2008]
    inline double paramCompApprox(const uint32_t n, const uint32_t L)
    {
        if (n == 0)
            return 0;
        double res = 0;
#pragma omp critical
{
        auto pCompIt = pComplexities.find({n,L});
        if (pCompIt == pComplexities.end())
        {
            // use float precision
            constexpr uint32_t d = 32;
            double sum = 1;
            double b = 1;
            const uint32_t bound = ceil(2.0 + sqrt(2*n*d*log(10)));
            for (uint32_t k = 1; k <= bound; ++k)
            {
                b = (double)(n-k+1)/(double)n * b;
                sum += b;
            }
            double old_sum = 1;
            for (uint32_t j = 3; j <= L; ++j)
            {
                const double new_sum = sum + (n*old_sum)/(j-2);
                old_sum = sum;
                sum = new_sum;
            }
            const double logPComp = log2(sum);
            pComplexities[{n,L}] = logPComp;
            res = logPComp;

        } else {
            res = pCompIt->second;
        }
}
        return res;
    }


} // end namespace detail

} // end namespace MDLAlgo


#endif // CODESTATS_H