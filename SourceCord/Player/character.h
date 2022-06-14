#pragma once
#include <DirectXMath.h>
#include <memory>

#include "../header_file/animation.h"
#include "../header_file/collision.h"
#include "../Graphics/sprite.h"

class CharacterManager;

class Character
{
protected:
    std::shared_ptr<Sprite> texture;
    //std::shared_ptr<Character> Chara;
    float Angle;                    //回転
    float Scale;                    //拡大の倍率
    float Gravity;                  //プレイヤーにかかる重力
    float JumpPower;                //ジャンプの勢い
    float Ground;                   //地面になる座標の値(yのみ)
    float Hp;                       //プレイヤーの体力
    int   Win;                      //プレイヤーの勝利数(最大2ラウンド取得)
    DirectX::XMFLOAT2 Pos;          //プレイヤーの座標
    DirectX::XMFLOAT2 sPos;         //弾の座標
    DirectX::XMFLOAT2 JumpPos;      //プレイヤーの座標(ジャンプ中)
    DirectX::XMFLOAT2 Size;         //プレイヤーの大きさ(基本的 横：80,縦：96)
    DirectX::XMFLOAT2 Velocity;     //プレイヤーの移動速度
    bool AttackFlg;                 //攻撃中かどうか
    bool JumpFlg;                   //ジャンプしているかどうか
    bool CrouchingFlg;              //しゃがんでいるかどうか
    Rect Anime_rect;                //アニメーション用矩形切り抜き用変数

    Push_Rect Push_Col;             //押し出し判定用矩形
    Damage_Rect hurt_box;           //やられ判定
    Color Rect_Color;               //矩形の色用変数

    bool Push_Debug;                //当たり判定用の矩形の表示・非表示
    bool Attack_Debug;
    bool HitBox_Debug;
public:
    bool Flip;                      //プレイヤーの振り向き
    static constexpr float right_offset = 13;//画像の切り抜きと押し出し判定矩形との差(判定用)
    static constexpr float left_offset = 17; //画像の切り抜きと押し出し判定矩形との差(判定用)

    //ゲッター
    DirectX::XMFLOAT2& GetPos() { return Pos; }
    DirectX::XMFLOAT2& GetSize() { return Size; }
    Push_Rect& GetCollision() { return Push_Col; }
    float& GetHp() { return Hp; }
    int& GetWinCount() { return Win; }

    Character();
    virtual ~Character() {}
    virtual void Init() = 0;                    //初期化関数
    virtual void RoundInit() = 0;               //ラウンドが変わる際に特定のパラメータのみ初期化するための変数
    virtual void Update(float elapsedTime) = 0; //キャラの動きに関する更新
    virtual void Draw(ID3D11DeviceContext* context) = 0;                    //キャラ描画
    virtual void Action() = 0;                  //ボタン入力とキャラの動き
    virtual void Attack() = 0;                  //攻撃ごとの当たり判定管理関数
    virtual void Damage() = 0;                  //ダメージを受けた処理関数
    virtual void Jump(float JumpPower);         //ジャンプ
    virtual void Wall();                        //キャラと壁(枠外)との判定
    virtual void Render_ImGui() = 0;            //ImGuiの描画関数
    virtual void Debug_Draw(ID3D11DeviceContext* context) = 0; //デバッグ用当たり判定の表示・非表示をまとめた関数
};
