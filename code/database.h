#ifndef DATABASE_H
#define DATABASE_H

#include <vector>

#include "bitcollection.h"

class Database
{

public:

    struct DataDescriptor
    {
        DataType dataType;
        uint32_t numCols;
        uint32_t numRows;
        uint32_t numOnes;
    };

    Database(const std::string& filePathFeatures, const std::string& filePathLabels);

    ~Database();

    std::vector<std::shared_ptr<BitCollection> >& getFeatures()
    {
        return dataFeatures;
    }

    std::vector<std::shared_ptr<BitCollection> >& getLabels()
    {
        return dataLabels;
    }

    DataDescriptor& getDescriptorFeatures()
    {
        return descriptorFeatures;
    }

    DataDescriptor& getDescriptorLabels()
    {
        return descriptorLabels;
    }

    // TODO: implement this more efficiently?
    std::shared_ptr<BitCollection> getNonzeroItems() const;


    friend std::ostream& operator<<(std::ostream& os, const Database& db);


protected:

    // reads database in dat format (either features or labels) and fills it into given data storage
    DataDescriptor readDatFile(const std::string& filePath, std::vector<std::shared_ptr<BitCollection> >& data);

    // the BitCollection in which the data is stored assumes that
    // the items are unique and ordered ascendingly
    // this might not be the case, especially for word-index input
    // this method sorts the input items and makes them unique per row.
    // order and count-within-a-row are anyways not taken into account
    void ensureValidItemOrder(std::vector<uint32_t>& row);

    // Determine type of a data structure
    DataDescriptor determineDS(uint32_t numRows, uint32_t numCols, uint32_t numOnes);

    // each element corresponds to the features of one transaction
    std::vector<std::shared_ptr<BitCollection> > dataFeatures;
    DataDescriptor descriptorFeatures = {};

    // each element corresponds to the labels of one transaction
    std::vector<std::shared_ptr<BitCollection> > dataLabels;
    DataDescriptor descriptorLabels = {};
};

#endif // DATABASE_H