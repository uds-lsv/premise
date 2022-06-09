#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "database.h"
#include "bitfactory.h"
#include "bitcollection.h"
#include "sparsebitvec.h"
#include "const.h"


Database::Database(const std::string& filePathFeatures, const std::string& filePathLabels)
{
    descriptorFeatures = readDatFile(filePathFeatures, dataFeatures);
    descriptorLabels = readDatFile(filePathLabels, dataLabels);

    std::cout << "Unique items: " << descriptorFeatures.numCols << "\nNum instances: " << descriptorFeatures.numRows;
    std::cout << "\nNum labels: " << descriptorLabels.numRows << std::endl;
    assert(descriptorFeatures.numRows == descriptorLabels.numRows);
}

Database::~Database()
{
}

Database::DataDescriptor Database::determineDS(uint32_t numRows, uint32_t numCols, uint32_t numOnes)
{
    // Just go for the generic dense bitvector implementation for testing
    DataType dataType = gen;
    // if (numOnes < CONST::SPARSETHRESH*numRows*numCols)
    // {
    //     dataType = sparse;
    // } else if (numCols <= 32)
    // {
    //     dataType = u32;
    // } else if (numCols <= 64)
    // {
    //     dataType = u64;
    // } else if (numCols <= 128)
    // {
    //     dataType = a128;
    // } else if (numCols <= 256)
    // {
    //     dataType = a256;
    // } else if (numCols <= 512)
    // {
    //     dataType = a512;
    // } else
    // {
    //     dataType = gen;
    // }
    Database::DataDescriptor descriptor{dataType, numCols, numRows, numOnes};
    return descriptor;
}

Database::DataDescriptor Database::readDatFile(const std::string& filePath, std::vector<std::shared_ptr<BitCollection> >& data)
{
    // parse whole file to determine number of rows, columns and ones
    // and extract the data
    std::ifstream ifs (filePath);
    std::string line;

    std::vector<std::vector<uint32_t> > rows;
    uint32_t numRows, numCols = 0, numOnes = 0;
    if (ifs.is_open())
    {
        while (std::getline(ifs,line))
        {
            // parse space separated line of sparse indices (positions of the ones)
            std::stringstream lineStream(line);
            std::string token;
            std::vector<uint32_t> row;
            while (std::getline(lineStream, token, ' ')) {
                row.push_back(std::stoi(token));
            }
            if (row.size() == 0)
                continue;

            ensureValidItemOrder(row);

            if (numCols < row.back())
            {
                numCols = row.back();
            }
            numOnes += row.size();
            rows.push_back(row);
        }
        numCols += 1; // contains the largest index (0 started), +1 to obtain number of columns
        numRows = rows.size();
        ifs.close();
    } else {
        throw std::runtime_error("File '" + filePath + "' not found");
    }

    // store data as BitCollections
    DataDescriptor descriptor = determineDS(numRows, numCols, numOnes);
    data.reserve(rows.size());
    for (auto& row: rows)
    {
        data.push_back(BitFactory::genBitrep(descriptor.dataType, row, descriptor.numCols));
    }
    return descriptor;
}

void Database::ensureValidItemOrder(std::vector<uint32_t> &row)
{
        // do not check if already sorted but sort directly
        // if already sorted, the time penalty should be small
        std::sort(row.begin(), row.end());
        // make entries unique within a row
        row.erase( std::unique( row.begin(), row.end() ), row.end() );
}

std::ostream& operator<<(std::ostream& os, const Database& db)
{
    os << db.dataLabels.size() << " labels and " << db.dataFeatures.size() << "features.\n";
    for (std::pair<std::vector<std::shared_ptr<BitCollection> >::const_iterator, std::vector<std::shared_ptr<BitCollection> >::const_iterator>                rowIterPair (db.dataFeatures.cbegin(),db.dataLabels.cbegin());
    rowIterPair.first != db.dataFeatures.cend();
    ++rowIterPair.first, ++rowIterPair.second)
    {
        (*rowIterPair.first)->print(os);
        os << " | ";
        (*rowIterPair.second)->print(os);
        os << "\n";
    }
    return os;
}



std::shared_ptr<BitCollection> Database::getNonzeroItems() const
{

    std::cout << "WARNING: Using inefficient SparseBitvector setBit() method.\n";
    std::shared_ptr<BitCollection> nonzeroIts (new SparseBitvec());
    for (const auto& row : dataFeatures)
    {
        std::unique_ptr<const std::vector<uint32_t> > rOnes (row->getOnes());
        for (const uint32_t idx : *rOnes)
        {
            nonzeroIts->setBit(idx, true);
        }
    }
    return nonzeroIts;
}