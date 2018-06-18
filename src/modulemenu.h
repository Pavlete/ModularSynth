#pragma once

#include <JuceHeader.h>

#include "nodes/common/uiaudionode.h"
#include "nodes/common/nodemodel.h"

class ModuleMenu: public TreeView
{    
    using Factory = std::function<std::unique_ptr<UIAudioNode>()>;
    struct Category: public TreeViewItem
    {
        Category(std::string name): m_categoryName(name) {}
        virtual ~Category() = default;

        virtual bool mightContainSubItems() override { return true; }

        virtual void paintItem(Graphics &g, int w, int h) override;

        virtual bool canBeSelected() const override { return false; };

        std::string m_categoryName;
    };

    struct Module: public TreeViewItem
    {
        Module(std::string name, Factory fact)
            : m_moduleName(name)
            , m_fact(fact)
        {}
        virtual ~Module() = default;

        virtual bool mightContainSubItems() override { return false; }

        virtual void paintItem(Graphics &g, int w, int h) override;

        virtual bool canBeSelected() const override { return false; };

        virtual void itemClicked(const MouseEvent &e) override;

        std::string m_moduleName;
        Factory m_fact;

    };

public:
    ModuleMenu(NodeModel& flow);

    virtual void paint(Graphics &g) override;

    template<class ModuleClass>
    void addElement(std::string name, std::string category)
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

private:
    Category m_rootElement;
    NodeModel& m_syntPath;
};
