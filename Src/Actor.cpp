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

/**
*コンストラクタ
*@param m 表示するメッシュ
*@param name アクターの名前
*@param health 耐久力
*@param position 位置
*@param rotation　回転
*@param scale 拡大率
*指定されたメッシュ、名前、耐久力、位置、回転、拡大率によってアクターを初期化する
*/
StaticMeshActor::StaticMeshActor(const Mesh::FilePtr& m,
	const std::string& name, int health, const glm::vec3& position,
	const glm::vec3& rotation, const glm::vec3& scale)
	: Actor(name, health, position, rotation, scale), mesh(m) {

}

/**
*描画
*/
void StaticMeshActor::Draw() {
	if (mesh) {
		const glm::mat4 matT = glm::translate(glm::mat4(1), position);
		const glm::mat4 matR_Y = glm::rotate(glm::mat4(1), rotation.y, glm::vec3(0, 1, 0));
		const glm::mat4 matR_ZY = glm::rotate(matR_Y, rotation.z, glm::vec3(0, 0, -1));
		const glm::mat4 matR_XZY = glm::rotate(matR_ZY, rotation.x, glm::vec3(1, 0, 0));
		const glm::mat4 matS = glm::scale(glm::mat4(1), scale);
		const glm::mat4 matModel = matT * matR_XZY*matS;
		Mesh::Draw(mesh, matModel);
	}
}

/**
*格納可能なアクター数を確保する
*@param reserveCount アクター配列の確保数.
*/
void ActorList::Reserve(size_t reserveCount) {
	actors.reserve(reserveCount);
}

/**
*アクターを追加する
*@param actor 追加するアクター
*/
void ActorList::Add(const ActorPtr&actor) {
	actors.push_back(actor);
}

/**
*アクターを削除する
*@param actor 削除するアクター
*/

bool ActorList::Remove(const ActorPtr&actor) {
	for (auto itr = actors.begin(); itr != actors.end(); ++itr) {
		if (*itr == actor) {
			actors.erase(itr);
			return true;
		}
	}
	return false;
}

/**
*アクターの状態を更新する
*
*@param deltaTime 前回の更新からの経過時間
*/
void ActorList::Update(float deltaTime) {
	for (const ActorPtr&e : actors) {
		if (e&&e->health > 0) {
			e->Update(deltaTime);
		}
	}
}
/**
*アクターの描画データを更新する
*@param deltaTime　前回の更新からの経過時間
*/
void ActorList::UpdateDrawData(float deltaTime) {
	for (const ActorPtr&e : actors) {
		if (e&&e->health > 0) {
			e->UpdateDrawData(deltaTime);
		}
	}
}

/**
*Actorを描画する.
*/
void ActorList::Draw() {
	for (const ActorPtr&e : actors) {
		if (e&&e->health > 0) {
			e->Draw();
		}
	}
}