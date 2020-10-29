/*!
 * File LibMaterialParser.h
 */

#pragma once

#include <unordered_map>
#include <glm/glm.hpp>
#include <pugixml.hpp>



namespace hpms
{

    struct FxInstanceData
    {
        std::unordered_map<std::string, glm::vec4> effectByInstanceId;
    };

    class LibEffectsParser
    {
    public:
        static void Process(FxInstanceData& fxData, pugi::xml_node& node);
    };
}
