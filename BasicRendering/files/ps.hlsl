float4 lightDirection: register(c0);

/* 
define the ps input.
	this must match the vertex shader output, except for oPosition (and other fixed-function
	things like fog)
*/

struct Input
{
	float3 oNormal: NORMAL;
	float4 oUV: TEXCOORD0;
};

sampler s;

float4 main(Input input): COLOR
{
	float4 tex = tex2D(s, input.oUV);
	float4 res;
	res.rgb = dot(input.oNormal, lightDirection) * tex;
	res.a  = tex.a;
	return res;
}
