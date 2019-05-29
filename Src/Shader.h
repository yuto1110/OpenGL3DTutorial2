#pragma once
/*
@file Shader.h
*/
#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include<memory>
struct  Mesh;

namespace Shader
{
	class Program;
	using ProgramPtr = std::shared_ptr<Program>;

	GLuint Build(const GLchar*vsCode, const GLchar*fsCode);
	GLuint BuildFromFile(const GLchar*vsPath, const GLchar*fsPath);

	//����
	struct AmbientLigt
	{
		glm::vec3 color;
	};

	//�w�������C�g
	struct DirectionalLight
	{
		glm::vec3 direction;
		glm::vec3 color;
	};

	//�|�C���g�E���C�g
	struct PointLight
	{
		glm::vec3 position[8];
		glm::vec3 color[8];
	};

	// �X�|�b�g���C�g
	struct SpotLight
	{
		glm::vec4 dirAndCutoff[4];
		glm::vec4 posAndInnerCutoff[4];
		glm::vec3 color[4];
	};

	/*
	 ���C�g���܂Ƃ߂��\����.
	*/
	struct  LightList
	{
		AmbientLigt ambient;
		DirectionalLight directional;
		PointLight point;
		SpotLight spot;

		void Init();
	};
	/*
	* �V�F�[�_�[�E�v���O����.
	*/
	class Program
	{
	public:
		static ProgramPtr Create(const char*vsPath, const char*fsPath);
		Program();
		explicit Program(GLuint programId);
		~Program();
		
		void Reset(GLuint programId);
		bool IsNull()const;
		void Use();
		void BindTexture(GLuint, GLuint);
		void SetLightList(const LightList&);
		void SetViewProjectionMatrix(const glm::mat4&);
		void Draw(const Mesh& mesh,
			const glm::vec3& translate, const glm::vec3& rotate, const glm::vec3& scale);
	private:
		GLuint id=0;//�v���O����ID

		//uniform�ϐ��̈ʒu
		GLint locMatMVP=1;
		GLint locPointLightPos = 1;
		GLint locPointLightCol = 1;
		GLint locDirLightDir=1;
		GLint locDirLightCol=1;
		GLint locAmbLightCol = 1;
		GLint locSpotLightPos = 1;
		GLint locSpotLightDir=1;
		GLint locSpotLightCol=1;
		
			glm::mat4 matVP=glm::mat4(1); // �r���[�E�v���W�F�N�V�����s��.
		LightList lights;
	};
}//Shader namespace
#endif // !SHADER_H_INCLUDED
