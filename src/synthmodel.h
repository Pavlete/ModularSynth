#pragma once

#include <JuceHeader.h>

namespace
{
static const String NodeType =  "node";
static const String ConnectionType =  "connection";
static const String SynthType =  "synth";
}

class SynthModel;

struct Connection
{
    Connection(int outID, uint32_t outPort, int inID, uint32_t inPort);
    Connection(const Connection& mod);

    CachedValue<int> connectionOutID;
    CachedValue<int> connectionOutPort;
    CachedValue<int> connectionInID;
    CachedValue<int> connectionInPort;

    SynthModel getParentSynth();

private:
    Connection(ValueTree& module);
    ValueTree m_tree{ConnectionType};

    void initCached();

    friend class SynthModel;
};

struct Node
{
    Node(int x, int y, String type);
    Node(const Node& mod);

    CachedValue<int> moduleX;
    CachedValue<int> moduleY;
    CachedValue<String> moduleType;
    CachedValue<int> moduleId;

    SynthModel getParentSynth();

private:
    Node(ValueTree& module);
    ValueTree m_tree{NodeType};

    void initCached();

    friend class SynthModel;
};

class SynthModel
{
public:
    class Listener : public ValueTree::Listener
    {
    public:
        virtual void nodeAdded(Node) {}
        virtual void nodeRemoved(Node) {}

        virtual void connectionAdded(Connection) {}
        virtual void connectionRemoved(Connection) {}

    private:
        void valueTreePropertyChanged (ValueTree&, const Identifier&) override;

        void valueTreeChildAdded (ValueTree &parent, ValueTree &added) override;

        void valueTreeChildRemoved (ValueTree &, ValueTree &, int) override;

        void valueTreeChildOrderChanged (ValueTree &, int, int ) override;

        void valueTreeParentChanged  (ValueTree &) override;
    };

    SynthModel() = default;
    SynthModel(ValueTree synth);

    void addListener(Listener* listener);

    void addModule(Node node);
    void addConnection(Connection connection);

    void removeConnection(Connection connection);


    void saveToFile(File fileName);
    void loadFromFile(File fileName);

private:
    ValueTree m_tree {SynthType};

    int getNextIndex();

    bool treeHasIndex(int index);
};
