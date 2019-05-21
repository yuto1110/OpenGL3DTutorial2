#include "Src\BufferObject.h"
#include <iostream>
/**
*�o�b�t�@�I�u�W�F�N�g���쐬����
*/

bool BufferObject::Create(GLenum target, GLsizeiptr size, const GLvoid*data, GLenum usage) {
	Destroy();
	glGenBuffers(1, &id);
	glBindBuffer(target, id);
	glBufferData(target, size, data, usage);
	glBindBuffer(target, 0);
	this->target = target;
	this->size = size;
	const GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "[�G���[]" << __func__ << ":�o�b�t�@�̍쐬�Ɏ��s.\n";
	}
	return error == GL_NO_ERROR;
}

/**
*�o�b�t�@�Ƀf�[�^��]������
*/
bool BufferObject::BufferSubData(GLintptr offset, GLsizeiptr size, const GLvoid*data) {
	if (offset + size >= this->size) {
		std::cerr << "[�x��]" << __func__ << ":�]����̈悪�o�b�t�@�T�C�Y�𒴂��Ă��܂�.\n";
		std::cerr<<" buffer size:"<<this->size<<"offset:"<<offset<<"size"<<size<< "\n";
		if (offset >= this->size) {
			return false;
		}

		//�\�Ȕ͈͂����]�����s��
		size = this->size - offset;

	}
	glBindBuffer(target, id);
	glBufferSubData(target, offset, size, data);
	glBindBuffer(target, 0);
	const GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "[�G���[]" << __func__ << ":�f�[�^�̓]���Ɏ��s.\n";
	}
	return error == GL_NO_ERROR;
}

/**
*BufferObject��j������
*/
void BufferObject::Destroy() {
	if (id) {
		glDeleteBuffers(1, &id);
		id = 0;
	}
}


/**
*VAO���쐬����
*/
bool VertexArrayObject::Create(GLuint vbo, GLuint ibo) {
	Destroy();
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	vboId = vbo;
	iboId = ibo;
	const GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "[�G���[]" << __func__ << ":VAO�̍쐬�Ɏ��s.\n";
	}
	return error == GL_NO_ERROR;
}

/**
*VAO��j������
*/
void VertexArrayObject::Destroy() {
	if (id) {
		glDeleteVertexArrays(1, &id);
		id = 0;
		vboId = 0;
		iboId = 0;
	}
}

/**
*VAO��openGL�R���e�L�X�g�Ƀo�C���h����
*/

void VertexArrayObject::Bind()const {
	glBindVertexArray(id);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
}

/**
*openGl�R���e�L�X�g�ւ̃o�C���h����������
*/
void VertexArrayObject::Unbind() const {
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/**
*���_�A�g���r���[�g��ݒ肷��
*/

void VertexArrayObject::VertexAttribPointer(GLuint index, GLuint size, GLenum type, GLboolean normalized,
	GLsizei stride, size_t offset) const {

	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, type, normalized, stride,
		reinterpret_cast<GLvoid*>(offset));
}

/**
*���ׂĂ̒��_�A�g���r���[�g�𖳌�������
*/
void VertexArrayObject::ResetVertexAttribPointer()const {
	GLint maxAttr;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttr);
	for (int i = 0; i < maxAttr; ++i) {
		glDisableVertexAttribArray(i);
	}
}