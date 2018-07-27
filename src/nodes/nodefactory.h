#pragma once

#include <unordered_map>
#include <map>
#include <memory>

#include "common/node.h"

class NodeFactory
{
    using NodeFactoryFromCoords = std::function< std::shared_ptr< Node >(int x, int y) >;
    using TreeNodeFactoryFromTree = std::function< std::shared_ptr< Node >(const ValueTree& tree) >;
    using NodeModelMap = std::map< std::string, std::pair<NodeFactoryFromCoords, TreeNodeFactoryFromTree> >;
    using CategoriesMap = std::map< std::string, std::string >;

public:
    NodeFactory(const std::string& name,
                const std::string& category,
                const NodeFactoryFromCoords& nodeFactory,
                const TreeNodeFactoryFromTree& treeFactory)
    {
        getAudioNodeMap().insert({name, {nodeFactory, treeFactory}});
        getCategoriesMap().insert({category, name});
    }

    static NodeFactoryFromCoords getNodeCoordsFactory(std::string name)
    {
        auto& map = getAudioNodeMap();
        auto item = map.find(name);
        if (item == map.end())
        {
            return nullptr;
        }

        return item->second.first;
    }

    static TreeNodeFactoryFromTree getNodeTreeFactory(std::string name)
    {
        auto& map = getAudioNodeMap();
        auto item = map.find(name);
        if (item == map.end())
        {
            return nullptr;
        }

        return item->second.second;
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
    static NodeFactory fact(#Name, #Category, \
                            [](int x, int y){return std::make_shared< Name##_Model >(x,y);},\
                            [](const ValueTree& tree){return std::make_shared< Name##_Model >(tree);});
