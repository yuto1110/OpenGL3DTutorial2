#ifndef TITLESCENE_H_INCLUDED
#define TITLESCENE_H_INCLUDED
#include "Scene.h"
#include"Sprite.h"
#include<vector>

class TitleScene :public Scene {
public:
	TitleScene():Scene("TitleScene"){}
	virtual ~TitleScene() = default;

	virtual bool Initialize() override ;
	virtual void ProcessInput() override;
	virtual void Update(float) override;
	virtual void Render() override;
	virtual void Finalize() override{}

private:
	std::vector<Sprite>sprites;
	SpriteRenderer spriteRenderer;
};
#endif // !TITLESCENE_H_INCLLUDED
