#include "engine.h"
#include <math.h>
#include <string.h>

eMatrix43 matrix_stack[128];
int matrix_top;

const eMatrix43 ident_matrix={{1,0,0,0},{0,1,0,0},{0,0,1,0}};

void build_rot_matrix(eMatrix43 d, float xrot, float yrot, float zrot)
{
	xrot *= M_PI / 180.0;
	yrot *= M_PI / 180.0;
	zrot *= M_PI / 180.0;
	float sinp = sinf(xrot), sinh = sinf(yrot), sinb = sinf(zrot);
	float cosp = cosf(xrot), cosh = cosf(yrot), cosb = cosf(zrot);
	
	d[0][0]=(cosh*cosb+sinb*sinp*sinh);
	d[0][1]=(cosh*sinb-sinh*sinp*cosb);
	d[0][2]=(sinh*cosp);
	d[0][3]=0;

	d[1][0]=(-sinb*cosp);
	d[1][1]=(cosp*cosb);
	d[1][2]=(sinp);
	d[1][3]=0;

	d[2][0]=(-sinh*cosb+cosh*sinp*sinb);
	d[2][1]=(-cosh*sinp*cosb-sinh*sinb);
	d[2][2]=(cosh*cosp);
	d[2][3]=0;
}

#define FIX(x) if (fabs(x) < 1e-16) x = 0;

void multiply_matrix(eMatrix43 d, eMatrix43 a, eMatrix43 b)
{
	int i;
	for (i=0; i<3; ++i)
	{
		float ai0=a[i][0], ai1=a[i][1], ai2=a[i][2], ai3=a[i][3];
		d[i][0]=ai0 * b[0][0] + ai1 * b[1][0] + ai2 * b[2][0];
		d[i][1]=ai0 * b[0][1] + ai1 * b[1][1] + ai2 * b[2][1];
		d[i][2]=ai0 * b[0][2] + ai1 * b[1][2] + ai2 * b[2][2];
		d[i][3]=ai0 * b[0][3] + ai1 * b[1][3] + ai2 * b[2][3] + ai3;
		FIX(d[i][0]);
		FIX(d[i][1]);
		FIX(d[i][2]);
		FIX(d[i][3]);
	}
}

void multiply_matrix_notranslate(eMatrix43 d, eMatrix43 a, eMatrix43 b)
{
	int i;
	for (i=0; i<3; ++i)
	{
		float ai0=a[i][0], ai1=a[i][1], ai2=a[i][2];
		d[i][0]=ai0 * b[0][0] + ai1 * b[1][0] + ai2 * b[2][0];
		d[i][1]=ai0 * b[0][1] + ai1 * b[1][1] + ai2 * b[2][1];
		d[i][2]=ai0 * b[0][2] + ai1 * b[1][2] + ai2 * b[2][2];
		d[i][3]=ai0 * b[0][3] + ai1 * b[1][3] + ai2 * b[2][3];
	}
}

void multiply_matrix_44(eMatrix44 d, eMatrix44 a, eMatrix44 b)
{
	int i;
	for (i=0; i<4; ++i)
	{
		float ai0=a[i][0], ai1=a[i][1], ai2=a[i][2], ai3=a[i][3];
		d[i][0]=ai0 * b[0][0] + ai1 * b[1][0] + ai2 * b[2][0] + ai3 * b[3][0];
		d[i][1]=ai0 * b[0][1] + ai1 * b[1][1] + ai2 * b[2][1] + ai3 * b[3][1];
		d[i][2]=ai0 * b[0][2] + ai1 * b[1][2] + ai2 * b[2][2] + ai3 * b[3][2];
		d[i][3]=ai0 * b[0][3] + ai1 * b[1][3] + ai2 * b[2][3] + ai3 * b[3][3];
	}
}

void build_proj_ortho(eMatrixProj proj, float t, float b, float l, float r, float n, float f)
{
	proj[0] = 2.0 / (r - l);
	proj[1] = -(r+l) / (r-l);
	proj[2] = 2.0 / (t-b);
	proj[3] = -(t+b)/(t-b);
	proj[4] = -1.0/(f-n);
	proj[5] = -(f)/(f-n);
	*(int*)(proj+6) = 1; // ortho
}

void build_proj_persp(eMatrixProj proj, float fovy, float aspect, float n, float f)
{
	float cot = 1.0f / tanf(fovy * 0.5F);
	float tmp;
	proj[0] = cot / aspect;
	proj[1] = 0;
	proj[2] = cot;
	proj[3] = 0;
	tmp = 1.0f / (f-n);
	proj[4] = -n * tmp;
	proj[5] = -(f*n) * tmp;
	*(int*)(proj+6) = 0; // perspective
}

void invert_matrix(eMatrix43 dst, eMatrix43 src)
{
	
}

void multiply_vector(eVector3 dst, eMatrix43 mtx, eVector3 src)
{
	dst[0] = mtx[0][0] * src[0] + mtx[0][1] * src[1] + src[2] * mtx[0][2] + mtx[0][3];
	dst[1] = mtx[1][0] * src[0] + mtx[1][1] * src[1] + src[2] * mtx[1][2] + mtx[1][3];
	dst[2] = mtx[2][0] * src[0] + mtx[2][1] * src[1] + src[2] * mtx[2][2] + mtx[2][3];
}

void glLoadIdentity()
{
	memcpy(matrix_stack[matrix_top], ident_matrix, sizeof(eMatrix43));
}

