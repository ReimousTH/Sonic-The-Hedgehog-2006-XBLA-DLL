// FontShader.hlsl
// Simplified font shader for BMFont (no compression, just regular texture sampling)

struct VS_IN
{
   float2   Pos             : POSITION;
   float2   Tex             : TEXCOORD0;
   float4   ChannelSelector : TEXCOORD1;
};

struct VS_OUT
{
   float4 Position        : POSITION;
   float4 Diffuse         : COLOR0;
   float2 TexCoord0       : TEXCOORD0;
   float4 ChannelSelector : TEXCOORD1;
};

uniform float4 Color       : register(c1);
uniform float2 TexScale    : register(c2);

sampler        FontTexture : register(s0);

VS_OUT FontVertexShader( VS_IN In )
{
   VS_OUT Out;
   Out.Position.x  = (In.Pos.x-0.5);
   Out.Position.y  = (In.Pos.y-0.5);
   Out.Position.z  = ( 0.0 );
   Out.Position.w  = ( 1.0 );
   Out.Diffuse     = Color;
   Out.TexCoord0.x = In.Tex.x * TexScale.x;
   Out.TexCoord0.y = In.Tex.y * TexScale.y;
   Out.ChannelSelector = In.ChannelSelector;
   return Out;
}

float4 FontPixelShader( VS_OUT In ) : COLOR0
{
   // For BMFont: just sample texture and multiply by vertex color
   // Skip the ATG compression extraction logic
   float4 FontTexel = tex2D( FontTexture, In.TexCoord0 );
   return FontTexel * In.Diffuse;
}