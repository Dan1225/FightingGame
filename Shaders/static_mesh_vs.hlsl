//UNIT.13
#include "static_mesh.hlsli"
VS_OUT main(float4 position : POSITION, float4 normal : NORMAL, float2 texcoord : TEXCOORD)
{
    VS_OUT vout;
    vout.position = mul(position, mul(world, view_projection));

    //UNIT.16
    vout.world_position = mul(position, world);
    normal.w = 0;
    vout.world_normal = normalize(mul(normal, world));

    vout.color = material_color;
    //UNIT.14
    vout.texcoord = texcoord;

    return vout;
}