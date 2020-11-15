/*!
 * File LibVisualSceneParser.h
 */

#pragma once

#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include <pugixml.hpp>

namespace hpms
{


    struct VisualData
    {
        bool jointType;
        std::string boneId;
        std::string boneSid;
        std::string boneName;
        std::unordered_map<std::string, std::string> matIdByMeshId;
        std::vector<VisualData> children;
    };

    class LibVisualSceneParser
    {

    public:
        static void Process(VisualData& visualScene, pugi::xml_node& node);

        static void ProcessRecursive(VisualData* data, pugi::xml_node& node);

        static void ProcessMaterialStructure(hpms::VisualData& rootData, VisualData* pData, pugi::xml_node& node);
    };
}
