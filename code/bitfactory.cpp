#include <iostream>

#include "bitfactory.h"

std::shared_ptr<BitCollection> BitFactory::getEmpty(const DataType dt, const uint32_t colnum)
{
    switch(dt)
    {
        case (u32):
            return std::shared_ptr<BitCollection>(new DenseBitvec<uint32_t>);

        case (u64):
            return std::shared_ptr<BitCollection>(new DenseBitvec<uint64_t>);

        case (a128):
            return std::shared_ptr<BitCollection>(new DenseBitvec<std::array<uint64_t,2> >);

        case (a256):
            return std::shared_ptr<BitCollection>(new DenseBitvec<std::array<uint64_t,4> >);

        case (a512):
            return std::shared_ptr<BitCollection>(new DenseBitvec<std::array<uint64_t,8> >);

        case (gen):
            return std::shared_ptr<BitCollection>(new DenseBitvec<std::vector<uint64_t> >(colnum));

        case (sparse):
            return std::shared_ptr<BitCollection>(new SparseBitvec);

        default:
            std::cerr << "Unsupported data type encountered when producing empty bit collection. Aborting.\n\n";
            exit(1);
    }
    // should not be reached - see default of switch
    return NULL;
}

std::vector<std::shared_ptr<BitCollection> > BitFactory::getSingletons(const DataType dt, const uint32_t colnum)
{
    assert(colnum > 0);
    std::vector<std::shared_ptr<BitCollection> > singletons(colnum);
    switch(dt)
    {
        case (u32):

            for (uint32_t cIdx = 0; cIdx < colnum; ++cIdx)
                singletons[cIdx] = std::shared_ptr<DenseBitvec<uint32_t> >(new DenseBitvec<uint32_t>((uint64_t)1 << (31 - cIdx), 1));
            break;

        case (u64):

            for (uint32_t cIdx = 0; cIdx < colnum; ++cIdx)
                singletons[cIdx] = std::shared_ptr<DenseBitvec<uint64_t> >(new DenseBitvec<uint64_t>((uint64_t)1 << (63 - cIdx), 1));
            break;

        case (a128):

            for (uint32_t cIdx = 0; cIdx < 64; ++cIdx)
                singletons[cIdx] = std::shared_ptr<DenseBitvec<std::array<uint64_t,2> > >(new DenseBitvec<std::array<uint64_t,2> >{ {(uint64_t)1 << cIdx, 0}, 1 });
            for (uint32_t cIdx = 64; cIdx < colnum; ++cIdx)
                singletons[cIdx] = std::shared_ptr<DenseBitvec<std::array<uint64_t,2> > >(new DenseBitvec<std::array<uint64_t,2> >{ {0, (uint64_t)1 << (127 - cIdx)}, 1 });
            break;

        case (a256):

            for (uint32_t cIdx = 0; cIdx < 64; ++cIdx)
                singletons[cIdx] = std::shared_ptr<DenseBitvec<std::array<uint64_t,4> > >(new DenseBitvec<std::array<uint64_t,4> >{ {(uint64_t)1 << cIdx, 0, 0, 0}, 1 });
            for (uint32_t cIdx = 64; cIdx < 128; ++cIdx)
                singletons[cIdx] = std::shared_ptr<DenseBitvec<std::array<uint64_t,4> > >(new DenseBitvec<std::array<uint64_t,4> >{ {0, (uint64_t)1 << cIdx, 0, 0}, 1 });
            for (uint32_t cIdx = 128; cIdx < 192; ++cIdx)
                singletons[cIdx] = std::shared_ptr<DenseBitvec<std::array<uint64_t,4> > >(new DenseBitvec<std::array<uint64_t,4> >{ {0, 0, (uint64_t)1 << cIdx, 0}, 1 });
            for (uint32_t cIdx = 192; cIdx < colnum; ++cIdx)
                singletons[cIdx] = std::shared_ptr<DenseBitvec<std::array<uint64_t,4> > >(new DenseBitvec<std::array<uint64_t,4> >{ {0, 0, 0, (uint64_t)1 << (255 - cIdx)}, 1 });
            break;

        case (a512):

            for (uint32_t cIdx = 0; cIdx < 64; ++cIdx)
                singletons[cIdx] = std::shared_ptr<DenseBitvec<std::array<uint64_t,8> > >(new DenseBitvec<std::array<uint64_t,8> >{ {(uint64_t)1 << cIdx, 0, 0, 0, 0, 0, 0, 0}, 1 });
            for (uint32_t cIdx = 64; cIdx < 128; ++cIdx)
                singletons[cIdx] = std::shared_ptr<DenseBitvec<std::array<uint64_t,8> > >(new DenseBitvec<std::array<uint64_t,8> >{ {0, (uint64_t)1 << cIdx, 0, 0, 0, 0, 0, 0}, 1 });
            for (uint32_t cIdx = 128; cIdx < 192; ++cIdx)
                singletons[cIdx] = std::shared_ptr<DenseBitvec<std::array<uint64_t,8> > >(new DenseBitvec<std::array<uint64_t,8> >{ {0, 0, (uint64_t)1 << cIdx, 0, 0, 0, 0, 0}, 1 });
            for (uint32_t cIdx = 192; cIdx < 256; ++cIdx)
                singletons[cIdx] = std::shared_ptr<DenseBitvec<std::array<uint64_t,8> > >(new DenseBitvec<std::array<uint64_t,8> >{ {0, 0, 0, (uint64_t)1 << cIdx, 0, 0, 0, 0}, 1 });
            for (uint32_t cIdx = 256; cIdx < 320; ++cIdx)
                singletons[cIdx] = std::shared_ptr<DenseBitvec<std::array<uint64_t,8> > >(new DenseBitvec<std::array<uint64_t,8> >{ {0, 0, 0, 0, (uint64_t)1 << cIdx, 0, 0, 0}, 1 });
            for (uint32_t cIdx = 320; cIdx < 384; ++cIdx)
                singletons[cIdx] = std::shared_ptr<DenseBitvec<std::array<uint64_t,8> > >(new DenseBitvec<std::array<uint64_t,8> >{ {0, 0, 0, 0, 0, (uint64_t)1 << cIdx, 0, 0}, 1 });
            for (uint32_t cIdx = 384; cIdx < 448; ++cIdx)
                singletons[cIdx] = std::shared_ptr<DenseBitvec<std::array<uint64_t,8> > >(new DenseBitvec<std::array<uint64_t,8> >{ {0, 0, 0, 0, 0, 0, (uint64_t)1 << cIdx, 0}, 1 });
            for (uint32_t cIdx = 448; cIdx < colnum; ++cIdx)
                singletons[cIdx] = std::shared_ptr<DenseBitvec<std::array<uint64_t,8> > >(new DenseBitvec<std::array<uint64_t,8> >{ {0, 0, 0, 0, 0, 0, 0, (uint64_t)1 << (511 - cIdx)}, 1 });
            break;

        case (gen):
            for (uint32_t cIdx = 0; cIdx < colnum; ++cIdx)
            {
                std::vector<uint64_t> st((colnum/64) + 1, 0);
                st[cIdx/64] = (uint64_t)1 << (63 - (cIdx % 64));
                singletons[cIdx] = std::shared_ptr<DenseBitvec<std::vector<uint64_t> > >(new DenseBitvec<std::vector<uint64_t> >(std::move(st), 1));
            }
            break;

        case (sparse):
            for (uint32_t cIdx = 0; cIdx < colnum; ++cIdx)
            {
                singletons[cIdx] = std::shared_ptr<SparseBitvec>(new SparseBitvec(std::vector<uint32_t>(1,cIdx)));
            }
            break;

        default:
            std::cerr << "Unsupported data type encountered when producing singleton. Aborting.\n\n";
            exit(1);
    }
    return singletons;
}


