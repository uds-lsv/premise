#include <iostream>
#include <bitset>

#include "densebitvec.h"
#include "bithacks.h"


template <>
DenseBitvec<std::vector<uint64_t> >* DenseBitvec<std::vector<uint64_t> >::setunion(const BitCollection& bc) const
{
    DenseBitvec<std::vector<uint64_t> >* res = new DenseBitvec<std::vector<uint64_t> >(
        bitvec.size() * 64
    );
    res->popcount = this->popcount + static_cast<const DenseBitvec<std::vector<uint64_t> >&>(bc).popcount;
    for (size_t idx = 0; idx < bitvec.size(); ++idx)
    {
        // update bitvectors
        res->bitvec[idx] = static_cast<const DenseBitvec<std::vector<uint64_t> >&>(bc).bitvec[idx] | this->bitvec[idx];
        // adapt popcounts
        res->popcount -= Bithacks::popper<uint64_t>(static_cast<const DenseBitvec<std::vector<uint64_t> >&>(bc).bitvec[idx] & this->bitvec[idx]);
    }
    return res;
}

template <>
DenseBitvec<uint32_t>* DenseBitvec<uint32_t>::setunion(const BitCollection& bc) const
{
    return new DenseBitvec<uint32_t>(
        static_cast<const DenseBitvec<uint32_t>&>(bc).bitvec | this->bitvec,
        this->popcount + static_cast<const DenseBitvec<uint32_t>&>(bc).popcount - Bithacks::popper<uint32_t>(static_cast<const DenseBitvec<uint32_t>& >(bc).bitvec & this->bitvec)
    );
}

template <>
DenseBitvec<uint64_t>* DenseBitvec<uint64_t>::setunion(const BitCollection& bc) const
{
    return new DenseBitvec<uint64_t>(
        static_cast<const DenseBitvec<uint64_t>&>(bc).bitvec | this->bitvec,
        this->popcount + static_cast<const DenseBitvec<uint64_t>&>(bc).popcount - Bithacks::popper<uint64_t>(static_cast<const DenseBitvec<uint64_t>& >(bc).bitvec & this->bitvec)
    );
}

template <>
DenseBitvec<std::array<uint64_t,2> >* DenseBitvec<std::array<uint64_t,2> >::setunion(const BitCollection& bc) const
{
    DenseBitvec<std::array<uint64_t,2> >* res = new DenseBitvec<std::array<uint64_t,2> >();
    res->popcount = this->popcount + static_cast<const DenseBitvec<std::array<uint64_t,2> >&>(bc).popcount;
    res->bitvec[0] = static_cast<const DenseBitvec<std::array<uint64_t,2> >&>(bc).bitvec[0] | this->bitvec[0];
    res->popcount -= Bithacks::popper<uint64_t>(static_cast<const DenseBitvec<std::array<uint64_t,2> >& >(bc).bitvec[0] & this->bitvec[0]);
    res->bitvec[1] = static_cast<const DenseBitvec<std::array<uint64_t,2> >&>(bc).bitvec[1] | this->bitvec[1];
    res->popcount -= Bithacks::popper<uint64_t>(static_cast<const DenseBitvec<std::array<uint64_t,2> >& >(bc).bitvec[1] & this->bitvec[1]);
    return res;
}

template <>
DenseBitvec<std::array<uint64_t,4> >* DenseBitvec<std::array<uint64_t,4> >::setunion(const BitCollection& bc) const
{
    DenseBitvec<std::array<uint64_t,4> >* res = new DenseBitvec<std::array<uint64_t,4> >();
    res->popcount = this->popcount + static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).popcount;
    res->bitvec[0] = static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[0] | this->bitvec[0];
    res->popcount -= Bithacks::popper<uint64_t>(static_cast<const DenseBitvec<std::array<uint64_t,4> >& >(bc).bitvec[0] & this->bitvec[0]);
    res->bitvec[1] = static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[1] | this->bitvec[1];
    res->popcount -= Bithacks::popper<uint64_t>(static_cast<const DenseBitvec<std::array<uint64_t,4> >& >(bc).bitvec[1] & this->bitvec[1]);
    res->bitvec[2] = static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[2] | this->bitvec[2];
    res->popcount -= Bithacks::popper<uint64_t>(static_cast<const DenseBitvec<std::array<uint64_t,4> >& >(bc).bitvec[2] & this->bitvec[2]);
    res->bitvec[3] = static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[3] | this->bitvec[3];
    res->popcount -= Bithacks::popper<uint64_t>(static_cast<const DenseBitvec<std::array<uint64_t,4> >& >(bc).bitvec[3] & this->bitvec[3]);

    return res;
}

template <>
DenseBitvec<std::array<uint64_t,8> >* DenseBitvec<std::array<uint64_t,8> >::setunion(const BitCollection& bc) const
{
    DenseBitvec<std::array<uint64_t,8> >* res = new DenseBitvec<std::array<uint64_t,8> >();
    res->popcount = this->popcount + static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).popcount;
    res->bitvec[0] = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[0] | this->bitvec[0];
    res->popcount -= Bithacks::popper<uint64_t>(static_cast<const DenseBitvec<std::array<uint64_t,8> >& >(bc).bitvec[0] & this->bitvec[0]);
    res->bitvec[1] = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[1] | this->bitvec[1];
    res->popcount -= Bithacks::popper<uint64_t>(static_cast<const DenseBitvec<std::array<uint64_t,8> >& >(bc).bitvec[1] & this->bitvec[1]);
    res->bitvec[2] = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[2] | this->bitvec[2];
    res->popcount -= Bithacks::popper<uint64_t>(static_cast<const DenseBitvec<std::array<uint64_t,8> >& >(bc).bitvec[2] & this->bitvec[2]);
    res->bitvec[3] = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[3] | this->bitvec[3];
    res->popcount -= Bithacks::popper<uint64_t>(static_cast<const DenseBitvec<std::array<uint64_t,8> >& >(bc).bitvec[3] & this->bitvec[3]);
    res->bitvec[4] = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[4] | this->bitvec[4];
    res->popcount -= Bithacks::popper<uint64_t>(static_cast<const DenseBitvec<std::array<uint64_t,8> >& >(bc).bitvec[4] & this->bitvec[4]);
    res->bitvec[5] = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[5] | this->bitvec[5];
    res->popcount -= Bithacks::popper<uint64_t>(static_cast<const DenseBitvec<std::array<uint64_t,8> >& >(bc).bitvec[5] & this->bitvec[5]);
    res->bitvec[6] = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[6] | this->bitvec[6];
    res->popcount -= Bithacks::popper<uint64_t>(static_cast<const DenseBitvec<std::array<uint64_t,8> >& >(bc).bitvec[6] & this->bitvec[6]);
    res->bitvec[7] = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[7] | this->bitvec[7];
    res->popcount -= Bithacks::popper<uint64_t>(static_cast<const DenseBitvec<std::array<uint64_t,8> >& >(bc).bitvec[7] & this->bitvec[7]);
    return res;
}



template <>
DenseBitvec<std::vector<uint64_t> >* DenseBitvec<std::vector<uint64_t> >::setunionMany(std::vector<BitCollection*>::const_iterator begin, std::vector<BitCollection*>::const_iterator end) const
{
    DenseBitvec<std::vector<uint64_t> >* res = new DenseBitvec<std::vector<uint64_t> >(
        bitvec.size() * 64
    );
    for (uint32_t blockIdx = 0; blockIdx < bitvec.size(); ++blockIdx)
    {
        for (auto it = begin; it != end; ++it)
        {
            // update bitvectors
            res->bitvec[blockIdx] = static_cast<const DenseBitvec<std::vector<uint64_t> >*>(*it)->bitvec[blockIdx] | res->bitvec[blockIdx];
        }
        // adapt popcounts
        res->popcount += Bithacks::popper<uint64_t>(res->bitvec[blockIdx]);
    }
    return res;
}

template <>
DenseBitvec<uint32_t>* DenseBitvec<uint32_t>::setunionMany(std::vector<BitCollection*>::const_iterator begin, std::vector<BitCollection*>::const_iterator end) const
{
    uint32_t newBits = 0;
    for (auto it = begin; it != end; ++it)
    {
        // update bitvectors
        newBits = static_cast<const DenseBitvec<uint32_t>*>(*it)->bitvec | newBits;
    }
    // adapt popcounts
    uint32_t pc = Bithacks::popper<uint32_t>(newBits);
    return new DenseBitvec<uint32_t>(std::move(newBits), pc);
}

template <>
DenseBitvec<uint64_t>* DenseBitvec<uint64_t>::setunionMany(std::vector<BitCollection*>::const_iterator begin, std::vector<BitCollection*>::const_iterator end) const
{
    uint64_t newBits = 0;
    for (auto it = begin; it != end; ++it)
    {
        // update bitvectors
        newBits = static_cast<const DenseBitvec<uint64_t>*>(*it)->bitvec | newBits;
    }
    // adapt popcounts
    uint32_t pc = Bithacks::popper<uint64_t>(newBits);
    return new DenseBitvec<uint64_t>(std::move(newBits), pc);
}

template <>
DenseBitvec<std::array<uint64_t,2> >* DenseBitvec<std::array<uint64_t,2> >::setunionMany(std::vector<BitCollection*>::const_iterator begin, std::vector<BitCollection*>::const_iterator end) const
{
    DenseBitvec<std::array<uint64_t,2> >* res = new DenseBitvec<std::array<uint64_t,2> >();
    for (uint32_t blockIdx = 0; blockIdx < 2; ++blockIdx)
    {
        for (auto it = begin; it != end; ++it)
        {
            // update bitvectors
            res->bitvec[blockIdx] = static_cast<const DenseBitvec<std::array<uint64_t,2> >*>(*it)->bitvec[blockIdx] | res->bitvec[blockIdx];
        }
        // adapt popcounts
        res->popcount += Bithacks::popper<uint64_t>(res->bitvec[blockIdx]);
    }
    return res;
}

template <>
DenseBitvec<std::array<uint64_t,4> >* DenseBitvec<std::array<uint64_t,4> >::setunionMany(std::vector<BitCollection*>::const_iterator begin, std::vector<BitCollection*>::const_iterator end) const
{
    DenseBitvec<std::array<uint64_t,4> >* res = new DenseBitvec<std::array<uint64_t,4> >();
    for (uint32_t blockIdx = 0; blockIdx < 4; ++blockIdx)
    {
        for (auto it = begin; it != end; ++it)
        {
            // update bitvectors
            res->bitvec[blockIdx] = static_cast<const DenseBitvec<std::array<uint64_t,4> >*>(*it)->bitvec[blockIdx] | res->bitvec[blockIdx];
        }
        // adapt popcounts
        res->popcount += Bithacks::popper<uint64_t>(res->bitvec[blockIdx]);
    }
    return res;
}

template <>
DenseBitvec<std::array<uint64_t,8> >* DenseBitvec<std::array<uint64_t,8> >::setunionMany(std::vector<BitCollection*>::const_iterator begin, std::vector<BitCollection*>::const_iterator end) const
{
    DenseBitvec<std::array<uint64_t,8> >* res = new DenseBitvec<std::array<uint64_t,8> >();
    for (uint32_t blockIdx = 0; blockIdx < 8; ++blockIdx)
    {
        for (auto it = begin; it != end; ++it)
        {
            // update bitvectors
            res->bitvec[blockIdx] = static_cast<const DenseBitvec<std::array<uint64_t,8> >*>(*it)->bitvec[blockIdx] | res->bitvec[blockIdx];
        }
        // adapt popcounts
        res->popcount += Bithacks::popper<uint64_t>(res->bitvec[blockIdx]);
    }
    return res;
}



template <>
DenseBitvec<std::vector<uint64_t> >* DenseBitvec<std::vector<uint64_t> >::setinterMany(std::vector<BitCollection*>::const_iterator begin, std::vector<BitCollection*>::const_iterator end) const
{
    DenseBitvec<std::vector<uint64_t> >* res = new DenseBitvec<std::vector<uint64_t> >(
        bitvec.size() * 64
    );
    for (uint32_t blockIdx = 0; blockIdx < bitvec.size(); ++blockIdx)
    {
        res->bitvec[blockIdx] = static_cast<const DenseBitvec<std::vector<uint64_t> >*>(*begin)->bitvec[blockIdx];
        for (auto it = begin; it != end; ++it)
        {
            // update bitvectors
            res->bitvec[blockIdx] = static_cast<const DenseBitvec<std::vector<uint64_t> >*>(*it)->bitvec[blockIdx] & res->bitvec[blockIdx];
        }
        // adapt popcounts
        res->popcount += Bithacks::popper<uint64_t>(res->bitvec[blockIdx]);
    }
    return res;
}

template <>
DenseBitvec<uint32_t>* DenseBitvec<uint32_t>::setinterMany(std::vector<BitCollection*>::const_iterator begin, std::vector<BitCollection*>::const_iterator end) const
{
    uint32_t newBits = static_cast<const DenseBitvec<uint32_t>*>(*begin)->bitvec;
    for (auto it = begin; it != end; ++it)
    {
        // update bitvectors
        newBits = static_cast<const DenseBitvec<uint32_t>*>(*it)->bitvec & newBits;
    }
    // adapt popcounts
    uint32_t pc = Bithacks::popper<uint32_t>(newBits);
    return new DenseBitvec<uint32_t>(std::move(newBits), pc);
}

template <>
DenseBitvec<uint64_t>* DenseBitvec<uint64_t>::setinterMany(std::vector<BitCollection*>::const_iterator begin, std::vector<BitCollection*>::const_iterator end) const
{
    uint64_t newBits = static_cast<const DenseBitvec<uint64_t>*>(*begin)->bitvec;
    for (auto it = begin; it != end; ++it)
    {
        // update bitvectors
        newBits = static_cast<const DenseBitvec<uint64_t>*>(*it)->bitvec & newBits;
    }
    // adapt popcounts
    uint32_t pc = Bithacks::popper<uint64_t>(newBits);
    return new DenseBitvec<uint64_t>(std::move(newBits), pc);
}

