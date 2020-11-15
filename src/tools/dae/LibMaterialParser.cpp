/*!
 * File LibMaterialParser.cpp
 */

#include <tools/dae/LibMaterialParser.h>
#include <tools/dae/DaeUtils.h>

void hpms::LibMaterialParser::Process(hpms::MaterialDefData& matData, pugi::xml_node& node)
{
    for (pugi::xml_node mat : node.children("material"))
    {
        std::string matId = mat.attribute("id").as_string();
        std::string name = mat.attribute("name").as_string();
        std::string instId = mat.child("instance_effect").attribute("url").as_string();
        instId = instId.replace(0, 1, ""); // First url character is '#', remove it for recreate instance id.
        std::pair<std::string, std::string> instanceAndName = std::make_pair(instId, name);
        matData.instanceAndNameIdByMatId.insert({matId, instanceAndName});
    }
}
