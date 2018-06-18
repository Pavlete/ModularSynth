#include "modulemenu.h"

ModuleMenu::ModuleMenu(NodeModel& path)
    : m_rootElement("")
    , m_syntPath(path)
{
    setRootItem(&m_rootElement);
    setBounds(0,0,100,200);
    setRootItemVisible(false);
    setAlwaysOnTop(true);
}

void ModuleMenu::paint(Graphics &g)
{
    g.fillAll(Colours::yellow);
}

void ModuleMenu::Category::paintItem(Graphics &g, int w, int h)
{
    g.setColour(Colours::black);
    g.drawText(m_categoryName, 0, 0, w, h, Justification::left);
}

void ModuleMenu::Module::paintItem(Graphics &g, int w, int h)
{
    g.setColour(Colours::black);
    g.drawText(m_moduleName, 0, 0, w, h, Justification::left);
}

void ModuleMenu::Module::itemClicked(const MouseEvent&)
{
    auto menu = static_cast<ModuleMenu*>(getOwnerView());
    menu->m_syntPath.addNode(m_fact(), menu->getBoundsInParent().getPosition());
    menu->setVisible(false);
}