template <>
DenseBitvec<std::array<uint64_t,2> >* DenseBitvec<std::array<uint64_t,2> >::setinterMany(std::vector<BitCollection*>::const_iterator begin, std::vector<BitCollection*>::const_iterator end) const
{
    DenseBitvec<std::array<uint64_t,2> >* res = new DenseBitvec<std::array<uint64_t,2> >();
    for (uint32_t blockIdx = 0; blockIdx < 2; ++blockIdx)
    {
        res->bitvec[blockIdx] = static_cast<const DenseBitvec<std::array<uint64_t,2> >*>(*begin)->bitvec[blockIdx];
        for (auto it = begin; it != end; ++it)
        {
            // update bitvectors
            res->bitvec[blockIdx] = static_cast<const DenseBitvec<std::array<uint64_t,2> >*>(*it)->bitvec[blockIdx] & res->bitvec[blockIdx];
        }
        // adapt popcounts
        res->popcount += Bithacks::popper<uint64_t>(res->bitvec[blockIdx]);
    }
    return res;
}

template <>
DenseBitvec<std::array<uint64_t,4> >* DenseBitvec<std::array<uint64_t,4> >::setinterMany(std::vector<BitCollection*>::const_iterator begin, std::vector<BitCollection*>::const_iterator end) const
{
    DenseBitvec<std::array<uint64_t,4> >* res = new DenseBitvec<std::array<uint64_t,4> >();
    for (uint32_t blockIdx = 0; blockIdx < 4; ++blockIdx)
    {
        res->bitvec[blockIdx] = static_cast<const DenseBitvec<std::array<uint64_t,4> >*>(*begin)->bitvec[blockIdx];
        for (auto it = begin; it != end; ++it)
        {
            // update bitvectors
            res->bitvec[blockIdx] = static_cast<const DenseBitvec<std::array<uint64_t,4> >*>(*it)->bitvec[blockIdx] & res->bitvec[blockIdx];
        }
        // adapt popcounts
        res->popcount += Bithacks::popper<uint64_t>(res->bitvec[blockIdx]);
    }
    return res;
}

template <>
DenseBitvec<std::array<uint64_t,8> >* DenseBitvec<std::array<uint64_t,8> >::setinterMany(std::vector<BitCollection*>::const_iterator begin, std::vector<BitCollection*>::const_iterator end) const
{
    DenseBitvec<std::array<uint64_t,8> >* res = new DenseBitvec<std::array<uint64_t,8> >();
    for (uint32_t blockIdx = 0; blockIdx < 8; ++blockIdx)
    {
        res->bitvec[blockIdx] = static_cast<const DenseBitvec<std::array<uint64_t,8> >*>(*begin)->bitvec[blockIdx];
        for (auto it = begin; it != end; ++it)
        {
            // update bitvectors
            res->bitvec[blockIdx] = static_cast<const DenseBitvec<std::array<uint64_t,8> >*>(*it)->bitvec[blockIdx] & res->bitvec[blockIdx];
        }
        // adapt popcounts
        res->popcount += Bithacks::popper<uint64_t>(res->bitvec[blockIdx]);
    }
    return res;
}


template <>
void DenseBitvec<std::vector<uint64_t> >::thisUnion(const BitCollection& bc)
{
    this->popcount = 0;
    for (size_t idx = 0; idx < bitvec.size(); ++idx)
    {
        // update bitvectors
        this->bitvec[idx] = static_cast<const DenseBitvec<std::vector<uint64_t> >&>(bc).bitvec[idx] | this->bitvec[idx];
        // adapt popcounts
        this->popcount += Bithacks::popper<uint64_t>(this->bitvec[idx]);
    }
}

template <>
void DenseBitvec<uint32_t>::thisUnion(const BitCollection& bc)
{
    this->bitvec = static_cast<const DenseBitvec<uint32_t>&>(bc).bitvec | this->bitvec;
    this->popcount = Bithacks::popper<uint32_t>(this->bitvec);
}

template <>
void DenseBitvec<uint64_t>::thisUnion(const BitCollection& bc)
{
    this->bitvec = static_cast<const DenseBitvec<uint64_t>&>(bc).bitvec | this->bitvec;
    this->popcount = Bithacks::popper<uint64_t>(this->bitvec);
}

template <>
void DenseBitvec<std::array<uint64_t,2> >::thisUnion(const BitCollection& bc)
{
    this->bitvec[0] = static_cast<const DenseBitvec<std::array<uint64_t,2> >&>(bc).bitvec[0] | this->bitvec[0];
    this->popcount = Bithacks::popper<uint64_t>(this->bitvec[0]);
    this->bitvec[1] = static_cast<const DenseBitvec<std::array<uint64_t,2> >&>(bc).bitvec[1] | this->bitvec[1];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[1]);
}

template <>
void DenseBitvec<std::array<uint64_t,4> >::thisUnion(const BitCollection& bc)
{
    this->bitvec[0] = static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[0] | this->bitvec[0];
    this->popcount = Bithacks::popper<uint64_t>(this->bitvec[0]);
    this->bitvec[1] = static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[1] | this->bitvec[1];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[1]);
    this->bitvec[2] = static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[2] | this->bitvec[2];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[2]);
    this->bitvec[3] = static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[3] | this->bitvec[3];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[3]);
}

template <>
void DenseBitvec<std::array<uint64_t,8> >::thisUnion(const BitCollection& bc)
{
    this->bitvec[0] = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[0] | this->bitvec[0];
    this->popcount = Bithacks::popper<uint64_t>(this->bitvec[0]);
    this->bitvec[1] = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[1] | this->bitvec[1];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[1]);
    this->bitvec[2] = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[2] | this->bitvec[2];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[2]);
    this->bitvec[3] = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[3] | this->bitvec[3];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[3]);
    this->bitvec[4] = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[4] | this->bitvec[4];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[4]);
    this->bitvec[5] = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[5] | this->bitvec[5];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[5]);
    this->bitvec[6] = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[6] | this->bitvec[6];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[6]);
    this->bitvec[7] = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[7] | this->bitvec[7];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[7]);
}



template <>
DenseBitvec<std::vector<uint64_t> >* DenseBitvec<std::vector<uint64_t> >::setinter(const BitCollection& bc) const
{
    DenseBitvec<std::vector<uint64_t> >* res = new DenseBitvec<std::vector<uint64_t> >(bitvec.size() * 64);
    for (size_t idx = 0; idx < bitvec.size(); ++idx)
    {
        uint64_t inter = static_cast<const DenseBitvec<std::vector<uint64_t> >&>(bc).bitvec[idx] & this->bitvec[idx];
        res->popcount += Bithacks::popper<uint64_t>(inter);
        res->bitvec[idx] = inter;
    }
    return res;
}

template <>
DenseBitvec<uint32_t>* DenseBitvec<uint32_t>::setinter(const BitCollection& bc) const
{
    uint32_t inter = static_cast<const DenseBitvec<uint32_t>&>(bc).bitvec & this->bitvec;
    const uint32_t pop = Bithacks::popper<uint32_t>(inter);
    return new DenseBitvec<uint32_t>(std::move(inter), pop);
}

template <>
DenseBitvec<uint64_t>* DenseBitvec<uint64_t>::setinter(const BitCollection& bc) const
{
    uint64_t inter = static_cast<const DenseBitvec<uint64_t>&>(bc).bitvec & this->bitvec;
    const uint64_t pop = Bithacks::popper<uint64_t>(inter);
    return new DenseBitvec<uint64_t>(std::move(inter), pop);
}

template <>
DenseBitvec<std::array<uint64_t,2> >* DenseBitvec<std::array<uint64_t,2> >::setinter(const BitCollection& bc) const
{
    DenseBitvec<std::array<uint64_t,2> >* res = new DenseBitvec<std::array<uint64_t,2> >();
    uint64_t inter = static_cast<const DenseBitvec<std::array<uint64_t,2> >&>(bc).bitvec[0] & this->bitvec[0];
    res->bitvec[0] = inter;
    res->popcount += Bithacks::popper<uint64_t>(inter);
    inter = static_cast<const DenseBitvec<std::array<uint64_t,2> >&>(bc).bitvec[1] & this->bitvec[1];
    res->bitvec[1] = inter;
    res->popcount += Bithacks::popper<uint64_t>(inter);
    return res;
}

template <>
DenseBitvec<std::array<uint64_t,4> >* DenseBitvec<std::array<uint64_t,4> >::setinter(const BitCollection& bc) const
{
    DenseBitvec<std::array<uint64_t,4> >* res = new DenseBitvec<std::array<uint64_t,4> >();
    uint64_t inter = static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[0] & this->bitvec[0];
    res->bitvec[0] = inter;
    res->popcount += Bithacks::popper<uint64_t>(inter);
    inter = static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[1] & this->bitvec[1];
    res->bitvec[1] = inter;
    res->popcount += Bithacks::popper<uint64_t>(inter);
    inter =  static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[2] & this->bitvec[2];
    res->bitvec[2] = inter;
    res->popcount += Bithacks::popper<uint64_t>(inter);
    inter = static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[3] & this->bitvec[3];
    res->bitvec[3] = inter;
    res->popcount += Bithacks::popper<uint64_t>(inter);

    return res;
}

template <>
DenseBitvec<std::array<uint64_t,8> >* DenseBitvec<std::array<uint64_t,8> >::setinter(const BitCollection& bc) const
{
    DenseBitvec<std::array<uint64_t,8> >* res = new DenseBitvec<std::array<uint64_t,8> >();
    uint64_t inter = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[0] & this->bitvec[0];
    res->bitvec[0] = inter;
    res->popcount += Bithacks::popper<uint64_t>(inter);
    inter = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[1] & this->bitvec[1];
    res->bitvec[1] = inter;
    res->popcount += Bithacks::popper<uint64_t>(inter);
    inter = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[2] & this->bitvec[2];
    res->bitvec[2] += inter;
    res->popcount += Bithacks::popper<uint64_t>(inter);
    inter = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[3] & this->bitvec[3];
    res->bitvec[3] = inter;
    res->popcount += Bithacks::popper<uint64_t>(inter);
    inter = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[4] & this->bitvec[4];
    res->bitvec[4] = inter;
    res->popcount += Bithacks::popper<uint64_t>(inter);
    inter = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[5] & this->bitvec[5];
    res->bitvec[5] = inter;
    res->popcount += Bithacks::popper<uint64_t>(inter);
    inter = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[6] & this->bitvec[6];
    res->bitvec[6] = inter;
    res->popcount += Bithacks::popper<uint64_t>(inter);
    inter = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[7] & this->bitvec[7];
    res->bitvec[7] = inter;
    res->popcount += Bithacks::popper<uint64_t>(inter);
    return res;
}



template <>
void DenseBitvec<std::vector<uint64_t> >::thisInter(const BitCollection& bc)
{
    this->popcount = 0;
    for (size_t idx = 0; idx < bitvec.size(); ++idx)
    {
        // update bitvectors
        this->bitvec[idx] = static_cast<const DenseBitvec<std::vector<uint64_t> >& >(bc).bitvec[idx] & this->bitvec[idx];
        // adapt popcounts
        this->popcount += Bithacks::popper<uint64_t>(this->bitvec[idx]);
    }
}

template <>
void DenseBitvec<uint32_t>::thisInter(const BitCollection& bc)
{
    this->bitvec = static_cast<const DenseBitvec<uint32_t>& >(bc).bitvec & this->bitvec;
    this->popcount = Bithacks::popper<uint32_t>(this->bitvec);
}

template <>
void DenseBitvec<uint64_t>::thisInter(const BitCollection& bc)
{
    this->bitvec = static_cast<const DenseBitvec<uint64_t>& >(bc).bitvec & this->bitvec;
    this->popcount = Bithacks::popper<uint64_t>(this->bitvec);
}

template <>
void DenseBitvec<std::array<uint64_t,2> >::thisInter(const BitCollection& bc)
{
    this->bitvec[0] = static_cast<const DenseBitvec<std::array<uint64_t,2> >& >(bc).bitvec[0] & this->bitvec[0];
    this->popcount = Bithacks::popper<uint64_t>(this->bitvec[0]);
    this->bitvec[1] = static_cast<const DenseBitvec<std::array<uint64_t,2> >& >(bc).bitvec[1] & this->bitvec[1];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[1]);
}

template <>
void DenseBitvec<std::array<uint64_t,4> >::thisInter(const BitCollection& bc)
{
    this->bitvec[0] = static_cast<const DenseBitvec<std::array<uint64_t,4> >& >(bc).bitvec[0] & this->bitvec[0];
    this->popcount = Bithacks::popper<uint64_t>(this->bitvec[0]);
    this->bitvec[1] = static_cast<const DenseBitvec<std::array<uint64_t,4> >& >(bc).bitvec[1] & this->bitvec[1];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[1]);
    this->bitvec[2] = static_cast<const DenseBitvec<std::array<uint64_t,4> >& >(bc).bitvec[2] & this->bitvec[2];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[2]);
    this->bitvec[3] = static_cast<const DenseBitvec<std::array<uint64_t,4> >& >(bc).bitvec[3] & this->bitvec[3];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[3]);
}

template <>
void DenseBitvec<std::array<uint64_t,8> >::thisInter(const BitCollection& bc)
{
    this->bitvec[0] = static_cast<const DenseBitvec<std::array<uint64_t,8> >& >(bc).bitvec[0] & this->bitvec[0];
    this->popcount = Bithacks::popper<uint64_t>(this->bitvec[0]);
    this->bitvec[1] = static_cast<const DenseBitvec<std::array<uint64_t,8> >& >(bc).bitvec[1] & this->bitvec[1];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[1]);
    this->bitvec[2] = static_cast<const DenseBitvec<std::array<uint64_t,8> >& >(bc).bitvec[2] & this->bitvec[2];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[2]);
    this->bitvec[3] = static_cast<const DenseBitvec<std::array<uint64_t,8> >& >(bc).bitvec[3] & this->bitvec[3];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[3]);
    this->bitvec[4] = static_cast<const DenseBitvec<std::array<uint64_t,8> >& >(bc).bitvec[4] & this->bitvec[4];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[4]);
    this->bitvec[5] = static_cast<const DenseBitvec<std::array<uint64_t,8> >& >(bc).bitvec[5] & this->bitvec[5];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[5]);
    this->bitvec[6] = static_cast<const DenseBitvec<std::array<uint64_t,8> >& >(bc).bitvec[6] & this->bitvec[6];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[6]);
    this->bitvec[7] = static_cast<const DenseBitvec<std::array<uint64_t,8> >& >(bc).bitvec[7] & this->bitvec[7];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[7]);
}




