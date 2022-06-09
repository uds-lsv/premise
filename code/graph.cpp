#include "graph.h"

Graph::Graph(const size_t n)
{
    auto defEntry = std::pair<uint32_t, std::vector<adjEntry> >({0, std::vector<adjEntry>(n, {false, 0})});
    this->adjMat = std::vector<std::pair<uint32_t, std::vector<adjEntry> > >(n, defEntry);
}

void Graph::addEdges(std::shared_ptr<const std::vector<uint32_t> > X, std::shared_ptr<const std::vector<uint32_t> > Y)
{
    for (uint32_t x : *X)
    {
        for (uint32_t y : *Y)
        {
            ++(adjMat[x].first);
            ++(adjMat[x].second[y].sCount);
        }
    }
}

void Graph::removeEdges(std::shared_ptr<const std::vector<uint32_t> > X, std::shared_ptr<const std::vector<uint32_t> > Y)
{
    for (uint32_t x : *X)
    {
        for (uint32_t y : *Y)
        {
            auto& adjEnt = adjMat[x].second[y];
            if (adjEnt.sCount == 0)
                continue;
            --adjMat[x].first;
            --adjEnt.sCount;
        }
    }
}

bool Graph::isAllowed(std::shared_ptr<const std::vector<uint32_t> > X, std::shared_ptr<const std::vector<uint32_t> > Y)
{
    this->addEdges(X, Y);
    const bool ret = this->hasCycle();
    this->removeEdges(X, Y);
    return !ret;
}

bool Graph::hasCycle()
{
    for (size_t pId = 0; pId < adjMat.size(); ++pId)
    {
        if (adjMat[pId].first > 0)
        {
            for (size_t cId = 0; cId < adjMat[pId].second.size(); ++cId)
            {
                if (!adjMat[pId].second[cId].dfsFlag && adjMat[pId].second[cId].sCount > 0)
                {
                    if (this->cycleDFS(pId, cId))
                    {
                        this->resetDFSColoring();
                        return true;
                    }
                }
            }
        }
    }
    this->resetDFSColoring();
    return false;
}

bool Graph::cycleDFS(size_t pId,size_t cId)
{
    if (adjMat[pId].second[cId].dfsFlag)
        return true;
    adjMat[pId].second[cId].dfsFlag = true;
    if (adjMat[cId].first == 0)
    {
        return false;
    }
    for (size_t cIdNew = 0; cIdNew < adjMat[cId].second.size(); ++cIdNew)
    {
        if (adjMat[cId].second[cIdNew].sCount > 0)
        {
            if (this->cycleDFS(cId, cIdNew))
                return true;
        }
    }
    return false;
}

void Graph::resetDFSColoring()
{
    for (size_t pId = 0; pId < adjMat.size(); ++pId)
    {
        if (adjMat[pId].first > 0)
        {
            for (size_t cId = 0; cId < adjMat[pId].second.size(); ++cId)
            {
                if (adjMat[pId].second[cId].sCount > 0)
                {
                    adjMat[pId].second[cId].dfsFlag = false;
                }
            }
        }
    }
}