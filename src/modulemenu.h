#pragma once

#include <JuceHeader.h>

#include "synthmodel.h"
#include "nodes/common/juceaudionode.h"


class ModuleMenu: public TreeView
{
    struct Category: public TreeViewItem
    {
        Category(std::string name): m_categoryName(name) {}

        bool mightContainSubItems() override { return true; }

        void paintItem(Graphics &g, int w, int h) override;

        bool canBeSelected() const override { return false; }

        const std::string m_categoryName;
    };

    struct Module: public TreeViewItem
    {
        Module(std::string name, SynthModel path)
            : m_moduleName(name)
            , m_synthPath(path)
        {}

        bool mightContainSubItems() override { return false; }

        void paintItem(Graphics &g, int w, int h) override;

        bool canBeSelected() const override { return false; }

        void itemClicked(const MouseEvent &e) override;

        const std::string m_moduleName;
        SynthModel m_synthPath;
    };

public:
    ModuleMenu(SynthModel flow);

    void paint(Graphics &g) override;

    template<class ModuleClass>
    void addElement(std::string name, std::string category);

private:
    Category m_rootElement;
    SynthModel m_path;
};



//---------------------------------------------------------------------------------//



template<class ModuleClass>
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
