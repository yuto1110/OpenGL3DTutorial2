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