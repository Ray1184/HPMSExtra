/*!
 * File LibGeometryParser.h
 */

#pragma once

#include <vector>
#include <tuple>
#include <glm/glm.hpp>
#include <pugixml.hpp>
#include <unordered_map>


namespace hpms
{
    struct GeometryData
    {
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<std::tuple<int, int, int>> indices;
        std::string meshId;
    };

    class LibGeometryParser
    {

        static void Process(const GeometryData& dataMap, pugi::xml_node& node);

    };
}