std::shared_ptr<BitCollection> BitFactory::genBitrep(const DataType dt, std::vector<uint32_t>& posOnes, const uint32_t colsize)
{
    switch(dt)
    {
        case (u32):
            return BitFactory::genBitrepD32(posOnes);

        case (u64):
            return BitFactory::genBitrepD64(posOnes);

        case (a128):
            return BitFactory::genBitrepD128(posOnes);

        case (a256):
            return BitFactory::genBitrepD256(posOnes);

        case (a512):
            return BitFactory::genBitrepD512(posOnes);

        case (gen):
            return BitFactory::genBitrepDGen(posOnes, colsize);

        case (sparse):
            return BitFactory::genBitrepSGen(posOnes);

        default:
            std::cerr << "Unsupported data type encountered when producing bit collection. Aborting.\n\n";
            exit(1);
    }
}

std::shared_ptr<DenseBitvec<uint32_t> > BitFactory::genBitrepD32(const std::vector<uint32_t>& posOnes)
{
    uint32_t prevPos = 0;
    uint32_t bits = 0;

    for (uint32_t pos : posOnes)
    {
        bits = bits << (pos - prevPos);
        prevPos = pos;
        bits += 1;
    }

    // shift remaining bits
    bits = bits << (31 - prevPos);

    return std::shared_ptr<DenseBitvec<uint32_t> >(new DenseBitvec<uint32_t>(std::move(bits), posOnes.size()));
}

