#include "juceaudionode.h"
#include "node.h"
#include "synthmodel.h"

Node::Node(int x, int y, String type)
    :m_tree ( type )
{
    initCached();
    moduleX = x;
    moduleY = y;
}

Node::Node(const Node &mod)
{
    m_tree = mod.m_tree;
    initCached();
}

void Node::addListener(Node::Listener *listener)
{
    m_tree.addListener(listener);
}

bool Node::isInitModule()
{
    return m_tree.getProperty(IDs::IsInitNode, false);
}

int Node::initPort()
{
    return m_tree.getProperty(IDs::InitPort, false);
}

Node::Node(const ValueTree& module)
    : m_tree (module)
{
    initCached();
}

SynthModel Node::getParentSynth()
{
    return SynthModel(m_tree.getParent().getParent());
}

void Node::initCached()
{
    moduleX.referTo(m_tree, "x", nullptr);
    moduleY.referTo(m_tree, "y", nullptr);
    moduleId.referTo(m_tree, IDs::NodeID, nullptr);
}

