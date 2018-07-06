#pragma once

#include "audioprocessgraph.h"

template <class DataType>
class AudioGraphObservable
{
    using NodeType = typename processGraph::AudioProcessGraph<DataType>::AudioNode;

public:
    using GraphFactory = std::function<std::unique_ptr<NodeType>()>;

    virtual ~AudioGraphObservable() = default;

    void registerGraph(processGraph::AudioProcessGraph<DataType>* modulable)
    {
        if (modulable)
        {
            m_observers.push_back(modulable);
        }
    }

    void unregisterGraph(processGraph::AudioProcessGraph<DataType>* modulable)
    {
        if (!modulable)
        {
            return;
        }
        auto found = std::find(m_observers.begin(), m_observers.end(), modulable);
        if(found != m_observers.end())
        {
            m_observers.erase(found);
        }
    }

    int addModule(GraphFactory factory)
    {
        int i = 0;
        for (auto modulable : m_observers)
            i = modulable->addNode(factory());
        return i;
    }

    void removeModule(int id)
    {
        for (auto modulable : m_observers)
            modulable->removeModule(id);
    }

    bool addConnection(const processGraph::ConnectionPoint& out,
                        const processGraph::ConnectionPoint& in)
    {
        bool ret = true;
        for (auto modulable : m_observers)
            ret &= modulable->addConnection(out, in);
        return ret;
    }

    bool removeConnection(const processGraph::ConnectionPoint& out,
                           const processGraph::ConnectionPoint& in)
    {
        bool ret = true;
        for (auto modulable : m_observers)
            ret &= modulable->removeConnection(out, in);
        return ret;
    }

    void setInitialModule(int moduleId, int out)
    {
        for (auto modulable : m_observers)
            modulable->setInitNode({moduleId,out});
    }

    void unsetInitialModule( )
    {
        for (auto modulable : m_observers)
            modulable->unsetInitialModule( );
    }

private:
    std::vector<processGraph::AudioProcessGraph<DataType>*> m_observers;
};
