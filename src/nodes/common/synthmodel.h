#pragma once

#include <JuceHeader.h>

#include "connection.h"
#include "identifiers.h"
#include "node.h"


using SharedNode = std::shared_ptr<Node>;

class SynthModel
{
public:
    class Listener : public ValueTree::Listener
    {
    public:
        virtual void nodeAdded(const SharedNode&) {}
        virtual void nodeRemoved(const SharedNode&) {}

        virtual void connectionAdded(const Connection&) {}
        virtual void connectionRemoved(const Connection&) {}

    private:
        void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
        void valueTreeChildAdded (ValueTree &parent, ValueTree &added) override;
        void valueTreeChildRemoved (ValueTree &, ValueTree &, int) override;
        void valueTreeChildOrderChanged (ValueTree &, int, int ) override {}
        void valueTreeParentChanged  (ValueTree &) override {}
    };

    SynthModel();

    void addListener(Listener* listener);

    void addModule(int x, int y, String module);
    void addConnection(int outID, int outPort, int inID, int inPort);

    void removeConnection(Connection& connection);

    void save();
    void restore();
private:
    SynthModel(ValueTree synth);

    ValueTree m_tree;
    ValueTree getNodesTree();
    ValueTree getConnectionTree();

    ValueTree generateConnectionTree(int outID, int outPort, int inID, int inPort);
    ValueTree basicNodeTree(int x, int y, String type);

    int getNextIndex();

    bool treeHasIndex(int index);
    static ValueTree loadFromFile(File fileName);
};
