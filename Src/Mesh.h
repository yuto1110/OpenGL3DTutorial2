/**
*@file Mesh.h
*/
#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED
#include<GL\glew.h>
#include"BufferObject.h"
#include"Texture.h"
#include"Shader.h"
#include<glm\glm.hpp>
#include<vector>
#include<unordered_map>
#include<string>
#include<memory>
namespace Mesh {
	//��s�錾
	class Mesh;
	using MeshPtr = std::shared_ptr<Mesh>;
	class MeshBuffer;
	using MeshBufferPtr = std::shared_ptr<Mesh>;

	/**
	*���_�f�[�^
	*/
	struct Vertex {
		glm::vec3 position;
		glm::vec2 texCoord;
		glm::vec3 normal;
	};

	/**
	*�v���~�e�B�u�̍ގ�
	*/
	struct Material {
		glm::vec4 baseColor = glm::vec4(1);
		Texture::Image2DPtr texture;
		Shader::ProgramPtr program;
	};

	/**
	*���_�f�[�^�̕`��p�����[�^
	*/
	struct Primitive {
		GLenum mode;
		GLsizei count;
		GLenum type;
		const GLvoid*indices;
		GLint baseVertex = 0;
		std::shared_ptr<VertexArrayObject>vao;
		int material = 0;
	};

	/**
	*���b�V��
	*/
	struct Mesh {
		std::string name;//���b�V����
		std::vector<Primitive>primitives;
	};

	/**
	*�t�@�C��
	*/
	struct File {
		std::string name;//�t�@�C����
		std::vector<Mesh> meshes;
		std::vector<Material>materials;
	};
	using FilePtr = std::shared_ptr<File>;

	/**
	*���b�V���Ǘ��N���X
	*/
	class Buffer {
	public :
		Buffer() = default;
		~Buffer() = default;

		bool Init(GLsizeiptr vboSize, GLsizeiptr iboSize);
		GLintptr AddVertexData(const void*data, size_t size);
		GLintptr AddIndexData(const void*data, size_t size);
		Primitive CreatePrimitive(size_t count, GLenum type, size_t iOffset, size_t vOffset)const;
		Material CreateMaterial(const glm::vec4&color, Texture::Image2DPtr texture)const;
		bool AddMesh(const char*name, const Primitive&primitive, const Material&material);
		FilePtr GetFile(const char*name)const;

		void Addcube(const char*name);

	private:
		BufferObject vbo;
		BufferObject ibo;
		GLintptr vboEnd = 0;
		GLintptr iboEnd = 0;
		std::unordered_map<std::string, FilePtr>files;
		Shader::ProgramPtr progStaticMesh;
	};

	void Draw(const FilePtr&, const glm::mat4& matVP, const glm::mat4& matM);
}//namespace Mesh
#endif // !MESH_H_INCLUDED
