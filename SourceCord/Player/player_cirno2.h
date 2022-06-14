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
    static constexpr float right_offset = 13;//�摜�̐؂蔲���Ɖ����o�������`�Ƃ̍�(����p)
    static constexpr float left_offset = 17; //�摜�̐؂蔲���Ɖ����o�������`�Ƃ̍�(����p)

    bool colliding;

    Frame Cirno_Frame;
    Animation* Current_animation;
    Animation Idle1, Idle2, Forward, Backward;//�ҋ@1 �ҋ@2 �O�i ���
    Animation crouching, standing, crouch;    //���Ⴊ�ނ܂� ���܂� ���Ⴊ��
    Animation lp, mp, hp;                     //��P ��P ��P
    Animation low_damage;                     //����_���[�W

    enum cirno2_states
    {
        Cirno2_ST_UNKNOWN,
        Cirno2_BUTTON_INIT,
        Cirno2_WAIT1,           //�ҋ@1
        Cirno2_WAIT2,           //�ҋ@2
        Cirno2_BACK_JUMP,       //�o�b�N�W�����v
        Cirno2_FORWARD_JUMP,    //�O�W�����v
        Cirno2_CROUCHING,       //���Ⴊ�݈ڍs
        Cirno2_CROUCH,          //���Ⴊ�ݒ�
        Cirno2_FORWARD_MOVE,    //�O�i
        Cirno2_BACKWARD_MOVE,   //���
        Cirno2_FORWARD_DUSH,    //�O�_�b�V��
        Cirno2_BACK_DUSH,       //�o�b�N�_�b�V��
        Cirno2_DAMAGE1,         //����1
        Cirno2_DAMAGE2,         //����2
        Cirno2_CROUCHING_DAMAGE,//���Ⴊ�݂���
        Cirno2_STANDING_GUARD,  //�����K�[�h
        Cirno2_CROUCHING_GUARD, //���Ⴊ�݃K�[�h
        Cirno2_STANDING_LIGHT,  //������p���`
        Cirno2_STANDING_MID,    //�������L�b�N
        Cirno2_CROUCHING_LIGHT, //���Ⴊ�ݎ�L�b�N
        Cirno2_SHOT,            //�e
        Cirno2_STANDING_HEAVY,  //�������p���`
        Cirno2_CROUCHING_MID,   //���Ⴊ�ݒ��p���`
        Cirno2_CROUCHING_HEAVY, //���Ⴊ�݋��p���`
    };

    cirno2_states State;                       //�v���C���[�́u��ԁv

public:
    //�Q�b�^�[
    //std::shared_ptr<CharacterManager>& GetPlayer_Cirno2() { return Chara; }
    Attack_Rect& GetAttack();
    Frame& GetCharaFrame() { return Cirno_Frame; }
    Damage_Rect& GetDamage() { return hurt_box; }

    Player_Cirno2(ID3D11Device* device);
    ~Player_Cirno2() {}
    void Init()                             override; //�ϐ��̏�������ݒ�
    void RoundInit()                        override; //���E���h���ς��ۂɓ���̃p�����[�^�̂ݏ��������邽�߂̕ϐ�
    void Update(float elapsedTime)          override; //�X�V����
    void Draw(ID3D11DeviceContext* context) override; //�`��֐�
    void Action()                           override; //�{�^�����͂ƃL�����̓���
    void Attack()                           override; //�U�����ʊ֐�(���i�K�ł͍U���ʂɓ����蔻��̑傫��������)
    void Jump(float JunmPower)              override; //�W�����v�֐�
    void Damage()                           override; //�_���[�W���󂯂��Ƃ��̏���
    void Render_ImGui()                     override; //ImGui�̕`��
    void Debug_Draw(ID3D11DeviceContext* context) override; //�f�o�b�O�p�����蔻��̕\���E��\�����܂Ƃ߂��֐�
};