/**
*@file Scene.cpp
*/
#include "Scene.h"
#include <iostream>
/**
*�R���X�g���N�^
*/
Scene::Scene(const char*name) :name(name) {
	std::cout << "Scene �R���X�g���N�^:" << name << "\n";

}

/**
*�f�X�g���N�^
*/
Scene::~Scene() {
	Finalize();
	std::cout << "Scene �f�X�g���N�^:" << name << "\n";
}

/**
*�V�[����������Ԃɂ���
*/
void Scene::Play() {
	isActive = true;
	std::cout << "Scene Play:" << name << "\n";
}

/**
*�V�[�����~��Ԃɂ���
*/
void Scene::Stop() {
	isActive = false;
	std::cout << "Scene Stop:" << name << "\n";
}

/**
*�V�[����\������
*/
void Scene::Show() {
	isVisible = true;
	std::cout << "Scene Show:" << name << "\n";
}

/**
*�V�[�����\���ɂ���
*/
void Scene::Hide() {
	isVisible = false;
	std::cout << "Scene Hide:" << name << "\n";
}

/**
*�V�[�����擾����
*/
const std::string&Scene::Name()const {
	return name;
}

/**
*�V�[���̊�����Ԃ𒲂ׂ�
*/
bool Scene::IsActive() const {
	return isActive;
}

/**
*�V�[���̕\����Ԃ𒲂ׂ�
*/
bool Scene::IsVisible()const {
	return isVisible;
}

/**
*�V�[���X�^�b�N���擾����
*
*@return �V�[���X�^�b�N
*/

SceneStack&SceneStack::Instance() {
	static SceneStack instance;
	return instance;
}

/**
*�R���X�g���N�^
*/
SceneStack::SceneStack() {
	stack.reserve(16);
}

/**
*�V�[�����v�b�V������
*
*@param p�@�V�����V�[��
*/
void SceneStack::push(ScenePtr p) {
	if (!stack.empty()) {
		Current().Stop();
	}
	stack.push_back(p);
	std::cout << "[�V�[�� �v�b�V��]" << p->Name() << "\n";
	Current().Initialize();
	Current().Play();
}

/**
*�V�[�����|�b�v����
*/
void SceneStack::pop() {
	if (stack.empty()) {
		std::cout << "[�V�[���|�b�v][�x��]�V�[���X�^�b�N����ł�.\n";
		return;
	}
	Current().Stop();
	Current().Finalize();
	const std::string sceneName = Current().Name();
	stack.pop_back();
	std::cout << "[�V�[���|�b�v]" << sceneName << "\n";
	if (!stack.empty()) {
		Current().Play();
	}
}

/**
*�V�[����u��������
*@param p �V�����V�[��
*/
void SceneStack::Replace(ScenePtr p) {
	std::string sceneName = "(Empty)";
	if (stack.empty()) {
		std::cout << "[�V�[�����v���[�X][�x��]�V�[���X�^�b�N����ł�.\n";
	}
	else {
		sceneName = Current().Name();
		Current().Stop();
		Current().Finalize();
		stack.pop_back();
	}
	stack.push_back(p);
	std::cout << "[�V�[�����v���[�X]" << sceneName << "->" << p->Name() << "\n";
	Current().Initialize();
	Current().Play();
}
/**
*���݂̃V�[�����擾����
*/

Scene&SceneStack::Current() {
	return*stack.back();

}
/**
*���݂̃V�[�����擾����
*/
const Scene&SceneStack::Current() const {
	return*stack.back();
}

/**
*�V�[���̐����擾����
*/

size_t SceneStack::Size() const {
	return stack.size();

}

/**
*�X�^�b�N���󂩂ǂ������ׂ�
*/
bool SceneStack::Empty() const {
	return stack.empty();
}

/**
*�V�[�����X�V����
*/
void SceneStack::Update(float deltaTime) {
	for (ScenePtr&e : stack) {
		e->ProcessInput();
	}
	for (ScenePtr&e : stack) {
		e->Update(deltaTime);
	}
}

/**
*�V�[����`�悷��
*/
void SceneStack::Render() {
	for (ScenePtr&e:stack) {
		if (e->IsVisible()) {
			e->Render();
		}
	}
}