template <>
void DenseBitvec<std::vector<uint64_t> >::interMany(const std::vector<const DenseBitvec<std::vector<uint64_t> >*>& bc)
{
    this->popcount = 0;
    for (size_t idx = 0; idx < bitvec.size(); ++idx)
    {
        this->bitvec[idx] = bc[0]->bitvec[idx];
        for (uint32_t i = 1; i < bc.size(); ++i)
        {
            this->bitvec[idx] &= bc[i]->bitvec[idx];
        }
        this->popcount += Bithacks::popper<uint64_t>(this->bitvec[idx]);
    }
}

template <>
void DenseBitvec<uint32_t>::interMany(const std::vector<const DenseBitvec<uint32_t>*>& bc)
{
    this->bitvec = bc[0]->bitvec;
    for (uint32_t i = 1; i < bc.size(); ++i)
    {
        this->bitvec &= bc[i]->bitvec;
    }
    this->popcount = Bithacks::popper<uint32_t>(this->bitvec);
}

template <>
void DenseBitvec<uint64_t>::interMany(const std::vector<const DenseBitvec<uint64_t>*>& bc)
{
    this->bitvec = bc[0]->bitvec;
    for (uint32_t i = 1; i < bc.size(); ++i)
    {
        this->bitvec &= bc[i]->bitvec;
    }
    this->popcount = Bithacks::popper<uint64_t>(this->bitvec);
}

template <>
void DenseBitvec<std::array<uint64_t,2> >::interMany(const std::vector<const DenseBitvec<std::array<uint64_t,2> >*>& bc)
{
    this->bitvec[0] = bc[0]->bitvec[0];
    this->bitvec[1] = bc[0]->bitvec[1];
    for (uint32_t i = 1; i < bc.size(); ++i)
    {
        this->bitvec[0] &= bc[i]->bitvec[0];
        this->bitvec[1] &= bc[i]->bitvec[1];
    }
    this->popcount = Bithacks::popper<uint64_t>(this->bitvec[0]);
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[1]);
}

template <>
void DenseBitvec<std::array<uint64_t,4> >::interMany(const std::vector<const DenseBitvec<std::array<uint64_t,4> >*>& bc)
{
    this->bitvec[0] = bc[0]->bitvec[0];
    this->bitvec[1] = bc[0]->bitvec[1];
    this->bitvec[2] = bc[0]->bitvec[2];
    this->bitvec[3] = bc[0]->bitvec[3];
    for (uint32_t i = 1; i < bc.size(); ++i)
    {
        this->bitvec[0] &= bc[i]->bitvec[0];
        this->bitvec[1] &= bc[i]->bitvec[1];
        this->bitvec[2] &= bc[i]->bitvec[2];
        this->bitvec[3] &= bc[i]->bitvec[3];
    }
    this->popcount = Bithacks::popper<uint64_t>(this->bitvec[0]);
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[1]);
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[3]);
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[4]);
}

template <>
void DenseBitvec<std::array<uint64_t,8> >::interMany(const std::vector<const DenseBitvec<std::array<uint64_t,8> >*>& bc)
{
    this->bitvec[0] = bc[0]->bitvec[0];
    this->bitvec[1] = bc[0]->bitvec[1];
    this->bitvec[2] = bc[0]->bitvec[2];
    this->bitvec[3] = bc[0]->bitvec[3];
    this->bitvec[4] = bc[0]->bitvec[4];
    this->bitvec[5] = bc[0]->bitvec[5];
    this->bitvec[6] = bc[0]->bitvec[6];
    this->bitvec[7] = bc[0]->bitvec[7];
    for (uint32_t i = 1; i < bc.size(); ++i)
    {
        this->bitvec[0] &= bc[i]->bitvec[0];
        this->bitvec[1] &= bc[i]->bitvec[1];
        this->bitvec[2] &= bc[i]->bitvec[2];
        this->bitvec[3] &= bc[i]->bitvec[3];
        this->bitvec[4] &= bc[i]->bitvec[4];
        this->bitvec[5] &= bc[i]->bitvec[5];
        this->bitvec[6] &= bc[i]->bitvec[6];
        this->bitvec[7] &= bc[i]->bitvec[7];
    }
    this->popcount = Bithacks::popper<uint64_t>(this->bitvec[0]);
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[1]);
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[2]);
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[3]);
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[4]);
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[5]);
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[6]);
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[7]);
}






template <>
inline DenseBitvec<std::vector<uint64_t> >* DenseBitvec<std::vector<uint64_t> >::setdiff(const BitCollection& bc) const
{
    DenseBitvec<std::vector<uint64_t> >* res = new DenseBitvec<std::vector<uint64_t> >(bitvec.size() * 64);
    for (size_t idx = 0; idx < bitvec.size(); ++idx)
    {
        uint64_t diff = ~static_cast<const DenseBitvec<std::vector<uint64_t> >&>(bc).bitvec[idx] & this->bitvec[idx];
        res->popcount += Bithacks::popper<uint64_t>(diff);
        res->bitvec[idx] = diff;
    }
    return res;
}

template <>
inline DenseBitvec<uint32_t>* DenseBitvec<uint32_t>::setdiff(const BitCollection& bc) const
{
    uint32_t diff = ~static_cast<const DenseBitvec<uint32_t>&>(bc).bitvec & this->bitvec;
    const uint32_t pop = Bithacks::popper<uint32_t>(diff);
    return new DenseBitvec<uint32_t>(std::move(diff), pop);
}

template <>
inline DenseBitvec<uint64_t>* DenseBitvec<uint64_t>::setdiff(const BitCollection& bc) const
{
    uint64_t diff = ~static_cast<const DenseBitvec<uint64_t>&>(bc).bitvec & this->bitvec;
    const uint32_t pop = Bithacks::popper<uint64_t>(diff);
    return new DenseBitvec<uint64_t>(std::move(diff), pop);
}

template <>
inline DenseBitvec<std::array<uint64_t,2> >* DenseBitvec<std::array<uint64_t,2> >::setdiff(const BitCollection& bc) const
{
    DenseBitvec<std::array<uint64_t,2> >* res = new DenseBitvec<std::array<uint64_t,2> >();
    uint64_t diff = ~static_cast<const DenseBitvec<std::array<uint64_t,2> >&>(bc).bitvec[0] & this->bitvec[0];
    res->bitvec[0] = diff;
    res->popcount += Bithacks::popper<uint64_t>(diff);
    diff = ~static_cast<const DenseBitvec<std::array<uint64_t,2> >&>(bc).bitvec[1] & this->bitvec[1];
    res->bitvec[1] = diff;
    res->popcount += Bithacks::popper<uint64_t>(diff);
    return res;
}

template <>
inline DenseBitvec<std::array<uint64_t,4> >* DenseBitvec<std::array<uint64_t,4> >::setdiff(const BitCollection& bc) const
{
    DenseBitvec<std::array<uint64_t,4> >* res = new DenseBitvec<std::array<uint64_t,4> >();
    uint64_t diff = ~static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[0] & this->bitvec[0];
    res->bitvec[0] = diff;
    res->popcount += Bithacks::popper<uint64_t>(diff);
    diff = ~static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[1] & this->bitvec[1];
    res->bitvec[1] = diff;
    res->popcount += Bithacks::popper<uint64_t>(diff);
    diff = ~static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[2] & this->bitvec[2];
    res->bitvec[2] = diff;
    res->popcount += Bithacks::popper<uint64_t>(diff);
    diff = ~static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[3] & this->bitvec[3];
    res->bitvec[3] = diff;
    res->popcount += Bithacks::popper<uint64_t>(diff);
    return res;
}

template <>
inline DenseBitvec<std::array<uint64_t,8> >* DenseBitvec<std::array<uint64_t,8> >::setdiff(const BitCollection& bc) const
{
    DenseBitvec<std::array<uint64_t,8> >* res = new DenseBitvec<std::array<uint64_t,8> >();
    uint64_t diff = ~static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[0] & this->bitvec[0];
    res->bitvec[0] = diff;
    res->popcount += Bithacks::popper<uint64_t>(diff);
    diff = ~static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[1] & this->bitvec[1];
    res->bitvec[1] = diff;
    res->popcount += Bithacks::popper<uint64_t>(diff);
    diff = ~static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[2] & this->bitvec[2];
    res->bitvec[2] = diff;
    res->popcount += Bithacks::popper<uint64_t>(diff);
    diff = ~static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[3] & this->bitvec[3];
    res->bitvec[3] = diff;
    res->popcount += Bithacks::popper<uint64_t>(diff);
    diff = ~static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[4] & this->bitvec[4];
    res->bitvec[4] = diff;
    res->popcount += Bithacks::popper<uint64_t>(diff);
    diff = ~static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[5] & this->bitvec[5];
    res->bitvec[5] = diff;
    res->popcount += Bithacks::popper<uint64_t>(diff);
    diff = ~static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[6] & this->bitvec[6];
    res->bitvec[6] = diff;
    res->popcount += Bithacks::popper<uint64_t>(diff);
    diff = ~static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[7] & this->bitvec[7];
    res->bitvec[7] = diff;
    res->popcount += Bithacks::popper<uint64_t>(diff);
    return res;
}


template <>
inline DenseBitvec<std::vector<uint64_t> >* DenseBitvec<std::vector<uint64_t> >::setsymdiff(const DenseBitvec<std::vector<uint64_t> >& bc) const
{
    DenseBitvec<std::vector<uint64_t> >* res = new DenseBitvec<std::vector<uint64_t> >(bitvec.size() * 64);
    for (size_t idx = 0; idx < bitvec.size(); ++idx)
    {
        uint64_t symdiff = bc.bitvec[idx] ^ this->bitvec[idx];
        res->popcount += Bithacks::popper<uint64_t>(symdiff);
        res->bitvec[idx] = symdiff;
    }
    return res;
}

template <>
inline DenseBitvec<uint32_t>* DenseBitvec<uint32_t>::setsymdiff(const DenseBitvec<uint32_t>& bc) const
{
    uint32_t symdiff = bc.bitvec ^ this->bitvec;
    const uint32_t pop = Bithacks::popper<uint32_t>(symdiff);
    return new DenseBitvec<uint32_t>(std::move(symdiff), pop);
}

template <>
inline DenseBitvec<uint64_t>* DenseBitvec<uint64_t>::setsymdiff(const DenseBitvec<uint64_t>& bc) const
{
    uint64_t symdiff = bc.bitvec ^ this->bitvec;
    const uint32_t pop = Bithacks::popper<uint64_t>(symdiff);
    return new DenseBitvec<uint64_t>(std::move(symdiff), pop);
}

template <>
inline DenseBitvec<std::array<uint64_t,2> >* DenseBitvec<std::array<uint64_t,2> >::setsymdiff(const DenseBitvec<std::array<uint64_t,2> >& bc) const
{
    DenseBitvec<std::array<uint64_t,2> >* res = new DenseBitvec<std::array<uint64_t,2> >();
    uint64_t symdiff = bc.bitvec[0] ^ this->bitvec[0];
    res->bitvec[0] = symdiff;
    res->popcount += Bithacks::popper<uint64_t>(symdiff);
    symdiff = bc.bitvec[1] ^ this->bitvec[1];
    res->bitvec[1] = symdiff;
    res->popcount += Bithacks::popper<uint64_t>(symdiff);
    return res;
}

template <>
inline DenseBitvec<std::array<uint64_t,4> >* DenseBitvec<std::array<uint64_t,4> >::setsymdiff(const DenseBitvec<std::array<uint64_t,4> >& bc) const
{
    DenseBitvec<std::array<uint64_t,4> >* res = new DenseBitvec<std::array<uint64_t,4> >();
    uint64_t symdiff = bc.bitvec[0] ^ this->bitvec[0];
    res->bitvec[0] = symdiff;
    res->popcount += Bithacks::popper<uint64_t>(symdiff);
    symdiff = bc.bitvec[1] ^ this->bitvec[1];
    res->bitvec[1] = symdiff;
    res->popcount += Bithacks::popper<uint64_t>(symdiff);
    symdiff = bc.bitvec[2] ^ this->bitvec[2];
    res->bitvec[2] = symdiff;
    res->popcount += Bithacks::popper<uint64_t>(symdiff);
    symdiff = bc.bitvec[3] ^ this->bitvec[3];
    res->bitvec[3] = symdiff;
    res->popcount += Bithacks::popper<uint64_t>(symdiff);
    return res;
}

template <>
inline DenseBitvec<std::array<uint64_t,8> >* DenseBitvec<std::array<uint64_t,8> >::setsymdiff(const DenseBitvec<std::array<uint64_t,8> >& bc) const
{
    DenseBitvec<std::array<uint64_t,8> >* res = new DenseBitvec<std::array<uint64_t,8> >();
    uint64_t symdiff = bc.bitvec[0] & this->bitvec[0];
    res->bitvec[0] = symdiff;
    res->popcount += Bithacks::popper<uint64_t>(symdiff);
    symdiff = bc.bitvec[1] ^ this->bitvec[1];
    res->bitvec[1] = symdiff;
    res->popcount += Bithacks::popper<uint64_t>(symdiff);
    symdiff = bc.bitvec[2] ^ this->bitvec[2];
    res->bitvec[2] = symdiff;
    res->popcount += Bithacks::popper<uint64_t>(symdiff);
    symdiff = bc.bitvec[3] ^ this->bitvec[3];
    res->bitvec[3] = symdiff;
    res->popcount += Bithacks::popper<uint64_t>(symdiff);
    symdiff = bc.bitvec[4] ^ this->bitvec[4];
    res->bitvec[4] = symdiff;
    res->popcount += Bithacks::popper<uint64_t>(symdiff);
    symdiff = bc.bitvec[5] ^ this->bitvec[5];
    res->bitvec[5] = symdiff;
    res->popcount += Bithacks::popper<uint64_t>(symdiff);
    symdiff = bc.bitvec[6] ^ this->bitvec[6];
    res->bitvec[6] = symdiff;
    res->popcount += Bithacks::popper<uint64_t>(symdiff);
    symdiff = bc.bitvec[7] ^ this->bitvec[7];
    res->bitvec[7] = symdiff;
    res->popcount += Bithacks::popper<uint64_t>(symdiff);
    return res;
}




