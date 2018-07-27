#include "modulemenu.h"

ModuleMenu::ModuleMenu(const SynthModel& path)
    : m_rootElement("")
    , m_path(path)
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
    auto pos = menu->getBoundsInParent().getPosition();
    menu->setVisible(false);

    m_synthPath.addModule(pos.getX(), pos.getY(), m_moduleName);
}

void ModuleMenu::addElement(std::string name, std::string category)
{
    auto root = getRootItem();
    Category* categoryItem = nullptr;
    for(int i = 0; i < root->getNumSubItems(); i++)
    {
        auto item = dynamic_cast<Category*>(root->getSubItem(i));
        if(item && item->m_categoryName == category)
        {
            categoryItem = item;
            break;
        }
    }

    if(!categoryItem)
    {
        categoryItem = new Category(category);
        root->addSubItem(categoryItem);
    }

    categoryItem->addSubItem(new Module(name, m_path));
}
