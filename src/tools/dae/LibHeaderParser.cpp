/*!
 * File LibHeaderParser.cpp
 */

#include <tools/dae/LibHeaderParser.h>

void hpms::LibHeaderParser::Process(hpms::HeaderData& header, pugi::xml_node& node)
{
    header.unit = node.child("asset").child("unit").attribute("meter").as_float();
    std::string upAxis = node.child("asset").child("up_axis").child_value();
    if (upAxis == "X_UP") {
        header.upAxis = glm::vec3(1, 0, 0);
    } else if (upAxis == "X_DOWN") {
        header.upAxis = glm::vec3(-1, 0, 0);
    } else if (upAxis == "Y_UP") {
        header.upAxis = glm::vec3(0, 1, 0);
    } else if (upAxis == "Y_DOWN") {
        header.upAxis = glm::vec3(0, -1, 0);
    } else if (upAxis == "Z_UP") {
        header.upAxis = glm::vec3(0, 0, 1);
    } else if (upAxis == "Z_DOWN") {
        header.upAxis = glm::vec3(0, 0, -1);
    }
}
