#include "synthmodel.h"

int Node::x() const
{
    return m_tree.getProperty(IDs::X);
}

int Node::y() const
{
    return m_tree.getProperty(IDs::Y);
}


int Node::ID() const
{
    return m_tree.getProperty(IDs::NodeID);
}

void Node::setX(int x)
{
    m_tree.setProperty(IDs::X, x, nullptr);
}

void Node::setY(int y)
{
    m_tree.setProperty(IDs::Y, y, nullptr);
}

void Node::addListener(Node::Listener *listener)
{
    m_tree.addListener(listener);
}

bool Node::isInitModule() const
{
    return m_tree.getProperty(IDs::IsInitNode, false);
}

int Node::initPort() const
{
    return m_tree.getProperty(IDs::InitPort, false);
}

Node::Node(const ValueTree& module)
    : m_tree (module)
{ }

SynthModel Node::getParentSynth()
{
    return SynthModel(m_tree.getParent().getParent());
}