void glTranslate(float x, float y, float z)
{
	eMatrix43 *m = &matrix_stack[matrix_top];
	
	(*m)[0][3] += (*m)[0][0] * x + (*m)[0][1] * y + (*m)[0][2] * z;
	(*m)[1][3] += (*m)[1][0] * x + (*m)[1][1] * y + (*m)[1][2] * z;
	(*m)[2][3] += (*m)[2][0] * x + (*m)[2][1] * y + (*m)[2][2] * z;
}

void glRotate(float angle, float x, float y, float z)
{
	float xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c, s, c;
	int optimized = 0;
	eMatrix43 m;
	
	s = sin(angle * M_PI / 180.0);
	c = cos(angle * M_PI / 180.0);
	
	memcpy(m, ident_matrix, sizeof(eMatrix43));
	if (x == 0.0F)	
	{
		if (y == 0.0F)
		{
			if (z != 0.0F)
			{
				optimized = 1;
				m[0][0] = c;
				m[1][1] = c;
				if (z < 0.0F)
				{
					m[0][1] = s;
					m[1][0] = -s;
				} else
				{
					m[0][1] = -s;
					m[1][0] = s;
				}
			}
		} else if (z == 0.0F)
		{
			optimized = 1;
			m[0][0] = c;
			m[2][2] = c;
			if (y < 0.0F)
			{
				m[0][2] = -s;
				m[2][0] = s;
			} else
			{
				m[0][2] = s;
				m[2][0] = -s;
			}
		}
	} else if (y == 0.0F)
	{
		if (z == 0.0F)
		{
			optimized = 1;
			m[1][1] = c;
			m[2][2] = c;
			if (x < 0.0F)
			{
				m[1][2] = s;
				m[2][1] = -s;
			} else
			{
				m[1][2] = -s;
				m[2][1] = s;
			}
		}
	}
	
	if (!optimized)
	{
		const float mag = sqrtf(x * x + y * y + z * z);
		if (mag <= 1.0e-4)
			return;
		
		x /= mag; y /= mag; z /= mag;
		xx = x * x;
		yy = y * y;
		zz = z * z;
		xy = x * y;
		yz = y * z;
		zx = z * x;
		xs = x * s;
		ys = y * s;
		zs = z * s;
		
		one_c = 1.0F - c;
		
		m[0][0] = one_c * xx + c;
		m[0][1] = one_c * xy - zs;
		m[0][2] = one_c * zx + ys;
		
		m[1][0] = one_c * xy + zs;
		m[1][1] = one_c * yy + c;
		m[1][2] = one_c * yz - xs;
		
		m[2][0] = one_c * zx - ys;
		m[2][1] = one_c * yz + xs;
		m[2][2] = one_c * zz + c;
	}
	
	multiply_matrix(matrix_stack[matrix_top], matrix_stack[matrix_top], m);
}

void glScale(float x, float y, float z)
{
	matrix_stack[matrix_top][0][0] *= x;
	matrix_stack[matrix_top][1][0] *= x;
	matrix_stack[matrix_top][2][0] *= x;

	matrix_stack[matrix_top][0][1] *= y;
	matrix_stack[matrix_top][1][1] *= y;
	matrix_stack[matrix_top][2][1] *= y;

	matrix_stack[matrix_top][0][2] *= z;
	matrix_stack[matrix_top][1][2] *= z;
	matrix_stack[matrix_top][2][2] *= z;
}

void glMultMatrix(eMatrix43 matrix)
{
	multiply_matrix(matrix_stack[matrix_top], matrix_stack[matrix_top], matrix);
}

void glLoadMatrix(eMatrix43 matrix)
{
	memcpy(matrix_stack[matrix_top], matrix, sizeof(eMatrix43));
}

void gluLookAt(float eyex, float eyey, float eyez,
		float centerx, float centery, float centerz,
		float upx, float upy, float upz)
{
	eMatrix43 m;
	float x[3], y[3], z[3], mag;
	
	z[0] = eyex - centerx;
	z[1] = eyey - centery;
	z[2] = eyez - centerz;
	mag = sqrtf(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
	if (mag)
	{
		z[0] /= mag;
		z[1] /= mag;
		z[2] /= mag;
	}
	
	y[0] = upx;
	y[1] = upy;
	y[2] = upz;
	
	x[0] =  y[1] * z[2] - y[2] * z[1];
	x[1] = -y[0] * z[2] + y[2] * z[0];
	x[2] =  y[0] * z[1] - y[1] * z[0];
	
	y[0] =  z[1] * x[2] - z[2] * x[1];
	y[1] = -z[0] * x[2] + z[2] * x[0];
	y[2] =  z[0] * x[1] - z[1] * x[0];
	
	mag = sqrtf(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
	if (mag) 
	{
		x[0] /= mag;
		x[1] /= mag;
		x[2] /= mag;
	}
	
	mag = sqrtf(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
	if (mag)
	{
		y[0] /= mag;
		y[1] /= mag;
		y[2] /= mag;
	}

	m[0][0] = x[0]; m[0][1] = x[1]; m[0][2] = x[2]; m[0][3] = 0;
	m[1][0] = y[0]; m[1][1] = y[1]; m[1][2] = y[2]; m[1][3] = 0;
	m[2][0] = z[0]; m[2][1] = z[1]; m[2][2] = z[2]; m[2][3] = 0;
	
	glMultMatrix(m);
	glTranslate(-eyex, -eyey, -eyez);

}
        
void glPushMatrix()
{
	++matrix_top;
	memcpy(matrix_stack + matrix_top, matrix_stack + matrix_top - 1, sizeof(eMatrix43));
}

void glPopMatrix()
{
	--matrix_top;
}

