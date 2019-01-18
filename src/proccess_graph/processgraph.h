#pragma once

#include <algorithm>
#include <atomic>
#include <memory>
#include <set>
#include <unordered_map>
#include <vector>

#include "audiobufferwrapper.h"


class AudioNode;

static const int INVALID_NODE_ID = -1;

struct ConnectionPoint
{
    int nodeId;
    unsigned portNumber;

    bool operator==(const ConnectionPoint& other)
    {
        return nodeId == other.nodeId &&
               portNumber == other.portNumber;
    }
};

enum class PointDirection
{
    Input,
    Output
};

class ProcessGraph
{
    using AudioNodeVector = std::vector<std::unique_ptr<AudioNode>>;
    class Edge;

public:
    ProcessGraph();

    void setNodesOn(float frequency, float velocity);

    void setNodesOff();

    bool isActive() const;

    bool addNode(int nodeID, std::unique_ptr<AudioNode> node);

    bool removeNode(int nodeId);

    bool addConnection(const ConnectionPoint& outputPoint,
                       const ConnectionPoint& inputPoint);

    bool removeConnection(const ConnectionPoint& outputPoint,
                          const ConnectionPoint& inputPoint);

    bool setInitNode(const ConnectionPoint& outputPoint);

    void proccessData(AudioBufferWrapper& outData);

private:
    bool updatePath();

    bool visitNode(int nodeId,
                   std::set<int>& visiting,
                   std::vector<int>& visited);


    std::unordered_map<int, std::unique_ptr<AudioNode>> m_nodes;
    std::shared_ptr<Edge> m_outEdge;
    std::shared_ptr<AudioNodeVector> m_currentPath;

    friend class AudioNode;
};

class ProcessGraph::Edge
{
    AudioBufferWrapper* getData() { return m_outsideData?  m_outsideData : &m_sharedData; }
    void setMyOwnData(AudioBufferWrapper* data) { m_outsideData = data; }

    AudioBufferWrapper m_sharedData;
    AudioBufferWrapper* m_outsideData = nullptr;

    ConnectionPoint m_outPoint;
    ConnectionPoint m_inPoint;

    bool operator==(const Edge& other)
    {
        return m_outPoint == other.m_outPoint &&
               m_inPoint == other.m_inPoint;
    }

    friend class ProcessGraph;
    friend class AudioNode;

public:
    Edge()
        : m_outPoint ({INVALID_NODE_ID,0})
        , m_inPoint ({INVALID_NODE_ID,0})
    {}

    Edge(ConnectionPoint outPoint,
         ConnectionPoint inPoint)
        : m_outPoint (outPoint)
        , m_inPoint (inPoint)
    {}
};
