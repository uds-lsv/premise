#ifndef BITHACKS_H
#define BITHACKS_H

#include <type_traits>

namespace Bithacks
{

    // Kernighans method for pop count
    static inline uint32_t kernigPop(uint64_t b)
    {
        uint32_t pop;
        for (pop = 0; b; pop++)
        {
            b &= b - 1;
        }
        return pop;
    }
    static inline uint32_t kernigPop(uint32_t b)
    {
        uint32_t pop;
        for (pop = 0; b; pop++)
        {
            b &= b - 1;
        }
        return pop;
    }

    // Even more crazy bitcounting
    template <typename T>
    using remove_cref_t = typename std::remove_const<typename std::remove_reference<T>::type>::type;
    template <  
        typename T,
        typename std::enable_if<
            std::is_integral<remove_cref_t<T> >::value
        >::type* = nullptr
    >
    static inline uint32_t popper(T b)
    {
        b = b - ((b >> 1) & static_cast<T>(~static_cast<T>(0)/3));
        b = (b & static_cast<T>(~static_cast<T>(0)/15*3)) + ((b >> 2) & static_cast<T>(~static_cast<T>(0)/15*3));
        b = (b + (b >> 4)) & static_cast<T>(~static_cast<T>(0)/255*15);
        // Assuming one byte is 8 bit
        return static_cast<T>((b * (static_cast<T>(~static_cast<T>(0)/255)))) >> (sizeof(T) - 1) * 8;
    }

    // floating point conversion trick to get index of least significant set bit
    // results is -bias iff bitseq had no bit set
    static inline int64_t getLowestIdx64(uint64_t& bitseq)
    {
        // First mask off least significant bit set
        uint64_t lowestSetBit = bitseq & -bitseq;
        // then convert to double
        double f = (double)lowestSetBit;
        // mask off this bit for next round
        bitseq ^= lowestSetBit;
        // get the exponent, substract bias according to IEEE standard
        return (*(uint64_t*)&f >> 52) - 0x3ff;
    }
    static inline int32_t getLowestIdx32(uint32_t& bitseq)
    {
        uint32_t lowestSetBit = bitseq & -bitseq;
        float f = (float)lowestSetBit;
        bitseq ^= lowestSetBit;
        return (*(uint32_t*)&f >> 23) - 0x7f;
    }

    // Methods to reverse bit sequences
    static inline uint64_t reverseBits64(uint64_t x)
    {
        constexpr uint64_t mu0 = 0x5555555555555555;
        constexpr uint64_t mu1 = 0x3333333333333333;
        constexpr uint64_t mu2 = 0x0f0f0f0f0f0f0f0f;
        constexpr uint64_t mu3 = 0x00ff00ff00ff00ff;
        constexpr uint64_t mu4 = 0x0000ffff0000ffff;
        x = ((x >> 1) & mu0) | ((x & mu0) << 1); 
        x = ((x >> 2) & mu1) | ((x & mu1) << 2); 
        x = ((x >> 4) & mu2) | ((x & mu2) << 4); 
        x = ((x >> 8) & mu3) | ((x & mu3) << 8); 
        x = ((x >> 16) & mu4) | ((x & mu4) << 16);
        return (x >> 32) | (x << 32);
    }
    static inline uint32_t reverseBits32(uint32_t x)
    {
        constexpr uint32_t mu0 = 0x55555555;
        constexpr uint32_t mu1 = 0x33333333;
        constexpr uint32_t mu2 = 0x0f0f0f0f;
        constexpr uint32_t mu3 = 0x00ff00ff;
        x = ((x >> 1) & mu0) | ((x & mu0) << 1); 
        x = ((x >> 2) & mu1) | ((x & mu1) << 2); 
        x = ((x >> 4) & mu2) | ((x & mu2) << 4); 
        x = ((x >> 8) & mu3) | ((x & mu3) << 8); 
        return (x >> 16) | (x << 16);
    }
} // end namespace Bithacks

#endif // BITHACKS_H