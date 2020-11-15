/*!
 * File LibMaterialParser.h
 */

#pragma once

#include <unordered_map>
#include <glm/glm.hpp>
#include <pugixml.hpp>



namespace hpms
{

    struct MaterialDefData
    {
        std::unordered_map<std::string, std::pair<std::string, std::string>> instanceAndNameIdByMatId;
    };

    class LibMaterialParser
    {
    public:
        static void Process(MaterialDefData& matData, pugi::xml_node& node);
    };
}
