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
        std::vector<glm::vec2> uvCoords;
        std::unordered_map<std::string, std::vector<std::tuple<int, int, int>>> indicesByMeshId;
    };

    class LibGeometryParser
    {

    public:
        static void Process(GeometryData& geomData, pugi::xml_node& node);

    private:
        static void ProcessPositions(GeometryData& data, pugi::xml_node node);

        static void ProcessNormals(GeometryData& data, pugi::xml_node node);

        static void ProcessUVCoords(GeometryData& data, pugi::xml_node node);

        static void ProcessIndices(GeometryData& data, pugi::xml_node node);
    };
}



