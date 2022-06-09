
#include <iostream>
#include <limits>

#include "sparsebitvec.h"

SparseBitvec::SparseBitvec()
{
}

SparseBitvec::SparseBitvec(std::vector<uint32_t>&& bits) :
        bitvec(bits)
{
}

SparseBitvec::~SparseBitvec()
{
}



SparseBitvec* SparseBitvec::setunion(const BitCollection& bc) const
{
    SparseBitvec* res = new SparseBitvec();
    res->bitvec.reserve(this->bitvec.size() + static_cast<const SparseBitvec&>(bc).bitvec.size());

    auto thisIt = this->bitvec.begin();
    auto bcIt = static_cast<const SparseBitvec&>(bc).bitvec.begin();

    while (thisIt != this->bitvec.end() && bcIt != static_cast<const SparseBitvec&>(bc).bitvec.end())
    {
        if (*thisIt < *bcIt)
        {
            res->bitvec.emplace_back(*thisIt);
            ++thisIt;
        } else if (*thisIt > *bcIt)
        {
            res->bitvec.emplace_back(*bcIt);
            ++bcIt;
        } else {
            res->bitvec.emplace_back(*thisIt);
            ++bcIt, ++thisIt;
        }
    }
    while (thisIt != this->bitvec.end())
    {
        res->bitvec.emplace_back(*thisIt);
        ++thisIt;
    }
    while (bcIt != static_cast<const SparseBitvec&>(bc).bitvec.end())
    {
        res->bitvec.emplace_back(*bcIt);
        ++bcIt;
    }
    res->bitvec.shrink_to_fit();
    return res;
}
void SparseBitvec::thisUnion(const SparseBitvec& bc)
{
    auto thisIt = this->bitvec.begin();
    auto bcIt = bc.bitvec.begin();

    while (thisIt != this->bitvec.end() && bcIt != bc.bitvec.end())
    {
        if (*thisIt < *bcIt)
        {
            ++thisIt;
        } else if (*thisIt > *bcIt)
        {
            thisIt = this->bitvec.insert(thisIt, *bcIt);
            ++bcIt;
        } else {
            ++bcIt, ++thisIt;
        }
    }
    while (bcIt != bc.bitvec.end())
    {
        this->bitvec.push_back(*bcIt);
        ++bcIt;
    }
}

SparseBitvec* SparseBitvec::setunionMany(std::vector<BitCollection*>::const_iterator begin, std::vector<BitCollection*>::const_iterator end) const
{
    SparseBitvec* res = new SparseBitvec();

    // current iterators
    std::vector<std::vector<uint32_t>::iterator> currIts (end-begin);
    // end iterators
    std::vector<std::vector<uint32_t>::iterator> endIts (end-begin);
    // flag if item is left to process
    bool hasItems = false;
    // fill them
    for (uint32_t idx = 0; begin < end; ++begin, ++idx)
    {
        currIts[idx] = static_cast<SparseBitvec*>(*begin)->bitvec.begin();
        endIts[idx] = static_cast<SparseBitvec*>(*begin)->bitvec.end();
        if (currIts[idx] != endIts[idx])
        {
            hasItems = true;
        }
    }

    // check all next items in all bit collections, if some are left to process
    while (hasItems)
    {
        // find minimum
        uint32_t min = std::numeric_limits<uint32_t>::max();
        for (uint32_t idx = 0; idx < currIts.size(); ++idx)
        {
            if (currIts[idx] != endIts[idx] && *currIts[idx] < min)
            {
                min = *currIts[idx];
            }
        }
        // advance all iterators in collections that have minimum as next item
        for (uint32_t idx = 0; idx < currIts.size(); ++idx)
        {
            if (currIts[idx] != endIts[idx] && *currIts[idx] == min)
            {
                ++currIts[idx];
            }
        }
        if (min < std::numeric_limits<uint32_t>::max())
        {
            // add minimum to res
            res->bitvec.push_back(min);
        } else {
            // if no new minimum that means no more items to process
            hasItems = false;
        }
    }

    return res;
}

