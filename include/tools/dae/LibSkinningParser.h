/*!
 * File LibSkinningParser.h
 */

#pragma once

#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include <pugixml.hpp>
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

        static void ProcessJointsName(std::vector<std::string>& names, pugi::xml_node node);

        static void ProcessJointsPoses(const std::vector<std::string>& names, std::vector<glm::mat4x4>& tranforms, SkinningData& data, pugi::xml_node node);

        static void ProcessJointsWeights(std::vector<float>& weights, pugi::xml_node& node);

        static void ProcessInfluences(const std::vector<float>& weights, const std::vector<glm::mat4x4>& tranforms, SkinningData& data, pugi::xml_node& node);
    };
}
