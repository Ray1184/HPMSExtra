/*!
 * File LibGeometryParser.cpp
 */

#include <tools/dae/LibGeometryParser.h>
#include <tools/dae/DaeUtils.h>


void hpms::LibGeometryParser::Process(hpms::GeometryData& geomData, pugi::xml_node& node)
{
    pugi::xml_node geomNode = node.child("geometry");
    std::string modelName = geomNode.attribute("id").as_string();
    for (pugi::xml_node& source : geomNode.child("mesh").children("source"))
    {
        if (source.attribute("id").as_string() == modelName + "-positions")
        {
            ProcessPositions(geomData, source);
        } else if (source.attribute("id").as_string() == modelName + "-normals")
        {
            ProcessNormals(geomData, source);
        } else if (source.attribute("id").as_string() == modelName + "-texcoord-0")
        {
            ProcessNormals(geomData, source);
        }
    }
    for (pugi::xml_node tri : geomNode.child("mesh").children("triangles"))
    {
        ProcessIndices(geomData, tri);
    }

}

void hpms::LibGeometryParser::ProcessPositions(hpms::GeometryData& data, pugi::xml_node node)
{
    std::vector<std::string> stringPositions = hpms::PlainStringToArray(node.child("float_array").child_value());
    std::vector<float> allPositions = hpms::ToFloatArray(stringPositions);
    std::vector<glm::vec3> positions = hpms::ToVec3Array(allPositions);
    data.positions = positions;
}

void hpms::LibGeometryParser::ProcessNormals(hpms::GeometryData& data, pugi::xml_node node)
{
    std::vector<std::string> stringNormals = hpms::PlainStringToArray(node.child("float_array").child_value());
    std::vector<float> allNormals = hpms::ToFloatArray(stringNormals);
    std::vector<glm::vec3> normals = hpms::ToVec3Array(allNormals);
    data.normals = normals;
}


void hpms::LibGeometryParser::ProcessUVCoords(hpms::GeometryData& data, pugi::xml_node node)
{
    std::vector<std::string> stringUVCoords = hpms::PlainStringToArray(node.child("float_array").child_value());
    std::vector<float> allUVCoords = hpms::ToFloatArray(stringUVCoords);
    std::vector<glm::vec2> uvCoords = hpms::ToVec2Array(allUVCoords);
    data.uvCoords = uvCoords;
}

void hpms::LibGeometryParser::ProcessIndices(hpms::GeometryData& data, pugi::xml_node node)
{
    std::string meshId = node.attribute("material").as_string();
    std::vector<std::string> stringIndices = hpms::PlainStringToArray(node.child("p").child_value());
    std::vector<int> allIndices = hpms::ToIntArray(stringIndices);
    std::vector<std::tuple<int, int, int>> indices = hpms::ToTripletArray(allIndices);
    data.indicesByMeshId.insert({meshId, indices});
}


