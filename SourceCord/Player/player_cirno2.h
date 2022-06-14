#include <memory>
#include "character_manager.h"

//#include "sprite.h"
//#include "animation.h"
//#include "character.h"
//#include "collision.h"
//#include "key_input.h"
////enum cirno2_inputs
////{
////    IN_LEFT_DOWN,
////    IN_LEFT_UP,
////
////    IN_LEFT_AND_CROUCH,
////    IN_LEFT_AND_JUMP,
////
////    IN_RIGHT_DOWN,
////    IN_RIGHT_UP,
////
////    IN_RIGHT_AND_CROUCH,
////    IN_RIGHT_AND_JUMP,
////
////    IN_JUMP,
////
////    IN_CROUCH_DOWN,
////    IN_CROUCH_UP,
////
////    IN_JUMP_AND_CROUCH,
////
////    IN_IDLE,
////
////    IN_DEFENDING,
////    IN_CROUCH_DEFENDING,
////
////    IN_L_PUNCH,
////    IN_L_KIK,
////    IN_M_PUNCH,
////    IN_M_KIK,
////    IN_F_PUNCH,
////    IN_F_KIK,
////
////    IN_HEAD_REEL,
////    IN_GUT_REEL,
////    IN_CROUCH_REEL,
////    IN_FALLING,
////    IN_SWEEP,
////    IN_BURNING,
////
////    IN_CROUCHING_FINISH,
////    IN_STANDING_FINISH,
////    IN_JUMP_FINISH,
////    IN_PUNCH_FINISH,
////    IN_KICK_FINISH,
////    IN_DEFENDING_FINISH,
////
////    IN_FALLING_FINISH,
////    IN_GETTING_UP_FINISH,
////    IN_SWEEP_FINISH,
////    IN_BURNING_FINISH,
////
////    IN_L_YFIRE,
////    IN_M_YFIRE,
////    IN_F_YFIRE,
////
////    IN_L_YFLAME,
////    IN_M_YFLAME,
////    IN_F_YFLAME,
////
////    IN_YFIRE_FINISH,
////    IN_YFLAME_FINISH,
////    IN_YDRILL_FINISH,
////    IN_YMUMMY_FINISH,
////
////    IN_TURNING,
////    IN_TURNING_FINISH,
////
////    IN_VICTORY,
////    IN_LOOSE,
////    IN_END
////};
class Player_Cirno2 : public Character
{
private:
    std::unique_ptr<Sprite> primitive;
public:
    static constexpr float right_offset = 13;//画像の切り抜きと押し出し判定矩形との差(判定用)
    static constexpr float left_offset = 17; //画像の切り抜きと押し出し判定矩形との差(判定用)

    bool colliding;

    Frame Cirno_Frame;
    Animation* Current_animation;
    Animation Idle1, Idle2, Forward, Backward;//待機1 待機2 前進 後退
    Animation crouching, standing, crouch;    //しゃがむまで 立つまで しゃがみ
    Animation lp, mp, hp;                     //弱P 中P 強P
    Animation low_damage;                     //やられダメージ

    enum cirno2_states
    {
        Cirno2_ST_UNKNOWN,
        Cirno2_BUTTON_INIT,
        Cirno2_WAIT1,           //待機1
        Cirno2_WAIT2,           //待機2
        Cirno2_BACK_JUMP,       //バックジャンプ
        Cirno2_FORWARD_JUMP,    //前ジャンプ
        Cirno2_CROUCHING,       //しゃがみ移行
        Cirno2_CROUCH,          //しゃがみ中
        Cirno2_FORWARD_MOVE,    //前進
        Cirno2_BACKWARD_MOVE,   //後退
        Cirno2_FORWARD_DUSH,    //前ダッシュ
        Cirno2_BACK_DUSH,       //バックダッシュ
        Cirno2_DAMAGE1,         //やられ1
        Cirno2_DAMAGE2,         //やられ2
        Cirno2_CROUCHING_DAMAGE,//しゃがみやられ
        Cirno2_STANDING_GUARD,  //立ちガード
        Cirno2_CROUCHING_GUARD, //しゃがみガード
        Cirno2_STANDING_LIGHT,  //立ち弱パンチ
        Cirno2_STANDING_MID,    //立ち中キック
        Cirno2_CROUCHING_LIGHT, //しゃがみ弱キック
        Cirno2_SHOT,            //弾
        Cirno2_STANDING_HEAVY,  //立ち強パンチ
        Cirno2_CROUCHING_MID,   //しゃがみ中パンチ
        Cirno2_CROUCHING_HEAVY, //しゃがみ強パンチ
    };

    cirno2_states State;                       //プレイヤーの「状態」

public:
    //ゲッター
    //std::shared_ptr<CharacterManager>& GetPlayer_Cirno2() { return Chara; }
    Attack_Rect& GetAttack();
    Frame& GetCharaFrame() { return Cirno_Frame; }
    Damage_Rect& GetDamage() { return hurt_box; }

    Player_Cirno2(ID3D11Device* device);
    ~Player_Cirno2() {}
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