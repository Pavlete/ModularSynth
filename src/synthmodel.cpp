#include <random>

#include "synthmodel.h"

namespace
{
static int getRandom()
{
    static std::mt19937 rng;
    static std::uniform_int_distribution<int> randomDistribution(1,100000);
    return randomDistribution(rng);
}
}

Connection::Connection(int outID, uint32 outPort, int inID, uint32 inPort)
{
    initCached();
    connectionOutID = outID;
    connectionOutPort = outPort;
    connectionInID = inID;
    connectionInPort = inPort;
}

Connection::Connection(const Connection &mod)
    : m_tree (mod.m_tree)
{
    m_tree = mod.m_tree;
    initCached();
}

SynthModel Connection::getParentSynth()
{
    return SynthModel(m_tree.getParent());
}

Connection::Connection(ValueTree& module)
    : m_tree (module)
{
    initCached();
}

void Connection::initCached()
{
    connectionOutID.referTo(m_tree, "outID", nullptr);
    connectionOutPort.referTo(m_tree, "outPort", nullptr);
    connectionInID.referTo(m_tree, "inID", nullptr);
    connectionInPort.referTo(m_tree, "inPort", nullptr);
}



//--------------------------------------------------------//



Node::Node(int x, int y, String type)
{
    initCached();
    moduleX = x;
    moduleY = y;
    moduleType = type;
}

Node::Node(const Node &mod)
{
    m_tree = mod.m_tree;
    initCached();
}

Node::Node(ValueTree& module)
{
    m_tree = module;
    initCached();
}

SynthModel Node::getParentSynth()
{
    return SynthModel(m_tree.getParent());
}

void Node::initCached()
{
    moduleX.referTo(m_tree, "x", nullptr);
    moduleY.referTo(m_tree, "y", nullptr);
    moduleType.referTo(m_tree, "type", nullptr);
    moduleId.referTo(m_tree, "id", nullptr);
}



//--------------------------------------------------------//



void SynthModel::Listener::valueTreePropertyChanged(ValueTree &, const Identifier &)
{}

void SynthModel::Listener::valueTreeChildAdded(ValueTree &, ValueTree &added)
{
    if(added.hasType(NodeType))
    {
        nodeAdded({added});
    }
    else if (added.hasType(ConnectionType))
    {
        connectionAdded({added});
    }
}

void SynthModel::Listener::valueTreeChildRemoved(ValueTree &, ValueTree &removed, int)
{
    if(removed.hasType(NodeType))
    {
        nodeRemoved({removed});
    }
    else if (removed.hasType(ConnectionType))
    {
        connectionRemoved({removed});
    }
}

void SynthModel::Listener::valueTreeChildOrderChanged(ValueTree &, int, int)
{
}

void SynthModel::Listener::valueTreeParentChanged(ValueTree &)
{
}


SynthModel::SynthModel(ValueTree synth)
{
    if(synth.hasType(SynthType))
    {
        m_tree = synth;
    }
}

void SynthModel::addListener(SynthModel::Listener *listener)
{
    m_tree.addListener(listener);
}

void SynthModel::addModule(Node module)
{
    module.moduleId = getNextIndex();
    m_tree.addChild(module.m_tree, -1, nullptr);
}

void SynthModel::addConnection(Connection connection)
{
    m_tree.addChild(connection.m_tree, -1, nullptr);
}

void SynthModel::removeConnection(Connection connection)
{
    m_tree.removeChild(connection.m_tree, nullptr);
}

void SynthModel::saveToFile(File fileName)
{
    const TemporaryFile temp (fileName);
    {
        FileOutputStream os (temp.getFile());

        if (! os.getStatus().wasOk())
        {
            return;
        }
        if (auto xml = std::unique_ptr<juce::XmlElement> (m_tree.createXml()))
            xml->writeToStream (os, juce::StringRef());
    }

    if (temp.getFile().existsAsFile())
    {
        temp.overwriteTargetFileWithTemporary();
    }
}

void SynthModel::loadFromFile(File fileName)
{
    FileInputStream is (fileName);
    ValueTree vt;

    if (auto xml = std::unique_ptr<juce::XmlElement> (XmlDocument::parse (fileName)))
        vt = juce::ValueTree::fromXml (*xml);

    if(vt.hasType(SynthType))
    {
        m_tree.removeAllChildren(nullptr);
        while(vt.getNumChildren() > 0)
        {
            ValueTree v = *vt.begin();
            vt.removeChild(v, nullptr);
            m_tree.addChild(v, -1, nullptr);
        }
    }
}

int SynthModel::getNextIndex()
{
    int ret = 0;
    do{
        ret = getRandom();
    } while(treeHasIndex(ret));

    return ret;
}

bool SynthModel::treeHasIndex(int index)
{
    for(auto tree : m_tree)
    {
        if(tree.hasType(NodeType) && Node(tree).moduleId == index)
        {
            return true;
        }
    }
    return false;
}

