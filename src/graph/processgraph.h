#pragma once
#include <iostream>


#include <algorithm>
#include <atomic>
#include <memory>
#include <set>
#include <unordered_map>
#include <vector>

namespace processGraph
{

static const int INVALID_NODE_ID = -1;

struct ConnectionPoint
{
    int32_t nodeId;
    uint32_t portNumber;

    bool operator==(const ConnectionPoint& other)
    {
        return nodeId == other.nodeId &&
               portNumber == other.portNumber;
    }
};

template <class DataType>
class ProcessGraph
{
    enum class Status
    {
        NO_CHANGE,
        CHANGING,
        CHANGED
    };

    enum class PointDirection
    {
        Input,
        Output
    };

    class Edge;

public:
    class Node;

    int32_t addNode(std::unique_ptr<Node> node){
        auto result = m_nodes.emplace(++m_currentId, std::move(node));
        return result.second? result.first->first : INVALID_NODE_ID;
    }

    bool removeNode(int nodeId)
    {
        //TODO: Check if current path is affected before deleting and
        //      take care of non deleting anything asynchronously while
        //      the process thread could be using the node
        return m_nodes.erase( nodeId ) != 0;
    }

    bool addConnection(const ConnectionPoint& outputPoint,
                       const ConnectionPoint& inputPoint)
    {
        if(!connectionPointValid(outputPoint, PointDirection::Output) ||
           hasConnection(outputPoint, PointDirection::Output) ||
           !connectionPointValid(inputPoint, PointDirection::Input) ||
           hasConnection(inputPoint, PointDirection::Input))
        {
            return false;
        }

        auto edge = std::make_shared<Edge>(outputPoint,
                                           inputPoint);

        Node* outNode = m_nodes.find(outputPoint.nodeId)->second.get();
        outNode->m_outEdges[outputPoint.portNumber] = edge;

        Node* inNode = m_nodes.find(inputPoint.nodeId)->second.get();
        inNode->m_inEdges[inputPoint.portNumber] = edge;

        m_edges.push_back(edge);

        return updatePath();
    }

    Node* getNode(int nodeId) const
    {
        auto nodeElement = m_nodes.find(nodeId);
        if(nodeElement == m_nodes.end())
        {
            return nullptr;
        }
        return nodeElement->second.get();
    }

    bool removeConnection(const ConnectionPoint& outputPoint,
                          const ConnectionPoint& inputPoint)
    {
        if(!connectionPointValid(outputPoint, PointDirection::Output) ||
           !hasConnection(outputPoint, PointDirection::Output) ||
           !connectionPointValid(inputPoint, PointDirection::Input) ||
           !hasConnection(inputPoint, PointDirection::Input))
        {
            return false;
        }

        auto edge = std::find_if(m_edges.begin(), m_edges.end(), [&](const std::shared_ptr<Edge>& n)
        {
            return n->m_outPoint.nodeId == outputPoint.nodeId &&
                   n->m_outPoint.portNumber == outputPoint.portNumber &&
                   n->m_inPoint.nodeId == inputPoint.nodeId &&
                   n->m_inPoint.portNumber == inputPoint.portNumber;
        });

        if(edge == m_edges.end())
        {
            return false;
        }

        m_edges.erase(edge);        

        return updatePath();
    }

    bool setInitNode(const ConnectionPoint& outputPoint)
    {
        if(!connectionPointValid(outputPoint, PointDirection::Output))
        {
            return false;
        }

        m_outEdge->m_inPoint = outputPoint;

        Node* outNode = m_nodes.find(outputPoint.nodeId)->second.get();
        outNode->m_outEdges[outputPoint.portNumber] = m_outEdge;

        return updatePath();
    }

    void proccessData(DataType& outData)
    {
        if(m_status == Status::CHANGED)
        {
            updateSynchronus();
        }

        m_outEdge->setMyOwnData(&outData);
        for(auto& element : m_currentPath)
        {
            m_nodes[element]->process();
        }
    }

private:
    bool updatePath()
    {
        if( m_outEdge->m_inPoint.nodeId == INVALID_NODE_ID)
        {
            return false;
        }

        m_status = Status::CHANGING;

        m_tempPath.clear();
        std::set<int> visitingSet;
        auto result = visitNode(m_outEdge->m_inPoint.nodeId, visitingSet, m_tempPath);

        m_status = Status::CHANGED;

        return result;
    }

    bool visitNode(int nodeId,
                   std::set<int>& visiting,
                   std::vector<int>& visited)
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

    bool connectionPointValid(const ConnectionPoint& point,
                              PointDirection direction) const
    {
        auto nodeElement = m_nodes.find(point.nodeId);
        if(nodeElement == m_nodes.end() )
        {
            return false;
        }

        auto node = nodeElement->second.get();
        auto edges = direction == PointDirection::Input?
                    node->m_inEdges : node->m_outEdges;

        return point.portNumber < edges.size();
    }

    bool hasConnection(const ConnectionPoint& point,
                       PointDirection direction) const
    {
        auto node = m_nodes.find(point.nodeId)->second.get();
        auto edges = direction == PointDirection::Input?
                    node->m_inEdges : node->m_outEdges;

        return !edges[point.portNumber].expired();
    }

    void updateSynchronus()
    {
        m_currentPath = std::move(m_tempPath);        
        m_status = Status::NO_CHANGE;
    }

    std::unordered_map<int, std::unique_ptr<Node>> m_nodes;
    std::vector<std::shared_ptr<Edge>> m_edges;

    std::vector<int> m_currentPath;
    std::vector<int> m_tempPath;

    std::atomic<Status> m_status;

    std::shared_ptr<Edge> m_outEdge = std::make_shared<Edge>();

    int m_currentId = 0;
};

template <class T>
class ProcessGraph<T>::Edge
{
    T* getData() { return m_outsideData?  m_outsideData : &m_sharedData; }
    void setMyOwnData(T* data) { m_outsideData = data; }

    T m_sharedData;
    T* m_outsideData = nullptr;

    ConnectionPoint m_outPoint;
    ConnectionPoint m_inPoint;

    friend class ProcessGraph<T>;
    friend class ProcessGraph<T>::Node;

public:
    Edge(ConnectionPoint outPoint = {INVALID_NODE_ID, 0},
         ConnectionPoint inPoint = {INVALID_NODE_ID, 0})
        : m_outPoint (outPoint)
        , m_inPoint (inPoint)
    {}
};

template <class T>
class ProcessGraph<T>::Node
{
public:
    Node(uint32_t numIn, uint32_t numOut)
        : m_inEdges(numIn)
        , m_outEdges(numOut)
    {}

    ~Node() {}

    virtual void process() = 0;

protected:
    T* getInputData(uint32_t index)
    {
        if(m_inEdges.size() < index)
        {
            return nullptr;
        }

        auto ptr = m_inEdges[index].lock();
        if(!ptr)
        {
            return nullptr;
        }

        return ptr->getData();
    }

    T* getOutputData(uint32_t index)
    {
        if(m_outEdges.size() < index)
        {
            return nullptr;
        }

        auto ptr = m_outEdges[index].lock();
        if(!ptr)
        {
            return nullptr;
        }

        return ptr->getData();
    }

private:
    std::vector<std::weak_ptr<Edge>> m_inEdges;
    std::vector<std::weak_ptr<Edge>> m_outEdges;

    friend class ProcessGraph<T>;
};

} //namespace processGraph
