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
        std::unordered_map<std::string, std::vector<glm::mat4x4>> transfMatricesByBoneId;
    };

    class LibAnimParser
    {
    public:
        static void Process(AnimData& anims, pugi::xml_node& node);

    };
}
