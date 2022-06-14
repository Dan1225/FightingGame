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
    float Angle;                    //��]
    float Scale;                    //�g��̔{��
    float Gravity;                  //�v���C���[�ɂ�����d��
    float JumpPower;                //�W�����v�̐���
    float Ground;                   //�n�ʂɂȂ���W�̒l(y�̂�)
    float Hp;                       //�v���C���[�̗̑�
    int   Win;                      //�v���C���[�̏�����(�ő�2���E���h�擾)
    DirectX::XMFLOAT2 Pos;          //�v���C���[�̍��W
    DirectX::XMFLOAT2 sPos;         //�e�̍��W
    DirectX::XMFLOAT2 JumpPos;      //�v���C���[�̍��W(�W�����v��)
    DirectX::XMFLOAT2 Size;         //�v���C���[�̑傫��(��{�I ���F80,�c�F96)
    DirectX::XMFLOAT2 Velocity;     //�v���C���[�̈ړ����x
    bool AttackFlg;                 //�U�������ǂ���
    bool JumpFlg;                   //�W�����v���Ă��邩�ǂ���
    bool CrouchingFlg;              //���Ⴊ��ł��邩�ǂ���
    Rect Anime_rect;                //�A�j���[�V�����p��`�؂蔲���p�ϐ�

    Push_Rect Push_Col;             //�����o������p��`
    Damage_Rect hurt_box;           //���ꔻ��
    Color Rect_Color;               //��`�̐F�p�ϐ�

    bool Push_Debug;                //�����蔻��p�̋�`�̕\���E��\��
    bool Attack_Debug;
    bool HitBox_Debug;
public:
    bool Flip;                      //�v���C���[�̐U�����
    static constexpr float right_offset = 13;//�摜�̐؂蔲���Ɖ����o�������`�Ƃ̍�(����p)
    static constexpr float left_offset = 17; //�摜�̐؂蔲���Ɖ����o�������`�Ƃ̍�(����p)

    //�Q�b�^�[
    DirectX::XMFLOAT2& GetPos() { return Pos; }
    DirectX::XMFLOAT2& GetSize() { return Size; }
    Push_Rect& GetCollision() { return Push_Col; }
    float& GetHp() { return Hp; }
    int& GetWinCount() { return Win; }

    Character();
    virtual ~Character() {}
    virtual void Init() = 0;                    //�������֐�
    virtual void RoundInit() = 0;               //���E���h���ς��ۂɓ���̃p�����[�^�̂ݏ��������邽�߂̕ϐ�
    virtual void Update(float elapsedTime) = 0; //�L�����̓����Ɋւ���X�V
    virtual void Draw(ID3D11DeviceContext* context) = 0;                    //�L�����`��
    virtual void Action() = 0;                  //�{�^�����͂ƃL�����̓���
    virtual void Attack() = 0;                  //�U�����Ƃ̓����蔻��Ǘ��֐�
    virtual void Damage() = 0;                  //�_���[�W���󂯂������֐�
    virtual void Jump(float JumpPower);         //�W�����v
    virtual void Wall();                        //�L�����ƕ�(�g�O)�Ƃ̔���
    virtual void Render_ImGui() = 0;            //ImGui�̕`��֐�
    virtual void Debug_Draw(ID3D11DeviceContext* context) = 0; //�f�o�b�O�p�����蔻��̕\���E��\�����܂Ƃ߂��֐�
};
