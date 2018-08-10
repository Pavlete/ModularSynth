#pragma once

#include <JuceHeader.h>

class SynthModel;

class ModuleMenu: public TreeView
{
    struct Category: public TreeViewItem
    {
        Category(String name): m_categoryName(name) {}

        bool mightContainSubItems() override { return true; }

        void paintItem(Graphics &g, int w, int h) override;

        bool canBeSelected() const override { return false; }

        const String m_categoryName;
    };

    struct Module: public TreeViewItem
    {
        Module(String name, SynthModel& path);

        bool mightContainSubItems() override { return false; }

        void paintItem(Graphics &g, int w, int h) override;

        bool canBeSelected() const override { return false; }

        void itemClicked(const MouseEvent &e) override;

        const String m_moduleName;
        SynthModel& m_synthPath;
    };

public:
    ModuleMenu(SynthModel& flow);

    void paint(Graphics &g) override;

    void addElement(String name, String category);

private:
    Category m_rootElement;
    SynthModel& m_path;
};



//---------------------------------------------------------------------------------//


