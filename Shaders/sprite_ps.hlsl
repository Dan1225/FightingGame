//UNIT.02
#include "sprite.hlsli"
//Texture2D color_map : register(t0);
//SamplerState point_sampler_state : register(s0);
//SamplerState linear_sampler_state : register(s1);
//SamplerState anisotropic_sampler_state : register(s2);
//float4 main(VS_OUT pin) : SV_TARGET
//{
//    //UNIT.05
//    return color_map.Sample(point_sampler_state, pin.texcoord)*pin.color;//ƒJƒ‰[‚ğŠ|‚¯‚é‚±‚Æ‚É‚æ‚Á‚Ä‰æ‘œ‚Érender‚Ìˆø”‚ÌF‚ª”½‰f‚³‚ê‚é
//}

//UNIT.08
Texture2D diffuse_map : register(s0);
SamplerState diffuse_map_sampler_state : register(s0);
float4 main(VS_OUT pin) : SV_Target
{
    return diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord) * pin.color;

}