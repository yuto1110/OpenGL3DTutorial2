#include "Src\BufferObject.h"
#include <iostream>
/**
*バッファオブジェクトを作成する
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
		std::cerr << "[エラー]" << __func__ << ":バッファの作成に失敗.\n";
	}
	return error == GL_NO_ERROR;
}

/**
*バッファにデータを転送する
*/
bool BufferObject::BufferSubData(GLintptr offset, GLsizeiptr size, const GLvoid*data) {
	if (offset + size >= this->size) {
		std::cerr << "[警告]" << __func__ << ":転送先領域がバッファサイズを超えています.\n";
		std::cerr<<" buffer size:"<<this->size<<"offset:"<<offset<<"size"<<size<< "\n";
		if (offset >= this->size) {
			return false;
		}

		//可能な範囲だけ転送を行う
		size = this->size - offset;

	}
	glBindBuffer(target, id);
	glBufferSubData(target, offset, size, data);
	glBindBuffer(target, 0);
	const GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "[エラー]" << __func__ << ":データの転送に失敗.\n";
	}
	return error == GL_NO_ERROR;
}

/**
*BufferObjectを破棄する
*/
void BufferObject::Destroy() {
	if (id) {
		glDeleteBuffers(1, &id);
		id = 0;
	}
}


/**
*VAOを作成する
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
		std::cerr << "[エラー]" << __func__ << ":VAOの作成に失敗.\n";
	}
	return error == GL_NO_ERROR;
}

/**
*VAOを破棄する
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
*VAOをopenGLコンテキストにバインドする
*/

void VertexArrayObject::Bind()const {
	glBindVertexArray(id);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
}

/**
*openGlコンテキストへのバインドを解除する
*/
void VertexArrayObject::Unbind() const {
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/**
*頂点アトリビュートを設定する
*/

void VertexArrayObject::VertexAttribPointer(GLuint index, GLuint size, GLenum type, GLboolean normalized,
	GLsizei stride, size_t offset) const {

	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, type, normalized, stride,
		reinterpret_cast<GLvoid*>(offset));
}

/**
*すべての頂点アトリビュートを無効化する
*/
void VertexArrayObject::ResetVertexAttribPointer()const {
	GLint maxAttr;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttr);
	for (int i = 0; i < maxAttr; ++i) {
		glDisableVertexAttribArray(i);
	}
}