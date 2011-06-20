#ifndef __xee_h
#define __xee_h

#ifdef __cplusplus
extern "C" {
#endif

#include <xenos/xe.h>
#include "engine.h"

void M_Load44(struct XenosDevice *xe, int base, eMatrix44 *matrix);
void M_Load43(struct XenosDevice *xe, int base, eMatrix43 *matrix);

extern const eMatrix44 g_ident;
void M_BuildPersp(eMatrix44 *m, float fovy, float aspect, float f, float n);
void M_Dump(const char *name, eMatrix44 *m);

extern eMatrix44 g_proj;
void M_LoadMV(struct XenosDevice *xe, int where);
void M_LoadMW(struct XenosDevice *xe, int where);

#ifdef __cplusplus
};
#endif

#endif