std::shared_ptr<DenseBitvec<uint64_t> > BitFactory::genBitrepD64(const std::vector<uint32_t>& posOnes)
{
    uint32_t prevPos = 0;
    uint64_t bits = 0;

    for (uint32_t pos : posOnes)
    {
        bits = bits << (pos - prevPos);
        prevPos = pos;
        bits += 1;
    }

    // shift remaining bits
    bits = bits << (63 - prevPos);

    return std::shared_ptr<DenseBitvec<uint64_t> >(new DenseBitvec<uint64_t>(std::move(bits), posOnes.size()));
}

std::shared_ptr<DenseBitvec<std::array<uint64_t, 2> > > BitFactory::genBitrepD128(const std::vector<uint32_t>& posOnes)
{
    std::array<uint64_t, 2> bits{ {0, 0} };
    std::vector<uint32_t>::const_iterator posIt = posOnes.begin();
    BitFactory::processBatch(posIt, posOnes.end(), 0, 63, bits[0]);
    BitFactory::processBatch(posIt, posOnes.end(), 64, 127, bits[1]);

    return std::shared_ptr<DenseBitvec<std::array<uint64_t, 2> > >(new DenseBitvec<std::array<uint64_t, 2> >(std::move(bits), posOnes.size()));
}

std::shared_ptr<DenseBitvec<std::array<uint64_t, 4> > > BitFactory::genBitrepD256(const std::vector<uint32_t>& posOnes)
{
    std::array<uint64_t, 4> bits{ {0, 0} };
    std::vector<uint32_t>::const_iterator posIt = posOnes.begin();

    BitFactory::processBatch(posIt, posOnes.end(), 0, 63, bits[0]);
    BitFactory::processBatch(posIt, posOnes.end(), 64, 127, bits[1]);
    BitFactory::processBatch(posIt, posOnes.end(), 128, 191, bits[2]);
    BitFactory::processBatch(posIt, posOnes.end(), 192, 255, bits[3]);

    return std::shared_ptr<DenseBitvec<std::array<uint64_t, 4> > >(new DenseBitvec<std::array<uint64_t, 4> >(std::move(bits), posOnes.size()));
}

std::shared_ptr<DenseBitvec<std::array<uint64_t, 8> > > BitFactory::genBitrepD512(const std::vector<uint32_t>& posOnes)
{
    std::array<uint64_t, 8> bits{ {0, 0} };
    std::vector<uint32_t>::const_iterator posIt = posOnes.begin();

    BitFactory::processBatch(posIt, posOnes.end(), 0, 63, bits[0]);
    BitFactory::processBatch(posIt, posOnes.end(), 64, 127, bits[1]);
    BitFactory::processBatch(posIt, posOnes.end(), 128, 191, bits[2]);
    BitFactory::processBatch(posIt, posOnes.end(), 192, 255, bits[3]);
    BitFactory::processBatch(posIt, posOnes.end(), 256, 319, bits[4]);
    BitFactory::processBatch(posIt, posOnes.end(), 320, 383, bits[5]);
    BitFactory::processBatch(posIt, posOnes.end(), 384, 447, bits[6]);
    BitFactory::processBatch(posIt, posOnes.end(), 448, 512, bits[7]);

    return std::shared_ptr<DenseBitvec<std::array<uint64_t, 8> > >(new DenseBitvec<std::array<uint64_t, 8> >(std::move(bits), posOnes.size()));
}

std::shared_ptr<DenseBitvec<std::vector<uint64_t> > > BitFactory::genBitrepDGen(const std::vector<uint32_t>& posOnes, const uint32_t colsize)
{

    assert(colsize > 0);

    const size_t bitsLen = (colsize / 64) + 1;
    std::vector<uint64_t> bits (bitsLen, 0);

    uint32_t offset = 0;
    auto posIt = posOnes.begin();
    for (size_t bitsIdx = 0; bitsIdx < bitsLen; ++bitsIdx, offset += 64)
    {
        BitFactory::processBatch(posIt, posOnes.end(), offset, offset + 63, bits[bitsIdx]);
    }

    return std::shared_ptr<DenseBitvec<std::vector<uint64_t> > >(new DenseBitvec<std::vector<uint64_t> >(std::move(bits), posOnes.size()));
}

std::shared_ptr<SparseBitvec> BitFactory::genBitrepSGen(std::vector<uint32_t>& posOnes)
{
    return std::shared_ptr<SparseBitvec>(new SparseBitvec(std::move(posOnes)));
}