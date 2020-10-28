/*!
 * File LibSkeletonParser.h
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
        std::vector<SkelTreeData> children;
    };

    class LibSkeletonParser
    {

        static void Process(const SkelTreeData& skeleton, pugi::xml_node& node);

    };
}
