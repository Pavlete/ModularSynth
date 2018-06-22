#pragma once

#include "audioprocessgraph.h"

template <class T>
class AudioGraphObservable
{
    using NodeType = typename processGraph::AudioProcessGraph<T>::AudioNode;

public:
    using GraphFactory = std::function<std::unique_ptr<NodeType>()>;

    virtual ~AudioGraphObservable() = default;

    void registerGraph(processGraph::AudioProcessGraph<T>* modulable)
    {
        if (modulable != nullptr)
            m_observers.push_back(modulable);
    }

    int addModule( GraphFactory factory)
    {
        int i = 0;
        for (auto modulable : m_observers)
            i = modulable->addNode( factory());
        return i;
    }

    void removeModule(int id)
    {
        for (auto modulable : m_observers)
            modulable->removeModule(id);
    }

    bool addConnection(int outModuleId, int moduleOutput,
                       int inModuleId, int moduleInput)
    {
        bool ret = true;
        for (auto modulable : m_observers)
            ret &= modulable->addConnection({outModuleId, moduleOutput },
                                            {inModuleId, moduleInput});
        return ret;
    }

    bool removeConnection(int outModuleId, int moduleOutput,
                          int inModuleId, int moduleInput)
    {
        bool ret;
        for (auto modulable : m_observers)
            ret &= modulable->removeConnection({outModuleId, moduleOutput },
                                               {inModuleId, moduleInput});
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
    std::vector<processGraph::AudioProcessGraph<T>*> m_observers;
};