template <>
DenseBitvec<std::vector<uint64_t> >* DenseBitvec<std::vector<uint64_t> >::symdiffMany(const std::vector<const DenseBitvec<std::vector<uint64_t> >*>& bc)
{
    DenseBitvec<std::vector<uint64_t> >* filtered = new DenseBitvec<std::vector<uint64_t> >(bitvec.size() * 64);
    this->popcount = 0;
    for (size_t idx = 0; idx < bitvec.size(); ++idx)
    {
        this->bitvec[idx] = bc[0]->bitvec[idx];
        filtered->bitvec[idx] = 0;

        for (uint32_t i = 1; i < bc.size(); ++i)
        {
            const uint64_t oldVal = this->bitvec[idx];
            this->bitvec[idx] = (this->bitvec[idx] ^ bc[i]->bitvec[idx]) & ~filtered->bitvec[idx];
            filtered->bitvec[idx] = (filtered->bitvec[idx] | oldVal | bc[i]->bitvec[idx]) & ~this->bitvec[idx];
        }
        this->popcount += Bithacks::popper<uint64_t>(this->bitvec[idx]);
        filtered->popcount += Bithacks::popper<uint64_t>(filtered->bitvec[idx]);
    }
    return filtered;
}

template <>
DenseBitvec<uint32_t>* DenseBitvec<uint32_t>::symdiffMany(const std::vector<const DenseBitvec<uint32_t>*>& bc)
{
    this->bitvec = bc[0]->bitvec;
    uint32_t filtered = 0;

    for (uint32_t i = 1; i < bc.size(); ++i)
    {
        const uint32_t oldVal = this->bitvec;
        this->bitvec = (this->bitvec ^ bc[i]->bitvec) & ~filtered;
        filtered = (filtered | oldVal | bc[i]->bitvec) & ~this->bitvec;
    }
    this->popcount = Bithacks::popper<uint32_t>(this->bitvec);
    return new DenseBitvec<uint32_t>(std::move(filtered), Bithacks::popper<uint32_t>(filtered));
}

template <>
DenseBitvec<uint64_t>* DenseBitvec<uint64_t>::symdiffMany(const std::vector<const DenseBitvec<uint64_t>*>& bc)
{
    this->bitvec = bc[0]->bitvec;
    uint64_t filtered = 0;

    for (uint32_t i = 1; i < bc.size(); ++i)
    {
        const uint64_t oldVal = this->bitvec;
        this->bitvec = (this->bitvec ^ bc[i]->bitvec) & ~filtered;
        filtered = (filtered | oldVal | bc[i]->bitvec) & ~this->bitvec;
    }
    this->popcount = Bithacks::popper<uint64_t>(this->bitvec);
    return new DenseBitvec<uint64_t>(std::move(filtered), Bithacks::popper<uint64_t>(filtered));
}

template <>
DenseBitvec<std::array<uint64_t,2> >* DenseBitvec<std::array<uint64_t,2> >::symdiffMany(const std::vector<const DenseBitvec<std::array<uint64_t,2> >*>& bc)
{
    DenseBitvec<std::array<uint64_t,2> >* filtered = new DenseBitvec<std::array<uint64_t,2> >();
    this->popcount = 0;
    for (size_t idx = 0; idx < bitvec.size(); ++idx)
    {
        this->bitvec[idx] = bc[0]->bitvec[idx];
        filtered->bitvec[idx] = 0;

        for (uint32_t i = 1; i < bc.size(); ++i)
        {
            const uint64_t oldVal = this->bitvec[idx];
            this->bitvec[idx] = (this->bitvec[idx] ^ bc[i]->bitvec[idx]) & ~filtered->bitvec[idx];
            filtered->bitvec[idx] = (filtered->bitvec[idx] | oldVal | bc[i]->bitvec[idx]) & ~this->bitvec[idx];
        }
        this->popcount += Bithacks::popper<uint64_t>(this->bitvec[idx]);
        filtered->popcount += Bithacks::popper<uint64_t>(filtered->bitvec[idx]);
    }
    return filtered;
}

template <>
DenseBitvec<std::array<uint64_t,4> >* DenseBitvec<std::array<uint64_t,4> >::symdiffMany(const std::vector<const DenseBitvec<std::array<uint64_t,4> >*>& bc)
{
    DenseBitvec<std::array<uint64_t,4> >* filtered = new DenseBitvec<std::array<uint64_t,4> >();
    this->popcount = 0;
    for (size_t idx = 0; idx < bitvec.size(); ++idx)
    {
        this->bitvec[idx] = bc[0]->bitvec[idx];
        filtered->bitvec[idx] = 0;

        for (uint32_t i = 1; i < bc.size(); ++i)
        {
            const uint64_t oldVal = this->bitvec[idx];
            this->bitvec[idx] = (this->bitvec[idx] ^ bc[i]->bitvec[idx]) & ~filtered->bitvec[idx];
            filtered->bitvec[idx] = (filtered->bitvec[idx] | oldVal | bc[i]->bitvec[idx]) & ~this->bitvec[idx];
        }
        this->popcount += Bithacks::popper<uint64_t>(this->bitvec[idx]);
        filtered->popcount += Bithacks::popper<uint64_t>(filtered->bitvec[idx]);
    }
    return filtered;
}

template <>
DenseBitvec<std::array<uint64_t,8> >* DenseBitvec<std::array<uint64_t,8> >::symdiffMany(const std::vector<const DenseBitvec<std::array<uint64_t,8> >*>& bc)
{
    DenseBitvec<std::array<uint64_t,8> >* filtered = new DenseBitvec<std::array<uint64_t,8> >();
    this->popcount = 0;
    for (size_t idx = 0; idx < bitvec.size(); ++idx)
    {
        this->bitvec[idx] = bc[0]->bitvec[idx];
        filtered->bitvec[idx] = 0;

        for (uint32_t i = 1; i < bc.size(); ++i)
        {
            const uint64_t oldVal = this->bitvec[idx];
            this->bitvec[idx] = (this->bitvec[idx] ^ bc[i]->bitvec[idx]) & ~filtered->bitvec[idx];
            filtered->bitvec[idx] = (filtered->bitvec[idx] | oldVal | bc[i]->bitvec[idx]) & ~this->bitvec[idx];
        }
        this->popcount += Bithacks::popper<uint64_t>(this->bitvec[idx]);
        filtered->popcount += Bithacks::popper<uint64_t>(filtered->bitvec[idx]);
    }
    return filtered;
}






template <>
DenseBitvec<std::vector<uint64_t> >* DenseBitvec<std::vector<uint64_t> >::xorJoin(const DenseBitvec<std::vector<uint64_t> >& thisFiltered, const DenseBitvec<std::vector<uint64_t> >& otherXor, const DenseBitvec<std::vector<uint64_t> >& otherFiltered) const
{
    DenseBitvec<std::vector<uint64_t> >* res = new DenseBitvec<std::vector<uint64_t> >(bitvec.size() * 64);
    for (size_t i = 0; i < this->bitvec.size(); ++i)
    {
        res->bitvec[i] = (this->bitvec[i] ^ otherXor.bitvec[i]) & ~(thisFiltered.bitvec[i] | otherFiltered.bitvec[i]);
        res->popcount += Bithacks::popper<uint64_t>(res->bitvec[i]);
    }
    return res;
}

template <>
DenseBitvec<uint32_t>* DenseBitvec<uint32_t>::xorJoin(const DenseBitvec<uint32_t>& thisFiltered, const DenseBitvec<uint32_t>& otherXor, const DenseBitvec<uint32_t>& otherFiltered) const
{
    uint32_t xord = (this->bitvec ^ otherXor.bitvec) & ~(thisFiltered.bitvec | otherFiltered.bitvec);
    const uint32_t pop = Bithacks::popper<uint32_t>(xord);
    return new DenseBitvec<uint32_t>(std::move(xord), pop);
}

template <>
DenseBitvec<uint64_t>* DenseBitvec<uint64_t>::xorJoin(const DenseBitvec<uint64_t>& thisFiltered, const DenseBitvec<uint64_t>& otherXor, const DenseBitvec<uint64_t>& otherFiltered) const
{
    uint64_t xord = (this->bitvec ^ otherXor.bitvec) & ~(thisFiltered.bitvec | otherFiltered.bitvec);
    const uint32_t pop = Bithacks::popper<uint64_t>(xord);
    return new DenseBitvec<uint64_t>(std::move(xord), pop);
}

template <>
DenseBitvec<std::array<uint64_t,2> >* DenseBitvec<std::array<uint64_t,2> >::xorJoin(const DenseBitvec<std::array<uint64_t,2> >& thisFiltered, const DenseBitvec<std::array<uint64_t,2> >& otherXor, const DenseBitvec<std::array<uint64_t,2> >& otherFiltered) const
{
    DenseBitvec<std::array<uint64_t,2> >* res = new DenseBitvec<std::array<uint64_t,2> >();
    uint64_t xord = (this->bitvec[0] ^ otherXor.bitvec[0]) & ~(thisFiltered.bitvec[0] | otherFiltered.bitvec[0]);
    res->bitvec[0] = xord;
    res->popcount += Bithacks::popper<uint64_t>(xord);
    xord = (this->bitvec[1] ^ otherXor.bitvec[1]) & ~(thisFiltered.bitvec[1] | otherFiltered.bitvec[1]);
    res->bitvec[1] = xord;
    res->popcount += Bithacks::popper<uint64_t>(xord);
    return res;
}

template <>
DenseBitvec<std::array<uint64_t,4> >* DenseBitvec<std::array<uint64_t,4> >::xorJoin(const DenseBitvec<std::array<uint64_t,4> >& thisFiltered, const DenseBitvec<std::array<uint64_t,4> >& otherXor, const DenseBitvec<std::array<uint64_t,4> >& otherFiltered) const
{
    DenseBitvec<std::array<uint64_t,4> >* res = new DenseBitvec<std::array<uint64_t,4> >();
    uint64_t xord = (this->bitvec[0] ^ otherXor.bitvec[0]) & ~(thisFiltered.bitvec[0] | otherFiltered.bitvec[0]);
    res->bitvec[0] = xord;
    res->popcount += Bithacks::popper<uint64_t>(xord);
    xord = (this->bitvec[1] ^ otherXor.bitvec[1]) & ~(thisFiltered.bitvec[1] | otherFiltered.bitvec[1]);
    res->bitvec[1] = xord;
    res->popcount += Bithacks::popper<uint64_t>(xord);
    xord = (this->bitvec[2] ^ otherXor.bitvec[2]) & ~(thisFiltered.bitvec[2] | otherFiltered.bitvec[2]);
    res->bitvec[2] = xord;
    res->popcount += Bithacks::popper<uint64_t>(xord);
    xord = (this->bitvec[3] ^ otherXor.bitvec[3]) & ~(thisFiltered.bitvec[3] | otherFiltered.bitvec[3]);
    res->bitvec[3] = xord;
    res->popcount += Bithacks::popper<uint64_t>(xord);
    return res;
}

template <>
DenseBitvec<std::array<uint64_t,8> >* DenseBitvec<std::array<uint64_t,8> >::xorJoin(const DenseBitvec<std::array<uint64_t,8> >& thisFiltered, const DenseBitvec<std::array<uint64_t,8> >& otherXor, const DenseBitvec<std::array<uint64_t,8> >& otherFiltered) const
{
    DenseBitvec<std::array<uint64_t,8> >* res = new DenseBitvec<std::array<uint64_t,8> >();
    uint64_t xord = (this->bitvec[0] ^ otherXor.bitvec[0]) & ~(thisFiltered.bitvec[0] | otherFiltered.bitvec[0]);
    res->bitvec[0] = xord;
    res->popcount += Bithacks::popper<uint64_t>(xord);
    xord = (this->bitvec[1] ^ otherXor.bitvec[1]) & ~(thisFiltered.bitvec[1] | otherFiltered.bitvec[1]);
    res->bitvec[1] = xord;
    res->popcount += Bithacks::popper<uint64_t>(xord);
    xord = (this->bitvec[2] ^ otherXor.bitvec[2]) & ~(thisFiltered.bitvec[2] | otherFiltered.bitvec[2]);
    res->bitvec[2] = xord;
    res->popcount += Bithacks::popper<uint64_t>(xord);
    xord = (this->bitvec[3] ^ otherXor.bitvec[3]) & ~(thisFiltered.bitvec[3] | otherFiltered.bitvec[3]);
    res->bitvec[3] = xord;
    res->popcount += Bithacks::popper<uint64_t>(xord);
    xord = (this->bitvec[4] ^ otherXor.bitvec[4]) & ~(thisFiltered.bitvec[4] | otherFiltered.bitvec[4]);
    res->bitvec[4] = xord;
    res->popcount += Bithacks::popper<uint64_t>(xord);
    xord = (this->bitvec[5] ^ otherXor.bitvec[5]) & ~(thisFiltered.bitvec[5] | otherFiltered.bitvec[5]);
    res->bitvec[5] = xord;
    res->popcount += Bithacks::popper<uint64_t>(xord);
    xord = (this->bitvec[6] ^ otherXor.bitvec[6]) & ~(thisFiltered.bitvec[6] | otherFiltered.bitvec[6]);
    res->bitvec[6] = xord;
    res->popcount += Bithacks::popper<uint64_t>(xord);
    xord = (this->bitvec[7] ^ otherXor.bitvec[7]) & ~(thisFiltered.bitvec[7] | otherFiltered.bitvec[7]);
    res->bitvec[7] = xord;
    res->popcount += Bithacks::popper<uint64_t>(xord);
    return res;
}




template <>
inline DenseBitvec<std::vector<uint64_t> >& DenseBitvec<std::vector<uint64_t> >::diffArg(const BitCollection& bc)
{
    this->popcount = 0;
    for (size_t idx = 0; idx < bitvec.size(); ++idx)
    {
        this->bitvec[idx] &= ~static_cast<const DenseBitvec<std::vector<uint64_t> >&>(bc).bitvec[idx];
        this->popcount += Bithacks::popper<uint64_t>(this->bitvec[idx]);
    }
    return *this;
}

template <>
inline DenseBitvec<uint32_t>& DenseBitvec<uint32_t>::diffArg(const BitCollection& bc)
{
    this->bitvec &= ~static_cast<const DenseBitvec<uint32_t>&>(bc).bitvec;
    this->popcount = Bithacks::popper<uint32_t>(this->bitvec);
    return *this;
}

template <>
inline DenseBitvec<uint64_t>& DenseBitvec<uint64_t>::diffArg(const BitCollection& bc)
{
    this->bitvec &= ~static_cast<const DenseBitvec<uint64_t>&>(bc).bitvec;
    this->popcount = Bithacks::popper<uint64_t>(this->bitvec);
    return *this;
}

