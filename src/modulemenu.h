#pragma once

#include <JuceHeader.h>

#include "nodes/common/juceaudionode.h"


class ModuleMenu: public TreeView
{    
    using Factory = std::function<std::unique_ptr<JuceAudioNode>()>;
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
        Module(std::string name, Factory fact)
            : m_moduleName(name)
            , m_fact(fact)
        {}

        bool mightContainSubItems() override { return false; }

        void paintItem(Graphics &g, int w, int h) override;

        bool canBeSelected() const override { return false; }

        void itemClicked(const MouseEvent &e) override;

        const std::string m_moduleName;
        const Factory m_fact;
    };

public:
    ModuleMenu(JuceGraphModel& flow);

    void paint(Graphics &g) override;

    template<class ModuleClass>
    void addElement(std::string name, std::string category);

private:
    Category m_rootElement;
    JuceGraphModel& m_syntPath;
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

    categoryItem->addSubItem(new Module(name, [&](){return std::make_unique<ModuleClass>(m_syntPath);}));
}
