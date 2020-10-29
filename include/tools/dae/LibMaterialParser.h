/*!
 * File LibMaterialParser.h
 */

#pragma once

#include <unordered_map>
#include <glm/glm.hpp>
#include <pugixml.hpp>



namespace hpms
{

    struct MaterialData
    {
        std::unordered_map<std::string, std::pair<std::string, std::string>> instanceAndNameIdByMatId;
    };

    class LibMaterialParser
    {
    public:
        static void Process(MaterialData& matData, pugi::xml_node& node);
    };
}