template <>
inline DenseBitvec<std::array<uint64_t,2> >& DenseBitvec<std::array<uint64_t,2> >::diffArg(const BitCollection& bc)
{
    this->bitvec[0] &= ~static_cast<const DenseBitvec<std::array<uint64_t,2> >&>(bc).bitvec[0];
    this->bitvec[1] &= ~static_cast<const DenseBitvec<std::array<uint64_t,2> >&>(bc).bitvec[1];
    this->popcount = Bithacks::popper<uint64_t>(this->bitvec[0]) + Bithacks::popper<uint64_t>(this->bitvec[1]);
    return *this;
}

template <>
inline DenseBitvec<std::array<uint64_t,4> >& DenseBitvec<std::array<uint64_t,4> >::diffArg(const BitCollection& bc)
{
    this->bitvec[0] &= ~static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[0];
    this->bitvec[1] &= ~static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[1];
    this->bitvec[2] &= ~static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[2];
    this->bitvec[3] &= ~static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[3];
    this->popcount = Bithacks::popper<uint64_t>(this->bitvec[0]) + Bithacks::popper<uint64_t>(this->bitvec[1])
        + Bithacks::popper<uint64_t>(this->bitvec[2]) + Bithacks::popper<uint64_t>(this->bitvec[3]);
    return *this;
}

template <>
inline DenseBitvec<std::array<uint64_t,8> >& DenseBitvec<std::array<uint64_t,8> >::diffArg(const BitCollection& bc)
{
    this->bitvec[0] &= ~static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[0];
    this->bitvec[1] &= ~static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[1];
    this->bitvec[2] &= ~static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[2];
    this->bitvec[3] &= ~static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[3];
    this->bitvec[4] &= ~static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[4];
    this->bitvec[5] &= ~static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[5];
    this->bitvec[6] &= ~static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[6];
    this->bitvec[7] &= ~static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[7];
    this->popcount = Bithacks::popper<uint64_t>(this->bitvec[0]) + Bithacks::popper<uint64_t>(this->bitvec[1])
        + Bithacks::popper<uint64_t>(this->bitvec[2]) + Bithacks::popper<uint64_t>(this->bitvec[3])
        + Bithacks::popper<uint64_t>(this->bitvec[4]) + Bithacks::popper<uint64_t>(this->bitvec[5])
        + Bithacks::popper<uint64_t>(this->bitvec[6]) + Bithacks::popper<uint64_t>(this->bitvec[7]);
    return *this;
}


template <>
void DenseBitvec<std::vector<uint64_t> >::thisAndNot(const BitCollection& other)
{
    this->popcount = 0;
    for (size_t idx = 0; idx < bitvec.size(); ++idx)
    {
        this->bitvec[idx] &= ~static_cast<const DenseBitvec<std::vector<uint64_t> >&>(other).bitvec[idx];
        this->popcount += Bithacks::popper<uint64_t>(this->bitvec[idx]);
    }
}

template <>
void DenseBitvec<uint32_t>::thisAndNot(const BitCollection& other)
{
    this->bitvec &= ~static_cast<const DenseBitvec<uint32_t>&>(other).bitvec;
    this->popcount = Bithacks::popper<uint32_t>(this->bitvec);
}

template <>
void DenseBitvec<uint64_t>::thisAndNot(const BitCollection& other)
{
    this->bitvec &= ~static_cast<const DenseBitvec<uint64_t>&>(other).bitvec;
    this->popcount = Bithacks::popper<uint64_t>(this->bitvec);
}

template <>
void DenseBitvec<std::array<uint64_t,2> >::thisAndNot(const BitCollection& other)
{
    this->bitvec[0] &= ~static_cast<const DenseBitvec<std::array<uint64_t,2> >&>(other).bitvec[0];
    this->popcount = Bithacks::popper<uint64_t>(this->bitvec[0]);
    this->bitvec[1] &= ~static_cast<const DenseBitvec<std::array<uint64_t,2> >&>(other).bitvec[1];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[1]);
}

template <>
void DenseBitvec<std::array<uint64_t,4> >::thisAndNot(const BitCollection& other)
{
    this->bitvec[0] &= ~static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(other).bitvec[0];
    this->popcount = Bithacks::popper<uint64_t>(this->bitvec[0]);
    this->bitvec[1] &= ~static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(other).bitvec[1];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[1]);
    this->bitvec[2] &= ~static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(other).bitvec[2];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[2]);
    this->bitvec[3] &= ~static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(other).bitvec[3];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[3]);
}

template <>
void DenseBitvec<std::array<uint64_t,8> >::thisAndNot(const BitCollection& other)
{
    this->bitvec[0] &= ~static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(other).bitvec[0];
    this->popcount = Bithacks::popper<uint64_t>(this->bitvec[0]);
    this->bitvec[1] &= ~static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(other).bitvec[1];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[1]);
    this->bitvec[2] &= ~static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(other).bitvec[2];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[2]);
    this->bitvec[3] &= ~static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(other).bitvec[3];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[3]);
    this->bitvec[4] &= ~static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(other).bitvec[4];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[4]);
    this->bitvec[5] &= ~static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(other).bitvec[5];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[5]);
    this->bitvec[6] &= ~static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(other).bitvec[6];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[6]);
    this->bitvec[7] &= ~static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(other).bitvec[7];
    this->popcount += Bithacks::popper<uint64_t>(this->bitvec[7]);
}


template<>
inline bool DenseBitvec<std::vector<uint64_t> >::argIsSubset(const BitCollection& bc) const
{
    for (size_t idx = 0; idx < this->bitvec.size(); ++idx)
    {
        if ( (this->bitvec[idx] ^ static_cast<const DenseBitvec<std::vector<uint64_t> >&>(bc).bitvec[idx]) & static_cast<const DenseBitvec<std::vector<uint64_t> >&>(bc).bitvec[idx] )
            return false;
    }
    return true;
}

template<>
inline bool DenseBitvec<uint32_t>::argIsSubset(const BitCollection& bc) const
{
    return !((this->bitvec ^ static_cast<const DenseBitvec<uint32_t>&>(bc).bitvec) & static_cast<const DenseBitvec<uint32_t>&>(bc).bitvec);
}

template<>
inline bool DenseBitvec<uint64_t>::argIsSubset(const BitCollection& bc) const
{
    return !((this->bitvec ^ static_cast<const DenseBitvec<uint64_t>&>(bc).bitvec) & static_cast<const DenseBitvec<uint64_t>&>(bc).bitvec);
}

template<>
inline bool DenseBitvec<std::array<uint64_t, 2> >::argIsSubset(const BitCollection& bc) const
{
    return !(((this->bitvec[0] ^ static_cast<const DenseBitvec<std::array<uint64_t,2> >&>(bc).bitvec[0]) & static_cast<const DenseBitvec<std::array<uint64_t,2> >&>(bc).bitvec[0]) ||
    ((this->bitvec[1] ^ static_cast<const DenseBitvec<std::array<uint64_t,2> >&>(bc).bitvec[1]) & static_cast<const DenseBitvec<std::array<uint64_t,2> >&>(bc).bitvec[1]));
}

template<>
inline bool DenseBitvec<std::array<uint64_t, 4> >::argIsSubset(const BitCollection& bc) const
{
    return !(((this->bitvec[0] ^ static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[0]) & static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[0]) ||
    ((this->bitvec[1] ^ static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[1]) & static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[1]) ||
    ((this->bitvec[2] ^ static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[2]) & static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[2]) ||
    ((this->bitvec[3] ^ static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[3]) & static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[3]));
}

template<>
inline bool DenseBitvec<std::array<uint64_t, 8> >::argIsSubset(const BitCollection& bc) const
{
    return !(((this->bitvec[0] ^ static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[0]) & static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[0]) ||
    ((this->bitvec[1] ^ static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[1]) & static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[1]) ||
    ((this->bitvec[2] ^ static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[2]) & static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[2]) ||
    ((this->bitvec[3] ^ static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[3]) & static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[3]) ||
    ((this->bitvec[4] ^ static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[4]) & static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[4]) ||
    ((this->bitvec[5] ^ static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[5]) & static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[5]) ||
    ((this->bitvec[6] ^ static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[6]) & static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[6]) ||
    ((this->bitvec[7] ^ static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[7]) & static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[7]));
}

template<>
bool DenseBitvec<std::vector<uint64_t> >::hasOverlap(const BitCollection& bc) const
{
    for (size_t idx = 0; idx < this->bitvec.size(); ++idx)
    {
        if (this->bitvec[idx] & static_cast<const DenseBitvec<std::vector<uint64_t> >&>(bc).bitvec[idx])
            return true;
    }
    return false;
}

template<>
bool DenseBitvec<uint32_t>::hasOverlap(const BitCollection& bc) const
{

    if (this->bitvec & static_cast<const DenseBitvec<uint32_t>&>(bc).bitvec)
        return true;
    else
        return false;
}

template<>
bool DenseBitvec<uint64_t>::hasOverlap(const BitCollection& bc) const
{

    if (this->bitvec & static_cast<const DenseBitvec<uint64_t>&>(bc).bitvec)
        return true;
    else
        return false;
}

template<>
bool DenseBitvec<std::array<uint64_t, 2> >::hasOverlap(const BitCollection& bc) const
{

    if (this->bitvec[0] & static_cast<const DenseBitvec<std::array<uint64_t, 2> >&>(bc).bitvec[0])
        return true;
    else if (this->bitvec[1] & static_cast<const DenseBitvec<std::array<uint64_t, 2> >&>(bc).bitvec[1])
        return true;
    else
        return false;
}

template<>
bool DenseBitvec<std::array<uint64_t, 4> >::hasOverlap(const BitCollection& bc) const
{

    if (this->bitvec[0] & static_cast<const DenseBitvec<std::array<uint64_t, 4> >&>(bc).bitvec[0])
        return true;
    else if (this->bitvec[1] & static_cast<const DenseBitvec<std::array<uint64_t, 4> >&>(bc).bitvec[1])
        return true;
    else if (this->bitvec[2] & static_cast<const DenseBitvec<std::array<uint64_t, 4> >&>(bc).bitvec[2])
        return true;
    else if (this->bitvec[3] & static_cast<const DenseBitvec<std::array<uint64_t, 4> >&>(bc).bitvec[3])
        return true;
    else
        return false;
}

template<>
bool DenseBitvec<std::array<uint64_t, 8> >::hasOverlap(const BitCollection& bc) const
{

    if (this->bitvec[0] & static_cast<const DenseBitvec<std::array<uint64_t, 8> >&>(bc).bitvec[0])
        return true;
    else if (this->bitvec[1] & static_cast<const DenseBitvec<std::array<uint64_t, 8> >&>(bc).bitvec[1])
        return true;
    else if (this->bitvec[2] & static_cast<const DenseBitvec<std::array<uint64_t, 8> >&>(bc).bitvec[2])
        return true;
    else if (this->bitvec[3] & static_cast<const DenseBitvec<std::array<uint64_t, 8> >&>(bc).bitvec[3])
        return true;
    else if (this->bitvec[4] & static_cast<const DenseBitvec<std::array<uint64_t, 8> >&>(bc).bitvec[4])
        return true;
    else if (this->bitvec[5] & static_cast<const DenseBitvec<std::array<uint64_t, 8> >&>(bc).bitvec[5])
        return true;
    else if (this->bitvec[6] & static_cast<const DenseBitvec<std::array<uint64_t, 8> >&>(bc).bitvec[6])
        return true;
    else if (this->bitvec[7] & static_cast<const DenseBitvec<std::array<uint64_t, 8> >&>(bc).bitvec[7])
        return true;
    else
        return false;
}


template <>
inline void DenseBitvec<std::vector<uint64_t> >::setBit(const size_t i, const bool v)
{
    uint64_t mask = ((uint64_t)1 << (63 - (i%64)));
    const size_t bitvecId = i/64;
    if (v)
    {
        // check if bit is not yet set
        if (!(bitvec[bitvecId] & mask))
        {
            this->popcount += 1;
        }
        bitvec[bitvecId] = bitvec[bitvecId] | mask;
    } else
    {
        // check if bit was set before
        if (bitvec[bitvecId] & mask)
        {
            this->popcount -= 1;
        }
        // reverse mask for unsetting bit
        mask = ~mask;
        bitvec[bitvecId] = bitvec[bitvecId] & mask;
    }
}

template <>
inline void DenseBitvec<uint32_t>::setBit(const size_t i, const bool v)
{
    uint32_t mask = (1 << (31 - i));
    if (v)
    {
        // check if bit is not yet set
        if (!(bitvec & mask))
        {
            this->popcount += 1;
        }
        bitvec = bitvec | mask;
    } else
    {
        // check if bit was set before
        if (bitvec & mask)
        {
            this->popcount -= 1;
        }
        // reverse mask for unsetting bit
        mask = ~mask;
        bitvec = bitvec & mask;
    }
}

template <>
inline void DenseBitvec<uint64_t>::setBit(const size_t i, const bool v)
{
    uint64_t mask = ((uint64_t)1 << (63 - i));
    if (v)
    {
        // check if bit is not yet set
        if (!(bitvec & mask))
        {
            this->popcount += 1;
        }
        bitvec = bitvec | mask;
    } else
    {
        // check if bit was set before
        if (bitvec & mask)
        {
            this->popcount -= 1;
        }
        // reverse mask for unsetting bit
        mask = ~mask;
        bitvec = bitvec & mask;
    }
}

template <>
inline void DenseBitvec<std::array<uint64_t,2> >::setBit(const size_t i, const bool v)
{
    uint64_t mask = ((uint64_t)1 << (63 - (i%64)));
    const size_t bitvecId = i/64;
    if (v)
    {
        // check if bit is not yet set
        if (!(bitvec[bitvecId] & mask))
        {
            this->popcount += 1;
        }
        bitvec[bitvecId] = bitvec[bitvecId] | mask;
    } else
    {
        // check if bit was set before
        if (bitvec[bitvecId] & mask)
        {
            this->popcount -= 1;
        }
        // reverse mask for unsetting bit
        mask = ~mask;
        bitvec[bitvecId] = bitvec[bitvecId] & mask;
    }
}

