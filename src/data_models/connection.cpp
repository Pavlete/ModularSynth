#include "connection.h"

#include "synthmodel.h"


int Connection::outputNode() const
{
    return m_tree.getProperty(IDs::OutId);
}

unsigned int Connection::outputPort() const
{
    return static_cast<int>(m_tree.getProperty(IDs::OutPort));
}

int Connection::inputNode() const
{
    return m_tree.getProperty(IDs::InId);
}

unsigned int Connection::inputPort() const
{
    return static_cast<int>(m_tree.getProperty(IDs::InPort));
}

SynthModel Connection::getParentSynth()
{
    return SynthModel(m_tree.getParent().getParent());
}

Connection::Connection(ValueTree& connectionTree) :
    m_tree(connectionTree)
{ }
