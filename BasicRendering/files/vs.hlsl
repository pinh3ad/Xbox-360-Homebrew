float4x4 modelView: register (c0);
float4x3 modelWorld: register (c4);

/* 
define the vs input.
	technically, there aren't restrictions on that, other than that all referenced
	semantics must be available in the vertex buffer format.
	if they are not, shader instantiation will fail. Neither the order nor the actual
	format does matter. Both will be fixed up on shader load anyway.
*/

struct Input
{
	float4 vPos: POSITION;
	float4 vNormal: NORMAL;
	float4 vUV: TEXCOORD0;
};

/*
define the vs output and ps input.
	This *must* match the ps input, as we do not patch shaders to match.
	However, in the pixel shader, you can leave out oPos etc.
*/

struct Output
{
	float4 oPos: POSITION;
	float3 oNormal: NORMAL;
	float4 oUV: TEXCOORD0;
};

Output main(Input input)
{
	Output output;
	output.oPos = mul(transpose(modelView), input.vPos);
	output.oNormal = mul(transpose(modelWorld), input.vNormal.xyz);
	output.oUV = input.vUV;
	return output;
}