template <>
inline void DenseBitvec<std::array<uint64_t,4> >::setBit(const size_t i, const bool v)
{
    uint64_t mask = ((uint64_t)1 << (63 - (i%64)));
    const size_t bitvecId = i/64;
    if (v)
    {
        // check if bit is not yet set
        if (!(bitvec[bitvecId] & mask))
        {
            this->popcount += 1;
        }
        bitvec[bitvecId] = bitvec[bitvecId] | mask;
    } else
    {
        // check if bit was set before
        if (bitvec[bitvecId] & mask)
        {
            this->popcount -= 1;
        }
        // reverse mask for unsetting bit
        mask = ~mask;
        bitvec[bitvecId] = bitvec[bitvecId] & mask;
    }
}

template <>
inline void DenseBitvec<std::array<uint64_t,8> >::setBit(const size_t i, const bool v)
{
    uint64_t mask = ((uint64_t)1 << (63 - (i%64)));
    const size_t bitvecId = i/64;
    if (v)
    {
        // check if bit is not yet set
        if (!(bitvec[bitvecId] & mask))
        {
            this->popcount += 1;
        }
        bitvec[bitvecId] = bitvec[bitvecId] | mask;
    } else
    {
        // check if bit was set before
        if (bitvec[bitvecId] & mask)
        {
            this->popcount -= 1;
        }
        // reverse mask for unsetting bit
        mask = ~mask;
        bitvec[bitvecId] = bitvec[bitvecId] & mask;
    }
}

template <>
inline bool DenseBitvec<std::vector<uint64_t> >::operator==(const BitCollection& bc) const
{
    for (size_t idx = 0; idx < this->bitvec.size(); ++idx)
    {
        if ( this->bitvec[idx] ^ static_cast<const DenseBitvec<std::vector<uint64_t> >&>(bc).bitvec[idx] )
            return false;
    }
    return true;
}

template <>
inline bool DenseBitvec<uint32_t>::operator==(const BitCollection& bc) const
{
    return this->bitvec & static_cast<const DenseBitvec<uint32_t>&>(bc).bitvec;
}

template <>
inline bool DenseBitvec<uint64_t>::operator==(const BitCollection& bc) const
{
    return this->bitvec & static_cast<const DenseBitvec<uint64_t>&>(bc).bitvec;
}

template <>
inline bool DenseBitvec<std::array<uint64_t,2> >::operator==(const BitCollection& bc) const
{
    if ( this->bitvec[0] & static_cast<const DenseBitvec<std::array<uint64_t,2> >&>(bc).bitvec[0] )
        return false;
    if ( this->bitvec[1] & static_cast<const DenseBitvec<std::array<uint64_t,2> >&>(bc).bitvec[1] )
        return false;
    return true;
}

template <>
inline bool DenseBitvec<std::array<uint64_t,4> >::operator==(const BitCollection& bc) const
{
    if ( this->bitvec[0] & static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[0] )
        return false;
    if ( this->bitvec[1] & static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[1] )
        return false;
    if ( this->bitvec[2] & static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[2] )
        return false;
    if ( this->bitvec[3] & static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[3] )
        return false;
    return true;
}

template <>
inline bool DenseBitvec<std::array<uint64_t,8> >::operator==(const BitCollection& bc) const
{
    if ( this->bitvec[0] & static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[0] )
        return false;
    if ( this->bitvec[1] & static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[1] )
        return false;
    if ( this->bitvec[2] & static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[2] )
        return false;
    if ( this->bitvec[3] & static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[3] )
        return false;
    if ( this->bitvec[4] & static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[4] )
        return false;
    if ( this->bitvec[5] & static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[5] )
        return false;
    if ( this->bitvec[6] & static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[6] )
        return false;
    if ( this->bitvec[7] & static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[7] )
        return false;
    return true;
}

template <>
inline bool DenseBitvec<std::vector<uint64_t> >::operator[](const size_t i) const
{
    uint64_t mask = ((uint64_t)1 << (63 - (i%64)));
    return bitvec[i/64] & mask;
}

template <>
inline bool DenseBitvec<uint32_t>::operator[](const size_t i) const
{
    assert(false); // mask is probably incorrect, check setBit() for correct mask
    const uint32_t mask = (1 << i%32);
    return bitvec & mask;
}

template <>
inline bool DenseBitvec<uint64_t>::operator[](const size_t i) const
{
    assert(false); // mask is probably incorrect, check setBit() for correct mask
    const uint64_t mask = (1 << i%64);
    return bitvec & mask;
}

template <>
inline bool DenseBitvec<std::array<uint64_t,2> >::operator[](const size_t i) const
{
    assert(false); // mask is probably incorrect, check setBit() for correct mask
    const uint64_t mask = (1 << i%64);
    return bitvec[i/64] & mask;
}

template <>
inline bool DenseBitvec<std::array<uint64_t,4> >::operator[](const size_t i) const
{
    assert(false); // mask is probably incorrect, check setBit() for correct mask
    const uint64_t mask = (1 << i%64);
    return bitvec[i/64] & mask;
}

template <>
inline bool DenseBitvec<std::array<uint64_t,8> >::operator[](const size_t i) const
{
    assert(false); // mask is probably incorrect, check setBit() for correct mask
    const uint64_t mask = (1 << i%64);
    return bitvec[i/64] & mask;
}


template <>
uint32_t DenseBitvec<std::vector<uint64_t> >::intersectionCount(const BitCollection& bc) const
{
    uint32_t popcnt = 0;
    for (size_t idx = 0; idx < bitvec.size(); ++idx)
    {
        const uint64_t inter = static_cast<const DenseBitvec<std::vector<uint64_t> >&>(bc).bitvec[idx] & this->bitvec[idx];
        popcnt += Bithacks::popper<uint64_t>(inter);
    }
    return popcnt;
}

template <>
uint32_t DenseBitvec<uint32_t>::intersectionCount(const BitCollection& bc) const
{
    const uint32_t inter = static_cast<const DenseBitvec<uint32_t>&>(bc).bitvec & this->bitvec;
    return Bithacks::popper<uint32_t>(inter);
}

template <>
uint32_t DenseBitvec<uint64_t>::intersectionCount(const BitCollection& bc) const
{
    const uint64_t inter = static_cast<const DenseBitvec<uint64_t>&>(bc).bitvec & this->bitvec;
    return Bithacks::popper<uint64_t>(inter);
}

template <>
uint32_t DenseBitvec<std::array<uint64_t,2> >::intersectionCount(const BitCollection& bc) const
{
    uint64_t inter = static_cast<const DenseBitvec<std::array<uint64_t,2> >&>(bc).bitvec[0] & this->bitvec[0];
    const uint32_t popcnt = Bithacks::popper<uint64_t>(inter);
    inter = static_cast<const DenseBitvec<std::array<uint64_t,2> >&>(bc).bitvec[1] & this->bitvec[1];
    return popcnt + Bithacks::popper<uint64_t>(inter);
}

template <>
uint32_t DenseBitvec<std::array<uint64_t,4> >::intersectionCount(const BitCollection& bc) const
{
    uint64_t inter = static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[0] & this->bitvec[0];
    uint32_t popcnt = Bithacks::popper<uint64_t>(inter);
    inter = static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[1] & this->bitvec[1];
    popcnt += Bithacks::popper<uint64_t>(inter);
    inter =  static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[2] & this->bitvec[2];
    popcnt += Bithacks::popper<uint64_t>(inter);
    inter = static_cast<const DenseBitvec<std::array<uint64_t,4> >&>(bc).bitvec[3] & this->bitvec[3];
    return popcnt + Bithacks::popper<uint64_t>(inter);
}

template <>
uint32_t DenseBitvec<std::array<uint64_t,8> >::intersectionCount(const BitCollection& bc) const
{
    uint64_t inter = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[0] & this->bitvec[0];
    uint32_t popcnt = Bithacks::popper<uint64_t>(inter);
    inter = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[1] & this->bitvec[1];
    popcnt += Bithacks::popper<uint64_t>(inter);
    inter = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[2] & this->bitvec[2];
    popcnt += Bithacks::popper<uint64_t>(inter);
    inter = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[3] & this->bitvec[3];
    popcnt += Bithacks::popper<uint64_t>(inter);
    inter = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[4] & this->bitvec[4];
    popcnt += Bithacks::popper<uint64_t>(inter);
    inter = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[5] & this->bitvec[5];
    popcnt += Bithacks::popper<uint64_t>(inter);
    inter = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[6] & this->bitvec[6];
    popcnt += Bithacks::popper<uint64_t>(inter);
    inter = static_cast<const DenseBitvec<std::array<uint64_t,8> >&>(bc).bitvec[7] & this->bitvec[7];
    return popcnt + Bithacks::popper<uint64_t>(inter);
}


template <>
uint32_t DenseBitvec<std::vector<uint64_t> >::intersectionCountMany(const std::vector<const BitCollection*> bcs) const
{
    uint32_t popcnt = 0;
    for (size_t idx = 0; idx < bitvec.size(); ++idx)
    {
        uint64_t inter = this->bitvec[idx];
        for (size_t bcIdx = 0; bcIdx < bcs.size(); ++bcIdx)
            inter &= static_cast<const DenseBitvec<std::vector<uint64_t> >*>(bcs[bcIdx])->bitvec[idx];
        popcnt += Bithacks::popper<uint64_t>(inter);
    }
    return popcnt;
}

template <>
uint32_t DenseBitvec<uint32_t>::intersectionCountMany(const std::vector<const BitCollection*> bcs) const
{
    uint32_t inter = this->bitvec;
    for (size_t bcIdx = 0; bcIdx < bcs.size(); ++bcIdx)
        inter &= static_cast<const DenseBitvec<uint32_t>*>(bcs[bcIdx])->bitvec;
    return Bithacks::popper<uint32_t>(inter);
}

template <>
uint32_t DenseBitvec<uint64_t>::intersectionCountMany(const std::vector<const BitCollection*> bcs) const
{
    uint64_t inter = this->bitvec;
    for (size_t bcIdx = 0; bcIdx < bcs.size(); ++bcIdx)
        inter &= static_cast<const DenseBitvec<uint64_t>*>(bcs[bcIdx])->bitvec;
    return Bithacks::popper<uint64_t>(inter);
}

template <>
uint32_t DenseBitvec<std::array<uint64_t,2> >::intersectionCountMany(const std::vector<const BitCollection*> bcs) const
{
    uint64_t inter = this->bitvec[0];
    for (size_t bcIdx = 0; bcIdx < bcs.size(); ++bcIdx)
        inter &= static_cast<const DenseBitvec<std::array<uint64_t,2> >*>(bcs[bcIdx])->bitvec[0];
    uint32_t popcnt = Bithacks::popper<uint64_t>(inter);
    inter = this->bitvec[1];
    for (size_t bcIdx = 0; bcIdx < bcs.size(); ++bcIdx)
        inter &= static_cast<const DenseBitvec<std::array<uint64_t,2> >*>(bcs[bcIdx])->bitvec[1];
    return popcnt + Bithacks::popper<uint64_t>(inter);
}

template <>
uint32_t DenseBitvec<std::array<uint64_t,4> >::intersectionCountMany(const std::vector<const BitCollection*> bcs) const
{
    uint64_t inter = this->bitvec[0];
    for (size_t bcIdx = 0; bcIdx < bcs.size(); ++bcIdx)
        inter &= static_cast<const DenseBitvec<std::array<uint64_t,4> >*>(bcs[bcIdx])->bitvec[0];
    uint32_t popcnt = Bithacks::popper<uint64_t>(inter);
    inter = this->bitvec[1];
    for (size_t bcIdx = 0; bcIdx < bcs.size(); ++bcIdx)
        inter &= static_cast<const DenseBitvec<std::array<uint64_t,4> >*>(bcs[bcIdx])->bitvec[1];
    popcnt += Bithacks::popper<uint64_t>(inter);
    inter = this->bitvec[2];
    for (size_t bcIdx = 0; bcIdx < bcs.size(); ++bcIdx)
        inter &= static_cast<const DenseBitvec<std::array<uint64_t,4> >*>(bcs[bcIdx])->bitvec[2];
    popcnt += Bithacks::popper<uint64_t>(inter);
    inter = this->bitvec[3];
    for (size_t bcIdx = 0; bcIdx < bcs.size(); ++bcIdx)
        inter &= static_cast<const DenseBitvec<std::array<uint64_t,4> >*>(bcs[bcIdx])->bitvec[3];
    return popcnt + Bithacks::popper<uint64_t>(inter);
}

template <>
uint32_t DenseBitvec<std::array<uint64_t,8> >::intersectionCountMany(const std::vector<const BitCollection*> bcs) const
{
    uint64_t inter = this->bitvec[0];
    for (size_t bcIdx = 0; bcIdx < bcs.size(); ++bcIdx)
        inter &= static_cast<const DenseBitvec<std::array<uint64_t,8> >*>(bcs[bcIdx])->bitvec[0];
    uint32_t popcnt = Bithacks::popper<uint64_t>(inter);
    inter = this->bitvec[1];
    for (size_t bcIdx = 0; bcIdx < bcs.size(); ++bcIdx)
        inter &= static_cast<const DenseBitvec<std::array<uint64_t,8> >*>(bcs[bcIdx])->bitvec[1];
    popcnt += Bithacks::popper<uint64_t>(inter);
    inter = this->bitvec[2];
    for (size_t bcIdx = 0; bcIdx < bcs.size(); ++bcIdx)
        inter &= static_cast<const DenseBitvec<std::array<uint64_t,8> >*>(bcs[bcIdx])->bitvec[2];
    popcnt += Bithacks::popper<uint64_t>(inter);
    inter = this->bitvec[3];
    for (size_t bcIdx = 0; bcIdx < bcs.size(); ++bcIdx)
        inter &= static_cast<const DenseBitvec<std::array<uint64_t,8> >*>(bcs[bcIdx])->bitvec[3];
    popcnt += Bithacks::popper<uint64_t>(inter);
    inter = this->bitvec[4];
    for (size_t bcIdx = 0; bcIdx < bcs.size(); ++bcIdx)
        inter &= static_cast<const DenseBitvec<std::array<uint64_t,8> >*>(bcs[bcIdx])->bitvec[4];
    popcnt += Bithacks::popper<uint64_t>(inter);
    inter = this->bitvec[5];
    for (size_t bcIdx = 0; bcIdx < bcs.size(); ++bcIdx)
        inter &= static_cast<const DenseBitvec<std::array<uint64_t,8> >*>(bcs[bcIdx])->bitvec[5];
    popcnt += Bithacks::popper<uint64_t>(inter);
    inter = this->bitvec[6];
    for (size_t bcIdx = 0; bcIdx < bcs.size(); ++bcIdx)
        inter &= static_cast<const DenseBitvec<std::array<uint64_t,8> >*>(bcs[bcIdx])->bitvec[6];
    popcnt += Bithacks::popper<uint64_t>(inter);
    inter = this->bitvec[7];
    for (size_t bcIdx = 0; bcIdx < bcs.size(); ++bcIdx)
        inter &= static_cast<const DenseBitvec<std::array<uint64_t,8> >*>(bcs[bcIdx])->bitvec[7];
    return popcnt + Bithacks::popper<uint64_t>(inter);
}