SparseBitvec* SparseBitvec::setinterMany(std::vector<BitCollection*>::const_iterator begin, std::vector<BitCollection*>::const_iterator end) const
{
    SparseBitvec* res = new SparseBitvec();

    // current iterators
    std::vector<std::vector<uint32_t>::iterator> currIts (end-begin);
    // end iterators
    std::vector<std::vector<uint32_t>::iterator> endIts (end-begin);
    // flag if item is left to process
    bool hasItems = false;
    // fill them
    for (uint32_t idx = 0; begin < end; ++begin, ++idx)
    {
        currIts[idx] = static_cast<SparseBitvec*>(*begin)->bitvec.begin();
        endIts[idx] = static_cast<SparseBitvec*>(*begin)->bitvec.end();
        if (currIts[idx] != endIts[idx])
        {
            hasItems = true;
        }
    }

    // check all next items in all bit collections, if some are left to process
    while (hasItems)
    {
        // find minimum
        uint32_t min = std::numeric_limits<uint32_t>::max();
        for (uint32_t idx = 0; idx < currIts.size(); ++idx)
        {
            if (currIts[idx] != endIts[idx] && *currIts[idx] < min)
            {
                min = *currIts[idx];
            }
        }
        // advance all iterators in collections that have minimum as next item
        bool add = true;
        for (uint32_t idx = 0; idx < currIts.size(); ++idx)
        {
            if (currIts[idx] != endIts[idx] && *currIts[idx] == min)
            {
                ++currIts[idx];
            } else {
                add = false;
            }
        }
        if (add)
        {
            // add minimum to res
            res->bitvec.push_back(min);
        } else if (min < std::numeric_limits<uint32_t>::max()) {
            // if no new minimum that means no more items to process
            hasItems = false;
        }
    }

    return res;
}

SparseBitvec* SparseBitvec::setinter(const BitCollection& bc) const
{
    SparseBitvec* res = new SparseBitvec();
    res->bitvec.reserve(std::min(this->bitvec.size(), static_cast<const SparseBitvec&>(bc).bitvec.size()));

    auto thisIt = this->bitvec.begin();
    auto bcIt = static_cast<const SparseBitvec&>(bc).bitvec.begin();

    while (thisIt != this->bitvec.end() && bcIt != static_cast<const SparseBitvec&>(bc).bitvec.end())
    {
        if (*thisIt < *bcIt)
        {
            ++thisIt;
        } else if (*thisIt > *bcIt)
        {
            ++bcIt;
        } else {
            res->bitvec.emplace_back(*thisIt);
            ++thisIt, ++bcIt;
        }
    }
    res->bitvec.shrink_to_fit();
    return res;
}

SparseBitvec* SparseBitvec::setdiff(const BitCollection& bc) const
{
    SparseBitvec* res = new SparseBitvec();
    res->bitvec.reserve(this->bitvec.size());

    auto thisIt = this->bitvec.cbegin();
    auto bcIt = static_cast<const SparseBitvec&>(bc).bitvec.cbegin();

    while (thisIt != this->bitvec.cend() && bcIt != static_cast<const SparseBitvec&>(bc).bitvec.cend())
    {
        if (*thisIt < *bcIt)
        {
            res->bitvec.push_back(*thisIt);
            ++thisIt;
        } else if (*thisIt > *bcIt)
        {
            ++bcIt;
        } else {
            ++thisIt, ++bcIt;
        }
    }
    while (thisIt != this->bitvec.end())
    {
        res->bitvec.push_back(*thisIt);
        ++thisIt;
    }
    res->bitvec.shrink_to_fit();
    return res;
}

SparseBitvec& SparseBitvec::diffArg(const BitCollection& bc)
{
    auto thisIt = this->bitvec.begin();
    auto bcIt = static_cast<const SparseBitvec&>(bc).bitvec.begin();
    auto writeIt = this->bitvec.begin();

    while (thisIt != this->bitvec.end() && bcIt != static_cast<const SparseBitvec&>(bc).bitvec.end())
    {
        if (*thisIt < *bcIt)
        {
            *writeIt = *thisIt;
            ++thisIt, ++writeIt;
        } else if (*thisIt > *bcIt)
        {
            ++bcIt;
        } else {
            ++thisIt, ++bcIt;
        }
    }
    while (thisIt != this->bitvec.end())
    {
        *writeIt = *thisIt;
        ++thisIt, ++writeIt;
    }
    this->bitvec.resize(writeIt - this->bitvec.begin());
    return *this;
}

void SparseBitvec::thisAndNot(const BitCollection& other)
{
    std::cerr << "Is this working correct? (SparseBitvec::thisAndNot)\n";
    auto thisIt = this->bitvec.begin();
    auto bcIt = static_cast<const SparseBitvec&>(other).bitvec.begin();

    while (thisIt != this->bitvec.end() && bcIt != static_cast<const SparseBitvec&>(other).bitvec.end())
    {
        if (*thisIt < *bcIt)
        {
            ++thisIt;
        } else if (*thisIt > *bcIt)
        {
            ++bcIt;
        } else {
            thisIt = this->bitvec.erase(thisIt);
        }
    }
}

bool SparseBitvec::argIsSubset(const BitCollection& bc) const
{
    auto thisIt = this->bitvec.begin();
    auto bcIt = static_cast<const SparseBitvec&>(bc).bitvec.begin();

    while (thisIt != this->bitvec.end() && bcIt != static_cast<const SparseBitvec&>(bc).bitvec.end())
    {
        if (*thisIt < *bcIt)
        {
            ++thisIt;
        } else if (*thisIt > *bcIt)
        {
            return false;
        } else {
            ++thisIt, ++bcIt;
        }
    }
    if (bcIt != static_cast<const SparseBitvec&>(bc).bitvec.end())
    {
        return false;
    }
    return true;
}

