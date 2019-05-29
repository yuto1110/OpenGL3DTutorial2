/**
*@ file GLFWEW.cpp
*/
#include "GLFWEW.h"
#include<iostream>
namespace GLFWEW {
/**
*GLFW����̃G���[�񍐂���������
*/
	void ErrorCallback(int error, const char*desc) {
		std::cerr << "ERROR:" << desc << std::endl;
	}

/**
*�V���O���g���C���X�^���X���擾����
*/
	Window&Window::Instance() {
		static Window instance;
		return instance;
	}


	/**
	*�R���X�g���N�^
	*/
	Window::Window() {

	}
	/**
	*�f�X�g���N�^
	*/
	Window::~Window() {
		if (isGLFWInitialized) {
			glfwTerminate();
		}
	}

	/**
	*GLFW/GLEW�̏�����
	*/
	bool Window::Init(int w, int h, const char*title) {
		if (isInitialized) {
			std::cerr << "ERROR:GLFWEW�͊��ɏ���������Ă��܂�." << std::endl;
			return false;
		}
		if (!isGLFWInitialized) {
			glfwSetErrorCallback(ErrorCallback);
			if (glfwInit() != GL_TRUE) {
				return false;
			}
			isGLFWInitialized = true;
		}
		if (!window) {
			window = glfwCreateWindow(w, h, title, nullptr, nullptr);
			if (!window) {
				return false;
			}
			glfwMakeContextCurrent(window);
		}
		if (glewInit() != GLEW_OK) {
			std::cerr << "ERROR:GLEW�̏������Ɏ��s���܂���." << std::endl;
			return false;
		}

		width = w;
		height = h;
		//OpenGL�̏����R���\�[���E�B���h�E�֏o�͂���
		const GLubyte*renderer = glGetString(GL_RENDERER);
		std::cout << "Renderer:" << renderer << std::endl;
		const GLubyte*version = glGetString(GL_VERSION);
		std::cout << "Version:" << version << std::endl;

		isInitialized = true;
		return true;
	}
	/**
	*�E�B���h�E�����ׂ������ׂ�
	*/
	bool Window::ShouoldClose() const {
		return glfwWindowShouldClose(window) != 0;
	}
	/**
	*�t�����g�o�b�t�@�ƃo�b�N�o�b�t�@��؂�ւ���
	*/
	void Window::SwapBuffers()const {
		glfwPollEvents();
		
		glfwSwapBuffers(window);
	}
	bool Window::IsKeyPressed(int key)const {
		return glfwGetKey(window, key) == GLFW_PRESS;
	}

	void Window::InitTimer() {
		glfwSetTime(0.0);
		previousTime = 0.0;
		deltaTime = 0.0;
	}
	void Window::UpdateTimer() {
		const double currentTime = glfwGetTime();
		deltaTime = currentTime - previousTime;
		previousTime = currentTime;
		const float upperLimit = 0.25f;
		if (deltaTime > upperLimit) {
			deltaTime = 1.0f / 60.0f;
		}
		UpdateGamePad();
	}

	double Window::DeltaTime()const {
		return deltaTime;
	}

	/**
	*�Q�[���p�b�h�̏�Ԃ��擾����
	*/
	const GamePad&Window::GetGamePad() const {
		return gamepad;
	}

	/**
	*�Q�[���p�b�h�̃A�i���O���͑��uID
	*/
	enum GAMEPAD_AXES {
		GAMEPAD_AXES_LEFT_X,
		GAMEPAD_AXES_LEFT_Y,
		GAMEPAD_AXES_TRIGGER,
		GAMEPAD_AXES_RIGHT_Y,
		GAMEPAD_AXES_RIGHT_X,
	};

	/**
	*�Q�[���p�b�h�̃f�W�^�����͑��uID
	*/
	enum GAMEPAD_BUTTON {
		GAMEPAD_BUTTON_A,
		GAMEPAD_BUTTON_B,
		GAMEPAD_BUTTON_X,
		GAMEPAD_BUTTON_Y,
		GAMEPAD_BUTTON_L,
		GAMEPAD_BUTTON_R,
		GAMEPAD_BUTTON_BACK,
		GAMEPAD_BUTTON_START,
		GAMEPAD_BUTTON_L_THUMB,
		GAMEPAD_BUTTON_R_THUMB,
		GAMEPAD_BUTTON_UP,
		GAMEPAD_BUTTON_RIGHT,
		GAMEPAD_BUTTON_DOWN,
		GAMEPAD_BUTTON_LEFT,

	};