template <>
uint32_t DenseBitvec<std::vector<uint64_t> >::thisAndNotCountInterOthers(const std::vector<const BitCollection*> bcs) const
{
    uint32_t popcnt = 0;
    for (size_t idx = 0; idx < bitvec.size(); ++idx)
    {
        uint64_t inter = static_cast<const DenseBitvec<std::vector<uint64_t> >*>(bcs[0])->bitvec[idx];
        for (size_t bcIdx = 1; bcIdx < bcs.size(); ++bcIdx)
        {
            inter &= static_cast<const DenseBitvec<std::vector<uint64_t> >*>(bcs[bcIdx])->bitvec[idx];
        }
        popcnt += Bithacks::popper<uint64_t>(this->bitvec[idx] & ~inter);
    }
    return popcnt;
}

template <>
uint32_t DenseBitvec<uint32_t>::thisAndNotCountInterOthers(const std::vector<const BitCollection*> bcs) const
{
    uint32_t inter = static_cast<const DenseBitvec<uint32_t>*>(bcs[0])->bitvec;
    for (size_t bcIdx = 1; bcIdx < bcs.size(); ++bcIdx)
        inter &= static_cast<const DenseBitvec<uint32_t>*>(bcs[bcIdx])->bitvec;
    uint32_t popcnt = Bithacks::popper<uint32_t>(this->bitvec & ~inter);
    return popcnt;
}

template <>
uint32_t DenseBitvec<uint64_t>::thisAndNotCountInterOthers(const std::vector<const BitCollection*> bcs) const
{
    uint64_t inter = static_cast<const DenseBitvec<uint64_t>*>(bcs[0])->bitvec;
    for (size_t bcIdx = 1; bcIdx < bcs.size(); ++bcIdx)
        inter &= static_cast<const DenseBitvec<uint64_t>*>(bcs[bcIdx])->bitvec;
    uint32_t popcnt = Bithacks::popper<uint64_t>(this->bitvec & ~inter);
    return popcnt;
}

template <>
uint32_t DenseBitvec<std::array<uint64_t,2> >::thisAndNotCountInterOthers(const std::vector<const BitCollection*> bcs) const
{
    uint32_t popcnt = 0;
    for (size_t idx = 0; idx < bitvec.size(); ++idx)
    {
        uint64_t inter = static_cast<const DenseBitvec<std::array<uint64_t,2> >*>(bcs[0])->bitvec[idx];
        for (size_t bcIdx = 1; bcIdx < bcs.size(); ++bcIdx)
            inter &= static_cast<const DenseBitvec<std::array<uint64_t,2> >*>(bcs[bcIdx])->bitvec[idx];
        popcnt += Bithacks::popper<uint64_t>(this->bitvec[idx] & ~inter);
    }
    return popcnt;
}

template <>
uint32_t DenseBitvec<std::array<uint64_t,4> >::thisAndNotCountInterOthers(const std::vector<const BitCollection*> bcs) const
{
    uint32_t popcnt = 0;
    for (size_t idx = 0; idx < bitvec.size(); ++idx)
    {
        uint64_t inter = static_cast<const DenseBitvec<std::array<uint64_t,4> >*>(bcs[0])->bitvec[idx];
        for (size_t bcIdx = 1; bcIdx < bcs.size(); ++bcIdx)
            inter &= static_cast<const DenseBitvec<std::array<uint64_t,4> >*>(bcs[bcIdx])->bitvec[idx];
        popcnt += Bithacks::popper<uint64_t>(this->bitvec[idx] & ~inter);
    }
    return popcnt;
}

template <>
uint32_t DenseBitvec<std::array<uint64_t,8> >::thisAndNotCountInterOthers(const std::vector<const BitCollection*> bcs) const
{
    uint32_t popcnt = 0;
    for (size_t idx = 0; idx < bitvec.size(); ++idx)
    {
        uint64_t inter = static_cast<const DenseBitvec<std::array<uint64_t,8> >*>(bcs[0])->bitvec[idx];
        for (size_t bcIdx = 1; bcIdx < bcs.size(); ++bcIdx)
            inter &= static_cast<const DenseBitvec<std::array<uint64_t,8> >*>(bcs[bcIdx])->bitvec[idx];
        popcnt += Bithacks::popper<uint64_t>(this->bitvec[idx] & ~inter);
    }
    return popcnt;
}

template <>
uint32_t DenseBitvec<std::vector<uint64_t> >::thisAndNotCountAnyOthers(const std::vector<const BitCollection*> bcs) const
{
    uint32_t popcnt = 0;
    for (size_t idx = 0; idx < bitvec.size(); ++idx)
    {
        uint64_t uni = static_cast<const DenseBitvec<std::vector<uint64_t> >*>(bcs[0])->bitvec[idx];
        for (size_t bcIdx = 1; bcIdx < bcs.size(); ++bcIdx)
        {
            uni |= static_cast<const DenseBitvec<std::vector<uint64_t> >*>(bcs[bcIdx])->bitvec[idx];
        }
        popcnt += Bithacks::popper<uint64_t>(this->bitvec[idx] & ~uni);
    }
    return popcnt;
}

template <>
uint32_t DenseBitvec<uint32_t>::thisAndNotCountAnyOthers(const std::vector<const BitCollection*> bcs) const
{
    assert(false); // not implemented
}

template <>
uint32_t DenseBitvec<uint64_t>::thisAndNotCountAnyOthers(const std::vector<const BitCollection*> bcs) const
{
    assert(false); // not implemented
}

template <>
uint32_t DenseBitvec<std::array<uint64_t,2> >::thisAndNotCountAnyOthers(const std::vector<const BitCollection*> bcs) const
{
    assert(false); // not implemented
}

template <>
uint32_t DenseBitvec<std::array<uint64_t,4> >::thisAndNotCountAnyOthers(const std::vector<const BitCollection*> bcs) const
{
    assert(false); // not implemented
}

template <>
uint32_t DenseBitvec<std::array<uint64_t,8> >::thisAndNotCountAnyOthers(const std::vector<const BitCollection*> bcs) const
{
    assert(false); // not implemented
}


template <>
uint32_t DenseBitvec<std::vector<uint64_t> >::xorCount(const DenseBitvec<std::vector<uint64_t> >& thisFiltered, const DenseBitvec<std::vector<uint64_t> >& otherXor, const DenseBitvec<std::vector<uint64_t> >& otherFiltered) const
{
    uint32_t res = 0;
    for (size_t i = 0; i < this->bitvec.size(); ++i)
    {
        res += Bithacks::popper<uint64_t>((this->bitvec[i] ^ otherXor.bitvec[i]) & ~(thisFiltered.bitvec[i] | otherFiltered.bitvec[i]));
    }
    return res;
}

template <>
uint32_t DenseBitvec<uint32_t>::xorCount(const DenseBitvec<uint32_t>& thisFiltered, const DenseBitvec<uint32_t>& otherXor, const DenseBitvec<uint32_t>& otherFiltered) const
{
    return Bithacks::popper<uint64_t>((this->bitvec ^ otherXor.bitvec) & ~(thisFiltered.bitvec | otherFiltered.bitvec));
}

template <>
uint32_t DenseBitvec<uint64_t>::xorCount(const DenseBitvec<uint64_t>& thisFiltered, const DenseBitvec<uint64_t>& otherXor, const DenseBitvec<uint64_t>& otherFiltered) const
{
    return Bithacks::popper<uint64_t>((this->bitvec ^ otherXor.bitvec) & ~(thisFiltered.bitvec | otherFiltered.bitvec));
}

template <>
uint32_t DenseBitvec<std::array<uint64_t,2> >::xorCount(const DenseBitvec<std::array<uint64_t,2> >& thisFiltered, const DenseBitvec<std::array<uint64_t,2> >& otherXor, const DenseBitvec<std::array<uint64_t,2> >& otherFiltered) const
{
    uint32_t res = Bithacks::popper<uint64_t>((this->bitvec[0] ^ otherXor.bitvec[0]) & ~(thisFiltered.bitvec[0] | otherFiltered.bitvec[0]));
    res += Bithacks::popper<uint64_t>((this->bitvec[1] ^ otherXor.bitvec[1]) & ~(thisFiltered.bitvec[1] | otherFiltered.bitvec[1]));
    return res;
}

template <>
uint32_t DenseBitvec<std::array<uint64_t,4> >::xorCount(const DenseBitvec<std::array<uint64_t,4> >& thisFiltered, const DenseBitvec<std::array<uint64_t,4> >& otherXor, const DenseBitvec<std::array<uint64_t,4> >& otherFiltered) const
{
    uint32_t res = Bithacks::popper<uint64_t>((this->bitvec[0] ^ otherXor.bitvec[0]) & ~(thisFiltered.bitvec[0] | otherFiltered.bitvec[0]));
    res += Bithacks::popper<uint64_t>((this->bitvec[1] ^ otherXor.bitvec[1]) & ~(thisFiltered.bitvec[1] | otherFiltered.bitvec[1]));
    res += Bithacks::popper<uint64_t>((this->bitvec[2] ^ otherXor.bitvec[2]) & ~(thisFiltered.bitvec[2] | otherFiltered.bitvec[2]));
    res += Bithacks::popper<uint64_t>((this->bitvec[3] ^ otherXor.bitvec[3]) & ~(thisFiltered.bitvec[3] | otherFiltered.bitvec[3]));
    return res;
}

template <>
uint32_t DenseBitvec<std::array<uint64_t,8> >::xorCount(const DenseBitvec<std::array<uint64_t,8> >& thisFiltered, const DenseBitvec<std::array<uint64_t,8> >& otherXor, const DenseBitvec<std::array<uint64_t,8> >& otherFiltered) const
{
    uint32_t res = Bithacks::popper<uint64_t>((this->bitvec[0] ^ otherXor.bitvec[0]) & ~(thisFiltered.bitvec[0] | otherFiltered.bitvec[0]));
    res += Bithacks::popper<uint64_t>((this->bitvec[1] ^ otherXor.bitvec[1]) & ~(thisFiltered.bitvec[1] | otherFiltered.bitvec[1]));
    res += Bithacks::popper<uint64_t>((this->bitvec[2] ^ otherXor.bitvec[2]) & ~(thisFiltered.bitvec[2] | otherFiltered.bitvec[2]));
    res += Bithacks::popper<uint64_t>((this->bitvec[3] ^ otherXor.bitvec[3]) & ~(thisFiltered.bitvec[3] | otherFiltered.bitvec[3]));
    res += Bithacks::popper<uint64_t>((this->bitvec[4] ^ otherXor.bitvec[4]) & ~(thisFiltered.bitvec[4] | otherFiltered.bitvec[4]));
    res += Bithacks::popper<uint64_t>((this->bitvec[5] ^ otherXor.bitvec[5]) & ~(thisFiltered.bitvec[5] | otherFiltered.bitvec[5]));
    res += Bithacks::popper<uint64_t>((this->bitvec[6] ^ otherXor.bitvec[6]) & ~(thisFiltered.bitvec[6] | otherFiltered.bitvec[6]));
    res += Bithacks::popper<uint64_t>((this->bitvec[7] ^ otherXor.bitvec[7]) & ~(thisFiltered.bitvec[7] | otherFiltered.bitvec[7]));
    return res;
}




template <>
inline const std::vector<uint32_t>* DenseBitvec<std::vector<uint64_t> >::getOnes() const
{
    std::vector<uint32_t>* ones = new std::vector<uint32_t>();
    ones->reserve(popcount);
    uint32_t offset = 0;
    for (size_t idx = 0; idx < this->bitvec.size(); ++idx, offset+=64)
    {
        // if no bit set in this block continue with next block
        if (!this->bitvec[idx])
        {
            continue;
        }
        // get reverse of bitsequence
        uint64_t bitseq = Bithacks::reverseBits64(this->bitvec[idx]);
        // while there are bits left to process
        while (bitseq)
        {
            // get index of lowest set bit
            int64_t lowId = Bithacks::getLowestIdx64(bitseq);
            ones->emplace_back((uint32_t)lowId + offset);
        }
    }
    return ones;
}

template <>
inline const std::vector<uint32_t>* DenseBitvec<uint32_t>::getOnes() const
{
    std::vector<uint32_t>* ones = new std::vector<uint32_t>();
    ones->reserve(popcount);
    // get reverse of bitsequence
    uint32_t bitseq = Bithacks::reverseBits32(this->bitvec);
    for (uint32_t procBits = 0; procBits < this->popcount; ++procBits)
    {
        // get index of lowest set bit
        int32_t lowId = Bithacks::getLowestIdx32(bitseq);
        ones->emplace_back((uint32_t)lowId);
    }
    return ones;
}

template <>
inline const std::vector<uint32_t>* DenseBitvec<uint64_t>::getOnes() const
{
    std::vector<uint32_t>* ones = new std::vector<uint32_t>();
    ones->reserve(popcount);
    // get reverse of bitsequence
    uint64_t bitseq = Bithacks::reverseBits64(this->bitvec);
    for (uint32_t procBits = 0; procBits < this->popcount; ++procBits)
    {
        // get index of lowest set bit
        int64_t lowId = Bithacks::getLowestIdx64(bitseq);
        ones->emplace_back((uint32_t)lowId);
    }
    return ones;
}

