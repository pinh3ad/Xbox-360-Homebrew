/* 
\This code utilizes the simple game engine provided in the LibXenon cube sample.
\The basic consept of this program is to create a writeable, visible surface on the 360 GPU.
\This basic rendering will produce a rectangular pattern on the screen, nothing more.
\
\Written by UNIX
\Libxenon.org
\XDK-forums.com
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <xenos/xe.h>
#include <xenos/xenos.h>
#include <xenos/edram.h>
#include <console/console.h>
#include "engine.h"
#include "xee.h"

//Globals structure variables.
struct XenosDevice _xe, *xe;
struct XenosSurface *fb;
struct XenosShader *sh_ps, *sh_vs;
struct XenosVertexBuffer *vb;
struct XenosIndexBuffer *ib;

uint64_t ld(volatile void *addr)
{
	uint64_t l;
	asm volatile ("ld %0, 0(%1)" : "=r" (l) : "r" (addr));
	return l;
}

int main(void)
{
		/* Initialize the console and GPU resolution */
	xenos_init(VIDEO_MODE_AUTO);
	console_init();
	printf("Hello, world!\n");

	xe = &_xe;
		/* initialize the GPU */
	Xe_Init(xe);

			/* create a render target (the framebuffer) */
	fb = Xe_GetFramebufferSurface(xe);
	Xe_SetRenderTarget(xe, fb);//le buffer est xe 

	/* let's define a vertex buffer format!... */
	/* a rectangle! */

	static const struct XenosVBFFormat vbf =
	{
		2, {
		  {XE_USAGE_POSITION, 0, XE_TYPE_FLOAT2},
		  {XE_USAGE_TEXCOORD, 0, XE_TYPE_FLOAT2},
		}
	};
	
	/* The positioning of the vertex buffer */
		/* This is dictated by the shaders you are using */	
	float cube[] = {
		-1,  1, 0, 0,
		-1, -1, 0, 1,
		 1,  1, 1, 0,
		 1, -1, 1, 1
	};

	unsigned short cube_indices[] = {0, 2, 1, 1, 2, 3};

	printf("loading pixel shader...\n");
		/* load pixel shader */

	/* Call the binary arrays for the pixel and vertex shaders */
		/* Located in ffs_content.c */

	extern unsigned char content_datapspsu[];
	extern unsigned char content_datavsvsu[];

	printf("loading pixel shader...\n");

		/* Load the pixel shader */
	sh_ps = Xe_LoadShaderFromMemory(xe,  content_datapspsu);
	Xe_InstantiateShader(xe, sh_ps, 0);

	printf("loading vertex shader...\n");
		/* load vertex shader */

	sh_vs = Xe_LoadShaderFromMemory(xe, content_datavsvsu);
	Xe_InstantiateShader(xe, sh_vs, 0);
	Xe_ShaderApplyVFetchPatches(xe, sh_vs, 0, &vbf);

	//M_BuildPersp(&g_proj, 45.0 / 180.0 * M_PI, 640.0/480.0, 1, 200.0);

	printf("create vb...\n");
	/* create and fill vertex buffer */
	vb = Xe_CreateVertexBuffer(xe, sizeof(cube));
	void *v = Xe_VB_Lock(xe, vb, 0, sizeof(cube), XE_LOCK_WRITE);
	memcpy(v, cube, sizeof(cube));
	Xe_VB_Unlock(xe, vb);

	printf("create ib...\n");
	/* create and fill index buffer */
	ib = Xe_CreateIndexBuffer(xe, sizeof(cube_indices), XE_FMT_INDEX16);
	unsigned short *i = Xe_IB_Lock(xe, ib, 0, sizeof(cube_indices), XE_LOCK_WRITE);
	memcpy(i, cube_indices, sizeof(cube_indices));
	Xe_IB_Unlock(xe, ib);

	/* stats */
	printf("render..\n");

		/* init EDRAM, this needs to be done right before beginning the first frame */
	edram_init(xe);
	
	/* These variables base the main render loop */
	int f;
	int framecount = 0;

	while(1){
		f++;
		framecount++;

			/* begin a new frame, i.e. reset all renderstates to the default */
		Xe_InvalidateState(xe);

			/* Compile cube and cube_indicies to variables declaring size and max number of verticies */
		int max_vertices = sizeof(cube)/(sizeof(*cube)*12);
		int nr_primitives = sizeof(cube_indices)/sizeof(*cube_indices) / 3;
	
			/* draw cube */
		Xe_SetShader(xe, SHADER_TYPE_PIXEL, sh_ps, 0);
		Xe_SetShader(xe, SHADER_TYPE_VERTEX, sh_vs, 0);
		Xe_SetStreamSource(xe, 0, vb, 0, 12); /* using this vertex buffer */
		Xe_SetIndices(xe, ib); /* ... this index buffer... */
		Xe_SetTexture(xe, 0, fb); /* ... and this texture */
		Xe_DrawIndexedPrimitive(xe, XE_PRIMTYPE_TRIANGLELIST, 0, 0, max_vertices, 0, nr_primitives);
			/* Draw the buffer itself using the variables containing the verticies and indicies */
		Xe_SetClearColor(xe, ~0x33FF33);
	}	

	printf("Main render loop exited...\n");

	return 0;

	/* End of file */
}


