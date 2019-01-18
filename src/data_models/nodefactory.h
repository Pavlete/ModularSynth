#pragma once

#include <unordered_map>
#include <map>
#include <memory>

#include "node.h"

class NodeCatalog
{
    using NodeFactory = std::function< std::shared_ptr< Node >(const ValueTree& tree) >;
    using NodeModelMap = std::map< String, NodeFactory >;
    using CategoriesMap = std::map< String, String >;

public:
    NodeCatalog(const String& name,
                const String& category,
                const NodeFactory& treeFactory)
    {
        audioNodeMap().insert({name, treeFactory});
        categoriesMap().insert({category, name});
    }

    static std::shared_ptr< Node > getNode(const ValueTree& tree)
    {
        auto item = audioNodeMap().find(tree.getType().toString());
        return item == audioNodeMap().end()? nullptr : item->second(tree);
    }

    static const CategoriesMap& categories()
    {
        return categoriesMap();
    }

private:
    static NodeModelMap& audioNodeMap()
    {
        static NodeModelMap funcmap;
        return funcmap;
    }

    static CategoriesMap& categoriesMap()
    {
        static CategoriesMap funcmap;
        return funcmap;
    }
};

#define REGISTER_FACTORY( Category, Name ) \
    static NodeCatalog fact(#Name, #Category, \
                            [](const ValueTree& tree){return std::make_shared< Name##_Model >(tree);});