bool SparseBitvec::hasOverlap(const BitCollection& bc) const
{
    auto thisIt = this->bitvec.begin();
    auto bcIt = static_cast<const SparseBitvec&>(bc).bitvec.begin();

    while (thisIt != this->bitvec.end() && bcIt != static_cast<const SparseBitvec&>(bc).bitvec.end())
    {
        if (*thisIt < *bcIt)
        {
            ++thisIt;
        } else if (*thisIt > *bcIt)
        {
            ++bcIt;
        } else {
            return true;
        }
    }
    return false;
}

void SparseBitvec::setBit(const size_t i, const bool v)
{
    for (std::vector<uint32_t>::iterator it = this->bitvec.begin(); it != this->bitvec.end(); ++it)
    {
        if (*it == i)
        {
            return;
        }
        if (*it > i)
        {
            this->bitvec.insert(it, i);
            return;
        }
    }
    bitvec.push_back(i);
}

bool SparseBitvec::operator==(const BitCollection& bc) const
{
    auto thisIt = this->bitvec.begin();
    auto bcIt = static_cast<const SparseBitvec&>(bc).bitvec.begin();

    while (thisIt != this->bitvec.end() && bcIt != static_cast<const SparseBitvec&>(bc).bitvec.end())
    {
        if (*thisIt != *bcIt)
        {
            return false;
        } else {
            ++thisIt, ++bcIt;
        }
    }
    if (thisIt != this->bitvec.end() || bcIt != static_cast<const SparseBitvec&>(bc).bitvec.end())
    {
        return false;
    }
    return true;
}

bool SparseBitvec::operator[](const size_t i) const
{
    auto thisIt = this->bitvec.begin();

    while (thisIt != this->bitvec.end())
    {
        if (*thisIt < i)
        {
            ++thisIt;
        } else if (*thisIt == i)
        {
            return true;
        } else {
            return false;
        }
    }
    return false;
}

uint32_t SparseBitvec::intersectionCount(const BitCollection& bc) const
{
    uint32_t popcnt = 0;

    auto thisIt = this->bitvec.begin();
    auto bcIt = static_cast<const SparseBitvec&>(bc).bitvec.begin();

    while (thisIt != this->bitvec.end() && bcIt != static_cast<const SparseBitvec&>(bc).bitvec.end())
    {
        if (*thisIt < *bcIt)
        {
            ++thisIt;
        } else if (*thisIt > *bcIt)
        {
            ++bcIt;
        } else {
            ++popcnt;
            ++thisIt, ++bcIt;
        }
    }
    return popcnt;
}

uint32_t SparseBitvec::intersectionCountMany(const std::vector<const BitCollection*> bcs) const
{
    uint32_t popcnt = 0;

    // initialize iterators
    auto thisIt = this->bitvec.begin();
    std::vector<std::vector<uint32_t>::const_iterator> bcIts (bcs.size());
    for (size_t i = 0; i < bcIts.size(); ++i)
    {
        bcIts[i] = static_cast<const SparseBitvec*>(bcs[i])->bitvec.begin();
    }

    // test if we reached the end for any of the bitvectors
    bool isNotDone = thisIt != this->bitvec.end();
    for (size_t i = 0; i < bcIts.size(); ++i)
    {
        isNotDone = isNotDone && bcIts[i] != static_cast<const SparseBitvec*>(bcs[i])->bitvec.end();
    }

    // loop over all collections simultaneously
    while (isNotDone)
    {
        uint32_t thisPos = *thisIt;
        bool hasChanged = false;
        uint32_t isEqualCount = 0;
        for (size_t j = 0; j < bcIts.size(); ++j)
        {
            // check if we have the same bit set as in 'this' iterator
            if (*bcIts[j] == thisPos)
            {
                ++bcIts[j];
                ++isEqualCount;
            } else {
                // otherwise count up iterators if possible until we reach the bit position of 'this' iterator
                while (bcIts[j] != static_cast<const SparseBitvec*>(bcs[j])->bitvec.end() && *bcIts[j] < thisPos)
                {
                    hasChanged = true;
                    ++bcIts[j];
                }
                if (bcIts[j] != static_cast<const SparseBitvec*>(bcs[j])->bitvec.end() && *bcIts[j] == thisPos)
                {
                    ++bcIts[j];
                    ++isEqualCount;
                }
            }
        }
        // check if all bits are set at 'this' position
        if (isEqualCount == bcIts.size())
        {
            ++thisIt;
            ++popcnt;
        } else {
            // if we did not change the other iterators that means 'this' is the smallest and must be moved further
            if (!hasChanged)
                ++thisIt;
        }
        // update if we reached the end of one of the collections
        isNotDone = thisIt != this->bitvec.end();
        for (size_t i = 0; i < bcIts.size(); ++i)
        {
            isNotDone = isNotDone && bcIts[i] != static_cast<const SparseBitvec*>(bcs[i])->bitvec.end();
        }
    }
    return popcnt;
}

