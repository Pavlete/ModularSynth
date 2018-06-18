#ifndef FLOWVIEW_H
#define FLOWVIEW_H

#include <JuceHeader.h>

#include "nodemodel.h"

class NodeView : public Component
{
public:
    NodeView(NodeModel& model)
    {
        model.m_nodeAdded = [&](juce::Component* comp, const Point<int>& p)
        {
            comp->setBounds(comp->getBounds() + p);
            this->addAndMakeVisible(comp);
        };
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NodeView)
};

#endif // FLOWVIEW_H
