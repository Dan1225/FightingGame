//#include "../header_file/player_tei.h"
//#include "../header_file/framework.h"
//#include "../header_file/key_input.h"
//#include "../header_file/player_cirno.h"
//
//Player_Tei* Player_Tei::Tei_inst = nullptr;
//
//void Player_Tei::Render_ImGui()
//{
//	ImGui::Begin("2P");
//	ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
//	if (ImGui::TreeNode("Param"))
//	{
//		ImGui::SliderFloat("Tei_Pos.x", &Pos.x, 0.0f, 1600.0f);
//		ImGui::SliderFloat("Tei_Pos.y", &Pos.y, 0.0f, 606.0f);
//
//
//		ImGui::TreePop();
//	}
//	ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
//	if (ImGui::TreeNode("Collision"))
//	{
//		ImGui::SliderFloat("CollisionPos.x",  &Pos.x,  0.0f, 1600.0f);
//		ImGui::SliderFloat("CollisionPos.y",  &Pos.y,  0.0f, 606.0f);
//		ImGui::SliderFloat("Collisionsize.x", &Size.x, 0.0f, 80.0f);
//		ImGui::SliderFloat("Collisionsize.y", &Size.y, 0.0f, 96.0f);
//
//		ImGui::TreePop();
//	}
//	ImGui::End();
//}
//
//int anime_data[][15] = {
//	{0,1,2,3,4,5,6,7,8,9,10,11,-1},      //待機
//	{0,1,2,3,4,5,6,7,8,9,10,11,12,13,-1},//前ジャンプ
//	{0,1,2,3,4,5,6,7,8,9,10,11,12,13,-1},//後ジャンプ
//	{0,1,2,3,4,5,6,7,8,-1},              //しゃがみ
//	{0,1,2,3,4,5,6,7,8,9,-1},            //前移動
//	{0,1,2,3,4,5,-1},                    //後移動
//	{0,1,2,3,4,5,6,7,8,9,10,-1},         //前ステップ
//	{0,1,2,3,4,5,6,7,8,9,-1},            //バックステップ
//	{0,1,2,-1},                          //立ち弱中段喰らい
//	{0,1,2,3,-1},                        //立ち強中段喰らい
//	{0,1,2,-1},                          //立ち弱下段喰らい
//	{0,1,2,3,-1},                        //立ち強下段喰らい
//	{0,1,2,-1},                          //しゃがみ弱喰らい
//	{0,1,2,3,-1},                        //しゃがみ強喰らい
//	{0,1,2,-1},                          //立ちガード
//	{0,1,2,-1},                          //しゃがみガード
//};
//
//Player_Tei::Player_Tei(ID3D11Device* device)
//{
//	Tei_inst = this;
//	player_tei = std::make_unique<Sprite>(device, L"Assets//Player_Tei//player2.png");
//	primitive = std::make_unique<Sprite>(device, L"Assets//Scene_Main//Rect.png");
//	Angle = 0;
//	Scale = 0;
//	Gravity = 0.0f;
//	Pos = { 0,0 };
//	JumpPos = { 0,0 };
//	Size = { 0,0 };
//	Anime_rect = { 0,0,0,0 };   //アニメーション切り抜き矩形
//	Velocity = { 0,0 };
//	AttackFlg = false;
//	JumpFlg = false;
//	CrouchingFlg = false;
//
//	State = Tei_BUTTON_INIT;
//	tei_states current_state = Tei_ST_UNKNOWN;
//
//	current_animation = &Idle;
//}
//
//
//void Player_Tei::Init()
//{
//	Angle = 0;
//	Scale = 4.0;
//	Gravity = 0.6f;
//	Pos = { 1200,600 };
//	JumpPos = { 0,0 };
//	Size = { 80,96 };
//	Velocity = { 10,10 };
//	AttackFlg = false;
//	JumpFlg = false;
//	CrouchingFlg = false;
//
//	State = Tei_BUTTON_INIT;
//	tei_states current_state = Tei_ST_UNKNOWN;
//	current_animation = &Idle;
//
//	Debug = false;
//	//アニメーション設定
//	const int Idle1Colliders = 1;//当たり判定のBOXの最大数
//	Rect IdleHitBox[Idle1Colliders] = { {0,0,80,96} };//当たり判定の位置と大きさ設定
//	COLLIDER_TYPE IdleColliderType[Idle1Colliders] = { {COLLIDER_PLAYER2} }; //当たり判定の属性
//	//待機状態1
//	Idle.PushBack({ 0,0,   80,96 }, 6, { 25,10 }, Idle1Colliders, IdleHitBox, IdleColliderType);
//	Idle.PushBack({ 80,0,  80,96 }, 6, { 25,10 }, Idle1Colliders, IdleHitBox, IdleColliderType);
//	Idle.PushBack({ 160,0, 80,96 }, 6, { 25,10 }, Idle1Colliders, IdleHitBox, IdleColliderType);
//	Idle.PushBack({ 240,0, 80,96 }, 6, { 25,10 }, Idle1Colliders, IdleHitBox, IdleColliderType);
//	Idle.PushBack({ 320,0, 80,96 }, 6, { 25,10 }, Idle1Colliders, IdleHitBox, IdleColliderType);
//	Idle.PushBack({ 400,0, 80,96 }, 6, { 25,10 }, Idle1Colliders, IdleHitBox, IdleColliderType);
//	Idle.PushBack({ 480,0, 80,96 }, 6, { 25,10 }, Idle1Colliders, IdleHitBox, IdleColliderType);
//	Idle.PushBack({ 560,0, 80,96 }, 6, { 25,10 }, Idle1Colliders, IdleHitBox, IdleColliderType);
//	Idle.PushBack({ 640,0, 80,96 }, 6, { 25,10 }, Idle1Colliders, IdleHitBox, IdleColliderType);
//	Idle.PushBack({ 720,0, 80,96 }, 6, { 25,10 }, Idle1Colliders, IdleHitBox, IdleColliderType);
//	Idle.PushBack({ 800,0, 80,96 }, 6, { 25,10 }, Idle1Colliders, IdleHitBox, IdleColliderType);
//	Idle.PushBack({ 880,0, 80,96 }, 6, { 25,10 }, Idle1Colliders, IdleHitBox, IdleColliderType);
//	//前進
//	const int ForwardColliders = 1;
//	Rect ForwardHitBox[ForwardColliders] = { {0,0,80,96} };
//	COLLIDER_TYPE ForwardColliderType[ForwardColliders] = { {COLLIDER_PLAYER2} };
//	forward.PushBack({ 0,  384, 80,96 }, 6, { 29,6 }, ForwardColliders, ForwardHitBox, ForwardColliderType);
//	forward.PushBack({ 80, 384, 80,96 }, 6, { 29,6 }, ForwardColliders, ForwardHitBox, ForwardColliderType);
//	forward.PushBack({ 160,384, 80,96 }, 6, { 29,6 }, ForwardColliders, ForwardHitBox, ForwardColliderType);
//	forward.PushBack({ 240,384, 80,96 }, 6, { 29,6 }, ForwardColliders, ForwardHitBox, ForwardColliderType);
//	forward.PushBack({ 320,384, 80,96 }, 6, { 29,6 }, ForwardColliders, ForwardHitBox, ForwardColliderType);
//	forward.PushBack({ 400,384, 80,96 }, 6, { 29,6 }, ForwardColliders, ForwardHitBox, ForwardColliderType);
//	forward.PushBack({ 480,384, 80,96 }, 6, { 29,6 }, ForwardColliders, ForwardHitBox, ForwardColliderType);
//	forward.PushBack({ 560,384, 80,96 }, 6, { 29,6 }, ForwardColliders, ForwardHitBox, ForwardColliderType);
//	forward.PushBack({ 640,384, 80,96 }, 6, { 29,6 }, ForwardColliders, ForwardHitBox, ForwardColliderType);
//	forward.PushBack({ 720,384, 80,96 }, 6, { 29,6 }, ForwardColliders, ForwardHitBox, ForwardColliderType);
//	//後退
//	const int BackwardColliders = 1;
//	Rect BackwardHitBox[BackwardColliders] = { {0,0,80,96} };
//	COLLIDER_TYPE BackwardColliderType[BackwardColliders] = { {COLLIDER_PLAYER2} };
//	backward.PushBack({ 0,    480, 80,96 }, 6, { 29,6 }, BackwardColliders, BackwardHitBox, BackwardColliderType);
//	backward.PushBack({ 80,   480, 80,96 }, 6, { 29,6 }, BackwardColliders, BackwardHitBox, BackwardColliderType);
//	backward.PushBack({ 160,  480, 80,96 }, 6, { 29,6 }, BackwardColliders, BackwardHitBox, BackwardColliderType);
//	backward.PushBack({ 240,  480, 80,96 }, 6, { 29,6 }, BackwardColliders, BackwardHitBox, BackwardColliderType);
//	backward.PushBack({ 320,  480, 80,96 }, 6, { 29,6 }, BackwardColliders, BackwardHitBox, BackwardColliderType);
//	backward.PushBack({ 320,  480, 80,96 }, 6, { 29,6 }, BackwardColliders, BackwardHitBox, BackwardColliderType);
//	//しゃがみ中
//	const int CrouchColliders = 1;
//	Rect CrouchHitBox[CrouchColliders] = { 0,0,80,96 };
//	COLLIDER_TYPE CrouchColliderType[CrouchColliders] = { {COLLIDER_PLAYER2} };
//	crouch.PushBack({ 240, 288, 80,96 }, 1, { 29,6 }, CrouchColliders, CrouchHitBox, CrouchColliderType);
//}
//
//void Player_Tei::Update(float elapsedTime)
//{
//	Player_Cirno& cirno = Player_Cirno::Instance();
//	Tei_Frame = current_animation->GetCurrentFrame();
//
//	Action();//行動
//
//	//Jump(); //ジャンプ
//
//	Wall(); //壁との判定
//
//	Debug_Mode();//当たり判定用矩形表示・非表示
//}
//
//void Player_Tei::Draw()
//{
//	Framework& framework = Framework::Instance();
//	ID3D11DeviceContext* context = framework.GetDeviceContext();
//
//	ID3D11BlendState* bs = framework.GetBlendState(Blender::BS_ALPHA);
//	context->OMSetBlendState(bs, nullptr, 0xFFFFFFFF);
//
//	int HitBoxesQnt = Tei_Frame.GetColliderQnt();
//
//	for (int i = 0; i < HitBoxesQnt; i++)
//	{
//		Anime_rect = Tei_Frame.hitboxrects[i];
//	}
//
//	Anime_rect = Tei_Frame.frame;
//	player_tei->render(context,
//		Pos.x, Pos.y,
//		Size.x * Scale , Size.y * Scale,
//		Anime_rect,
//		Angle,
//		1, 1, 1, 1,
//		true);
//
//	if (Debug == true)
//	{
//		primitive->render(context,
//			Pos.x, Pos.y,
//			Size.x * Scale, Size.y * Scale,
//			0, 0,
//			1200, 448,
//			0,
//			Rect_Color
//		);
//	}
//}
//
//void Player_Tei::Action()
//{
//#if 0
//	switch (state)
//	{
//	case Tei_BUTTON_INIT:
//		if (KeyInput::KeyState() & KEY_D && JumpFlg == false) { state = Tei_FORWARD_MOVE; }
//		else if (KeyInput::KeyState() & KEY_A && JumpFlg == false) { state = Tei_BACKWARD_MOVE; }
//		else if (KeyInput::KeyState() & KEY_S) { state = Tei_CROUCH; }
//		else { state = Tei_WAIT; }
//		break;
//	case Tei_WAIT:
//		current_animation = &Idle;
//		state = Tei_BUTTON_INIT;
//		break;
//	case Tei_FORWARD_MOVE:
//		current_animation = &forward;
//		Pos.x -= Velocity.x;
//		if (KeyInput::KeyRelease() & KEY_D)
//		{
//			state = Tei_BUTTON_INIT;
//		}
//		break;
//	case Tei_BACKWARD_MOVE:
//		current_animation = &backward;
//		Pos.x += Velocity.x;
//		if (KeyInput::KeyRelease() & KEY_A)
//		{
//			state = Tei_BUTTON_INIT;
//		}
//		break;
//	case Tei_CROUCH:
//		current_animation = &crouch;
//		if (KeyInput::KeyRelease() & KEY_S)
//		{
//			state = Tei_BUTTON_INIT;
//		}
//		break;
//	}
//#endif
//}
//
//
////void Player_Tei::Jump()
////{
////	if (KeyInput::KeyState() & KEY_W && JumpPos.y == 0 && JumpFlg == false)
////	{
////		JumpPos.y = -20; //初速度
////		JumpFlg = true;
////	}
////	Pos.y = Pos.y + JumpPos.y;     //フレーム毎に速度を加算
////	JumpPos.y = JumpPos.y + Gravity;  //フレーム毎に加速度を速度に加算
////
////	if (Pos.y >= 600)           //Y座標が基準位置より下にあれば、地面に衝突したことになる
////	{
////		JumpPos.y = 0;
////		JumpFlg = false;
////	}
////}
//
////void Player_Tei::Wall()
////{
////	//画面行き過ぎないように
////	if (SCREEN_WIDTH - Size.x * Scale < Pos.x)       Pos.x = SCREEN_WIDTH - Size.x * Scale;       //右画面端
////	if (0 > Pos.x)                                   Pos.x = 0;                                   //左画面端
////	if (SCREEN_HEIGHT - Size.y * Scale - 90 < Pos.y) Pos.y = SCREEN_HEIGHT - Size.y * Scale - 90; //下画面端(54は地面にしたい座標と下画面端の差分)
////	if (0 > Pos.y)                                   Pos.y = 0;                                   //上画面端
////}
//
//void Player_Tei::Debug_Mode()
//{
//	if (KeyInput::KeyTrigger() & KEY_M)
//	{
//		Debug = !Debug;
//	}
//}