#include "audionode.h"
#include "processgraph.h"

ProcessGraph::ProcessGraph()
    : m_outEdge(std::make_shared<Edge>())
    , m_currentPath(std::make_shared<AudioNodeVector>())
{ }

void ProcessGraph::setNodesOn(float frequency, float velocity)
{
    std::for_each(m_currentPath->begin(), m_currentPath->end(), [frequency, velocity](auto& node)
    {
        node->setActive(frequency, velocity);
    });
}

void ProcessGraph::setNodesOff()
{
    std::for_each(m_currentPath->begin(), m_currentPath->end(),[](auto& node)
    {
        node->setInactive();
    });
}

bool ProcessGraph::isActive() const
{
    auto element = std::find_if(m_currentPath->begin(), m_currentPath->end(), [](auto& node)
    {
        return node->isActive();
    });

    return element != m_currentPath->end();
}

bool ProcessGraph::addNode(int nodeID, std::unique_ptr<AudioNode> node){
    auto result = m_nodes.emplace(nodeID, std::move(node));
    return result.second;
}

bool ProcessGraph::removeNode(int nodeId)
{
    //TODO: check if node is connected and return false in case it is
    return m_nodes.erase( nodeId ) != 0;
}

bool ProcessGraph::addConnection(const ConnectionPoint &outputPoint, const ConnectionPoint &inputPoint)
{
    if(!m_nodes.at(outputPoint.nodeId)->isAvailable(outputPoint.portNumber, PointDirection::Output) ||
            !m_nodes.at(outputPoint.nodeId)->isAvailable(outputPoint.portNumber, PointDirection::Output) )
    {
        return false;
    }

    auto edge = std::make_shared<Edge>(outputPoint,
                                       inputPoint);

    AudioNode* outNode = m_nodes.find(outputPoint.nodeId)->second.get();
    outNode->m_outEdges[outputPoint.portNumber] = edge;

    AudioNode* inNode = m_nodes.find(inputPoint.nodeId)->second.get();
    inNode->m_inEdges[inputPoint.portNumber] = edge;   

    return updatePath();
}

bool ProcessGraph::removeConnection(const ConnectionPoint &outputPoint, const ConnectionPoint &inputPoint)
{
    auto& outEdge = m_nodes.at(outputPoint.nodeId)->m_outEdges.at(outputPoint.portNumber);
    auto& inEdge = m_nodes.at(inputPoint.nodeId)->m_inEdges.at(inputPoint.portNumber);

    if(!outEdge || !inEdge || outEdge != inEdge)
    {
        return false;
    }

    outEdge.reset();
    inEdge.reset();

    return updatePath();
}

bool ProcessGraph::setInitNode(const ConnectionPoint &newInit)
{
    if(!m_nodes.at(newInit.nodeId)->isAvailable(newInit.portNumber,
                                                PointDirection::Output))
    {
        return false;
    }

    m_outEdge->m_inPoint = newInit;

    auto outNode = m_nodes.find(newInit.nodeId)->second.get();
    outNode->m_outEdges[newInit.portNumber] = m_outEdge;

    return updatePath();
}

void ProcessGraph::proccessData(AudioBufferWrapper& outData)
{
    m_outEdge->setMyOwnData(&outData);
    auto syncPath = std::atomic_load(&m_currentPath);
    for_each(syncPath->begin(), syncPath->end(), [] (auto& node){ node->process();});
}

bool ProcessGraph::updatePath()
{
    if( m_outEdge->m_inPoint.nodeId == INVALID_NODE_ID)
    {
        return false;
    }

    std::set<int> visiting;
    std::vector<int> visited;
    if (!visitNode(m_outEdge->m_inPoint.nodeId, visiting, visited))
    {
        return false;
    }

    auto newPath = std::make_shared<AudioNodeVector>();
    std::transform(visited.begin(), visited.end(), std::back_inserter(*newPath),
                   [&](auto id) {return m_nodes[id]->clone();});
    std::atomic_store(&m_currentPath, newPath);

    return true;
}

bool ProcessGraph::visitNode(int nodeId, std::set<int> &visiting, std::vector<int>& visited)
{
    if(visiting.find(nodeId) != visiting.end())
    {
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
        if(inputEdge)
        {
            if(!visitNode(inputEdge->m_outPoint.nodeId, visiting, visited))
            {
                return false;
            }
        }
    }

    visiting.erase(nodeId);
    visited.push_back(nodeId);

    return true;
}


