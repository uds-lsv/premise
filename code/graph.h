#ifndef GRAPH_H
#define GRAPH_H

#include <memory>
#include <unordered_map>
#include <vector>
#include <cstdint>

class Graph
{

public:

    Graph() = delete;
    // number of nodes in graph
    Graph(const size_t n);

    void addEdges(std::shared_ptr<const std::vector<uint32_t> > X, std::shared_ptr<const std::vector<uint32_t> > Y);

    void removeEdges(std::shared_ptr<const std::vector<uint32_t> > X, std::shared_ptr<const std::vector<uint32_t> > Y);

    bool isAllowed(std::shared_ptr<const std::vector<uint32_t> > X, std::shared_ptr<const std::vector<uint32_t> > Y);

private:

    struct adjEntry {
        // flag if node has been visited
        bool dfsFlag;
        // # rules introducing this edge
        uint32_t sCount;
    };

    bool hasCycle();

    bool cycleDFS(size_t pId, size_t cId);

    void resetDFSColoring();

    // graph representation
    // adjacency matrix
    // outer vector contains parent nodes,
    // inner structure contains information on children
    // and sum of all counts children
    std::vector<std::pair<uint32_t, std::vector<adjEntry> > > adjMat;
};

#endif // GRAPH_H