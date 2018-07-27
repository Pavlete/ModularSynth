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

static const String FileName = "Hola.xml";
}


void SynthModel::Listener::valueTreePropertyChanged(ValueTree &, const Identifier &)
{}

void SynthModel::Listener::valueTreeChildAdded(ValueTree &parent, ValueTree &added)
{
    if (parent.hasType(IDs::Connections))
    {
        connectionAdded({added});
    }
    else if (parent.hasType(IDs::Nodes))
    {
        auto node = NodeFactory::getNodeTreeFactory(added.getType().toString().toStdString())(added);
        nodeAdded(node);
    }    
}

void SynthModel::Listener::valueTreeChildRemoved(ValueTree &parent, ValueTree &removed, int)
{    
    if (parent.hasType(IDs::Connections))
    {
        connectionRemoved({removed});
    }
    else if (parent.hasType(IDs::Nodes))
    {
        //nodeRemoved({removed});
    }
}

SynthModel::SynthModel()
    : m_tree(IDs::SynthType)
    , isValid(false)
{
    ValueTree nodes(IDs::Nodes);
    ValueTree connections(IDs::Connections);
    m_tree.addChild(nodes, IDs::NodesIndex, nullptr);
    m_tree.addChild(connections, IDs::ConnectionsIndex, nullptr);

    isValid = true;
}

SynthModel::SynthModel(ValueTree synth)
    : isValid(false)
{
    if(synth.hasType(IDs::SynthType))
    {
        m_tree = synth;
        isValid = true;
    }
}

void SynthModel::addListener(SynthModel::Listener *listener)
{
    m_tree.addListener(listener);
}

void SynthModel::addModule(int x, int y, std::string name)
{    
    auto module = NodeFactory::getNodeCoordsFactory(name)(x,y);
    module->moduleId = getNextIndex();
    getNodesTree().addChild(module->m_tree, -1, nullptr);
}

void SynthModel::addConnection(int outID, int outPort, int inID, int inPort)
{
    auto tree = generateConnectionTree(outID, outPort, inID, inPort);
    getConnectionTree().addChild(tree, -1, nullptr);
}

void SynthModel::removeConnection(Connection& connection)
{
    getConnectionTree().removeChild(connection.m_tree, nullptr);
}

void SynthModel::save()
{
    File file(File::getSpecialLocation(File::userHomeDirectory).getChildFile(FileName));
    const TemporaryFile temp (file);
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

void SynthModel::restore()
{
    File file(File::getSpecialLocation(File::userHomeDirectory).getChildFile(FileName));
    if(!file.exists())
    {
        return;
    }

    SynthModel otherSynth( SynthModel::loadFromFile(file) );
    if(otherSynth.isValid)
    {
        auto nodes = otherSynth.getNodesTree();
        while(nodes.getNumChildren() > 0)
        {
            ValueTree v = *nodes.begin();
            nodes.removeChild(v, nullptr);
            getNodesTree().addChild(v, -1, nullptr);
        }

        auto connections = otherSynth.getConnectionTree();
        while(connections.getNumChildren() > 0)
        {
            ValueTree v = *connections.begin();
            connections.removeChild(v, nullptr);
            getConnectionTree().addChild(v, -1, nullptr);
        }
    }
}

ValueTree SynthModel::getNodesTree()
{
    return m_tree.getChild(IDs::NodesIndex);
}

ValueTree SynthModel::getConnectionTree()
{
    return m_tree.getChild(IDs::ConnectionsIndex);
}

ValueTree SynthModel::generateConnectionTree(int outID, int outPort, int inID, int inPort)
{
    ValueTree tree(IDs::ConnectionType);
    tree.setProperty(IDs::OutId, outID, nullptr);
    tree.setProperty(IDs::OutPort, static_cast<int>(outPort), nullptr);
    tree.setProperty(IDs::InId, inID, nullptr);
    tree.setProperty(IDs::InPort, static_cast<int>(inPort), nullptr);
    return tree;
}

ValueTree SynthModel::loadFromFile(File fileName)
{
    FileInputStream is (fileName);
    ValueTree vt;

    if (auto xml = std::unique_ptr<juce::XmlElement> (XmlDocument::parse (fileName)))
        vt = juce::ValueTree::fromXml (*xml);
    return vt;
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
    for(auto node : getNodesTree())
    {
        if(static_cast<int>(node.getProperty(IDs::NodeID)) == index)
        {
            return true;
        }
    }
    return false;
}

