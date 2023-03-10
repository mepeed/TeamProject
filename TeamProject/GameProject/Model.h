#pragma once

#include "Common.h"
#include "Mesh.h"
#include "Animation.h"
#include "Material.h"
#include <map>
#include "Shader.h"
#include "Serializeable.h"
#include "Animation.h"

namespace SSB
{
	typedef std::string AnimationName;

	class Model : public Common, public Serializeable
	{
	private:
		static Animation DefaultAnimation;

	private:
		std::vector<TVector3> _meshElementMinMaxVertexList;
		TVector3 _minVertex;
		TVector3 _maxVertex;

		std::map<MaterialIndex, Material*> _materials;
		std::map<MeshIndex, MeshInterface*> _meshes;

		std::map<AnimationName, Animation*> _animations;
		Animation* _currentAnimation;

		Shader* _ps;

	public:
		Model();
		virtual ~Model();

	private:
		void SizeCheck();

	public:
		void Initialize_RegisterMaterial(MaterialIndex index, Material* material);
		void Initialize_RegisterMesh(MeshIndex index, MeshInterface* mesh);
		void Initialize_RegisterAnimation(AnimationName name, Animation* animation);

	public:
		void SetCurrentAnimation(AnimationName name);
		void SetVertexShader(Shader* shader);
		void SetPixelShader(Shader* shader);

	public:
		bool Init() override;
		bool Frame() override;
		bool Render() override;
		bool Release() override;
		std::string Serialize(int tabCount) { return ""; }
		void Deserialize(std::string& serialedString) override;
	};
}
