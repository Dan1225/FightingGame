//#include "../header_file/collision.h"
//
//void Collision::Push(Push_Rect p1, Push_Rect p2, float Scale)
//{
//	//1P
//	float l1 = p1.x;                 //左
//	float r1 = p1.x + (p1.w * Scale);//右
//	float u1 = p1.y;                 //上
//	float b1 = p1.y + (p1.h * Scale);//下
//	//2P
//	float l2 = p2.x;
//	float r2 = p2.x + (p2.w * Scale);
//	float u2 = p2.y;
//	float b2 = p2.y + (p2.h * Scale);
//
//
//	//仮押し出し判定
//	if (r1 > l2)
//	{
//		//Move(player_cirno.get(), player_cirno2.get(), Scale);
//		//Move(player_cirno->GetChara(), player_cirno2->GetPlayer_Cirno2(), 4.0f);
//		Move(Characters->chara[0], Characters->chara[1], Scale);
//	}
//	//if (l1 > r2)   return false;
//	//if (b1 < u2)   return false;
//	//if (u1 > b2)   return false;
//
//	//return true;
//}
//
//void Collision::Move(Character* p1, Character* p2, float Scale)
//{
//	p1->GetCollision().x = p2->GetCollision().x - p2->GetCollision().w * Scale;//押し出し判定用の矩形をずらさないように
//	p1->GetPos().x = p1->GetCollision().x - p1->GetCollision().w - p1->right_offset;
//	p2->GetPos().x += 10;      //当たっていたら相手を押し出す
//	p2->GetCollision().x += 10;//押し出し用の矩形も動かす
//
//	//p1->lp.Attack_HitBox.x = p1->GetPos().x + 50 * Scale;//攻撃判定のずれないようにここで更新
//	//p1->mp.Attack_HitBox.x = p1->GetPos().x + p1->right_offset * Scale;
//}
//
//bool Collision::Hit(Attack_Rect p1, Damage_Rect p2, float Scale)
//{
//	//1P
//	float l1 = p1.x;                 //左
//	float r1 = p1.x + p1.w;          //右
//	float u1 = p1.y;                 //上
//	float b1 = p1.y + p1.h;          //下
//	//2P
//	float l2 = p2.x;
//	float r2 = p2.x + (p2.w * Scale);
//	float u2 = p2.y;
//	float b2 = p2.y + (p2.h * Scale);
//
//	if (r1 < l2)   return false;
//	if (l1 > r2)   return false;
//	if (b1 < u2)   return false;
//	if (u1 > b2)   return false;
//	return true;
//}
