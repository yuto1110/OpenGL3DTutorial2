/**
*@file Mesh.cpp
*/
#define NOMIMAX
#include"Mesh.h"
#include"json11\json11.hpp"
#include<glm\gtc\quaternion.hpp>
#include<fstream>
#include<algorithm>
#include<glm\gtc\matrix_transform.hpp>
#include<glm\gtc\constants.hpp>
#include<iostream>

/**
*���b�V���Ɋւ���@�\���i�[���閼�O���
*/
namespace Mesh {

	/**
	* �t�@�C����ǂݍ���.
	*
	* @param path �t�@�C����.
	*
	* @return �t�@�C���̓��e.
	*/
	std::vector<char>ReadFile(const char*path) {
		std::ifstream ifs(path, std::ios_base::binary);
		if (!ifs) {
			std::cerr << "[�G���[]" << __func__ << ":" << "���J���܂���.\n";
			return{};
		}
		std::vector<char>tmp(1'000'000);
		ifs.rdbuf()->pubsetbuf(tmp.data(), tmp.size());

		ifs.seekg(0, std::ios_base::end);
		const std::streamoff size = ifs.tellg();
		ifs.seekg(0, std::ios_base::beg);

		std::vector<char>buf;
		buf.resize(static_cast<size_t>(size));
		ifs.read(&buf[0], size);

		return buf;
	}

	/**
	*JSON�̔z��f�[�^��glm::vec3�ɕϊ�����.
	*
	*@param json �ϊ����ƂȂ�z��f�[�^.
	*
	*@return json��ϊ����Ăł���vec3�̒l.
	*/

	glm::vec3 GetVec3(const json11::Json&json) {
		const std::vector<json11::Json>& a = json.array_items();
		if (a.size() < 3) {
			return glm::vec3(0);
		}
		return glm::vec3(a[0].number_value(), a[1].number_value(), a[2].number_value());
	}

	/**
	* JSON�̔z��f�[�^��glm::quat�ɕϊ�����.
	*
	* @param json �ϊ����ƂȂ�z��f�[�^.
	*
	* @return json��ϊ����Ăł���quat�̒l.
	*/

	glm::quat GetQuat(const json11::Json&json) {
		const std::vector<json11::Json>&a = json.array_items();
		if (a.size() < 4) {
			return glm::quat(0, 0, 0, 1);
		}
		return glm::quat(
			static_cast<float>(a[3].number_value()),
			static_cast<float>(a[0].number_value()),
			static_cast<float>(a[1].number_value()),
			static_cast<float>(a[2].number_value())
		);
	}
	/**
	*JSON�z��f�[�^��glm::mat4�ɕϊ�����
	*@param json �ϊ����ƂȂ�z��f�[�^
	*
	*@return json��ϊ����Ăł���mat4�̒l
	*/
	glm::mat4 GetMat4(const json11::Json&json) {
		const std::vector < json11::Json>&a = json.array_items();
		if (a.size() < 16) {
			return glm::mat4(1);
		}
		return glm::mat4(
			a[0].number_value(), a[1].number_value(), a[2].number_value(), a[3].number_value(),
			a[4].number_value(), a[5].number_value(), a[6].number_value(), a[7].number_value(),
			a[8].number_value(), a[9].number_value(), a[10].number_value(), a[11].number_value(),
			a[12].number_value(), a[13].number_value(), a[14].number_value(), a[15].number_value()
		);
	}

	/**
	* �A�N�Z�b�T���w�肷��o�C�i���f�[�^�̈ʒu�ƃo�C�g�����擾����.
	*
	* @param accessor    glTF�A�N�Z�b�T
	* @param bufferViews �o�C�i���f�[�^�𕪊��Ǘ����邽�߂̃f�[�^�z��.
	* @param binFiles    �o�C�i���t�@�C���̔z��.
	* @param pp          �擾�����o�C�i���f�[�^�̈ʒu.
	* @param pLength     �擾�����o�C�i���f�[�^�̃o�C�g��.
	* @param pStride     �擾�����o�C�i���f�[�^�̃f�[�^��(���_�f�[�^�̒�`�Ŏg�p).
	*/

