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
//	Tei_BUTTON_INIT,     //�{�^����t
//	Tei_WAIT,            //�ҋ@
//	Tei_FORWRAD_JUMP,    //�O�W�����v
//	Tei_BACK_JUMP,       //�o�b�N�W�����v
//	Tei_CROUCHING,       //���Ⴊ�݈ڍs
//	Tei_CROUCH,          //���Ⴊ�ݒ�
//	Tei_FORWARD_MOVE,    //�O�i
//	Tei_BACKWARD_MOVE,   //���
//	Tei_FORWARD_DUSH,    //�O�_�b�V��
//	Tei_BACK_DUSH,       //�o�b�N�_�b�V��
//	Tei_DAMAGE1,         //����1
//	Tei_DAMAGE2,         //����2
//	Tei_DAMAGE3,         //����2
//	Tei_CROUCHING_DAMAGE1,//���Ⴊ�݂���1
//	Tei_CROUCHING_DAMAGE2,//���Ⴊ�݂���2
//	Tei_STANDING_GUARD,  //�����K�[�h
//	Tei_CROUCHING_GUARD, //���Ⴊ�݃K�[�h
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
//	static constexpr float right_offset = 13;//�摜�̐؂蔲���Ɖ����o�������`�Ƃ̍�(����p)
//	static constexpr float left_offset = 17; //�摜�̐؂蔲���Ɖ����o�������`�Ƃ̍�(����p)
//
//	bool colliding;
//	Frame Tei_Frame;
//	Animation* current_animation;
//	Animation Idle, forward, backward;//�ҋ@ �O�i ���
//	Animation crouching, standing, crouch;    //���Ⴊ�ނ܂� ���܂� ���Ⴊ��
//
//	tei_states State;
//
//	static Player_Tei* Tei_inst;
//	static Player_Tei& Instance() { return *Tei_inst; }
//
//public:
//	//�Q�b�^�[
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
//	void Action() override;       //�{�^�����͂ƃL�����̓���
//	//void Jump() override;         //�W�����v
//	//void Wall();         //�L�����ƕ�(�g�O)�Ƃ̔���
//	void Render_ImGui() override;
//	void Debug_Mode() override;   //��`�\��
//};