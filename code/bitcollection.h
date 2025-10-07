#ifndef BITCOLLECTION_H
#define BITCOLLECTION_H

#include <memory> // shared_ptr
#include <vector>
#include <iostream>

#include "stdlib.h"
#include <cstdint>



enum DataType {u32, u64, a128, a256, a512, gen, sparse};

// Base class for all representations of transactions/itemsets
class BitCollection
{

public:

    virtual ~BitCollection();

    // Get the union of this and argument and return the union as new object
    virtual BitCollection* setunion(const BitCollection&) const = 0;
    // Get the union of all BitCollections given by iterators (keeps *this untouched)
    virtual BitCollection* setunionMany(std::vector<BitCollection*>::const_iterator begin, std::vector<BitCollection*>::const_iterator end) const = 0;
    // Get the intersection of all BitCollections given by iterators (keeps *this untouched)
    virtual BitCollection* setinterMany(std::vector<BitCollection*>::const_iterator begin, std::vector<BitCollection*>::const_iterator end) const = 0;
    // Intersect argument with this and return the intersection as new object
    virtual BitCollection* setinter(const BitCollection&) const = 0;
    // Get set difference (set minus) between *this and argument as new object
    virtual BitCollection* setdiff(const BitCollection&) const = 0;
    // Get set diff *this \ bc, modifying *this to be the result
    virtual BitCollection& diffArg(const BitCollection&) = 0;
    // modify this to be the result of (this) AND NOT(other)
    virtual void thisAndNot(const BitCollection& other) = 0;

    // Check if the argument is a subset of *this
    virtual bool argIsSubset(const BitCollection&) const = 0;

    // Check if they share at least one item
    virtual bool hasOverlap(const BitCollection&) const = 0;

    // set a specific element to argument v
    virtual void setBit(const size_t, const bool) = 0;

    // Compare if two bitvectors are equal
    virtual bool operator==(const BitCollection&) const = 0;
    // Note: Does NOT give a reference. internal bits need to be modified through 'setBit' function
    virtual bool operator[](const size_t) const = 0;

    // Get number of ones set in bit collection
    virtual uint32_t getPopCount() const = 0;
    // Get number of ones set if we would make an intersection with the argument
    virtual uint32_t intersectionCount(const BitCollection&) const = 0;
    // Get number of ones set in intersection of this with all other arguments
    virtual uint32_t intersectionCountMany(const std::vector<const BitCollection*>) const = 0;
    // Get the number of ones set in this but not in the intersection of the others
    virtual uint32_t thisAndNotCountInterOthers(const std::vector<const BitCollection*>) const = 0;
    // Get the number of ones set in this but not in any of the others (union of the others)
    virtual uint32_t thisAndNotCountAnyOthers(const std::vector<const BitCollection*>) const = 0;

    virtual void thisUnion(const BitCollection&) = 0;
    virtual void thisInter(const BitCollection&) = 0;

    // Check if bit collection is empty
    virtual bool isEmpty() const = 0;

    // Copy bitcollection
    virtual BitCollection* copy() const = 0;
    // Copy argument's content into this bit collection
    virtual void copyContent(const BitCollection* bc) = 0;

    // Get a list of all set ones (item ids)
    virtual const std::vector<uint32_t>* getOnes() const = 0;

    // Compare two bit collections lexicographically
    // Returns -1 iff *this is lexicographically smaller than *bc
    //          0 iff *this is equal to *bc
    //          1 iff *this is lex. smaller than *bc
    // Lexicographical comparison is based on lexicographical ordering extension on list of indices:
    // empty larger than all other elements;
    // compare starting with index 0, the one that has this element set is smaller or equal than the other,
    // if both have this element set increase index and repeat
    // E.g.:
    //  ABC < BC     First index is smaller
    //  AFG < AG     Second index is smaller
    //  AD < A       Second index is smaller than empty
    //  A < {}       First index is smaller than empty
    virtual const int lexComp(const BitCollection* bc) const = 0;

    // set every bit to zero
    virtual void setZero() = 0;


    // TODO: for debugging
    virtual void print(std::ostream&) const = 0;
    virtual void print(std::ostream&, const std::vector<uint32_t>&) const = 0;
    virtual void print(std::ostream&, const std::vector<uint32_t>&, const std::string) const = 0;

protected:

    BitCollection();

};

#endif // BITCOLLECTION_H