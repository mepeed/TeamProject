#pragma once

#include <d3d11.h>
#include "Common.h"
#include "TMath.h"
#include "Shader.h"
#include <vector>
#include "Serializeable.h"
#include "SerializeableDataType.h"

namespace SSB
{
	typedef DWORD IndexForMeshVertice;
	typedef int MeshIndex;

	static const std::string Vertex_PC_Keyword = "Vertex_PC";
	static const std::string Vertex_PCNT_Keyword = "Vertex_PCNT";
	static const std::string Vertex_PCNT_Animatable_Keyword = "Vertex_PCNT_Animatable";
	static const std::string Vertex_PCNT_Skinning_Keyword = "Vertex_PCNT_Skinning";
	static const std::string Vertex_PCNTs_Keyword = "Vertex_PCNTs";
	static const std::string Vertex_PCNTs_Animatable_Keyword = "Vertex_PCNTs_Animatable";
	static const std::string Vertex_PCNTs_Skinning_Keyword = "Vertex_PCNTs_SKinning";

	class MeshInterface : public Common, public Serializeable
	{
	protected:
		virtual void InitialShader() = 0;
		virtual bool CreateVertexLayout() = 0;
		virtual bool CreateVertexBuffer() = 0;
		virtual bool CreateIndexBuffer() = 0;
		virtual void Build() = 0;

	public:
		virtual void Initialize_AddSubMesh(MeshInterface* mesh) = 0;

	public:
		virtual void SetShader(Shader* shader) = 0;

	public:
		virtual TVector3 GetMinVertex() = 0;
		virtual TVector3 GetMaxVertex() = 0;
	};

	template<typename VertexType>
	class Mesh : public MeshInterface
	{
	private:
		ID3D11InputLayout* _vertexLayout;
		std::vector<VertexType> _vertexList;
		ID3D11Buffer* _vertexBuffer = nullptr;
		std::vector<IndexForMeshVertice> _indexList;
		ID3D11Buffer* _indexBuffer = nullptr;

		TVector3 _minVertex;
		TVector3 _maxVertex;

		std::vector<MeshInterface*> _subMeshes;

		Shader* _vs;

	public:
		Mesh();
		virtual ~Mesh();

	private:
		void CheckSize();

	private:
		bool CreateVertexLayout() override;
		bool CreateVertexBuffer() override;
		bool CreateIndexBuffer() override;

	protected:
		void SetVertexList(std::vector<VertexType> vertexList);
		void SetIndexList(std::vector<IndexForMeshVertice> indexList);

	protected:
		virtual void SetVertexLayoutDesc(D3D11_INPUT_ELEMENT_DESC** desc, int& count) = 0;

	protected:
		void Build() { }

	public:
		void Initialize_AddSubMesh(MeshInterface* mesh) override;

	public:
		void SetShader(Shader* shader) override;

	public:
		TVector3 GetMinVertex() override;
		TVector3 GetMaxVertex() override;

	public:
		bool Init() override;
		bool Frame() override;
		bool Render() override;
		bool Release() override;

	protected:
		virtual std::string GetVertexType() = 0;
		virtual VertexType GetDeserializedVertex(std::string str) = 0;

	public:
		std::string Serialize(int tabCount) override;
		void Deserialize(std::string& serialedString) override;
	};

	class Mesh_Vertex_PC : public Mesh<Vertex_PC>
	{
	private:
		std::string GetVertexType() override;
		Vertex_PC GetDeserializedVertex(std::string str) override;

	private:
		void SetVertexLayoutDesc(D3D11_INPUT_ELEMENT_DESC** desc, int& count) override;
		void InitialShader() override;
	};
	class Mesh_Vertex_PCNT : public Mesh<Vertex_PCNT>
	{
	private:
		std::string GetVertexType() override;
		Vertex_PCNT GetDeserializedVertex(std::string str) override;

	private:
		void SetVertexLayoutDesc(D3D11_INPUT_ELEMENT_DESC** desc, int& count) override;
		void InitialShader() override;
	};
	class Mesh_Vertex_PCNT_Animatable : public Mesh<Vertex_PCNT>
	{
	private:
		MeshData _meshData;
		ID3D11Buffer* _meshBuffer = nullptr;

	private:
		bool CreateMeshBuffer();

	private:
		std::string GetVertexType() override;
		Vertex_PCNT GetDeserializedVertex(std::string str) override;

