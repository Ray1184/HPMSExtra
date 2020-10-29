/*!
 * File LibSkinningParser.h
 */

#pragma once

#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include <pugixml.hpp>

namespace hpms
{


    struct SkinningData
    {
        std::unordered_map<std::string, glm::mat4x4> poseMatrixByBoneSid;
        std::unordered_map<int, std::vector<std::pair<glm::mat4x4, float>>> weightAndTransfMatrixInfluencesByVertexId;
    };

    class LibSkinningParser
    {

    public:
        static void Process(SkinningData& skinning, pugi::xml_node& node);

    };
}