	void GetBuffer(const json11::Json&accessor, const json11::Json&bufferViews, const std::vector<std::vector<char>>&binFiles, const void**pp, size_t*pLength, int*pStride = nullptr) {
		const int bufferViewId = accessor["bufferView"].int_value();
		const int byteOffset = accessor["byteOffset"].int_value();
		const json11::Json bufferView = bufferViews[bufferViewId];
		const int bufferId = bufferView["buffer"].int_value();
		const int baesByteOffset = bufferView["byteOffset"].int_value();
		int byteLength = bufferView["byteLength"].int_value();
		if (!accessor["count"].is_null()) {
			int unitByteSize;
			switch (accessor["componentType"].int_value()) {
			case GL_BYTE:
			case GL_UNSIGNED_BYTE:
				unitByteSize = 1;
				break;
			case GL_SHORT:
			case GL_UNSIGNED_SHORT:
				unitByteSize = 2;
				break;
			default:
				unitByteSize = 4;
				break;
			}
			const std::string&type = accessor["type"].string_value();
			static const char*const typeNameList[] = { "SCALA","VEC2","VEC3","VEC4","MAT4" };
			static const int typeSizeList[] = { 1,2,3,4,16 };
			int typeSize = -1;
			for (size_t i = 0; i < 5; ++i) {
				if (type == typeNameList[i]) {
					typeSize = typeSizeList[i];
					break;
				}
			}
			if (typeSize < 0) {
				std::cerr << "[�G���[]" << __func__ << ":�A�N�Z�b�T�̌^(" << type << ")�ɂ͖��Ή��ł�.\n";
				byteLength = 0;
			}
			else {
				byteLength = std::min(byteLength, accessor["count"].int_value() * unitByteSize * typeSize);
			}
		}
		if (pStride) {
			*pStride = bufferView["byteStride"].int_value();
		}
		*pLength = byteLength;
		*pp = binFiles[bufferId].data() + baesByteOffset + byteOffset;
	}
	/**
	*���b�V���o�b�t�@������������
	*/
	 bool Buffer::Init(GLsizeiptr vboSize, GLsizeiptr iboSize) {
		if (!vbo.Create(GL_ARRAY_BUFFER, vboSize)) {
			return false;
		}
		if (!ibo.Create(GL_ELEMENT_ARRAY_BUFFER, iboSize)) {
			return false;
		}
		progStaticMesh = Shader::Program::Create("Res/StaticMesh.vert", "Res/StaticMesh.frag");
		if (progStaticMesh->IsNull()) {
			return false;
		}
		vboEnd = 0;
		iboEnd = 0;
		files.reserve(100);

		Addcube("Cube");

		return true;
	}

/**
*���_�f�[�^��ǉ�����
*/
	 GLintptr Buffer::AddVertexData(const void*data, size_t size) {
		 vbo.BufferSubData(vboEnd, size, data);
		 const GLintptr tmp = vboEnd;
		 vboEnd += size;
		 return tmp;
	 }
	 /**
	 *�C���f�b�N�X�f�[�^��ǉ�����
	 */
	 GLintptr Buffer::AddIndexData(const void*data, size_t size) {
		 ibo.BufferSubData(iboEnd, size, data);
		 const GLintptr tmp = iboEnd;
		 iboEnd += size;
		 return tmp;
	 }

