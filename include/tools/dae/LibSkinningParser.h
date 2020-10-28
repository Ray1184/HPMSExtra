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
        std::unordered_map<std::string, glm::mat4x4> poseMatrixByBoneId;
        std::unordered_map<int, std::vector<std::pair<glm::mat4x4, float>>> weightAndTransfMatrixInfluencesByVertexId;
    };

    class LibSkinningParser
    {

        static void Process(const SkinningData& skinning, pugi::xml_node& node);

    };
}
