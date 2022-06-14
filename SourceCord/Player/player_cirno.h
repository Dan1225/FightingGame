#include "character_manager.h"
//enum cirno_inputs
//{
//    IN_LEFT_DOWN,
//    IN_LEFT_UP,
//
//    IN_LEFT_AND_CROUCH,
//    IN_LEFT_AND_JUMP,
//
//    IN_RIGHT_DOWN,
//    IN_RIGHT_UP,
//
//    IN_RIGHT_AND_CROUCH,
//    IN_RIGHT_AND_JUMP,
//
//    IN_JUMP,
//
//    IN_CROUCH_DOWN,
//    IN_CROUCH_UP,
//
//    IN_JUMP_AND_CROUCH,
//
//    IN_IDLE,
//
//    IN_DEFENDING,
//    IN_CROUCH_DEFENDING,
//
//    IN_L_PUNCH,
//    IN_L_KIK,
//    IN_M_PUNCH,
//    IN_M_KIK,
//    IN_F_PUNCH,
//    IN_F_KIK,
//
//    IN_HEAD_REEL,
//    IN_GUT_REEL,
//    IN_CROUCH_REEL,
//    IN_FALLING,
//    IN_SWEEP,
//    IN_BURNING,
//
//    IN_CROUCHING_FINISH,
//    IN_STANDING_FINISH,
//    IN_JUMP_FINISH,
//    IN_PUNCH_FINISH,
//    IN_KICK_FINISH,
//    IN_DEFENDING_FINISH,
//
//    IN_FALLING_FINISH,
//    IN_GETTING_UP_FINISH,
//    IN_SWEEP_FINISH,
//    IN_BURNING_FINISH,
//
//    IN_L_YFIRE,
//    IN_M_YFIRE,
//    IN_F_YFIRE,
//
//    IN_L_YFLAME,
//    IN_M_YFLAME,
//    IN_F_YFLAME,
//
//    IN_YFIRE_FINISH,
//    IN_YFLAME_FINISH,
//    IN_YDRILL_FINISH,
//    IN_YMUMMY_FINISH,
//
//    IN_TURNING,
//    IN_TURNING_FINISH,
//
//    IN_VICTORY,
//    IN_LOOSE,
//    IN_END
//};

class Player_Cirno : public Character
{
private:
	std::unique_ptr<Sprite> primitive;
public:
	Frame Cirno_Frame;
	Animation Idle1, Idle2, Forward, Backward, JumpAnim;//待機1 待機2 前進 後退
	Animation crouching, standing, crouch;    //しゃがむまで 立つまで しゃがみ
	Animation lp, mp, hp;                     //弱P 中P 強P
	Animation low_damage;                     //ダメージアニメーション
	Animation round_emote;                    //ラウンド開始時のアニメーション

	Animation* Current_animation;

	enum cirno_states
	{
		Cirno_ST_UNKNOWN,
		Cirno_BUTTON_INIT,
		Cirno_WAIT1,           //待機1
		Cirno_WAIT2,           //待機2
		Cirno_BACK_JUMP,       //バックジャンプ
		Cirno_FORWARD_JUMP,    //前ジャンプ
		Cirno_CROUCHING,       //しゃがみ移行
		Cirno_CROUCH,          //しゃがみ中
		Cirno_FORWARD_MOVE,    //前進
		Cirno_BACKWARD_MOVE,   //後退
		Cirno_FORWARD_DUSH,    //前ダッシュ
		Cirno_BACK_DUSH,       //バックダッシュ
		Cirno_DAMAGE1,         //やられ1
		Cirno_DAMAGE2,         //やられ2
		Cirno_CROUCHING_DAMAGE,//しゃがみやられ
		Cirno_STANDING_GUARD,  //立ちガード
		Cirno_CROUCHING_GUARD, //しゃがみガード
		Cirno_STANDING_LIGHT,  //立ち弱パンチ
		Cirno_STANDING_MID,    //立ち中キック
		Cirno_CROUCHING_LIGHT, //しゃがみ弱キック
		Cirno_SHOT,            //弾
		Cirno_STANDING_HEAVY,  //立ち強パンチ
		Cirno_CROUCHING_MID,   //しゃがみ中パンチ
		Cirno_CROUCHING_HEAVY, //しゃがみ強パンチ
	};

	cirno_states State;                       //プレイヤーの「状態」

	//std::shared_ptr<CharacterManager>& GetChara() { return Chara; }
	Frame& GetCharaFrame() { return Cirno_Frame; }
	//DirectX::XMFLOAT2& GetPos();
	Attack_Rect& GetAttack();

	Player_Cirno(ID3D11Device* device);
	~Player_Cirno() {}
	void Init()                             override; //変数の初期化や設定
	void RoundInit()                        override; //ラウンドが変わる際に特定のパラメータのみ初期化するための変数
	void Update(float elapsedTime)          override; //更新処理
	void Draw(ID3D11DeviceContext* context) override; //描画関数
	void Action()                           override; //ボタン入力とキャラの動き
	void Attack()                           override; //攻撃判別関数(現段階では攻撃別に当たり判定の大きさを可視化)
	void Jump(float JunmPower)              override; //ジャンプ関数
	void Damage()                           override; //ダメージを受けたときの処理
	void Render_ImGui()                     override; //ImGuiの描画
	void Debug_Draw(ID3D11DeviceContext* context) override; //デバッグ用当たり判定の表示・非表示をまとめた関数
};