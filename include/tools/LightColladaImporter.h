/*!
 * File LightColladaImporter.h
 */

#pragma once

#include <common/Utils.h>
#include <core/AdvModelItem.h>
#include <string>
#include <pugixml.hpp>
#include <tools/RawDataParser.h>

#define MAX_WEIGHTS 4
#define NO_BONE_MATCH "__no_bone_match__"

namespace hpms
{

    struct Bone
    {
        size_t boneId;

        std::string boneName;

        glm::mat4 offsetMatrix;

        Bone()
        {}

        Bone(size_t boneId, const std::string& boneName, const glm::mat4& offsetMatrix) : boneId(boneId),
                                                                                          boneName(boneName),
                                                                                          offsetMatrix(offsetMatrix)
        {}


    };

    struct VertexWeight
    {
        unsigned int boneId;

        unsigned int vertexId;

        float weight;

        VertexWeight()
        {}

        VertexWeight(unsigned int boneId, unsigned int vertexId, float weight) : boneId(boneId), vertexId(vertexId),
                                                                                 weight(weight)
        {}


    };

    class AnimNode : public HPMSObject
    {
    private:
        std::vector<AnimNode*> children;
        std::vector<glm::mat4> transformations;
        std::string name;
        AnimNode* parent;

    public:


        AnimNode(std::string pname, AnimNode* pparent) : name(pname), parent(pparent)
        {

        }

        ~AnimNode()
        {
            for (AnimNode* child : children)
            {
                hpms::SafeDelete(child);
            }
        }

        inline void AddTransform(const glm::mat4 mat)
        {
            transformations.push_back(mat);
        }

        inline const std::vector<AnimNode*>& GetChildren() const
        {
            return children;
        }

        inline void AddChild(AnimNode* node)
        {
            children.push_back(node);
        }

        inline void SetChildren(const std::vector<AnimNode*>& children)
        {
            AnimNode::children = children;
        }

        inline const std::vector<glm::mat4>& GetTransformations() const
        {
            return transformations;
        }

        inline void SetTransformations(const std::vector<glm::mat4>& transformations)
        {
            AnimNode::transformations = transformations;
        }

        inline const std::string& GetName() const
        {
            return name;
        }

        inline void SetName(const std::string& name)
        {
            AnimNode::name = name;
        }

        inline AnimNode* GetParent() const
        {
            return parent;
        }

        inline void SetParent(AnimNode* parent)
        {
            AnimNode::parent = parent;
        }

        inline const std::string Name() const override
        {
            return "AnimNode";
        }


    };


    class LightColladaImporter
    {
    public:
        static AdvModelItem* LoadModelItem(const std::string& path, const std::string& textDirs);

    private:

        static void ProcessMesh(const hpms::MeshData& meshData, std::vector<Mesh>& meshes,
                                std::vector<Bone>& bones, const std::string& textDirs);


        static void
        ProcessAnimations(const AnimData& animData, std::vector<Bone>& bones, AnimNode* rootNode,
                          glm::mat4 rootTransform,
                          std::vector<hpms::Animation>& animations);

        static void BuildAnimationFrames(const AnimData& animData, std::vector<Bone>& bones, AnimNode* animRootNode, glm::mat4 rootTransform,
                                         std::vector<hpms::Frame>& animationFrames);

        inline static AnimNode* ProcessGraph(SkelData& skelData, AnimNode* parent)
        {
            std::string nodeName = skelData.boneName;
            AnimNode* animNode = hpms::SafeNew<AnimNode>(nodeName, parent);
            for (auto& child : skelData.children)
            {
                AnimNode* childAnimNode = ProcessGraph(child, animNode);
                animNode->AddChild(childAnimNode);
            }
            return animNode;
        }

        inline static AnimNode* Find(const std::string& name, AnimNode* parent)
        {
            AnimNode* res = nullptr;
            if (parent->GetName() == name)
            {
                res = parent;
            } else
            {
                for (AnimNode* child : parent->GetChildren())
                {
                    res = Find(name, child);
                    if (res != nullptr)
                    {
                        break;
                    }
                }
            }
            return res;
        }

        inline static void
        BuildTransFormationMatrices(const std::vector<glm::mat4x4>& channelTransforms, AnimNode* animNode)
        {
            for (auto& transf : channelTransforms)
            {
                animNode->AddTransform(transf);
            }
        }

        inline static unsigned int GetAnimationFrames(AnimNode* parent)
        {
            unsigned int numFrames = parent->GetTransformations().size();
            for (AnimNode* child : parent->GetChildren())
            {
                unsigned int childFrames = GetAnimationFrames(child);
                numFrames = std::max(numFrames, childFrames);
            }
            return numFrames;
        }


        inline static glm::mat4 GetParentTransforms(AnimNode* node, unsigned int framePos)
        {
            if (node == nullptr)
            {
                return glm::mat4(1.0);

            }
            glm::mat4 parentTransform = GetParentTransforms(node->GetParent(), framePos);
            std::vector<glm::mat4> transformations = node->GetTransformations();


            glm::mat4 nodeTransform(1.0);
            unsigned int transfSize = transformations.size();
            if (framePos < transfSize)
            {
                nodeTransform = transformations.at(framePos);
            } else if (transfSize > 0)
            {
                nodeTransform = transformations.at(transfSize - 1);
            }
            glm::mat4 res = parentTransform * nodeTransform;
            return res;
        }

        inline static std::string BoneNameBySid(const SkelData& skelData, const std::string& sid)
        {
            if (sid == skelData.boneSid) {
                return skelData.boneName;
            }
            for (auto& skChild : skelData.children) {
                std::string name = BoneNameBySid(skChild, sid);
                if (name != NO_BONE_MATCH)
                {
                    return name;
                }
            }

            return NO_BONE_MATCH;
        }


    };


}


