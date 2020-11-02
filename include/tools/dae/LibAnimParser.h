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

    struct AnimFramesData
    {
        int frames;
        std::unordered_map<std::string, std::vector<glm::mat4x4>> transfMatricesByBoneId;
    };

    class LibAnimParser
    {
    public:
        static void Process(AnimFramesData& anims, pugi::xml_node& node);

    };
}
