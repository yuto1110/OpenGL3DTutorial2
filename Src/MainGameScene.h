#ifndef MAINGAMESCENE_H_INCLUDED
#define MAINGAMESCENE_H_INCLUDED
#include "Scene.h"
#include"Sprite.h"
#include"Font.h"
#include<vector>


class MainGameScene :public Scene {
public:
	MainGameScene():Scene("MainGameScene"){}
	virtual~MainGameScene() = default;

	virtual bool Initialize() override;
	virtual void ProcessInput() override;
	virtual void Update(float) override;
	virtual void Render() override;
	virtual void Finalize() override{}

private:
	bool flag = false;
	std::vector<Sprite>sprites;
	SpriteRenderer spriteRenderer;
	FontRenderer fontRenderer;
};
#endif // !MAINGAMESCENE_H_INCLUDED
