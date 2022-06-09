#ifndef SPARSEBITVEC_H
#define SPARSEBITVEC_H

#include <vector>
#include <cstdint>
#include <assert.h>

#include "bitcollection.h"




// Sparse Bitvector implementation
// Internal datastructure is sorted ascendingly on indices
class SparseBitvec : public BitCollection
{

public:

    // ---- Ctor
    SparseBitvec();

    // Argument:
    //          bits    raw representation of bitvector
    SparseBitvec(std::vector<uint32_t>&& bits);

    // ---------

    // ---- Destructor
    ~SparseBitvec();

    // ---------

    // Get the union of this and argument and return the union as new object
    virtual SparseBitvec* setunion(const BitCollection& bc) const;
    // Get the union of this and argument and modify this to be the result
    virtual void thisUnion(const SparseBitvec& bc);
    // Get the union of all BitCollections given by iterators
    virtual SparseBitvec* setunionMany(std::vector<BitCollection*>::const_iterator begin, std::vector<BitCollection*>::const_iterator end) const;
    // Get the intersection of all BitCollections given by iterators
    virtual SparseBitvec* setinterMany(std::vector<BitCollection*>::const_iterator begin, std::vector<BitCollection*>::const_iterator end) const;
    // Intersect argument with this and return the intersection as new object
    virtual SparseBitvec*  setinter(const BitCollection& bc) const;
    // Get set difference (set minus) between *this and argument as new object
    virtual SparseBitvec* setdiff(const BitCollection&) const;
    // Get set diff *this \ dvec, modifying *this to be the result
    virtual SparseBitvec& diffArg(const BitCollection& bc);
    // modify this to be the result of (this) AND NOT(other)
    virtual void thisAndNot(const BitCollection& other);

    virtual void thisUnion(const BitCollection& other);
    virtual void thisInter(const BitCollection& other);

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

    // Get the first one that is set in the bitcollection
    virtual uint32_t firstBitId()
    {
        if (bitvec.size() > 0)
        {
            return bitvec.front();
        }
        return 0;
    }

    // Get number of ones set in transaction
    virtual inline uint32_t getPopCount() const
    {
        return bitvec.size();
    }
    virtual uint32_t intersectionCount(const BitCollection& bc) const;
    // Get number of ones set in intersection of this with all other arguments
    virtual uint32_t intersectionCountMany(const std::vector<const BitCollection*>) const;
    // Get the number of ones set in this but not in the intersection of the others
    virtual uint32_t thisAndNotCountInterOthers(const std::vector<const BitCollection*>) const;
    // Get the number of ones set in this but not in any of the others (union of the others)
    virtual uint32_t thisAndNotCountAnyOthers(const std::vector<const BitCollection*>) const {
        assert(false); // not implemented
    }

    // Check if bit collection is empty
    virtual inline bool isEmpty() const
    {
        return bitvec.empty();
    }

    // Copy bitcollection
    virtual inline SparseBitvec* copy() const
    {
        return new SparseBitvec(std::vector<uint32_t>(this->bitvec));
    }
    // Copy argument's content into this bit collection
    virtual void copyContent(const BitCollection* bc)
    {
        this->bitvec = static_cast<const SparseBitvec*>(bc)->bitvec;
    }

    // get a vector of indices of bits set in the DS
    virtual const std::vector<uint32_t>* getOnes() const
    {
        return new std::vector<uint32_t>(this->bitvec);
    }
    // retrieves iterator over items in bitcollection
    virtual std::pair<std::vector<uint32_t>::const_iterator, std::vector<uint32_t>::const_iterator> getIts() const
    {
        return {bitvec.cbegin(), bitvec.cend()};
    }

    virtual const int lexComp(const BitCollection* bc) const;

    virtual inline void setZero()
    {
        this->bitvec.clear();
    }


    // TODO: for debugging
    virtual void print(std::ostream&) const;
    virtual void print(std::ostream&, const std::vector<uint32_t>&) const;
    virtual void print(std::ostream&, const std::vector<uint32_t>&, const std::string) const;

// protected:

    // Stores the indices of the 1s in the transaction
    std::vector<uint32_t> bitvec;


};

#endif // SPARSEBITVEC_H
