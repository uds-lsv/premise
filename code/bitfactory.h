#ifndef BITFACTORY_H
#define BITFACTORY_H

#include <memory>

#include "densebitvec.h"
#include "sparsebitvec.h"

namespace BitFactory
{

    // Generate an empty bit collection of the given data type
    // represents the empty word
    std::shared_ptr<BitCollection> getEmpty(const DataType dt, const uint32_t colnum);

    // generates all singleton bit collections setting only one bit to 1
    // ARGUMENTS:
    //          dt          data type to use for representing the bit collections
    //          colnum      used to determine the size of the DS in the generic case (dense database format with ncol > 512)
    std::vector<std::shared_ptr<BitCollection> > getSingletons(const DataType dt, const uint32_t colnum);

    // Generate a bit collection of the given data type
    // that has ones at all position specified in posOnes
    // assumes that indices in posOnes are sorted ascendingly
    //
    // ARGUMENTS:
    //          dt          data type to use
    //          posOnes     positions were ones are set; everything else is set to zero
    //          colSize     used to determine the size of the DS in the generic case (dense database format with ncol > 512)
    // WARNING:
    //          The vector posOnes could be modified. Accessing it results in undef. behav.
    std::shared_ptr<BitCollection> genBitrep(const DataType dt, std::vector<uint32_t>& posOnes, const uint32_t colsize = 0);

    // specialized methods for the different data types
    std::shared_ptr<DenseBitvec<uint32_t> > genBitrepD32(const std::vector<uint32_t>& posOnes);
    std::shared_ptr<DenseBitvec<uint64_t> > genBitrepD64(const std::vector<uint32_t>& posOnes);
    std::shared_ptr<DenseBitvec<std::array<uint64_t, 2> > > genBitrepD128(const std::vector<uint32_t>& posOnes);
    std::shared_ptr<DenseBitvec<std::array<uint64_t, 4> > > genBitrepD256(const std::vector<uint32_t>& posOnes);
    std::shared_ptr<DenseBitvec<std::array<uint64_t, 8> > > genBitrepD512(const std::vector<uint32_t>& posOnes);
    std::shared_ptr<DenseBitvec<std::vector<uint64_t> > > genBitrepDGen(const std::vector<uint32_t>& posOnes, const uint32_t colsize);
    std::shared_ptr<SparseBitvec> genBitrepSGen(std::vector<uint32_t>& posOnes);

    inline void processBatch(
        std::vector<uint32_t>::const_iterator& posIt,
        std::vector<uint32_t>::const_iterator itEnd,
        uint32_t prevPos,
        const uint32_t maxIdx,
        uint64_t& bitSlice
    )
    {
        while (posIt != itEnd && *posIt <= maxIdx)
        {
            bitSlice = bitSlice << (*posIt - prevPos);
            prevPos = *posIt;
            bitSlice += 1;
            ++posIt;
        }
        // shift remaining bits
        bitSlice = bitSlice << (maxIdx - prevPos);
    }
}

#endif // BITFACTORY_H