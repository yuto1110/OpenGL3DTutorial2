#include"MainGameScene.h"
#include"StatusScene.h"
#include"GameOverScene.h"
#include"GLFWEW.h"

bool MainGameScene::Initialize() {
	spriteRenderer.Init(1000, "Res/Sprite.vert", "Res/Sprite.frag");
	sprites.reserve(100);
	Sprite spr(Texture::Image2D::Create("Res/asphalt.tga"));
	spr.Scale(glm::vec2(2));
	sprites.push_back(spr);
	fontRenderer.Init(1000);
	fontRenderer.LoadFromFile("Res/font.fnt");
	return true;
}
void MainGameScene::ProcessInput() {
	GLFWEW::Window&window = GLFWEW::Window::Instance();
	if (!flag) {
		
		if (window.GetGamePad().buttonDown&GamePad::START) {
			flag = true;
			SceneStack::Instance().push(std::make_shared<StatusScene>());
		}
	}
	else {
		
		if (window.GetGamePad().buttonDown&GamePad::START) {
			SceneStack::Instance().Replace(std::make_shared<GameOverScene>());
		}
	}
}
/**
*シーンを更新する
*/
void MainGameScene::Update(float deltaTime) {
	spriteRenderer.BeginUpdate();
	for (const Sprite&e : sprites) {
		spriteRenderer.AddVertices(e);
	}
	spriteRenderer.EndUpdate();

	const GLFWEW::Window& window = GLFWEW::Window::Instance();
	const float w = window.Width();
	const float h = window.Height();
	const float lineHeight = fontRenderer.LineHeight();
	fontRenderer.BeginUpdate();
	fontRenderer.AddString(glm::vec2(-w * 0.5f + 32, h * 0.5f - lineHeight), L"メイン画面");
	fontRenderer.AddString(glm::vec2(-128, 0), L"アクションゲーム");
	fontRenderer.EndUpdate();
}
/**
*シーンを描画する
*/

void MainGameScene::Render()
{
	const GLFWEW::Window& window = GLFWEW::Window::Instance();
	const glm::vec2 screenSize(window.Width(), window.Height());
	spriteRenderer.Draw(screenSize);
	fontRenderer.Draw(screenSize);
}