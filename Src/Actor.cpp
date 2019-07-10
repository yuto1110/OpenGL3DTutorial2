/**
*@file.Actor.cpp
*/
#include "Actor.h"
#include<glm\gtc\matrix_transform.hpp>

/**
*�R���X�g���N�^
*
*@param name �A�N�^�[�̖��O
*@param health �ϋv��
*@param position �ʒu
*@param rotation ��]
*@param scale �g�嗦
*
*�w�肳�ꂽ���O�A�ϋv�́A�ʒu�A��]�A�g�嗦�ɂ���ăA�N�^�[������������
*/

Actor::Actor(const std::string&name, int health, const glm::vec3&positon, const glm::vec3&rotation, const glm::vec3&scale) :name(name), health(health), position(position), rotation(rotation), scale(scale) {
	
}

/**
*�A�N�^�[�̏�Ԃ��X�V����
*@param deltaTime�o�ߎ���
*UodateDrawData()���O�Ɏ��s���邱��
*/
void Actor::Update(float deltaTime) {
	position += velocity * deltaTime;
}


/**
*�`����̍X�V
@param deltaTime �o�ߎ���
*Update()�̌�Ŏ��s���邱��
*/
void Actor::UpdateDrawData(float deltaTime) {

}

/**
*�A�N�^�[�̕`��
*/
void Actor::Draw() {

}

