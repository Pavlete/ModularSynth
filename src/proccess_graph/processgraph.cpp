#include "audionode.h"
#include "processgraph.h"

#include <chrono>
#include <thread>

namespace
{
class AtomicBoolGuard
{
public:
    AtomicBoolGuard(std::atomic_bool& ab)
        : m_ab(ab)
    { m_ab = true; }

    AtomicBoolGuard(const AtomicBoolGuard& other) = delete;
    AtomicBoolGuard& operator =(const AtomicBoolGuard& other) = delete;


    ~AtomicBoolGuard()
    { m_ab = false; }

private:
    std::atomic_bool& m_ab;
};
}

void ProcessGraph::setNodesOn(float frequency, float velocity)
{
    std::for_each(m_currentPath.begin(), m_currentPath.end(), [&](int nodeID)
    {
        m_nodes[nodeID]->setActive(frequency, velocity);
    });
}

void ProcessGraph::setNodesOff()
{
    std::for_each(m_currentPath.begin(), m_currentPath.end(),
                  [&](int nodeID)
    {
        m_nodes[nodeID]->setInactive();
    });
}

bool ProcessGraph::isActive() const
{
    auto element = std::find_if(m_currentPath.begin(), m_currentPath.end(), [&](int nodeID)
    {
        return m_nodes.find(nodeID)->second->isActive();
    });

    return element != m_currentPath.end();
}

bool ProcessGraph::addNode(int nodeID, std::unique_ptr<AudioNode> node){
    auto result = m_nodes.emplace(nodeID, std::move(node));
    return result.second;
}

bool ProcessGraph::removeNode(int nodeId)
{
    return m_nodes.erase( nodeId ) != 0;
}

bool ProcessGraph::addConnection(const ConnectionPoint &outputPoint, const ConnectionPoint &inputPoint)
{
    auto edge = std::make_shared<Edge>(outputPoint,
                                       inputPoint);

    if(std::find_if(m_edges.begin(), m_edges.end(),
                    [&edge](const auto& e) {return *e  == *edge;})
            != m_edges.end())
    {
        return false;
    }

    AudioNode* outNode = m_nodes.find(outputPoint.nodeId)->second.get();
    outNode->m_outEdges[outputPoint.portNumber] = edge;

    AudioNode* inNode = m_nodes.find(inputPoint.nodeId)->second.get();
    inNode->m_inEdges[inputPoint.portNumber] = edge;

    AtomicBoolGuard g(m_editing);

    while (m_processing)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }

    m_edges.push_back(edge);

    return updatePath();
}

bool ProcessGraph::removeConnection(const ConnectionPoint &outputPoint, const ConnectionPoint &inputPoint)
{

    auto edge = std::find_if(m_edges.begin(), m_edges.end(),
                    [&](const auto& e) {return e->m_inPoint == inputPoint &&
                                               e->m_outPoint == outputPoint ;});

    if(edge == m_edges.end())
    {
        return false;
    }

    AtomicBoolGuard guard(m_editing);

    while (m_processing)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }

    m_edges.erase(edge);

    return updatePath();
}

bool ProcessGraph::setInitNode(const ConnectionPoint &newInit)
{
    if(!m_nodes.at(newInit.nodeId)->isAvailable(newInit.portNumber,
                                                PointDirection::Output))
    {
        return false;
    }

    AtomicBoolGuard guard(m_editing);

    while (m_processing)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }

    m_outEdge->m_inPoint = newInit;

    auto outNode = m_nodes.find(newInit.nodeId)->second.get();
    outNode->m_outEdges[newInit.portNumber] = m_outEdge;

    return updatePath();
}

void ProcessGraph::proccessData(AudioBufferWrapper& outData)
{
    AtomicBoolGuard guard(m_processing);
    if(m_editing)
        return;

    m_outEdge->setMyOwnData(&outData);
    for(auto& element : m_currentPath)
    {
        m_nodes[element]->process();
    }
}

bool ProcessGraph::updatePath()
{
    if( m_outEdge->m_inPoint.nodeId == INVALID_NODE_ID)
    {
        return false;
    }

    m_currentPath.clear();
    std::set<int> visitingSet;
    return visitNode(m_outEdge->m_inPoint.nodeId, visitingSet, m_currentPath);
}

bool ProcessGraph::visitNode(int nodeId, std::set<int> &visiting, std::vector<int> &visited)
{
    if(visiting.find(nodeId) != visiting.end())
    {
        visited.clear();
        return false;
    }

    if(std::find(visited.begin(), visited.end(), nodeId) != std::end(visited))
    {
        return true;
    }

    visiting.insert(nodeId);
    auto node = m_nodes.find(nodeId)->second.get();
    for(auto inputEdge : node->m_inEdges)
    {
        if(auto ptr = inputEdge.lock())
        {
            if(!visitNode(ptr->m_outPoint.nodeId, visiting, visited))
            {
                return false;
            }
        }
    }

    visiting.erase(nodeId);
    visited.push_back(nodeId);

    return true;
}