	/**
	*�Q�[���p�b�h�̏�Ԃ��X�V����
	*/
	void Window::UpdateGamePad() {
		const uint32_t prevButtons = gamepad.buttons;

		//�A�i���O���͂ƃ{�^�����͂��擾
		int axesCount, buttonCount;
		const float*axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
		const uint8_t*buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1,&buttonCount);

		//�����̔z��nullptr�łȂ��A�Œ���K�v�ȃf�[�^���𖞂����Ă���ΗE�����ރQ�[���p�b�h���ڑ������
		if (axes&&buttons&&axesCount >= 2 && buttonCount >= 8) {
			//�L���ȃQ�[���p�b�h���ڑ�����Ă���ꍇ
			gamepad.buttons &= ~(GamePad::DPAD_UP | GamePad::DPAD_DOWN | GamePad::DPAD_LEFT | GamePad::DPAD_RIGHT);
			static const float digitalThreshold = 0.3f;
			if (axes[GAMEPAD_AXES_LEFT_Y] >= digitalThreshold) {
				gamepad.buttons |= GamePad::DPAD_UP;
			}
			else if (axes[GAMEPAD_AXES_LEFT_Y] <= -digitalThreshold) {
				gamepad.buttons |= GamePad::DPAD_DOWN;
			}
			if (axes[GAMEPAD_AXES_LEFT_X] >= digitalThreshold) {
				gamepad.buttons |= GamePad::DPAD_LEFT;
			}
			else if (axes[GAMEPAD_AXES_LEFT_X] <= -digitalThreshold) {
				gamepad.buttons |= GamePad::DPAD_RIGHT;
			}

			//�z��C���f�b�N�X��GamePad�L�[�̑Ή��\
			static const struct {
				int dataIndex;
				uint32_t gamepadBit;
			}keyMap[]={
				{GAMEPAD_BUTTON_A,GamePad::A},
				{GAMEPAD_BUTTON_B,GamePad::B},
			{ GAMEPAD_BUTTON_X,GamePad::X },
			{ GAMEPAD_BUTTON_Y,GamePad::Y },
			{ GAMEPAD_BUTTON_L,GamePad::L },
			{ GAMEPAD_BUTTON_R,GamePad::R },
			{ GAMEPAD_BUTTON_START,GamePad::START },
			{ GAMEPAD_BUTTON_UP,GamePad::DPAD_UP },
			{ GAMEPAD_BUTTON_DOWN,GamePad::DPAD_DOWN },
			{ GAMEPAD_BUTTON_LEFT,GamePad::DPAD_LEFT },
			{ GAMEPAD_BUTTON_RIGHT,GamePad::DPAD_RIGHT },
		};
			for (const auto&e : keyMap) {
				if (buttons[e.dataIndex] == GLFW_PRESS) {
					gamepad.buttons |= e.gamepadBit;
				}
				else if (buttons[e.dataIndex] == GLFW_RELEASE) {
					gamepad.buttons &= ~e.gamepadBit;
				}
			}
		}
		else {
			//�L���ȃQ�[���p�b�h���ڑ�����Ă��Ȃ��̂ŃL�[�{�[�h�œ���

			//�z��C���f�b�N�X��gamePad�L�[�̑Ή��\
			static const struct {
				int keyCode;
				uint32_t gamepadBit;
			}keyMap[]{
				{GLFW_KEY_J,GamePad::A},
			{ GLFW_KEY_K,GamePad::B },
			{ GLFW_KEY_U,GamePad::X },
			{ GLFW_KEY_I,GamePad::Y },
			{ GLFW_KEY_O,GamePad::L },
			{ GLFW_KEY_L,GamePad::R },
			{ GLFW_KEY_ENTER,GamePad::START },
			{ GLFW_KEY_W,GamePad::DPAD_UP },
			{ GLFW_KEY_A,GamePad::DPAD_LEFT },
			{ GLFW_KEY_S,GamePad::DPAD_DOWN},
			{ GLFW_KEY_D,GamePad::DPAD_RIGHT },
		};
			for (const auto&e : keyMap) {
				const int key = glfwGetKey(window, e.keyCode);
				if (key == GLFW_PRESS) {
					gamepad.buttons |= e.gamepadBit;
				}
				else if (key == GLFW_RELEASE) {
					gamepad.buttons &= ~e.gamepadBit;
				}
			}
		}
		//�O��̍X�V�ŉ�����Ă��Ȃ��č��񉟂���Ă���L�[�̏���buttonDown�Ɋi�[
		gamepad.buttonDown = gamepad.buttons&~prevButtons;
	}
}//namespace GLFWEW
