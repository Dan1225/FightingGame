//#include <memory>
//#include "sprite.h"
//#include "animation.h"
//#include "collision.h"
//#include "character.h"
//
//
//enum tei_states
//{
//	Tei_ST_UNKNOWN,
//	Tei_BUTTON_INIT,     //ボタン受付
//	Tei_WAIT,            //待機
//	Tei_FORWRAD_JUMP,    //前ジャンプ
//	Tei_BACK_JUMP,       //バックジャンプ
//	Tei_CROUCHING,       //しゃがみ移行
//	Tei_CROUCH,          //しゃがみ中
//	Tei_FORWARD_MOVE,    //前進
//	Tei_BACKWARD_MOVE,   //後退
//	Tei_FORWARD_DUSH,    //前ダッシュ
//	Tei_BACK_DUSH,       //バックダッシュ
//	Tei_DAMAGE1,         //やられ1
//	Tei_DAMAGE2,         //やられ2
//	Tei_DAMAGE3,         //やられ2
//	Tei_CROUCHING_DAMAGE1,//しゃがみやられ1
//	Tei_CROUCHING_DAMAGE2,//しゃがみやられ2
//	Tei_STANDING_GUARD,  //立ちガード
//	Tei_CROUCHING_GUARD, //しゃがみガード
//};
//
//
//class Player_Tei : public Character
//{
//private:
//	std::unique_ptr<Sprite> player_tei;
//	std::unique_ptr<Sprite> primitive;
//
//public:
//	static constexpr float right_offset = 13;//画像の切り抜きと押し出し判定矩形との差(判定用)
//	static constexpr float left_offset = 17; //画像の切り抜きと押し出し判定矩形との差(判定用)
//
//	bool colliding;
//	Frame Tei_Frame;
//	Animation* current_animation;
//	Animation Idle, forward, backward;//待機 前進 後退
//	Animation crouching, standing, crouch;    //しゃがむまで 立つまで しゃがみ
//
//	tei_states State;
//
//	static Player_Tei* Tei_inst;
//	static Player_Tei& Instance() { return *Tei_inst; }
//
//public:
//	//ゲッター
//	DirectX::XMFLOAT2& GetPos() { return Pos; }
//	DirectX::XMFLOAT2& GetSize() { return Size; }
//	Push_Rect& GetCollision() { return Push_Col; }
//	Frame& GetCharaFrame() { return Tei_Frame; }
//
//	Player_Tei(ID3D11Device* device);
//	~Player_Tei() {}
//	void Init() override;
//	void Update(float elapsedTime) override;
//	void Draw() override;
//	void Action() override;       //ボタン入力とキャラの動き
//	//void Jump() override;         //ジャンプ
//	//void Wall();         //キャラと壁(枠外)との判定
//	void Render_ImGui() override;
//	void Debug_Mode() override;   //矩形表示
//};