	private:
		void SetVertexLayoutDesc(D3D11_INPUT_ELEMENT_DESC** desc, int& count) override;
		void InitialShader() override;

	public:
		void Initialize_SetMeshData(MeshData meshData);

	public:
		bool Init() override;
		bool Render() override;
		bool Release() override;

	public:
		std::string Serialize(int tabCount) override;
		void Deserialize(std::string& serialedString) override;
	};
	class Mesh_Vertex_PCNT_Skinning : public Mesh<Vertex_PCNT_Skinning>
	{
	private:
		MeshToBoneSpaceTransformData _boneSpaceTransformData;
		ID3D11Buffer* _boneSpaceTransformBuffer = nullptr;
		int _maxBoneCount;

	private:
		std::string GetVertexType() override;
		Vertex_PCNT_Skinning GetDeserializedVertex(std::string str) override;

	private:
		void SetVertexLayoutDesc(D3D11_INPUT_ELEMENT_DESC** desc, int& count) override;
		void InitialShader() override;

	private:
		bool CreateBoneSpaceTransformBuffer();

	public:
		void SetMeshData(MeshToBoneSpaceTransformData data);
		void SetMeshAnimationWeight(float weight);
		void Initialize_SetMaxBoneCount(int maxBoneCount);
		void Initialize_SetMeshData(int index);

	public:
		bool Init() override;
		bool Render() override;
		bool Release() override;

	public:
		std::string Serialize(int tabCount) override;
		void Deserialize(std::string& serialedString) override;
	};
	class Mesh_Vertex_PCNTs : public Mesh<Vertex_PCNTs>
	{
	private:
		std::string GetVertexType() override;
		Vertex_PCNTs GetDeserializedVertex(std::string str) override;

	private:
		void SetVertexLayoutDesc(D3D11_INPUT_ELEMENT_DESC** desc, int& count) override;
		void InitialShader() override;
	};

	class Mesh_Vertex_PCNTs_Animatable : public Mesh<Vertex_PCNTs>
	{
	private:
		MeshData _meshData;
		ID3D11Buffer* _meshBuffer = nullptr;

	private:
		std::string GetVertexType() override;
		Vertex_PCNTs GetDeserializedVertex(std::string str) override;

	private:
		bool CreateMeshBuffer();

	private:
		void SetVertexLayoutDesc(D3D11_INPUT_ELEMENT_DESC** desc, int& count) override;
		void InitialShader() override;

	public:
		void Initialize_SetMeshData(MeshData meshData);

	public:
		bool Init() override;
		bool Render() override;
		bool Release() override;

	public:
		std::string Serialize(int tabCount) override;
		void Deserialize(std::string& serialedString) override;
	};
	class Mesh_Vertex_PCNTs_Skinning : public Mesh<Vertex_PCNTs_Skinning>
	{
	private:
		MeshToBoneSpaceTransformData _boneSpaceTransformData;
		ID3D11Buffer* _boneSpaceTransformBuffer = nullptr;
		int _maxBoneCount;

	private:
		std::string GetVertexType() override;
		Vertex_PCNTs_Skinning GetDeserializedVertex(std::string str) override;

	private:
		void SetVertexLayoutDesc(D3D11_INPUT_ELEMENT_DESC** desc, int& count) override;
		void InitialShader() override;

	private:
		bool CreateBoneSpaceTransformBuffer();

	public:
		void SetMeshData(MeshToBoneSpaceTransformData data);
		void Initialize_SetMaxBoneCount(int maxBoneCount);
		void Initialize_SetMeshData(int index);
		void SetMeshAnimationWeight(float weight);

	public:
		bool Init() override;
		bool Render() override;
		bool Release() override;

	public:
		std::string Serialize(int tabCount) override;
		void Deserialize(std::string& serialedString) override;
	};

	//class Direction : public Mesh_Vertex_PC
	//{
	//public:
	//	Direction();
	//};
	/*
	class Triangle : public Mesh_Vertex_PCNT
	{
	public:
		Triangle();

	private:
		void Build() override;
	};

	class Box : public Mesh_Vertex_PCNT
	{
	private:
		float _width;
		float _height;
		float _depth;

	public:
		Box(float width = 1.0f, float height = 1.0f, float depth = 1.0f);

	private:
		void Build() override;
	};
	*/
}

//#include "Mesh.hpp"
