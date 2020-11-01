/*!
 * File LibSkinningParser.cpp
 */

#include <tools/dae/LibSkinningParser.h>
#include <tools/dae/DaeUtils.h>

void hpms::LibSkinningParser::Process(SkinningData& skinning, pugi::xml_node& node)
{
    std::string ctrlName = node.child("controller").attribute("id").as_string();
    std::vector<std::string> names;
    std::vector<float> weights;
    std::vector<glm::mat4x4> transforms;
    for (pugi::xml_node& srcNode : node.child("controller").child("skin").children("source"))
    {
        std::string id = srcNode.attribute("id").as_string();
        if (id == ctrlName + "-joints")
        {
            ProcessJointsName(names, srcNode);
        } else if (id == ctrlName + "-bind_poses")
        {
            ProcessJointsPoses(names, transforms, skinning, srcNode);
        } else if (id == ctrlName + "-skin_weights")
        {
            ProcessJointsWeights(weights, srcNode);
        }

    }
    pugi::xml_node weightsNode = node.child("controller").child("skin").child("vertex_weights");
    ProcessInfluences(weights, transforms, skinning, weightsNode);
}


void hpms::LibSkinningParser::ProcessJointsName(std::vector<std::string>& names, pugi::xml_node node)
{
    std::string plainNames = node.child("Name_array").child_value();
    std::vector<std::string> namesArray = hpms::PlainStringToArray(plainNames);
    names.insert(std::end(names), std::begin(namesArray), std::end(namesArray));
}

void hpms::LibSkinningParser::ProcessJointsPoses(const std::vector<std::string>& names, std::vector<glm::mat4x4>& tranforms, hpms::SkinningData& data, pugi::xml_node node)
{
    std::string plainMat = node.child("float_array").child_value();
    std::vector<std::string> matArray = hpms::PlainStringToArray(plainMat);
    std::vector<float> floatArray = hpms::ToFloatArray(matArray);
    std::vector<glm::mat4x4> matrices = hpms::ToMat4x4Array(floatArray);
    for (size_t i = 0; i < matrices.size(); i++)
    {
        data.poseMatrixByBoneSid.insert({names[i], matrices[i]});
    }
    tranforms.insert(std::end(tranforms), std::begin(matrices), std::end(matrices));
}


void hpms::LibSkinningParser::ProcessJointsWeights(std::vector<float>& weights, pugi::xml_node& node)
{
    std::string plainWeights = node.child("float_array").child_value();
    std::vector<std::string> weightsArray = hpms::PlainStringToArray(plainWeights);
    std::vector<float> weightsFloatArray = hpms::ToFloatArray(weightsArray);
    weights.insert(std::end(weights), std::begin(weightsFloatArray), std::end(weightsFloatArray));
}

void hpms::LibSkinningParser::ProcessInfluences(const std::vector<float>& weights, const std::vector<glm::mat4x4>& tranforms, hpms::SkinningData& data, pugi::xml_node& node)
{
    std::string plainInfluences = node.child("vcount").child_value();
    std::vector<std::string> influencesArray = hpms::PlainStringToArray(plainInfluences);
    std::vector<int> influences = hpms::ToIntArray(influencesArray);

    std::string plainAssignment = node.child("v").child_value();
    std::vector<std::string> assignmentArray = hpms::PlainStringToArray(plainAssignment);
    std::vector<int> assignmentIntArray = hpms::ToIntArray(influencesArray);
    std::vector<std::pair<int, int>> assignments = hpms::ToPairArray(assignmentIntArray);

    unsigned int vertexId = 0;
    unsigned int assignmentIndex = 0;
    for (int influence : influences)
    {
        std::vector<std::pair<glm::mat4x4, float>> weightAndTransformForVertex;
        for (int i = 0; i < influence; i++)
        {
            std::pair<int, int> assignment = assignments[assignmentIndex++];
            glm::mat4x4 bonePose = tranforms[assignment.first];
            float weight = weights[assignment.second];
            weightAndTransformForVertex.emplace_back(bonePose, weight);
        }
        data.weightAndTransfMatrixInfluencesByVertexId.insert({vertexId, weightAndTransformForVertex});
        vertexId++;
    }
}


