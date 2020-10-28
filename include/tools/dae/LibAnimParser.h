/*!
 * File LibAnimParser.h
 */

#pragma once

#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include <pugixml.hpp>

namespace hpms
{

    struct AnimData
    {
        int frames;
        std::unordered_map<std::string, std::unordered_map<int, glm::mat4x4>> transfMatrixByFrameByBoneId;
    };

    class LibSkinningParser
    {

        static void Process(const AnimData& anims, pugi::xml_node& node);

    };
}
