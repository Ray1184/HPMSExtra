/*!
 * File LightColladaImporter.cpp
 */


#include <tools/RawDataParser.h>
#include <tools/LightColladaImporter.h>


hpms::AdvModelItem* hpms::LightColladaImporter::LoadModelItem(const std::string& path, const std::string& textDirs)
{
    RawData data;
    hpms::RawDataParser::LoadAndProcessModel(path, data);
    std::vector<Bone> bones;
    std::vector<Mesh> meshes;
    for (auto& rMesh : data.meshData)
    {
        ProcessMesh(rMesh, meshes, bones, textDirs);
    }

    AnimNode* rootAnimNode = ProcessGraph(data.animData.skelData, nullptr);
    std::vector<Animation> animations;
    glm::mat4x4 rootNodeTransf = data.animData.tranformByBoneSid[data.animData.skelData.boneSid][0];
    ProcessAnimations(data.animData, bones, rootAnimNode, rootNodeTransf, animations);
    hpms::SafeDelete(rootAnimNode);
    AdvModelItem* advModelItem = hpms::SafeNew<AdvModelItem>();
    advModelItem->SetMeshes(meshes);
    advModelItem->SetAnimations(animations);
    return advModelItem;
}


void hpms::LightColladaImporter::ProcessMesh(const hpms::MeshData& meshData, std::vector<Mesh>& meshes,
                                             std::vector<Bone>& bones, const std::string& textDirs)
{

    std::vector<float> positions;
    std::vector<float> textCoords;
    std::vector<float> normals;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> jointIndices;
    std::vector<float> weights;
    bool textured = false;
    bool animated = false;

    for (auto& v : meshData.vertices)
    {
        positions.push_back(v.x);
        positions.push_back(v.y);
        positions.push_back(v.z);
    }

    for (auto& t : meshData.uvMaps)
    {
        textCoords.push_back(t.x);
        textCoords.push_back(t.y);
        textured = true;
    }

    for (auto& n : meshData.normals)
    {
        normals.push_back(n.x);
        normals.push_back(n.y);
        normals.push_back(n.z);
    }

    for (auto& f : meshData.indices)
    {
        indices.push_back(std::get<0>(f));
        indices.push_back(std::get<1>(f));
        indices.push_back(std::get<2>(f));
    }

    for (auto& bEntry : meshData.boneDataByBoneSid)
    {
        animated = true;
        std::unordered_map<unsigned int, std::vector<VertexWeight>> weightSet;
        BoneData boneData = bEntry.second;
        std::string boneSid = bEntry.first;
        unsigned int id = bones.size();
        Bone bone(id, boneSid, boneData.weightAndTransform.first);
        bones.push_back(bone);
        for (auto& wPair : boneData.weightAndTransform.second)
        {
            VertexWeight vertexWeight(bone.boneId, wPair.first, wPair.second);
            weightSet[vertexWeight.vertexId].push_back(vertexWeight);
        }
        for (size_t i = 0; i < meshData.vertices.size(); i++)
        {
            size_t size;
            if (weightSet.find(i) == weightSet.end())
            {
                size = 0;
            } else
            {
                size = weightSet[i].size();
            }
            for (int j = 0; j < MAX_WEIGHTS; j++)
            {
                if (j < size)
                {
                    VertexWeight vw = weightSet[i][j];
                    weights.push_back(vw.weight);
                    jointIndices.push_back(vw.boneId);
                } else
                {
                    weights.push_back(0);
                    jointIndices.push_back(0);
                }
            }
        }
    }

    float boundingRad = 0.0f;
    for (float p : positions)
    {
        boundingRad = std::max(std::abs(p), boundingRad);
    }

    Material material;
    material.SetAmbient(meshData.materialData.color);
    material.SetDiffuse(meshData.materialData.color);
    material.SetTextureName(textured ? meshData.materialData.name : "");

    Mesh mesh;
    mesh.SetMaterial(material);
    mesh.SetPositions(positions);
    mesh.SetNormals(normals);
    mesh.SetTextCoords(textCoords);
    mesh.SetIndices(indices);
    mesh.SetJointIndices(jointIndices);
    mesh.SetWeights(weights);
    mesh.SetVertexCount(indices.size());
    mesh.SetBoundingRadious(boundingRad);
    mesh.SetTextured(textured);
    mesh.SetAnimated(animated);
    mesh.SetKey(meshData.name);
    mesh.SetName(meshData.materialData.name);
    meshes.push_back(mesh);
}

void hpms::LightColladaImporter::ProcessAnimations(const hpms::AnimData& animData, std::vector<Bone>& bones,
                                                   hpms::AnimNode* rootNode, glm::mat4 rootTransform,
                                                   std::vector<hpms::Animation>& animations)
{
    for (auto& aEntry : animData.tranformByBoneSid)
    {

        std::string nodeName = BoneNameBySid(animData.skelData, aEntry.first);
        if (nodeName == NO_BONE_MATCH)
        {
            std::stringstream ss;
            ss << "Impossible to retrieve bone name with sid " << aEntry.first << " from skeleton tree.";
            LOG_ERROR(ss.str().c_str());
        }
        AnimNode* animNode = Find(nodeName, rootNode);
        BuildTransFormationMatrices(aEntry.second, animNode);
    }

    std::vector<Frame> frames;
    BuildAnimationFrames(animData, bones, rootNode, rootTransform, frames);
    Animation anim("default", frames, frames.size());
    animations.push_back(anim);
}

void hpms::LightColladaImporter::BuildAnimationFrames(const AnimData& animData, std::vector<Bone>& bones, hpms::AnimNode* animRootNode,
                                                      glm::mat4 rootTransform,
                                                      std::vector<hpms::Frame>& animationFrames)
{
    unsigned int numFrames = GetAnimationFrames(animRootNode);
    for (int i = 0; i < numFrames; i++)
    {
        Frame animatedFrame;;
        unsigned int numBones = bones.size();
        for (int j = 0; j < numBones; j++)
        {
            Bone bone = bones.at(j);
            std::string nodeName = BoneNameBySid(animData.skelData, bone.boneName);
            AnimNode* animNode = Find(nodeName, animRootNode);
            glm::mat4 boneMatrix = GetParentTransforms(animNode, i);
            boneMatrix = rootTransform * boneMatrix * bone.offsetMatrix;
            FrameTransform frameTransform(boneMatrix);
            animatedFrame.frameTransformations.push_back(frameTransform);
        }
        for (unsigned int j = animatedFrame.frameTransformations.size(); j < MAX_JOINTS; j++)
        {
            animatedFrame.frameTransformations.emplace_back();
        }
        animationFrames.push_back(animatedFrame);
    }
}