	 /**
	 *�v���~�e�B�u���쐬����
	 */
	 Primitive Buffer::CreatePrimitive(size_t count, GLenum type, size_t iOffset, size_t vOffset)const {
		 //�v���~�e�B�u�p��VAO���쐬
		 std::shared_ptr<VertexArrayObject>vao = std::make_shared<VertexArrayObject>();
		 vao->Create(vbo.Id(), ibo.Id());
		 vao->Bind();
		 vao->VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, position));
		 vao->VertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, texCoord));
		 vao->VertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, normal));
		 vao->Unbind();

		 //�v���~�e�B�u�̃����o�֐���ݒ�
		 Primitive p;
		 p.mode = GL_TRIANGLES;
		 p.count = static_cast<GLsizei>(count);
		 p.type = type;
		 p.indices = reinterpret_cast<const GLvoid*>(iOffset);
		 p.baseVertex = vOffset / sizeof(Vertex);
		 p.vao = vao;
		 p.material = 0;//�}�e���A����0�ӂŌŒ�

		 return p;
	 }

	 /**
	 *�}�e���A�����쐬����
	 */
	 Material Buffer::CreateMaterial(const glm::vec4&color, Texture::Image2DPtr texture)const {
		 Material m;
		 m.baseColor = color;
		 m.texture = texture;
		 m.program = progStaticMesh;
		 return m;
	 }

	 /**
	 *���b�V����ǉ�����
	 */
	 bool Buffer::AddMesh(const char*name, const Primitive&primitive, const Material&material) {
		 if (files.find(name)!=files.end()) {
			 std::cerr << "[�x��]" << __func__ << ":" << name << "�Ƃ������O�͒ǉ�����Ă��܂�.\n";
			 return false;
		 }

		 FilePtr p = std::make_shared<File>();
		 p->name = name;
		 p->materials.push_back(material);
		 p->meshes.resize(1);
		 p->meshes[0].name = name;
		 p->meshes[0].primitives.push_back(primitive);

		 files.insert(std::make_pair(p->name, p));
		 std::cout << "[���]" << __func__ << ":���b�V��'" << name << "'��ǉ�.\n";
		 return true;
	 }

	 /**
	 * ���_������ݒ肷��.
	 *
	 * @param prim        ���_�f�[�^��ݒ肷��v���~�e�B�u.
	 * @param index       �ݒ肷�钸�_�����̃C���f�b�N�X.
	 * @param accessor    ���_�f�[�^�̊i�[���.
	 * @param bufferViews ���_�f�[�^���Q�Ƃ��邽�߂̃o�b�t�@�E�r���[�z��.
	 * @param binFiles    ���_�f�[�^���i�[���Ă���o�C�i���f�[�^�z��.
	 *
	 * @retval true  �ݒ萬��.
	 * @retval false �ݒ莸�s.
	 */
	 bool Buffer::SetAttribute(Primitive*prim, int index, const json11::Json&accessor,
		 const json11::Json&bufferViews, const std::vector<std::vector<char>> &binFiles) {
		 if (accessor.is_null()) {
			 std::cerr << "[�G���[]" << __func__ << ":�A�N�Z�b�T��null�ł�\n";
			 return true;
		 }
		 static const char*const typeNameList[] = { "SCALAR","VEC2","VEC3","VEC4" };
		 static const int typeSizeList[] = { 1,2,3,4 };
		 const std::string&type = accessor["type"].string_value();
		 int size = -1;
		 for(size_t i=0;i<4;++i){
			 if (type == typeNameList[i]) {
				 size = typeSizeList[i];
				 break;
			 } 
		 }
		 if (size < 0) {
			 std::cerr << "[�G���[]" << __func__ << ":" << type << "�͒��_�����ɐݒ�ł��܂���.\n";
			 return false;
		 }
		 const void*p;
		 size_t byteLength;
		 int byteStride;
		 GetBuffer(accessor, bufferViews, binFiles, &p, &byteLength, &byteStride);
		 const GLenum componentType = accessor["componentType"].int_value();
		 prim->vao->Bind();
		 prim->vao->VertexAttribPointer(index, size, componentType, GL_FALSE, byteStride, vboEnd);
		 prim->vao->Unbind();

		 vbo.BufferSubData(vboEnd, byteLength, p);
		 vboEnd += ((byteLength + 3) / 4) * 4;//4�o�C�g����ɐ���
		 return true;
	 }
