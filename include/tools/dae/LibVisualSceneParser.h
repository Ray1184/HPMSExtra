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

    struct SkelTreeData
    {
        std::string boneId;
        std::string boneSid;
        std::string boneName;
        std::vector<SkelTreeData> children;
    };

    struct VisualData
    {
        SkelTreeData skelData;
        std::unordered_map<std::string, std::string> matIdByMeshId;
        std::unordered_map<std::string, std::string> meshIdByMatId;
    };

    class LibSkeletonParser
    {

    public:
        static void Process(const VisualData& visualScene, pugi::xml_node& node);

    };
}
