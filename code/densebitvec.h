#ifndef DENSEBITVEC_H
#define DENSEBITVEC_H

#include <type_traits>
#include <cstddef>
#include <vector>
#include <cstdint>
#include <assert.h>
#include <array>

#include "bitcollection.h"




// Dense Bitvector implementation
// template argument allows to specify special, short bitvector implementations.
// Currently supported as specs:
//          uint32_t,
//          uint64_t,
//          std::array<uint64_t,2>  (128 bit vector)
//          std::array<uint64_t,4>  (256 bit vector)
//          std::array<uint64_t,8>  (512 bit vector)
//          std::vector<uint64_t>   (default for generic number of bits)
//
// Convention:
// Indexing is from most to least significant bit, e.g. if at position zero there is a 1
//      the uint will have a 1 at the most significant bit.
template <typename T = std::vector<uint64_t> >
class DenseBitvec : public BitCollection
{

public:

    // ---- Ctor
    DenseBitvec();
    // Argument:
    //          size    how many bits should the bitvector hold
    template<typename dummy=std::enable_if<std::is_same<T, std::vector<uint64_t>>::value, size_t> >
    DenseBitvec(typename dummy::type size);
    // Argument:
    //          bits    raw representation of bitvector
    DenseBitvec(T&& bits, uint32_t pop = 0);


    // ---------

    // Get the union of this and argument and return the union as new object
    virtual DenseBitvec* setunion(const BitCollection& bc) const;
    // Get the union of all BitCollections given by iterators (keeps *this untouched)
    virtual DenseBitvec* setunionMany(std::vector<BitCollection*>::const_iterator begin, std::vector<BitCollection*>::const_iterator end) const;
    // Get the intersection of all BitCollections given by iterators (keeps *this untouched)
    virtual DenseBitvec* setinterMany(std::vector<BitCollection*>::const_iterator begin, std::vector<BitCollection*>::const_iterator end) const;
    // Get the union of this and argument, modifying this to be the result
    virtual void thisUnion(const BitCollection&);
    // Intersect argument with this and return the intersection as new object
    virtual DenseBitvec*  setinter(const BitCollection& bc) const;
    virtual void thisInter(const BitCollection&);
    // Intersect all arguments (excluding this) and modify this to be the results
    virtual void interMany(const std::vector<const DenseBitvec<T>*>&);
    // Get set difference (set minus) between *this and argument as new object
    virtual DenseBitvec* setdiff(const BitCollection&) const;
    // Get symmetric difference between *this and argument as new object
    virtual DenseBitvec* setsymdiff(const DenseBitvec<T>&) const;
    // compute the xor of all arguments and store it in this, return filtered bits
    virtual DenseBitvec* symdiffMany(const std::vector<const DenseBitvec<T>*>&);

    // computes the xor of arguments, with corresponding filtered lists and returns as new object
    virtual DenseBitvec* xorJoin(const DenseBitvec<T>& thisFiltered, const DenseBitvec<T>& otherXor, const DenseBitvec<T>& otherFiltered) const;
    // Get set diff *this \ dvec, modifying *this to be the result
    virtual DenseBitvec& diffArg(const BitCollection& bc);
    // modify this to be the result of (this) AND NOT(other)
    virtual void thisAndNot(const BitCollection& other);

    // Check if the argument is a subset of *this
    virtual bool argIsSubset(const BitCollection& bc) const;

    // Check if they share at least one item
    virtual bool hasOverlap(const BitCollection&) const;

    // set a specific element to argument v
    virtual void setBit(const size_t i, const bool v);

    // Compare if two bitvectors are equal
    virtual bool operator==(const BitCollection& bc) const;
    // Note: Does NOT give a reference. internal bits need to be modified through 'setBit' function
    virtual bool operator[](const size_t i) const;

    // Get number of ones set in bit collection
    virtual inline uint32_t getPopCount() const
    {
        return popcount;
    }
    virtual uint32_t intersectionCount(const BitCollection& bc) const;
    // Get number of ones set in intersection of this with all other arguments
    virtual uint32_t intersectionCountMany(const std::vector<const BitCollection*> bcs) const;
    // Get the number of ones set in this but not in the intersection of the others
    virtual uint32_t thisAndNotCountInterOthers(const std::vector<const BitCollection*>) const;
    // Get the number of ones set in this but not in any of the others (union of the others)
    virtual uint32_t thisAndNotCountAnyOthers(const std::vector<const BitCollection*>) const;
    // Get the number of ones after combining two n-ary xors into one
    virtual uint32_t xorCount(const DenseBitvec<T>& thisFiltered, const DenseBitvec<T>& otherXor, const DenseBitvec<T>& otherFiltered) const;

    // Check if bit collection is empty
    virtual inline bool isEmpty() const
    {
        return this->popcount == 0;
    }

    // Copy bitcollection
    virtual inline DenseBitvec* copy() const
    {
        return new DenseBitvec(T(this->bitvec), this->popcount);
    }
    // Copy argument's content into this bit collection
    virtual void copyContent(const BitCollection* bc)
    {
        this->bitvec = static_cast<const DenseBitvec<T>* >(bc)->bitvec;
        this->popcount = static_cast<const DenseBitvec<T>* >(bc)->popcount;
    }

    // get a vector of indices of bits set in the DS
    const std::vector<uint32_t>* getOnes() const;

    // lexicographical comparison
    const int lexComp(const BitCollection* bc) const;

    // set every bit to zero
    virtual void setZero();
    // set first n bits to one
    // ASSUMPTION: trailing bits after n-th bit were zero before
    virtual void setOnes(const size_t n);

    // TODO: for debugging
    virtual void print(std::ostream&) const;
    virtual void print(std::ostream&, const std::vector<uint32_t>&) const;
    virtual void print(std::ostream&, const std::vector<uint32_t>&, const std::string) const;

// protected:

    // stores how many 1s are set in the bitvector
    uint32_t popcount;
    T bitvec;


};


template <typename T>
DenseBitvec<T>::DenseBitvec() :
        popcount()
    ,   bitvec()
{
}


template<typename T>
template<typename VecType>
DenseBitvec<T>::DenseBitvec(typename VecType::type size) :
        popcount(0)
    ,   bitvec(size/64 + (size % 64 == 0 ? 0 : 1))
{
}

template <typename T>
DenseBitvec<T>::DenseBitvec(T&& bits, uint32_t pop) :
        popcount(pop)
    ,   bitvec(bits)
{
}


template <typename T>
void DenseBitvec<T>::print(std::ostream& os) const
{
    const std::vector<uint32_t>* ones = this->getOnes();
    for (uint32_t id : *ones)
    {
        os << id << "::";
    }
    delete ones;
}
template <typename T>
void DenseBitvec<T>::print(std::ostream& os, const std::vector<uint32_t>& idMap) const
{
    const std::vector<uint32_t>* ones = this->getOnes();
    for (uint32_t id : *ones)
    {
        os << idMap[id] << "::";
    }
    delete ones;
}
template <typename T>
void DenseBitvec<T>::print(std::ostream& os, const std::vector<uint32_t>& idMap, const std::string sep) const
{
    const std::vector<uint32_t>* ones = this->getOnes();
    for (uint32_t id : *ones)
    {
        os << idMap[id] << sep;
    }
    delete ones;
}


using dbitvec = DenseBitvec<std::vector<uint64_t> >;

#endif // DENSEBITVEC_H