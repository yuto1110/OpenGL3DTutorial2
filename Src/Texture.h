#pragma once
/**
* @file Texture.h
*/
#ifndef TEXTURE_H_INCLUDED
 #define TEXTURE_H_INCLUDED
 #include <GL/glew.h>
#include<glm/glm.hpp>
#include<vector>
#include<memory>

namespace Texture {
	class Image2D;
		using Image2DPtr = std::shared_ptr<Image2D>;

		
		/**
		*�摜�f�[�^
		*/
		class ImageData {
		public:
			glm::vec4 GetColer(int x, int y) const;

		public:
			GLint width = 0;///<���̃s�N�Z����
			GLint height = 0;///<�c�̃s�N�Z����
			GLenum format = GL_NONE;///<�L�^����Ă���F�̎��
			GLenum type = GL_NONE;///<���ꂼ��̐F���̃r�b�g�z�u
			std::vector<uint8_t>data;///<�o�C�g�f�[�^
			
		};

GLuint CreateImage2D(GLsizei width, GLsizei height, const GLvoid* data,
	GLenum format,GLenum type);
GLuint LoadImage2D(const char*path);
bool LoadImage2D(const char*path, ImageData*imageData);

/**
 * 2D�e�N�X�`��.
 */
	class Image2D
	{
	public:
		static Image2DPtr Create(const char*);
	Image2D() = default;
	explicit Image2D(GLuint texId);
	~Image2D();
	
	void Reset(GLuint texId);
	bool IsNull() const;	
	GLint Width()const { return width; }
	GLint Height() const { return height; }
	GLuint Get() const;
		
			private:
				GLuint id = 0;
				GLint width = 0;
				GLint height = 0;

	};

} // namespace Texture

#endif // TEXTURE_H_INCLUDED