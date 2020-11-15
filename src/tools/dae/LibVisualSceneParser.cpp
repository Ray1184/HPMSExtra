/*!
 * File LibVisualSceneParser.cpp
 */

#include <tools/dae/LibVisualSceneParser.h>

void hpms::LibVisualSceneParser::Process(VisualData& visualScene, pugi::xml_node& node)
{
    pugi::xml_node rootNode = node.child("visual_scene").child("node");
    visualScene.jointType = true; // Root node it's a joint.
    visualScene.boneId = rootNode.attribute("id").as_string();
    visualScene.boneSid = visualScene.boneId; // For root same of bone sid;
    visualScene.boneName = rootNode.attribute("name").as_string();

    ProcessRecursive(&visualScene, rootNode);

}

void hpms::LibVisualSceneParser::ProcessRecursive(hpms::VisualData* data, pugi::xml_node& node)
{
    for (pugi::xml_node child : node.children("node"))
    {
        VisualData visualScene;
        std::string jointType = child.attribute("type").as_string();
        visualScene.boneId = child.attribute("id").as_string();
        visualScene.boneName = child.attribute("name").as_string();
        if (jointType == "JOINT")
        {
            visualScene.jointType = true;
            visualScene.boneSid = child.attribute("sid").as_string();
            ProcessRecursive(&visualScene, child);
        } else
        {
            visualScene.jointType = false;
            visualScene.boneSid = visualScene.boneId;
            ProcessMaterialStructure(*data, &visualScene, child);
        }

        data->children.push_back(visualScene);
    }
}

void hpms::LibVisualSceneParser::ProcessMaterialStructure(hpms::VisualData& rootData, hpms::VisualData* data, pugi::xml_node& node)
{
    pugi::xml_node techNode = node.child("instance_controller").child("bind_material").child("technique_common");
    for (pugi::xml_node child : techNode.children("instance_material"))
    {
        std::string meshId = child.attribute("symbol").as_string();
        std::string matId = child.attribute("target").as_string();
        matId = matId.replace(0, 1, ""); // First url character is '#', remove it for recreate material id.
        rootData.matIdByMeshId.insert({meshId, matId});
    }
}