/**
*gltf�t�@�C���ɓǂݍ���
*/
	 bool Buffer::LoadMesh(const char*path) {

		 //glTF�t�@�C����ǂݍ���
		 std::vector<char>gltfFile = ReadFile(path);
		 if (gltfFile.empty()) {
			 return false;
		 }
		 gltfFile.push_back('\0');

		 //JSON���
		 std::string error;
		 const json11::Json json = json11::Json::parse(gltfFile.data(), error);
		 if (!error.empty()) {
			 std::cerr << "[�G���[]" << __func__ <<": " << path << "�̓ǂݍ��݂Ɏ��s���܂���.\n" << error << "\n";
			 return false;
		 }

		 //�o�C�i���t�@�C����ǂݍ���
		 std::vector<std::vector<char>>binFiles;
		 for (const json11::Json&buffer : json["buffers"].array_items()) {
			 const json11::Json&uri = buffer["uri"];
			 if (!uri.is_string()) {
				 std::cerr << "[�G���[]" << __func__ << ": " << path << "�ɕs����uri������܂�.\n";
				 return false;
			 }
			 const std::string binPath = std::string("Res/") + uri.string_value();
			 binFiles.push_back(ReadFile(binPath.c_str()));
			 if (binFiles.back().empty()) {
				 return false;
			 }
		 }

		 FilePtr pFile = std::make_shared<File>();
		 File&file = *pFile;

		 //���_�f�[�^�ƃC���f�b�N�X�f�[�^��GPU�������֓]��
		 const json11::Json&accessors = json["accessors"];
		 const json11::Json&bufferViews = json["bufferViews"];
		 file.meshes.reserve(json["meshes"].array_items().size());
		 for (const auto&currentMesh : json["meshes"].array_items()) {
			 Mesh mesh;
			 mesh.name = currentMesh["name"].string_value();
			 const std::vector<json11::Json>&primitves = currentMesh["primitives"].array_items();
			 mesh.primitives.resize(primitves.size());
			 for (size_t primId = 0; primId < primitves.size(); ++primId) {
				 const json11::Json&primitive = currentMesh["primitives"][primId];

				 //���_�C���f�b�N�X
				 {
					 const int accessorId_index = primitive["indices"].int_value();
					 const json11::Json&accessor = accessors[accessorId_index];
					 if (accessor["type"].string_value() != "SCALAR") {
						 std::cerr << "ERROR:�C���f�b�N�X�f�[�^�^�C�v��SCALAR�łȂ��Ă͂Ȃ�܂���\n";
						 std::cerr << "type=" << accessor["type"].string_value() << "\n";
						 return false;
					 }
					 mesh.primitives[primId].mode = primitive["mode"].is_null() ? GL_TRIANGLES : primitive["mode"].int_value();
					 mesh.primitives[primId].count = accessor["count"].int_value();
					 mesh.primitives[primId].type = accessor["componentType"].int_value();
					 mesh.primitives[primId].indices = reinterpret_cast<const GLvoid*>(iboEnd);

					 const void*p;
					 size_t byteLength;
					 GetBuffer(accessor, bufferViews, binFiles, &p, &byteLength);
					 ibo.BufferSubData(iboEnd, byteLength, p);
					 iboEnd += ((byteLength + 3) / 4) * 4;//���ɗ���̂��ǂ̃f�[�^�^�ł����v�Ȃ悤��4�o�C�g���E����.
				 }

				 //���_����
				 const json11::Json&attributes = primitive["attributes"];
				 const int accessorId_position = attributes["POSITION"].int_value();
				 const int accessorId_normal = attributes["NORMAL"].is_null() ? -1 : attributes["NORMAL"].int_value();
				 const int accessorId_texcoord = attributes["TEXCOORD_0"].is_null() ? -1 : attributes["TEXCOORD_0"].int_value();
				 mesh.primitives[primId].vao = std::make_shared<VertexArrayObject>();
				 mesh.primitives[primId].vao->Create(vbo.Id(),ibo.Id());
				 SetAttribute(&mesh.primitives[primId], 0, accessors[accessorId_position], bufferViews, binFiles);
				 SetAttribute(&mesh.primitives[primId], 1, accessors[accessorId_texcoord], bufferViews, binFiles);
				 SetAttribute(&mesh.primitives[primId], 2, accessors[accessorId_normal], bufferViews, binFiles);

				 mesh.primitives[primId].material = primitive["material"].int_value();

			 }
			 file.meshes.push_back(mesh);
		 }

		 //�}�e���A�����擾
		 {
			 const std::vector<json11::Json>materials = json["materials"].array_items();
			 file.materials.reserve(materials.size());
			 for (const json11::Json&material : materials) {
				 std::string texturePath;
				 const json11::Json&pbr = material["pbrMetallicRoughness"];
				 const json11::Json&index = pbr["baseColorTexture"]["index"];
				 if (index.is_number()) {
					 const int textureId = index.int_value();
					 const json11::Json&texture = json["textures"]["index"];
					 if (index.is_number()) {
						 const int textureId = index.int_value();
						 const json11::Json&texture = json["textures"][textureId];
						 const int imageSourceId = texture["source"].int_value();
						 const json11::Json&imageName = json["images"][imageSourceId]["name"];
						 if (imageName.is_string()) {
							 texturePath = std::string("Res/") + imageName.string_value() + ".tga";

						 }
					 }
					 glm::vec4 col(0, 0, 0, 1);
					 const std::vector<json11::Json>&baseColorFactor = pbr["baseColorFactor"].array_items();
					 if (baseColorFactor.size() >= 4) {
						 for (size_t i = 0; i < 4; ++i) {
							 col[i] = static_cast<float>(baseColorFactor[i].number_value());
						 }
					 }
					 Texture::Image2DPtr tex;
					 if (!texturePath.empty()) {
						 tex = Texture::Image2D::Create(texturePath.c_str());
					 }
					 file.materials.push_back(CreateMaterial(col, tex));
				 }
			 }
			 file.name = path;
			 files.insert(std::make_pair(file.name, pFile));

			 std::cout << "[INFO]" << __func__ << ": " << path << "'��ǂݍ��݂܂���.\n";
			 for (size_t i = 0; i < file.meshes.size(); ++i) {
				 std::cout << "mesh[" << i << "]=" << file.meshes[i].name << "\n";
			 }
		 }
		 return true;
	 }

	 /**
	 *�t�@�C�����擾����
	 */
	 FilePtr Buffer::GetFile(const char*name)const {
		 const auto itr = files.find(name);
		 if (itr == files.end()) {
			 std::cerr << "[�x��]" << __func__ << ":" << name << "�Ƃ������O�̃��b�V���͒ǉ�����Ă��܂���.\n";
			 static FilePtr empty = std::make_shared<File>();
			 return empty;
		 }
		 return itr->second;
	 }

	 /**
	 *�����̂�ǉ�����
	 */
	 void Buffer::Addcube(const char*name) {

		 //    6---7      +Y -Z
		   //   /|  /|       |/
			   //  / 5-/-4  -X --*-- +X
			   // 3---2 /       /|
			   // |/  |/     +Z -Y
			   // 0---1
			 const glm::vec3 basePositions[] = {
			 {-1,-1, 1}, { 1,-1, 1}, { 1, 1, 1}, {-1, 1, 1},
			 { 1,-1,-1}, {-1,-1,-1}, {-1, 1,-1}, { 1, 1,-1},
			 };
		 const glm::vec2 baseTexCoords[] = { { 0, 1}, { 0, 0}, { 1, 0}, { 1, 1} };
		 const glm::vec3 normals[] = {
		 { 0, 0, 1}, { 1, 0, 0}, { 0, 0,-1}, {-1, 0, 0},
		 { 0, -1, 0}, { 0, 1, 0} };
		 const int planes[6][4] = {
		 { 0, 1, 2, 3}, { 1, 4, 7, 2}, { 4, 5, 6, 7}, { 5, 0, 3, 6},
		 { 5, 4, 1, 0}, { 3, 2, 7, 6} };
		 const GLubyte baseIndices[] = { 0, 1, 2, 2, 3, 0 };
		 
		 std::vector<Vertex> vertices;
		 vertices.reserve(4 * 6);
		 std::vector<GLubyte> indices;
		 indices.reserve(6 * 6);

		 // ���_�f�[�^�ƃC���f�b�N�X�f�[�^���쐬.
		 for (size_t plane = 0; plane < 6; ++plane) {
			for (size_t i = 0; i < 4; ++i) {
				 Vertex v;
				 v.position = basePositions[planes[plane][i]];
				 v.texCoord = baseTexCoords[i];
				 v.normal = normals[plane];
				 vertices.push_back(v);
				 
			 }
			 for (size_t i = 0; i < 6; ++i) {
				 indices.push_back(static_cast<GLubyte>(baseIndices[i] + (plane * 4)));
				 
			 }
			 
		 }
		 // ���b�V����ǉ�.
		 const size_t vOffset =
		     AddVertexData(vertices.data(), vertices.size() * sizeof(Vertex));
		 const size_t iOffset =
			 AddIndexData(indices.data(), indices.size() * sizeof(GLubyte));
		 const Primitive p =
			 CreatePrimitive(indices.size(), GL_UNSIGNED_BYTE, iOffset, vOffset);
		 const Material m = CreateMaterial(glm::vec4(1), nullptr);
		 
		AddMesh(name, p, m);
		 
	 }

	 /**
	 *�V�F�[�_�[�Ƀr���[�E�v���W�F�N�V�����s���ݒ肷��
	 *@param matVP �r���[�E�v���W�F�N�V�����s��
	 */
	 void Buffer::SetViewProjectionMatrix(const glm::mat4&matVP)const {
		 progStaticMesh->Use();
		 progStaticMesh->SetViewProjectionMatrix(matVP);
		 glUseProgram(0);
	 }

	 /**
	 *���b�V����`�悷��
	 */
	 void Draw(const FilePtr& file, const glm::mat4& matM)
		  {
		 if (!file || file->meshes.empty() || file->materials.empty()) {
			 return;
			 
		 }
		 
			const Mesh& mesh = file->meshes[0];
		 for (const Primitive& p : mesh.primitives) {
			if (p.material < file->materials.size()) {
				 p.vao->Bind();
				 const Material& m = file->materials[p.material];
				 m.program->Use();
				// m.program->SetViewProjectionMatrix(matVP);
				 m.program->SetModelMatrix(matM);
				 glActiveTexture(GL_TEXTURE0);
				 
					       // �e�N�X�`��������Ƃ��́A���̃e�N�X�`��ID��ݒ肷��. �Ȃ��Ƃ���0��ݒ肷��.
					 if (m.texture) {
					 glBindTexture(GL_TEXTURE_2D, m.texture->Get());
					 
				 }
					 else {
					 glBindTexture(GL_TEXTURE_2D, 0);
					 
				 }
				 
					 glDrawElementsBaseVertex(p.mode, p.count, p.type, p.indices, p.baseVertex);
				 p.vao->Unbind();
				 
			 }
			 
		 }
		 glActiveTexture(GL_TEXTURE0);
		 glBindTexture(GL_TEXTURE_2D, 0);
		 glUseProgram(0);
	 }



}//namespace Mesh