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
        getAudioNodeMap().insert({name, treeFactory});
        getCategoriesMap().insert({category, name});
    }

    static std::shared_ptr< Node > getNode(const ValueTree& tree)
    {
        auto map = getAudioNodeMap();
        auto item = map.find(tree.getType().toString());
        return item == map.end()? nullptr : item->second(tree);
    }
    static const CategoriesMap& getCategories()
    {
        return getCategoriesMap();
    }

private:
    static NodeModelMap& getAudioNodeMap()
    {
        static NodeModelMap funcmap;
        return funcmap;
    }
    static CategoriesMap& getCategoriesMap()
    {
        static CategoriesMap funcmap;
        return funcmap;
    }
};

#define REGISTER_FACTORY( Category, Name ) \
    static NodeCatalog fact(#Name, #Category, \
                            [](const ValueTree& tree){return std::make_shared< Name##_Model >(tree);});

