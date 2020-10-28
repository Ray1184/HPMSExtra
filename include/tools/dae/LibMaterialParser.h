/*!
 * File LibMaterialParser.h
 */

#pragma once

#include <unordered_map>
#include <glm/glm.hpp>
#include <pugixml.hpp>



namespace hpms
{
    struct EffectData
    {
        glm::vec3 ambient;
        glm::vec3 emission;
        glm::vec3 diffuse;
        float transparency;
    };

    struct MaterialData
    {
        std::unordered_map<std::string&, MaterialData>& effectsByMeshId;
    };

    class LibMaterialParser
    {
        static void Process(const MaterialData& matData, pugi::xml_node& node);
    };
}