template <>
inline const std::vector<uint32_t>* DenseBitvec<std::array<uint64_t, 2> >::getOnes() const
{
    std::vector<uint32_t>* ones = new std::vector<uint32_t>();
    ones->reserve(popcount);
    // get reverse of bitsequence
    uint64_t bitseq = Bithacks::reverseBits64(this->bitvec[0]);
    // while there are bits left to process
    while (bitseq)
    {
        // get index of lowest set bit
        int64_t lowId = Bithacks::getLowestIdx64(bitseq);
        ones->emplace_back((uint32_t)lowId);
    }
    bitseq = Bithacks::reverseBits64(this->bitvec[1]);
    while (bitseq)
    {
        // get index of lowest set bit
        int64_t lowId = Bithacks::getLowestIdx64(bitseq);
        ones->emplace_back((uint32_t)lowId + 64);
    }
    return ones;
}

template <>
inline const std::vector<uint32_t>* DenseBitvec<std::array<uint64_t, 4> >::getOnes() const
{
    std::vector<uint32_t>* ones = new std::vector<uint32_t>();
    ones->reserve(popcount);
    // get reverse of bitsequence
    uint64_t bitseq = Bithacks::reverseBits64(this->bitvec[0]);
    // while there are bits left to process
    while (bitseq)
    {
        // get index of lowest set bit
        int64_t lowId = Bithacks::getLowestIdx64(bitseq);
        ones->emplace_back((uint32_t)lowId);
    }
    bitseq = Bithacks::reverseBits64(this->bitvec[1]);
    while (bitseq)
    {
        // get index of lowest set bit
        int64_t lowId = Bithacks::getLowestIdx64(bitseq);
        ones->emplace_back((uint32_t)lowId + 64);
    }
    bitseq = Bithacks::reverseBits64(this->bitvec[2]);
    while (bitseq)
    {
        // get index of lowest set bit
        int64_t lowId = Bithacks::getLowestIdx64(bitseq);
        ones->emplace_back((uint32_t)lowId + 128);
    }
    bitseq = Bithacks::reverseBits64(this->bitvec[3]);
    while (bitseq)
    {
        // get index of lowest set bit
        int64_t lowId = Bithacks::getLowestIdx64(bitseq);
        ones->emplace_back((uint32_t)lowId + 192);
    }
    return ones;
}

template <>
inline const std::vector<uint32_t>* DenseBitvec<std::array<uint64_t, 8> >::getOnes() const
{
    std::vector<uint32_t>* ones = new std::vector<uint32_t>();
    ones->reserve(popcount);
    // get reverse of bitsequence
    uint64_t bitseq = Bithacks::reverseBits64(this->bitvec[0]);
    // while there are bits left to process
    while (bitseq)
    {
        // get index of lowest set bit
        int64_t lowId = Bithacks::getLowestIdx64(bitseq);
        ones->emplace_back((uint32_t)lowId);
    }
    bitseq = Bithacks::reverseBits64(this->bitvec[1]);
    while (bitseq)
    {
        // get index of lowest set bit
        int64_t lowId = Bithacks::getLowestIdx64(bitseq);
        ones->emplace_back((uint32_t)lowId + 64);
    }
    bitseq = Bithacks::reverseBits64(this->bitvec[2]);
    while (bitseq)
    {
        // get index of lowest set bit
        int64_t lowId = Bithacks::getLowestIdx64(bitseq);
        ones->emplace_back((uint32_t)lowId + 128);
    }
    bitseq = Bithacks::reverseBits64(this->bitvec[3]);
    while (bitseq)
    {
        // get index of lowest set bit
        int64_t lowId = Bithacks::getLowestIdx64(bitseq);
        ones->emplace_back((uint32_t)lowId + 192);
    }
    bitseq = Bithacks::reverseBits64(this->bitvec[4]);
    while (bitseq)
    {
        // get index of lowest set bit
        int64_t lowId = Bithacks::getLowestIdx64(bitseq);
        ones->emplace_back((uint32_t)lowId + 256);
    }
    bitseq = Bithacks::reverseBits64(this->bitvec[5]);
    while (bitseq)
    {
        // get index of lowest set bit
        int64_t lowId = Bithacks::getLowestIdx64(bitseq);
        ones->emplace_back((uint32_t)lowId + 320);
    }
    bitseq = Bithacks::reverseBits64(this->bitvec[6]);
    while (bitseq)
    {
        // get index of lowest set bit
        int64_t lowId = Bithacks::getLowestIdx64(bitseq);
        ones->emplace_back((uint32_t)lowId + 384);
    }
    bitseq = Bithacks::reverseBits64(this->bitvec[7]);
    while (bitseq)
    {
        // get index of lowest set bit
        int64_t lowId = Bithacks::getLowestIdx64(bitseq);
        ones->emplace_back((uint32_t)lowId + 448);
    }
    return ones;
}


template <>
inline const int DenseBitvec<std::vector<uint64_t> >::lexComp(const BitCollection* bc) const
{
    for (size_t idx = 0; idx < this->bitvec.size(); ++idx)
    {
        uint64_t bitseqThis = this->bitvec[idx];
        uint64_t bitseqBc = static_cast<const DenseBitvec<std::vector<uint64_t> >*>(bc)->bitvec[idx];
        // if a is bigger than b, than the first differing bit reading from most significant to least
        // is set in a but not in b
        // which corresponds to the first differing index.
        if (bitseqThis > bitseqBc)
        {
            return -1;
        } else if (bitseqThis < bitseqBc)
        {
            return 1;
        }
        // if they are equal continue with next batch of bits
    }
    return 0;
}

template <>
inline const int DenseBitvec<uint32_t>::lexComp(const BitCollection* bc) const
{
    if (this->bitvec > static_cast<const DenseBitvec<uint32_t>*>(bc)->bitvec)
    {
        return -1;
    } else if (this->bitvec < static_cast<const DenseBitvec<uint32_t>*>(bc)->bitvec)
    {
        return 1;
    } else {
        return 0;
    }
}

template <>
inline const int DenseBitvec<uint64_t>::lexComp(const BitCollection* bc) const
{
    if (this->bitvec > static_cast<const DenseBitvec<uint64_t>*>(bc)->bitvec)
    {
        return -1;
    } else if (this->bitvec < static_cast<const DenseBitvec<uint64_t>*>(bc)->bitvec)
    {
        return 1;
    } else {
        return 0;
    }
}

template <>
inline const int DenseBitvec<std::array<uint64_t, 2> >::lexComp(const BitCollection* bc) const
{
    uint64_t bitseqThis = this->bitvec[0];
    uint64_t bitseqBc = static_cast<const DenseBitvec<std::array<uint64_t,2> >*>(bc)->bitvec[0];
    if (bitseqThis > bitseqBc)
    {
        return -1;
    } else if (bitseqThis < bitseqBc)
    {
        return 1;
    }
    bitseqThis = this->bitvec[1];
    bitseqBc = static_cast<const DenseBitvec<std::array<uint64_t,2> >*>(bc)->bitvec[1];
    if (bitseqThis > bitseqBc)
    {
        return -1;
    } else if (bitseqThis < bitseqBc)
    {
        return 1;
    } else {
        return 0;
    }
}

template <>
inline const int DenseBitvec<std::array<uint64_t, 4> >::lexComp(const BitCollection* bc) const
{
    uint64_t bitseqThis = this->bitvec[0];
    uint64_t bitseqBc = static_cast<const DenseBitvec<std::array<uint64_t,4> >*>(bc)->bitvec[0];
    if (bitseqThis > bitseqBc)
    {
        return -1;
    } else if (bitseqThis < bitseqBc)
    {
        return 1;
    }
    bitseqThis = this->bitvec[1];
    bitseqBc = static_cast<const DenseBitvec<std::array<uint64_t,4> >*>(bc)->bitvec[1];
    if (bitseqThis > bitseqBc)
    {
        return -1;
    } else if (bitseqThis < bitseqBc)
    {
        return 1;
    }
    bitseqThis = this->bitvec[2];
    bitseqBc = static_cast<const DenseBitvec<std::array<uint64_t,4> >*>(bc)->bitvec[2];
    if (bitseqThis > bitseqBc)
    {
        return -1;
    } else if (bitseqThis < bitseqBc)
    {
        return 1;
    }
    bitseqThis = this->bitvec[3];
    bitseqBc = static_cast<const DenseBitvec<std::array<uint64_t,4> >*>(bc)->bitvec[3];
    if (bitseqThis > bitseqBc)
    {
        return -1;
    } else if (bitseqThis < bitseqBc)
    {
        return 1;
    } else {
        return 0;
    }
}

template <>
inline const int DenseBitvec<std::array<uint64_t, 8> >::lexComp(const BitCollection* bc) const
{
    uint64_t bitseqThis = this->bitvec[0];
    uint64_t bitseqBc = static_cast<const DenseBitvec<std::array<uint64_t,8> >*>(bc)->bitvec[0];
    if (bitseqThis > bitseqBc)
    {
        return -1;
    } else if (bitseqThis < bitseqBc)
    {
        return 1;
    }
    bitseqThis = this->bitvec[1];
    bitseqBc = static_cast<const DenseBitvec<std::array<uint64_t,8> >*>(bc)->bitvec[1];
    if (bitseqThis > bitseqBc)
    {
        return -1;
    } else if (bitseqThis < bitseqBc)
    {
        return 1;
    }
    bitseqThis = this->bitvec[2];
    bitseqBc = static_cast<const DenseBitvec<std::array<uint64_t,8> >*>(bc)->bitvec[2];
    if (bitseqThis > bitseqBc)
    {
        return -1;
    } else if (bitseqThis < bitseqBc)
    {
        return 1;
    }
    bitseqThis = this->bitvec[3];
    bitseqBc = static_cast<const DenseBitvec<std::array<uint64_t,8> >*>(bc)->bitvec[3];
    if (bitseqThis > bitseqBc)
    {
        return -1;
    } else if (bitseqThis < bitseqBc)
    {
        return 1;
    }
    bitseqThis = this->bitvec[4];
    bitseqBc = static_cast<const DenseBitvec<std::array<uint64_t,8> >*>(bc)->bitvec[4];
    if (bitseqThis > bitseqBc)
    {
        return -1;
    } else if (bitseqThis < bitseqBc)
    {
        return 1;
    }
    bitseqThis = this->bitvec[5];
    bitseqBc = static_cast<const DenseBitvec<std::array<uint64_t,8> >*>(bc)->bitvec[5];
    if (bitseqThis > bitseqBc)
    {
        return -1;
    } else if (bitseqThis < bitseqBc)
    {
        return 1;
    }
    bitseqThis = this->bitvec[6];
    bitseqBc = static_cast<const DenseBitvec<std::array<uint64_t,8> >*>(bc)->bitvec[6];
    if (bitseqThis > bitseqBc)
    {
        return -1;
    } else if (bitseqThis < bitseqBc)
    {
        return 1;
    }
    bitseqThis = this->bitvec[7];
    bitseqBc = static_cast<const DenseBitvec<std::array<uint64_t,8> >*>(bc)->bitvec[7];
    if (bitseqThis > bitseqBc)
    {
        return -1;
    } else if (bitseqThis < bitseqBc)
    {
        return 1;
    } else {
        return 0;
    }
}


template <>
void DenseBitvec<std::vector<uint64_t> >::setZero()
{
    std::fill(this->bitvec.begin(), this->bitvec.end(), 0);
    this->popcount = 0;
}

template <>
void DenseBitvec<uint32_t>::setZero()
{
    this->bitvec = 0;
    this->popcount = 0;
}

template <>
void DenseBitvec<uint64_t>::setZero()
{
    this->bitvec = 0;
    this->popcount = 0;
}

template <>
void DenseBitvec<std::array<uint64_t, 2> >::setZero()
{
    std::fill(this->bitvec.begin(), this->bitvec.end(), 0);
    this->popcount = 0;
}

template <>
void DenseBitvec<std::array<uint64_t, 4> >::setZero()
{
    std::fill(this->bitvec.begin(), this->bitvec.end(), 0);
    this->popcount = 0;
}

template <>
void DenseBitvec<std::array<uint64_t, 8> >::setZero()
{
    std::fill(this->bitvec.begin(), this->bitvec.end(), 0);
    this->popcount = 0;
}



template <>
void DenseBitvec<std::vector<uint64_t> >::setOnes(const size_t n)
{
    for (size_t i = 0; i < n/64; ++i)
    {
        this->bitvec[i] = 0xffffffffffffffffUL;
    }
    const size_t nmod = n % 64;
    if (nmod)
    {
        this->bitvec[n/64] = Bithacks::reverseBits64(((uint64_t)1 << nmod) - 1);
    }
    this->popcount = n;
}

template <>
void DenseBitvec<uint32_t>::setOnes(const size_t n)
{
    const size_t nmod = n % 32;
    if (nmod)
    {
        this->bitvec = Bithacks::reverseBits32(((uint32_t)1 << nmod) - 1);
    } else {
        this->bitvec = 0xffffffff;
    }
    this->popcount = 32;
}

template <>
void DenseBitvec<uint64_t>::setOnes(const size_t n)
{
    const size_t nmod = n % 64;
    if (nmod)
    {
        this->bitvec = Bithacks::reverseBits64(((uint64_t)1 << nmod) - 1);
    } else {
        this->bitvec = 0xffffffffffffffffUL;
    }
    this->popcount = 64;
}

template <>
void DenseBitvec<std::array<uint64_t, 2> >::setOnes(const size_t n)
{
    for (size_t i = 0; i < n/64; ++i)
    {
        this->bitvec[i] = 0xffffffffffffffffUL;
    }
    const size_t nmod = n % 64;
    if (nmod)
    {
        this->bitvec[n/64] = Bithacks::reverseBits64(((uint64_t)1 << nmod) - 1);
    }
    this->popcount = n;
}

template <>
void DenseBitvec<std::array<uint64_t, 4> >::setOnes(const size_t n)
{
    for (size_t i = 0; i < n/64; ++i)
    {
        this->bitvec[i] = 0xffffffffffffffffUL;
    }
    const size_t nmod = n % 64;
    if (nmod)
    {
        this->bitvec[n/64] = Bithacks::reverseBits64(((uint64_t)1 << nmod) - 1);
    }
    this->popcount = n;
}

template <>
void DenseBitvec<std::array<uint64_t, 8> >::setOnes(const size_t n)
{
    for (size_t i = 0; i < n/64; ++i)
    {
        this->bitvec[i] = 0xffffffffffffffffUL;
    }
    const size_t nmod = n % 64;
    if (nmod)
    {
        this->bitvec[n/64] = Bithacks::reverseBits64(((uint64_t)1 << nmod) - 1);
    }
    this->popcount = n;
}