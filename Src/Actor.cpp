/**
*@file.Actor.cpp
*/
#include "Actor.h"
#include<glm\gtc\matrix_transform.hpp>

/**
*コンストラクタ
*
*@param name アクターの名前
*@param health 耐久力
*@param position 位置
*@param rotation 回転
*@param scale 拡大率
*
*指定された名前、耐久力、位置、回転、拡大率によってアクターを初期化する
*/

Actor::Actor(const std::string&name, int health, const glm::vec3&positon, const glm::vec3&rotation, const glm::vec3&scale) :name(name), health(health), position(position), rotation(rotation), scale(scale) {
	
}

/**
*アクターの状態を更新する
*@param deltaTime経過時間
*UodateDrawData()より前に実行すること
*/
void Actor::Update(float deltaTime) {
	position += velocity * deltaTime;
}


/**
*描画情報の更新
@param deltaTime 経過時間
*Update()の後で実行すること
*/
void Actor::UpdateDrawData(float deltaTime) {

}

/**
*アクターの描画
*/
void Actor::Draw() {

}