uint32_t SparseBitvec::thisAndNotCountInterOthers(const std::vector<const BitCollection*> bcs) const
{

    std::cout << "TODO: check for correctness of this function (SparseBitvec::thisAndNotCountInterOthers)\n";
    uint32_t popcnt = 0;

    // initialize iterators
    auto thisIt = this->bitvec.begin();
    std::vector<std::vector<uint32_t>::const_iterator> bcIts (bcs.size());
    for (size_t i = 0; i < bcIts.size(); ++i)
    {
        bcIts[i] = static_cast<const SparseBitvec*>(bcs[i])->bitvec.begin();
    }

    // test if we reached the end for any of the bitvectors
    bool isNotDone = thisIt != this->bitvec.end();
    for (size_t i = 0; i < bcIts.size(); ++i)
    {
        isNotDone = isNotDone && bcIts[i] != static_cast<const SparseBitvec*>(bcs[i])->bitvec.end();
    }

    // loop over all collections simultaneously
    while (isNotDone)
    {
        uint32_t thisPos = *thisIt;
        uint32_t isEqualCount = 0;
        for (size_t j = 0; j < bcIts.size(); ++j)
        {
            // check if we have the same bit set as in 'this' iterator
            if (*bcIts[j] == thisPos)
            {
                ++bcIts[j];
                ++isEqualCount;
            } else {
                // otherwise count up iterators if possible until we reach the bit position of 'this' iterator
                while (bcIts[j] != static_cast<const SparseBitvec*>(bcs[j])->bitvec.end() && *bcIts[j] < thisPos)
                {
                    ++bcIts[j];
                }
                if (bcIts[j] != static_cast<const SparseBitvec*>(bcs[j])->bitvec.end() && *bcIts[j] == thisPos)
                {
                    ++bcIts[j];
                    ++isEqualCount;
                }
            }
        }
        // check if all bits are set at 'this' position
        if (isEqualCount == bcIts.size())
        {
            ++thisIt;
        } else {
            ++popcnt;
            ++thisIt;
        }
        // update if we reached the end of one of the collections
        isNotDone = thisIt != this->bitvec.end();
        for (size_t i = 0; i < bcIts.size(); ++i)
        {
            isNotDone = isNotDone && bcIts[i] != static_cast<const SparseBitvec*>(bcs[i])->bitvec.end();
        }
    }
    popcnt += std::distance(thisIt, this->bitvec.end());
    return popcnt;
}

inline const int SparseBitvec::lexComp(const BitCollection* bc) const
{
    auto bcIt = static_cast<const SparseBitvec*>(bc)->bitvec.begin();
    auto bcEnd = static_cast<const SparseBitvec*>(bc)->bitvec.end();
    auto thisIt = this->bitvec.begin();
    // iterate through both bitvecs
    for(; thisIt != this->bitvec.end() && bcIt != bcEnd; ++thisIt, ++bcIt)
    {
        // if the elements differ, check who is smaller, else just go on comparing next element
        if (*thisIt < *bcIt)
        {
            return -1;
        } else if (*thisIt > *bcIt)
        {
            return 1;
        }
    }
    // check for empty if they were not of equal length
    if (thisIt == this->bitvec.end())
    {
        if (bcIt == bcEnd)
        {
            return 0;
        } else {
            return 1;
        }
    } else {
        return -1;
    }

}

void SparseBitvec::thisUnion(const BitCollection& other)
{
    std::cerr << "I am not implemented! (SparseBitvec::thisUnion)\n\n";
    exit(1);
}

void SparseBitvec::thisInter(const BitCollection& other)
{
    std::cerr << "I am not implemented! (SparseBitvec::thisUnion)\n\n";
    exit(1);
}

void SparseBitvec::print(std::ostream& os) const
{
    for (uint32_t id : this->bitvec)
    {
        os << id << "::";
    }
}
void SparseBitvec::print(std::ostream& os, const std::vector<uint32_t>& idMap) const
{
    for (uint32_t id : this->bitvec)
    {
        os << idMap[id] << "::";
    }
}
void SparseBitvec::print(std::ostream& os, const std::vector<uint32_t>& idMap, const std::string sep) const
{
    for (uint32_t id : this->bitvec)
    {
        os << idMap[id] << sep;
    }
}