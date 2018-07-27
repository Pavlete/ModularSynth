#include "connection.h"

#include "synthmodel.h"

Connection::Connection(const Connection &mod)
    : m_tree (mod.m_tree)
{ }

int Connection::connectionOutID() const
{
    return m_tree.getProperty(IDs::OutId);
}

int Connection::connectionOutPort() const
{
    return m_tree.getProperty(IDs::OutPort);
}

int Connection::connectionInID() const
{
    return m_tree.getProperty(IDs::InId);
}

int Connection::connectionInPort() const
{
    return m_tree.getProperty(IDs::InPort);
}

SynthModel Connection::getParentSynth()
{
    return SynthModel(m_tree.getParent().getParent());
}

Connection::Connection(ValueTree& connectionTree) :
    m_tree(connectionTree)
{ }
