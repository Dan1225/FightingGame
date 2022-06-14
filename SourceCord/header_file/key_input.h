#pragma once

//////// キー入力ビット割り当て ////////
#define KEY_UP			0x000000000000001
#define KEY_DOWN		0x000000000000002
#define KEY_RIGHT		0x000000000000004
#define KEY_LEFT		0x000000000000008
#define KEY_START		0x000000000000010
#define KEY_ENTER		0x000000000000020
#define KEY_A			0x000000000000040
#define KEY_B			0x000000000000080
#define KEY_C			0x000000000000100
#define KEY_D			0x000000000000200
#define KEY_E			0x000000000000400
#define KEY_F			0x000000000000800
#define KEY_G			0x000000000001000
#define KEY_H			0x000000000002000
#define KEY_I			0x000000000004000
#define KEY_J			0x000000000008000
#define KEY_K			0x000000000010000
#define KEY_L			0x000000000020000
#define KEY_M			0x000000000040000
#define KEY_N			0x000000000080000
#define KEY_O			0x000000000100000
#define KEY_P			0x000000000200000
#define KEY_Q			0x000000000400000
#define KEY_R			0x000000000800000
#define KEY_S			0x000000001000000
#define KEY_T			0x000000002000000
#define KEY_U			0x000000004000000
#define KEY_V			0x000000008000000
#define KEY_W			0x000000010000000
#define KEY_X			0x000000020000000
#define KEY_Y			0x000000040000000
#define KEY_Z			0x000000080000000

#define KEY_1			0x000000100000000
#define KEY_2			0x000000200000000
#define KEY_3			0x000000400000000
#define KEY_4			0x000000800000000
#define KEY_5			0x000001000000000
#define KEY_6			0x000002000000000
#define KEY_7			0x000004000000000
#define KEY_8			0x000008000000000
#define KEY_9			0x000010000000000
#define KEY_0			0x000020000000000
class KeyInput
{
private:
	static INT64 keyState;
	static INT64 keyTrigger;
	static INT64 keyRelease;

public:
	static void Update();
	static INT64 KeyState() { return keyState; }
	static INT64 KeyTrigger() { return keyTrigger; }
	static INT64 KeyRelease() { return keyRelease; }

};

using GamePadButton = unsigned int;

class GamePad
{
public:
	static const GamePadButton BTN_UP    = (1 << 0);
	static const GamePadButton BTN_RIGHT = (1 << 1);
	static const GamePadButton BTN_DOWN  = (1 << 2);
	static const GamePadButton BTN_LEFT  = (1 << 3);
	static const GamePadButton BTN_A     = (1 << 4);
	static const GamePadButton BTN_B     = (1 << 5);
	static const GamePadButton BTN_X     = (1 << 6);
	static const GamePadButton BTN_Y     = (1 << 7);
	static const GamePadButton BTN_START = (1 << 8);
	static const GamePadButton BTN_BACK  = (1 << 9);
	static const GamePadButton BTN_LS    = (1 << 10);
	static const GamePadButton BTN_RS    = (1 << 11);
	static const GamePadButton BTN_LB    = (1 << 12);
	static const GamePadButton BTN_RB    = (1 << 13);
	static const GamePadButton BTN_LT    = (1 << 14);
	static const GamePadButton BTN_RT    = (1 << 15);

public:
	//更新
	void Update();

	//スロット設定
	void SetSlot(int slot) { this->slot = slot; }
	// ボタン入力状態の取得
	GamePadButton GetButton() const { return buttonState[0]; }

	// ボタン押下状態の取得
	GamePadButton GetButtonDown() const { return buttonDown; }

	// ボタン押上状態の取得
	GamePadButton GetButtonUp() const { return buttonUp; }

	// 左スティックX軸入力状態の取得
	float GetAxisLX() const { return axisLx; }

	// 左スティックY軸入力状態の取得
	float GetAxisLY() const { return axisLy; }

	// 右スティックX軸入力状態の取得
	float GetAxisRX() const { return axisRx; }

	// 右スティックY軸入力状態の取得
	float GetAxisRY() const { return axisRy; }

	// 左トリガー入力状態の取得
	float GetTriggerL() const { return triggerL; }

	// 右トリガー入力状態の取得
	float GetTriggerR() const { return triggerR; }

private:
	GamePadButton		buttonState[2] = { 0 };
	GamePadButton		buttonDown = 0;
	GamePadButton		buttonUp = 0;
	float				axisLx = 0.0f;
	float				axisLy = 0.0f;
	float				axisRx = 0.0f;
	float				axisRy = 0.0f;
	float				triggerL = 0.0f;
	float				triggerR = 0.0f;
	int					slot = 0;
};

