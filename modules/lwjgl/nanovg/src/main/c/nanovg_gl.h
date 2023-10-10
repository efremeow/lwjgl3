//
// Copyright (c) 2009-2013 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//
#ifndef NANOVG_GL_H
#define NANOVG_GL_H

#ifdef __cplusplus
extern "C" {
#endif

// Create flags

enum NVGcreateFlags {
	// Flag indicating if geometry based anti-aliasing is used (may not be needed when using MSAA).
	NVG_ANTIALIAS 		= 1<<0,
	// Flag indicating if strokes should be drawn using stencil buffer. The rendering will be a little
	// slower, but path overlaps (i.e. self-intersecting or sharp turns) will be drawn just once.
	NVG_STENCIL_STROKES	= 1<<1,
	// Flag indicating that additional debug checks are done.
	NVG_DEBUG 			= 1<<2,
};

#if defined NANOVG_GL2_IMPLEMENTATION
#  define NANOVG_GL2 1
#  define NANOVG_GL_IMPLEMENTATION 1
#  define EXT(name) name##GL2
#  define CONFIG_METHOD "configGL"
#elif defined NANOVG_GL3_IMPLEMENTATION
#  define NANOVG_GL3 1
#  define NANOVG_GL_IMPLEMENTATION 1
#  define NANOVG_GL_USE_UNIFORMBUFFER 1
#  define EXT(name) name##GL3
#  define CONFIG_METHOD "configGL"
#elif defined NANOVG_GLES2_IMPLEMENTATION
#  define NANOVG_GLES2 1
#  define NANOVG_GL_IMPLEMENTATION 1
#  define EXT(name) name##GLES2
#  define CONFIG_METHOD "configGLES"
#elif defined NANOVG_GLES3_IMPLEMENTATION
#  define NANOVG_GLES3 1
#  define NANOVG_GL_IMPLEMENTATION 1
#  define EXT(name) name##GLES3
#  define CONFIG_METHOD "configGLES"
#endif

#define NANOVG_GL_USE_STATE_FILTER (1)

// Creates NanoVG contexts for different OpenGL (ES) versions.
// Flags should be combination of the create flags above.

// These are additional flags on top of NVGimageFlags.
enum NVGimageFlagsGL {
	NVG_IMAGE_NODELETE			= 1<<16,	// Do not delete GL texture handle.
};

#ifdef __cplusplus
}
#endif

#endif /* NANOVG_GL_H */

#ifdef NANOVG_GL_IMPLEMENTATION

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "nanovg.h"
#include <jni.h>

// ---------- LWJGL --------------

typedef void GLvoid;
typedef unsigned char GLboolean;
typedef char GLchar;
typedef int GLsizei;
typedef int GLint;
typedef unsigned int GLuint;
typedef unsigned int GLenum;
typedef float GLfloat;

typedef ptrdiff_t GLsizeiptr;
typedef ptrdiff_t GLintptr;

#define GL_ALWAYS                         0x0207
#define GL_ARRAY_BUFFER                   0x8892
#define GL_BACK                           0x0405
#define GL_BLEND                          0x0BE2
#define GL_CCW                            0x0901
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_COLOR                          0x1800
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_COMPILE_STATUS                 0x8B81
#define GL_CULL_FACE                      0x0B44
#define GL_DECR                           0x1E03
#define GL_DECR_WRAP                      0x8508
#define GL_DEPTH                          0x1801
#define GL_DEPTH_TEST                     0x0B71
#define GL_DEPTH24_STENCIL8               0x88F0
#define GL_DST_ALPHA                      0x0304
#define GL_DST_COLOR                      0x0306
#define GL_EQUAL                          0x0202
#define GL_FALSE                          0
#define GL_FLOAT                          0x1406
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_FRAMEBUFFER                    0x8D40
#define GL_FRAMEBUFFER_BINDING            0x8CA6
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_FRONT                          0x0404
#define GL_GENERATE_MIPMAP                0x8191
#define GL_INCR                           0x1E02
#define GL_INCR_WRAP                      0x8507
#define GL_INVALID_ENUM                   0x0500
#define GL_KEEP                           0x1E00
#define GL_LINE                           0x1B01
#define GL_LINEAR                         0x2601
#define GL_LINEAR_MIPMAP_LINEAR           0x2703
#define GL_LINK_STATUS                    0x8B82
#define GL_LUMINANCE                      0x1909
#define GL_NEAREST                        0x2600
#define GL_NEAREST_MIPMAP_NEAREST         0x2700
#define GL_NOTEQUAL                       0x0205
#define GL_NO_ERROR                       0
#define GL_ONE                            1
#define GL_ONE_MINUS_DST_ALPHA            0x0305
#define GL_ONE_MINUS_DST_COLOR            0x0307
#define GL_ONE_MINUS_SRC_ALPHA            0x0303
#define GL_ONE_MINUS_SRC_COLOR            0x0301
#define GL_R8                             0x8229
#define GL_RED                            0x1903
#define GL_RENDERBUFFER                   0x8D41
#define GL_RENDERBUFFER_BINDING           0x8CA7
#define GL_REPEAT                         0x2901
#define GL_RGB                            0x1907
#define GL_RGBA                           0x1908
#define GL_SCISSOR_TEST                   0x0C11
#define GL_SRC_ALPHA                      0x0302
#define GL_SRC_ALPHA_SATURATE             0x0308
#define GL_SRC_COLOR                      0x0300
#define GL_STENCIL                        0x1802
#define GL_STENCIL_ATTACHMENT             0x8D20
#define GL_STENCIL_INDEX                  0x1901
#define GL_STENCIL_INDEX8                 0x8D48
#define GL_STENCIL_TEST                   0x0B90
#define GL_STREAM_DRAW                    0x88E0
#define GL_TEXTURE                        0x1702
#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE_2D                     0x0DE1
#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_TEXTURE_WRAP_S                 0x2802
#define GL_TEXTURE_WRAP_T                 0x2803
#define GL_TRIANGLES                      0x0004
#define GL_TRIANGLE_FAN                   0x0006
#define GL_TRIANGLE_STRIP                 0x0005
#define GL_TRUE                           1
#define GL_UNIFORM_BUFFER                 0x8A11
#define GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT 0x8A34
#define GL_UNPACK_ALIGNMENT               0x0CF5
#define GL_UNPACK_ROW_LENGTH              0x0CF2
#define GL_UNPACK_SKIP_PIXELS             0x0CF4
#define GL_UNPACK_SKIP_ROWS               0x0CF3
#define GL_UNSIGNED_BYTE                  0x1401
#define GL_VERTEX_SHADER                  0x8B31
#define GL_ZERO                           0

#ifndef APIENTRY
 #ifdef _WIN32
  #define APIENTRY __stdcall
 #else
  #define APIENTRY
 #endif
#endif
#define GLAPI

typedef void (APIENTRY *glActiveTexturePROC) (GLenum texture);
typedef void (APIENTRY *glAttachShaderPROC) (GLuint program, GLuint shader);
typedef void (APIENTRY *glBindAttribLocationPROC) (GLuint program, GLuint index, const GLchar *name);
typedef void (APIENTRY *glBindBufferPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY *glBindBufferRangePROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (APIENTRY *glBindFramebufferPROC) (GLenum target, GLuint framebuffer);
typedef void (APIENTRY *glBindRenderbufferPROC) (GLenum target, GLuint renderbuffer);
typedef void (APIENTRY *glBindTexturePROC) (GLenum target, GLuint texture);
typedef void (APIENTRY *glBindVertexArrayPROC) (GLuint array);
typedef void (APIENTRY *glBlendFuncPROC) (GLenum sfactor, GLenum dfactor);
typedef void (APIENTRY *glBlendFuncSeparatePROC) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
typedef void (APIENTRY *glBufferDataPROC) (GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef GLenum (APIENTRY *glCheckFramebufferStatusPROC) (GLenum target);
typedef void (APIENTRY *glColorMaskPROC) (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
typedef void (APIENTRY *glCompileShaderPROC) (GLuint shader);
typedef GLuint (APIENTRY *glCreateProgramPROC) (void);
typedef GLuint (APIENTRY *glCreateShaderPROC) (GLenum type);
typedef void (APIENTRY *glCullFacePROC) (GLenum mode);
typedef void (APIENTRY *glDeleteBuffersPROC) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRY *glDeleteFramebuffersPROC) (GLsizei n, const GLuint *framebuffers);
typedef void (APIENTRY *glDeleteProgramPROC) (GLuint program);
typedef void (APIENTRY *glDeleteRenderbuffersPROC) (GLsizei n, const GLuint *renderbuffers);
typedef void (APIENTRY *glDeleteShaderPROC) (GLuint shader);
typedef void (APIENTRY *glDeleteTexturesPROC) (GLsizei n, const GLuint *textures);
typedef void (APIENTRY *glDeleteVertexArraysPROC) (GLsizei n, const GLuint *arrays);
typedef void (APIENTRY *glDetachShaderPROC) (GLuint program, GLuint shader);
typedef void (APIENTRY *glDisablePROC) (GLenum cap);
typedef void (APIENTRY *glDisableVertexAttribArrayPROC) (GLuint index);
typedef void (APIENTRY *glDrawArraysPROC) (GLenum mode, GLint first, GLsizei count);
typedef void (APIENTRY *glEnablePROC) (GLenum cap);
typedef void (APIENTRY *glEnableVertexAttribArrayPROC) (GLuint index);
typedef void (APIENTRY *glFinishPROC) (void);
typedef void (APIENTRY *glFlushPROC) (void);
typedef void (APIENTRY *glFramebufferRenderbufferPROC) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (APIENTRY *glFramebufferTexture2DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (APIENTRY *glFrontFacePROC) (GLenum mode);
typedef void (APIENTRY *glGenBuffersPROC) (GLsizei n, GLuint *buffers);
typedef void (APIENTRY *glGenFramebuffersPROC) (GLsizei n, GLuint *framebuffers);
typedef void (APIENTRY *glGenRenderbuffersPROC) (GLsizei n, GLuint *renderbuffers);
typedef void (APIENTRY *glGenTexturesPROC) (GLsizei n, GLuint *textures);
typedef void (APIENTRY *glGenVertexArraysPROC) (GLsizei n, GLuint *arrays);
typedef void (APIENTRY *glGenerateMipmapPROC) (GLenum target);
typedef GLenum (APIENTRY *glGetErrorPROC) (void);
typedef void (APIENTRY *glGetIntegervPROC) (GLenum pname, GLint *data);
typedef void (APIENTRY *glGetProgramivPROC) (GLuint program, GLenum pname, GLint *params);
typedef void (APIENTRY *glGetProgramInfoLogPROC) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (APIENTRY *glGetShaderivPROC) (GLuint shader, GLenum pname, GLint *params);
typedef void (APIENTRY *glGetShaderInfoLogPROC) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef GLuint (APIENTRY *glGetUniformBlockIndexPROC) (GLuint program, const GLchar *uniformBlockName);
typedef GLint (APIENTRY *glGetUniformLocationPROC) (GLuint program, const GLchar *name);
typedef void (APIENTRY *glLinkProgramPROC) (GLuint program);
typedef void (APIENTRY *glPixelStoreiPROC) (GLenum pname, GLint param);
typedef void (APIENTRY *glRenderbufferStoragePROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (APIENTRY *glShaderSourcePROC) (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
typedef void (APIENTRY *glStencilFuncPROC) (GLenum func, GLint ref, GLuint mask);
typedef void (APIENTRY *glStencilMaskPROC) (GLuint mask);
typedef void (APIENTRY *glStencilOpPROC) (GLenum fail, GLenum zfail, GLenum zpass);
typedef void (APIENTRY *glStencilOpSeparatePROC) (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
typedef void (APIENTRY *glTexImage2DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (APIENTRY *glTexParameteriPROC) (GLenum target, GLenum pname, GLint param);
typedef void (APIENTRY *glTexSubImage2DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
typedef void (APIENTRY *glUniform1iPROC) (GLint location, GLint v0);
typedef void (APIENTRY *glUniform2fvPROC) (GLint location, GLsizei count, const GLfloat *value);
typedef void (APIENTRY *glUniform4fvPROC) (GLint location, GLsizei count, const GLfloat *value);
typedef void (APIENTRY *glUniformBlockBindingPROC) (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
typedef void (APIENTRY *glUseProgramPROC) (GLuint program);
typedef void (APIENTRY *glVertexAttribPointerPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);

#define glActiveTexture gl->ActiveTexture
#define glAttachShader gl->AttachShader
#define glBindAttribLocation gl->BindAttribLocation
#define glBindBuffer gl->BindBuffer
#define glBindBufferRange gl->BindBufferRange
#define glBindFramebuffer gl->BindFramebuffer
#define glBindRenderbuffer gl->BindRenderbuffer
#define glBindTexture gl->BindTexture
#define glBindVertexArray gl->BindVertexArray
#define glBlendFunc gl->BlendFunc
#define glBlendFuncSeparate gl->BlendFuncSeparate
#define glBufferData gl->BufferData
#define glCheckFramebufferStatus gl->CheckFramebufferStatus
#define glColorMask gl->ColorMask
#define glCompileShader gl->CompileShader
#define glCreateProgram gl->CreateProgram
#define glCreateShader gl->CreateShader
#define glCullFace gl->CullFace
#define glDeleteBuffers gl->DeleteBuffers
#define glDeleteFramebuffers gl->DeleteFramebuffers
#define glDeleteProgram gl->DeleteProgram
#define glDeleteRenderbuffers gl->DeleteRenderbuffers
#define glDeleteShader gl->DeleteShader
#define glDeleteTextures gl->DeleteTextures
#define glDeleteVertexArrays gl->DeleteVertexArrays
#define glDetachShader gl->DetachShader
#define glDisable gl->Disable
#define glDisableVertexAttribArray gl->DisableVertexAttribArray
#define glDrawArrays gl->DrawArrays
#define glEnable gl->Enable
#define glEnableVertexAttribArray gl->EnableVertexAttribArray
#define glFinish gl->Finish
#define glFlush gl->Flush
#define glFramebufferRenderbuffer gl->FramebufferRenderbuffer
#define glFramebufferTexture2D gl->FramebufferTexture2D
#define glFrontFace gl->FrontFace
#define glGenBuffers gl->GenBuffers
#define glGenFramebuffers gl->GenFramebuffers
#define glGenRenderbuffers gl->GenRenderbuffers
#define glGenTextures gl->GenTextures
#define glGenVertexArrays gl->GenVertexArrays
#define glGenerateMipmap gl->GenerateMipmap
#define glGetError gl->GetError
#define glGetIntegerv gl->GetIntegerv
#define glGetProgramiv gl->GetProgramiv
#define glGetProgramInfoLog gl->GetProgramInfoLog
#define glGetShaderiv gl->GetShaderiv
#define glGetShaderInfoLog gl->GetShaderInfoLog
#define glGetUniformBlockIndex gl->GetUniformBlockIndex
#define glGetUniformLocation gl->GetUniformLocation
#define glLinkProgram gl->LinkProgram
#define glPixelStorei gl->PixelStorei
#define glRenderbufferStorage gl->RenderbufferStorage
#define glShaderSource gl->ShaderSource
#define glStencilFunc gl->StencilFunc
#define glStencilMask gl->StencilMask
#define glStencilOp gl->StencilOp
#define glStencilOpSeparate gl->StencilOpSeparate
#define glTexImage2D gl->TexImage2D
#define glTexParameteri gl->TexParameteri
#define glTexSubImage2D gl->TexSubImage2D
#define glUniform1i gl->Uniform1i
#define glUniform2fv gl->Uniform2fv
#define glUniform4fv gl->Uniform4fv
#define glUniformBlockBinding gl->UniformBlockBinding
#define glUseProgram gl->UseProgram
#define glVertexAttribPointer gl->VertexAttribPointer

// ---------- LWJGL --------------

enum GLNVGuniformLoc {
	GLNVG_LOC_VIEWSIZE,
	GLNVG_LOC_TEX,
	GLNVG_LOC_FRAG,
	GLNVG_MAX_LOCS
};

enum GLNVGshaderType {
	NSVG_SHADER_FILLGRAD=0,
	NSVG_SHADER_FILLIMG=1,
	NSVG_SHADER_SIMPLE=2,
	NSVG_SHADER_IMG=3,
	NSVG_SHADER_FAST_FILLCOLOR = 5,
	NSVG_SHADER_FAST_FILLIMG=6,
	NSVG_SHADER_FILLCOLOR=7,
	NSVG_SHADER_FAST_FILLGLYPH=8,
};

#if NANOVG_GL_USE_UNIFORMBUFFER
enum GLNVGuniformBindings {
	GLNVG_FRAG_BINDING = 0,
};
#endif

struct GLNVGshader {
	GLuint prog;
	GLuint frag;
	GLuint vert;
	GLint loc[GLNVG_MAX_LOCS];
};
typedef struct GLNVGshader GLNVGshader;

struct GLNVGtexture {
	int id;
	GLuint tex;
	int width, height;
	int type;
	int flags;
};
typedef struct GLNVGtexture GLNVGtexture;

struct GLNVGblend
{
	GLenum srcRGB;
	GLenum dstRGB;
	GLenum srcAlpha;
	GLenum dstAlpha;
};
typedef struct GLNVGblend GLNVGblend;

enum GLNVGcallType {
	GLNVG_NONE = 0,
	GLNVG_FILL,
	GLNVG_CONVEXFILL,
	GLNVG_STROKE,
	GLNVG_TRIANGLES,
};

struct GLNVGcall {
	int type;
	int image;
	int pathOffset;
	int pathCount;
	int triangleOffset;
	int triangleCount;
	int uniformOffset;
	GLNVGblend blendFunc;
};
typedef struct GLNVGcall GLNVGcall;

struct GLNVGpath {
	int fillOffset;
	int fillCount;
	int strokeOffset;
	int strokeCount;
};
typedef struct GLNVGpath GLNVGpath;

struct GLNVGfragUniforms {
	#if NANOVG_GL_USE_UNIFORMBUFFER
		float scissorMat[12]; // matrices are actually 3 vec4s
		float paintMat[12];
		struct NVGcolor innerCol;
		struct NVGcolor outerCol;
		float scissorExt[2];
		float scissorScale[2];
		float extent[2];
		float radius;
		float feather;
		float strokeMult;
		float strokeThr;
		int texType;
		int type;
	#else
		// note: after modifying layout or size of uniform array,
		// don't forget to also update the fragment shader source!
		#define NANOVG_GL_UNIFORMARRAY_SIZE 11
		union {
			struct {
				float scissorMat[12]; // matrices are actually 3 vec4s
				float paintMat[12];
				struct NVGcolor innerCol;
				struct NVGcolor outerCol;
				float scissorExt[2];
				float scissorScale[2];
				float extent[2];
				float radius;
				float feather;
				float strokeMult;
				float strokeThr;
				float texType;
				float type;
			};
			float uniformArray[NANOVG_GL_UNIFORMARRAY_SIZE][4];
		};
	#endif
};
typedef struct GLNVGfragUniforms GLNVGfragUniforms;

struct GLNVGcontext {
  GLNVGshader shader;
  GLNVGtexture* textures;
  float view[2];
  int ntextures;
  int ctextures;
  int textureId;
  GLuint vertBuf;
  float devicePixelRatio;
#if defined NANOVG_GL3
	GLuint vertArr;
#endif
#if NANOVG_GL_USE_UNIFORMBUFFER
	GLuint fragBuf;
#endif
	int fragSize;
	int flags;
	float g_xform[6];

	// Per frame buffers
	GLNVGcall* calls;
	int ccalls;
	int ncalls;
	GLNVGpath* paths;
	int cpaths;
	int npaths;
	struct NVGvertex* verts;
	int cverts;
	int nverts;
	unsigned char* uniforms;
	int cuniforms;
	int nuniforms;

	// cached state
	#if NANOVG_GL_USE_STATE_FILTER
	GLuint boundTexture;
	GLuint stencilMask;
	GLenum stencilFunc;
	GLint stencilFuncRef;
	GLuint stencilFuncMask;
	GLNVGblend blendFunc;
	#endif

	int dummyTex;

	// Function pointers
	glActiveTexturePROC ActiveTexture;
	glAttachShaderPROC AttachShader;
	glBindAttribLocationPROC BindAttribLocation;
	glBindBufferPROC BindBuffer;
	glBindBufferRangePROC BindBufferRange;
	glBindFramebufferPROC BindFramebuffer;
	glBindRenderbufferPROC BindRenderbuffer;
	glBindTexturePROC BindTexture;
	glBindVertexArrayPROC BindVertexArray;
	glBlendFuncPROC BlendFunc;
	glBlendFuncSeparatePROC BlendFuncSeparate;
	glBufferDataPROC BufferData;
	glCheckFramebufferStatusPROC CheckFramebufferStatus;
	glColorMaskPROC ColorMask;
	glCompileShaderPROC CompileShader;
	glCreateProgramPROC CreateProgram;
	glCreateShaderPROC CreateShader;
	glCullFacePROC CullFace;
	glDeleteBuffersPROC DeleteBuffers;
	glDeleteFramebuffersPROC DeleteFramebuffers;
	glDeleteProgramPROC DeleteProgram;
	glDeleteRenderbuffersPROC DeleteRenderbuffers;
	glDeleteShaderPROC DeleteShader;
	glDeleteTexturesPROC DeleteTextures;
	glDeleteVertexArraysPROC DeleteVertexArrays;
	glDetachShaderPROC DetachShader;
	glDisablePROC Disable;
	glDisableVertexAttribArrayPROC DisableVertexAttribArray;
	glDrawArraysPROC DrawArrays;
	glEnablePROC Enable;
	glEnableVertexAttribArrayPROC EnableVertexAttribArray;
	glFinishPROC Finish;
	glFlushPROC Flush;
	glFramebufferRenderbufferPROC FramebufferRenderbuffer;
	glFramebufferTexture2DPROC FramebufferTexture2D;
	glFrontFacePROC FrontFace;
	glGenBuffersPROC GenBuffers;
	glGenFramebuffersPROC GenFramebuffers;
	glGenRenderbuffersPROC GenRenderbuffers;
	glGenTexturesPROC GenTextures;
	glGenVertexArraysPROC GenVertexArrays;
	glGenerateMipmapPROC GenerateMipmap;
	glGetErrorPROC GetError;
	glGetIntegervPROC GetIntegerv;
	glGetProgramivPROC GetProgramiv;
	glGetProgramInfoLogPROC GetProgramInfoLog;
	glGetShaderivPROC GetShaderiv;
	glGetShaderInfoLogPROC GetShaderInfoLog;
	glGetUniformBlockIndexPROC GetUniformBlockIndex;
	glGetUniformLocationPROC GetUniformLocation;
	glLinkProgramPROC LinkProgram;
	glPixelStoreiPROC PixelStorei;
	glRenderbufferStoragePROC RenderbufferStorage;
	glShaderSourcePROC ShaderSource;
	glStencilFuncPROC StencilFunc;
	glStencilMaskPROC StencilMask;
	glStencilOpPROC StencilOp;
	glStencilOpSeparatePROC StencilOpSeparate;
	glTexImage2DPROC TexImage2D;
	glTexParameteriPROC TexParameteri;
	glTexSubImage2DPROC TexSubImage2D;
	glUniform1iPROC Uniform1i;
	glUniform2fvPROC Uniform2fv;
	glUniform4fvPROC Uniform4fv;
	glUniformBlockBindingPROC UniformBlockBinding;
	glUseProgramPROC UseProgram;
	glVertexAttribPointerPROC VertexAttribPointer;
};
typedef struct GLNVGcontext GLNVGcontext;

static int glnvg__maxi(int a, int b) { return a > b ? a : b; }

#ifdef NANOVG_GLES2
static unsigned int glnvg__nearestPow2(unsigned int num)
{
	unsigned n = num > 0 ? num - 1 : 0;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	n++;
	return n;
}
#endif

static void glnvg__bindTexture(GLNVGcontext* gl, GLuint tex)
{
#if NANOVG_GL_USE_STATE_FILTER
	if (gl->boundTexture != tex) {
		gl->boundTexture = tex;
		glBindTexture(GL_TEXTURE_2D, tex);
	}
#else
	glBindTexture(GL_TEXTURE_2D, tex);
#endif
}

static void glnvg__stencilMask(GLNVGcontext* gl, GLuint mask)
{
#if NANOVG_GL_USE_STATE_FILTER
	if (gl->stencilMask != mask) {
		gl->stencilMask = mask;
		glStencilMask(mask);
	}
#else
	glStencilMask(mask);
#endif
}

static void glnvg__stencilFunc(GLNVGcontext* gl, GLenum func, GLint ref, GLuint mask)
{
#if NANOVG_GL_USE_STATE_FILTER
	if ((gl->stencilFunc != func) ||
		(gl->stencilFuncRef != ref) ||
		(gl->stencilFuncMask != mask)) {

		gl->stencilFunc = func;
		gl->stencilFuncRef = ref;
		gl->stencilFuncMask = mask;
		glStencilFunc(func, ref, mask);
	}
#else
	glStencilFunc(func, ref, mask);
#endif
}
static void glnvg__blendFuncSeparate(GLNVGcontext* gl, const GLNVGblend* blend)
{
#if NANOVG_GL_USE_STATE_FILTER
	if ((gl->blendFunc.srcRGB != blend->srcRGB) ||
		(gl->blendFunc.dstRGB != blend->dstRGB) ||
		(gl->blendFunc.srcAlpha != blend->srcAlpha) ||
		(gl->blendFunc.dstAlpha != blend->dstAlpha)) {

		gl->blendFunc = *blend;
		glBlendFuncSeparate(blend->srcRGB, blend->dstRGB, blend->srcAlpha,blend->dstAlpha);
	}
#else
	glBlendFuncSeparate(blend->srcRGB, blend->dstRGB, blend->srcAlpha,blend->dstAlpha);
#endif
}

static GLNVGtexture* glnvg__allocTexture(GLNVGcontext* gl)
{
	GLNVGtexture* tex = NULL;
	int i;

	for (i = 0; i < gl->ntextures; i++) {
		if (gl->textures[i].id == 0) {
			tex = &gl->textures[i];
			break;
		}
	}
	if (tex == NULL) {
		if (gl->ntextures+1 > gl->ctextures) {
			GLNVGtexture* textures;
			int ctextures = glnvg__maxi(gl->ntextures+1, 4) +  gl->ctextures/2; // 1.5x Overallocate
			textures = (GLNVGtexture*)NVG_REALLOC(gl->textures, sizeof(GLNVGtexture)*ctextures);
			if (textures == NULL) return NULL;
			gl->textures = textures;
			gl->ctextures = ctextures;
		}
		tex = &gl->textures[gl->ntextures++];
	}

	memset(tex, 0, sizeof(*tex));
	tex->id = ++gl->textureId;

	return tex;
}

static GLNVGtexture* glnvg__findTexture(GLNVGcontext* gl, int id)
{
	int i;
	for (i = 0; i < gl->ntextures; i++)
		if (gl->textures[i].id == id)
			return &gl->textures[i];
	return NULL;
}

static int glnvg__deleteTexture(GLNVGcontext* gl, int id)
{
	int i;
	for (i = 0; i < gl->ntextures; i++) {
		if (gl->textures[i].id == id) {
			if (gl->textures[i].tex != 0 && (gl->textures[i].flags & NVG_IMAGE_NODELETE) == 0)
				glDeleteTextures(1, &gl->textures[i].tex);
			memset(&gl->textures[i], 0, sizeof(gl->textures[i]));
			return 1;
		}
	}
	return 0;
}

static void glnvg__dumpShaderError(GLNVGcontext* gl, GLuint shader, const char* name, const char* type)
{
	GLchar str[512+1];
	GLsizei len = 0;
	glGetShaderInfoLog(shader, 512, &len, str);
	if (len > 512) len = 512;
	str[len] = '\0';
	printf("Shader %s/%s error:\n%s\n", name, type, str);
}

static void glnvg__dumpProgramError(GLNVGcontext* gl, GLuint prog, const char* name)
{
	GLchar str[512+1];
	GLsizei len = 0;
	glGetProgramInfoLog(prog, 512, &len, str);
	if (len > 512) len = 512;
	str[len] = '\0';
	printf("Program %s error:\n%s\n", name, str);
}

static void glnvg__checkError(GLNVGcontext* gl, const char* str)
{
	GLenum err;
	if ((gl->flags & NVG_DEBUG) == 0) return;
	err = glGetError();
	if (err != GL_NO_ERROR) {
		printf("Error %08x after %s\n", err, str);
		return;
	}
}

static int glnvg__createShader(GLNVGcontext* gl, GLNVGshader* shader, const char* name, const char* header, const char* opts, const char* vshader, const char* fshader)
{
	GLint status;
	GLuint prog, vert, frag;
	const char* str[3];
	str[0] = header;
	str[1] = opts != NULL ? opts : "";

	memset(shader, 0, sizeof(*shader));

	prog = glCreateProgram();
	vert = glCreateShader(GL_VERTEX_SHADER);
	frag = glCreateShader(GL_FRAGMENT_SHADER);
	str[2] = vshader;
	glShaderSource(vert, 3, str, 0);
	str[2] = fshader;
	glShaderSource(frag, 3, str, 0);

	glCompileShader(vert);
	glGetShaderiv(vert, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		glnvg__dumpShaderError(gl, vert, name, "vert");
		return 0;
	}

	glCompileShader(frag);
	glGetShaderiv(frag, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		glnvg__dumpShaderError(gl, frag, name, "frag");
		return 0;
	}

	glAttachShader(prog, vert);
	glAttachShader(prog, frag);

	glBindAttribLocation(prog, 0, "vertex");
	glBindAttribLocation(prog, 1, "tcoord");

	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		glnvg__dumpProgramError(gl, prog, name);
		return 0;
	}

	shader->prog = prog;
	shader->vert = vert;
	shader->frag = frag;

	return 1;
}

static void glnvg__deleteShader(GLNVGcontext* gl, GLNVGshader* shader)
{
	if (shader->prog != 0)
		glDeleteProgram(shader->prog);
	if (shader->vert != 0)
		glDeleteShader(shader->vert);
	if (shader->frag != 0)
		glDeleteShader(shader->frag);
}

static void glnvg__getUniforms(GLNVGcontext* gl, GLNVGshader* shader)
{
	shader->loc[GLNVG_LOC_VIEWSIZE] = glGetUniformLocation(shader->prog, "viewSize");
	shader->loc[GLNVG_LOC_TEX] = glGetUniformLocation(shader->prog, "tex");

#if NANOVG_GL_USE_UNIFORMBUFFER
	shader->loc[GLNVG_LOC_FRAG] = glGetUniformBlockIndex(shader->prog, "frag");
#else
	shader->loc[GLNVG_LOC_FRAG] = glGetUniformLocation(shader->prog, "frag");
#endif
}

static int glnvg__renderCreateTexture(void* uptr, int type, int w, int h, int imageFlags, const unsigned char* data);

static int glnvg__renderCreate(void* uptr)
{
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	int align = 4;

	// TODO: mediump float may not be enough for GLES2 in iOS.
	// see the following discussion: https://github.com/memononen/nanovg/issues/46
	static const char* shaderHeader =
#if defined NANOVG_GL2
		"#define NANOVG_GL2 1\n"
#elif defined NANOVG_GL3
		"#version 150 core\n"
		"#define NANOVG_GL3 1\n"
#elif defined NANOVG_GLES2
		"#version 100\n"
		"#define NANOVG_GL2 1\n"
#elif defined NANOVG_GLES3
		"#version 300 es\n"
		"#define NANOVG_GL3 1\n"
#endif

#if NANOVG_GL_USE_UNIFORMBUFFER
	"#define USE_UNIFORMBUFFER 1\n"
#else
	"#define UNIFORMARRAY_SIZE 11\n"
#endif
	"\n";

	static const char* fillVertShader =
		"#ifdef NANOVG_GL3\n"
		"	uniform vec2 viewSize;\n"
		"	in vec2 vertex;\n"
		"	in vec2 tcoord;\n"
		"	out vec2 ftcoord;\n"
		"	out vec2 fpos;\n"
		"#else\n"
		"	uniform vec2 viewSize;\n"
		"	attribute vec2 vertex;\n"
		"	attribute vec2 tcoord;\n"
		"	varying vec2 ftcoord;\n"
		"	varying vec2 fpos;\n"
		"#endif\n"
		"void main(void) {\n"
		"	ftcoord = tcoord;\n"
		"	fpos = vertex;\n"
		"	gl_Position = vec4(2.0*vertex.x/viewSize.x - 1.0, 1.0 - 2.0*vertex.y/viewSize.y, 0, 1);\n"
		"}\n";

  static const char* fillFragShader =
      "#ifdef GL_ES\n"
      "#if defined(GL_FRAGMENT_PRECISION_HIGH) || defined(NANOVG_GL3)\n"
      " precision highp float;\n"
      "#else\n"
      " precision mediump float;\n"
      "#endif\n"
      "#endif\n"
      "#ifdef NANOVG_GL3\n"
      "#ifdef USE_UNIFORMBUFFER\n"
      " layout(std140) uniform frag {\n"
      "   mat3 scissorMat;\n"
      "   mat3 paintMat;\n"
      "   vec4 innerCol;\n"
      "   vec4 outerCol;\n"
      "   vec2 scissorExt;\n"
      "   vec2 scissorScale;\n"
      "   vec2 extent;\n"
      "   float radius;\n"
      "   float feather;\n"
      "   float strokeMult;\n"
      "   float strokeThr;\n"
      "   int texType;\n"
      "   int type;\n"
      " };\n"
      "#else\n"  // NANOVG_GL3 && !USE_UNIFORMBUFFER
      " uniform vec4 frag[UNIFORMARRAY_SIZE];\n"
      "#endif\n"
      " uniform sampler2D tex;\n"
      " in vec2 ftcoord;\n"
      " in vec2 fpos;\n"
      " out vec4 outColor;\n"
      "#else\n"  // !NANOVG_GL3
      " uniform vec4 frag[UNIFORMARRAY_SIZE];\n"
      " uniform sampler2D tex;\n"
      " varying vec2 ftcoord;\n"
      " varying vec2 fpos;\n"
      "#endif\n"
      "#ifndef USE_UNIFORMBUFFER\n"
      " #define scissorMat mat3(frag[0].xyz, frag[1].xyz, frag[2].xyz)\n"
      " #define paintMat mat3(frag[3].xyz, frag[4].xyz, frag[5].xyz)\n"
      " #define innerCol frag[6]\n"
      " #define outerCol frag[7]\n"
      " #define scissorExt frag[8].xy\n"
      " #define scissorScale frag[8].zw\n"
      " #define extent frag[9].xy\n"
      " #define radius frag[9].z\n"
      " #define feather frag[9].w\n"
      " #define strokeMult frag[10].x\n"
      " #define strokeThr frag[10].y\n"
      " #define texType int(frag[10].z)\n"
      " #define type int(frag[10].w)\n"
      "#endif\n"
      "\n"
      "float sdroundrect(vec2 pt, vec2 ext, float rad) {\n"
      " vec2 ext2 = ext - vec2(rad,rad);\n"
      " vec2 d = abs(pt) - ext2;\n"
      " return min(max(d.x,d.y),0.0) + length(max(d,0.0)) - rad;\n"
      "}\n"
      "\n"
      "// Scissoring\n"
      "float scissorMask(vec2 p) {\n"
      " vec2 sc = (abs((scissorMat * vec3(p,1.0)).xy) - scissorExt);\n"
      " sc = vec2(0.5,0.5) - sc * scissorScale;\n"
      " return clamp(sc.x,0.0,1.0) * clamp(sc.y,0.0,1.0);\n"
      "}\n"
      "// Stroke - from [0..1] to clipped pyramid, where the slope is 1px.\n"
      "float strokeMask() {\n"
      " return min(1.0, (1.0-abs(ftcoord.x*2.0-1.0))*strokeMult) * min(1.0, ftcoord.y);\n"
      "}\n"
      "\n"
      "void main(void) {\n"
      " vec4 result;\n"
      " float strokeAlpha;\n"
      " if (type == 5) {    //fast fill color\n"
      "   result = innerCol;\n"
      " } else if (type == 6) {   //fast fill image\n"
      "   vec2 pt = (paintMat * vec3(fpos,1.0)).xy / extent;\n"
      "#ifdef NANOVG_GL3\n"
      "   vec4 color = texture(tex, pt);\n"
      "#else\n"
      "   vec4 color = texture2D(tex, pt);\n"
      "#endif\n"
//      "   strokeAlpha = strokeMask();\n"
      "   strokeAlpha = 0.3;\n"
//      "   if (strokeAlpha < strokeThr) discard;\n"
//      "   if (texType == 1) color = vec4(color.xyz*color.w,color.w);"
      "   result = innerCol * color * strokeAlpha;\n"
      " } else if(type == 7) {      // fill color\n"
      "   strokeAlpha = strokeMask();\n"
      "   if (strokeAlpha < strokeThr) discard;\n"
      "   float scissor = scissorMask(fpos);\n"
      "   vec4 color = innerCol;\n"
      "   color *= strokeAlpha * scissor;\n"
      "   result = color;\n"
      " } else if (type == 8) {   // fast fill glyph\n"
      "#ifdef NANOVG_GL3\n"
      "   vec4 color = texture(tex, ftcoord);\n"
      "#else\n"
      "   vec4 color = texture2D(tex, ftcoord);\n"
      "#endif\n"
      "   if(color.x < 0.02) discard;\n"
      "   result = innerCol * color.x;\n"
      " } else if (type == 0) {   // gradient\n"
      "   strokeAlpha = strokeMask();\n"
      "   if (strokeAlpha < strokeThr) discard;\n"
      "   // Calculate gradient color using box gradient\n"
      "   vec2 pt = (paintMat * vec3(fpos,1.0)).xy;\n"
      "   float d = clamp((sdroundrect(pt, extent, radius) + feather*0.5) / feather, 0.0, 1.0);\n"
      "   vec4 color = mix(innerCol,outerCol,d);\n"
      "   // Combine alpha\n"
      "   float scissor = scissorMask(fpos);\n"
      "   color *= strokeAlpha * scissor;\n"
      "   result = color;\n"
      " } else if (type == 1) {   // Image\n"
      "   strokeAlpha = strokeMask();\n"
      "   if (strokeAlpha < strokeThr) discard;\n"
      "   // Calculate color fron texture\n"
      "   vec2 pt = (paintMat * vec3(fpos,1.0)).xy / extent;\n"
      "#ifdef NANOVG_GL3\n"
      "   vec4 color = texture(tex, pt);\n"
      "#else\n"
      "   vec4 color = texture2D(tex, pt);\n"
      "#endif\n"
      "   if (texType == 1) color = vec4(color.xyz*color.w,color.w);"
      "   if (texType == 2) color = vec4(color.x);"
      "   // Apply color tint and alpha.\n"
      "   color *= innerCol;\n"
      "   // Combine alpha\n"
      "   float scissor = scissorMask(fpos);\n"
      "   color *= strokeAlpha * scissor;\n"
      "   result = color;\n"
      " } else if (type == 2) {   // Stencil fill\n"
      "   result = vec4(1,1,1,1);\n"
      " } else if (type == 3) {   // Textured tris\n"
      "#ifdef NANOVG_GL3\n"
      "   vec4 color = texture(tex, ftcoord);\n"
      "#else\n"
      "   vec4 color = texture2D(tex, ftcoord);\n"
      "#endif\n"
      "   if(color.x < 0.02) discard;\n"
      "   strokeAlpha = strokeMask();\n"
      "   if (strokeAlpha < strokeThr) discard;\n"
      "   float scissor = scissorMask(fpos);\n"
      "   color = vec4(color.x);"
      "   color *= scissor;\n"
      "   result = color * innerCol;\n"
      " }\n"
      "#ifdef NANOVG_GL3\n"
      " outColor = result;\n"
      "#else\n"
      " gl_FragColor = result;\n"
      "#endif\n"
      "}\n";

	glnvg__checkError(gl, "init");

  if (gl->flags & NVG_ANTIALIAS) {
    if (glnvg__createShader(gl, &gl->shader, "shader", shaderHeader, "#define EDGE_AA 1\n",
                            fillVertShader, fillFragShader) == 0)
      return 0;
  } else {
    if (glnvg__createShader(gl, &gl->shader, "shader", shaderHeader, NULL, fillVertShader,
                            fillFragShader) == 0)
      return 0;
  }

	glnvg__checkError(gl, "uniform locations");
	glnvg__getUniforms(gl, &gl->shader);

	// Create dynamic vertex array
#if defined NANOVG_GL3
	glGenVertexArrays(1, &gl->vertArr);
#endif
	glGenBuffers(1, &gl->vertBuf);

#if NANOVG_GL_USE_UNIFORMBUFFER
	// Create UBOs
	glUniformBlockBinding(gl->shader.prog, gl->shader.loc[GLNVG_LOC_FRAG], GLNVG_FRAG_BINDING);
	glGenBuffers(1, &gl->fragBuf);
	glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &align);
#endif
	gl->fragSize = sizeof(GLNVGfragUniforms) + align - sizeof(GLNVGfragUniforms) % align;

	// Some platforms does not allow to have samples to unset textures.
    // Create empty one which is bound when there's no texture specified.
//    gl->dummyTex = glnvg__renderCreateTexture(gl, NVG_TEXTURE_ALPHA, 1, 1, 0, NULL);

	glnvg__checkError(gl, "create done");

	glFinish();

	return 1;
}

static int glnvg__renderCreateTexture(void* uptr, int type, int w, int h, int imageFlags, const unsigned char* data)
{
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	GLNVGtexture* tex = glnvg__allocTexture(gl);

	if (tex == NULL) return 0;

#ifdef NANOVG_GLES2
	// Check for non-power of 2.
	if (glnvg__nearestPow2(w) != (unsigned int)w || glnvg__nearestPow2(h) != (unsigned int)h) {
		// No repeat
		if ((imageFlags & NVG_IMAGE_REPEATX) != 0 || (imageFlags & NVG_IMAGE_REPEATY) != 0) {
			printf("Repeat X/Y is not supported for non power-of-two textures (%d x %d)\n", w, h);
			imageFlags &= ~(NVG_IMAGE_REPEATX | NVG_IMAGE_REPEATY);
		}
		// No mips.
		if (imageFlags & NVG_IMAGE_GENERATE_MIPMAPS) {
			printf("Mip-maps is not support for non power-of-two textures (%d x %d)\n", w, h);
			imageFlags &= ~NVG_IMAGE_GENERATE_MIPMAPS;
		}
	}
#endif

	glGenTextures(1, &tex->tex);
	tex->width = w;
	tex->height = h;
	tex->type = type;
	tex->flags = imageFlags;
	glnvg__bindTexture(gl, tex->tex);

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
#ifndef NANOVG_GLES2
	glPixelStorei(GL_UNPACK_ROW_LENGTH, tex->width);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
#endif

#if defined (NANOVG_GL2)
	// GL 1.4 and later has support for generating mipmaps using a tex parameter.
	if (imageFlags & NVG_IMAGE_GENERATE_MIPMAPS) {
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	}
#endif

	if (type == NVG_TEXTURE_RGBA)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else
#if defined(NANOVG_GLES2) || defined (NANOVG_GL2)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, w, h, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
#elif defined(NANOVG_GLES3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, data);
#else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, data);
#endif

	if (imageFlags & NVG_IMAGE_GENERATE_MIPMAPS) {
		if (imageFlags & NVG_IMAGE_NEAREST) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		} else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}
	} else {
		if (imageFlags & NVG_IMAGE_NEAREST) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		} else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
	}

	if (imageFlags & NVG_IMAGE_NEAREST) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	if (imageFlags & NVG_IMAGE_REPEATX)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	if (imageFlags & NVG_IMAGE_REPEATY)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
#ifndef NANOVG_GLES2
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
#endif

	// The new way to build mipmaps on GLES and GL3
#if !defined(NANOVG_GL2)
	if (imageFlags & NVG_IMAGE_GENERATE_MIPMAPS) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}
#endif

	glnvg__checkError(gl, "create tex");
	glnvg__bindTexture(gl, 0);

	return tex->id;
}


static int glnvg__renderDeleteTexture(void* uptr, int image)
{
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	return glnvg__deleteTexture(gl, image);
}

static int glnvg__renderUpdateTexture(void* uptr, int image, int x, int y, int w, int h, const unsigned char* data)
{
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	GLNVGtexture* tex = glnvg__findTexture(gl, image);

	if (tex == NULL) return 0;
	glnvg__bindTexture(gl, tex->tex);

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

#ifndef NANOVG_GLES2
	glPixelStorei(GL_UNPACK_ROW_LENGTH, tex->width);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, x);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, y);
#else
	// No support for all of skip, need to update a whole row at a time.
	if (tex->type == NVG_TEXTURE_RGBA)
		data += y*tex->width*4;
	else
		data += y*tex->width;
	x = 0;
	w = tex->width;
#endif

	if (tex->type == NVG_TEXTURE_RGBA)
		glTexSubImage2D(GL_TEXTURE_2D, 0, x,y, w,h, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else
#if defined(NANOVG_GLES2) || defined(NANOVG_GL2)
		glTexSubImage2D(GL_TEXTURE_2D, 0, x,y, w,h, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
#else
		glTexSubImage2D(GL_TEXTURE_2D, 0, x,y, w,h, GL_RED, GL_UNSIGNED_BYTE, data);
#endif

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
#ifndef NANOVG_GLES2
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
#endif

	glnvg__bindTexture(gl, 0);

	return 1;
}

static int glnvg__renderGetTextureSize(void* uptr, int image, int* w, int* h)
{
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	GLNVGtexture* tex = glnvg__findTexture(gl, image);
	if (tex == NULL) return 0;
	*w = tex->width;
	*h = tex->height;
	return 1;
}

static void glnvg__xformToMat3x4(float* m3, float* t)
{
	m3[0] = t[0];
	m3[1] = t[1];
	m3[2] = 0.0f;
	m3[3] = 0.0f;
	m3[4] = t[2];
	m3[5] = t[3];
	m3[6] = 0.0f;
	m3[7] = 0.0f;
	m3[8] = t[4];
	m3[9] = t[5];
	m3[10] = 1.0f;
	m3[11] = 0.0f;
}

static NVGcolor glnvg__premulColor(NVGcolor c)
{
	c.r *= c.a;
	c.g *= c.a;
	c.b *= c.a;
	return c;
}

static int glnvg__convertPaint(GLNVGcontext* gl, GLNVGfragUniforms* frag, NVGpaint* paint,
							   NVGscissor* scissor, float width, float fringe, float strokeThr)
{
	GLNVGtexture* tex = NULL;
	float invxform[6];

	memset(frag, 0, sizeof(*frag));

	frag->innerCol = glnvg__premulColor(paint->innerColor);
	frag->outerCol = glnvg__premulColor(paint->outerColor);

	if (scissor->extent[0] < -0.5f || scissor->extent[1] < -0.5f) {
		memset(frag->scissorMat, 0, sizeof(frag->scissorMat));
		frag->scissorExt[0] = 1.0f;
		frag->scissorExt[1] = 1.0f;
		frag->scissorScale[0] = 1.0f;
		frag->scissorScale[1] = 1.0f;
	} else {
		nvgTransformInverse(invxform, scissor->xform);
		glnvg__xformToMat3x4(frag->scissorMat, invxform);
		frag->scissorExt[0] = scissor->extent[0];
		frag->scissorExt[1] = scissor->extent[1];
		frag->scissorScale[0] = sqrtf(scissor->xform[0]*scissor->xform[0] + scissor->xform[2]*scissor->xform[2]) / fringe;
		frag->scissorScale[1] = sqrtf(scissor->xform[1]*scissor->xform[1] + scissor->xform[3]*scissor->xform[3]) / fringe;
	}

	memcpy(frag->extent, paint->extent, sizeof(frag->extent));
	frag->strokeMult = (width*0.5f + fringe*0.5f) / fringe;
	frag->strokeThr = strokeThr;

	if (paint->image != 0) {
		tex = glnvg__findTexture(gl, paint->image);
		if (tex == NULL) return 0;
		if ((tex->flags & NVG_IMAGE_FLIPY) != 0) {
			float m1[6], m2[6];
			nvgTransformTranslate(m1, 0.0f, frag->extent[1] * 0.5f);
			nvgTransformMultiply(m1, paint->xform);
			nvgTransformScale(m2, 1.0f, -1.0f);
			nvgTransformMultiply(m2, m1);
			nvgTransformTranslate(m1, 0.0f, -frag->extent[1] * 0.5f);
			nvgTransformMultiply(m1, m2);
			nvgTransformInverse(invxform, m1);
		} else {
			nvgTransformInverse(invxform, paint->xform);
		}
		frag->type = NSVG_SHADER_FILLIMG;

		#if NANOVG_GL_USE_UNIFORMBUFFER
		if (tex->type == NVG_TEXTURE_RGBA)
			frag->texType = (tex->flags & NVG_IMAGE_PREMULTIPLIED) ? 0 : 1;
		else
			frag->texType = 2;
		#else
		if (tex->type == NVG_TEXTURE_RGBA)
			frag->texType = (tex->flags & NVG_IMAGE_PREMULTIPLIED) ? 0.0f : 1.0f;
		else
			frag->texType = 2.0f;
		#endif
		printf("frag->texType = %d\n", frag->texType);
	} else {
		frag->type = NSVG_SHADER_FILLGRAD;
		frag->radius = paint->radius;
		frag->feather = paint->feather;
		nvgTransformInverse(invxform, paint->xform);
	}

	glnvg__xformToMat3x4(frag->paintMat, invxform);

	return 1;
}

static GLNVGfragUniforms* nvg__fragUniformPtr(GLNVGcontext* gl, int i);

static void glnvg__setUniforms(GLNVGcontext* gl, int uniformOffset, int image)
{
    GLNVGtexture* tex = NULL;
#if NANOVG_GL_USE_UNIFORMBUFFER
	glBindBufferRange(GL_UNIFORM_BUFFER, GLNVG_FRAG_BINDING, gl->fragBuf, uniformOffset, sizeof(GLNVGfragUniforms));
#else
	GLNVGfragUniforms* frag = nvg__fragUniformPtr(gl, uniformOffset);
	glUniform4fv(gl->shader.loc[GLNVG_LOC_FRAG], NANOVG_GL_UNIFORMARRAY_SIZE, &(frag->uniformArray[0][0]));
#endif

	if (image != 0) {
        tex = glnvg__findTexture(gl, image);
    }
    // If no image is set, use empty texture
    if (tex == NULL) {
        tex = glnvg__findTexture(gl, gl->dummyTex);
    }
    glnvg__bindTexture(gl, tex != NULL ? tex->tex : 0);
    glnvg__checkError(gl, "tex paint tex");
}

static void glnvg__renderViewport(void* uptr, float width, float height, float devicePixelRatio) {
  //NVG_NOTUSED(devicePixelRatio);
  GLNVGcontext* gl = (GLNVGcontext*)uptr;
  gl->view[0] = width;
  gl->view[1] = height;
  gl->devicePixelRatio = devicePixelRatio;
}

static void glnvg__fill(GLNVGcontext* gl, GLNVGcall* call)
{
	GLNVGpath* paths = &gl->paths[call->pathOffset];
	int i, npaths = call->pathCount;

	// Draw shapes
	glEnable(GL_STENCIL_TEST);
	glnvg__stencilMask(gl, 0xff);
	glnvg__stencilFunc(gl, GL_ALWAYS, 0, 0xff);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	// set bindpoint for solid loc
	glnvg__setUniforms(gl, call->uniformOffset, 0);
	glnvg__checkError(gl, "fill simple");

	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, GL_INCR_WRAP);
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_DECR_WRAP);
	glDisable(GL_CULL_FACE);
	for (i = 0; i < npaths; i++)
		glDrawArrays(GL_TRIANGLE_FAN, paths[i].fillOffset, paths[i].fillCount);
	glEnable(GL_CULL_FACE);

	// Draw anti-aliased pixels
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	glnvg__setUniforms(gl, call->uniformOffset + gl->fragSize, call->image);
	glnvg__checkError(gl, "fill fill");

	if (gl->flags & NVG_ANTIALIAS) {
		glnvg__stencilFunc(gl, GL_EQUAL, 0x00, 0xff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		// Draw fringes
		for (i = 0; i < npaths; i++)
			glDrawArrays(GL_TRIANGLE_STRIP, paths[i].strokeOffset, paths[i].strokeCount);
	}

	// Draw fill
	glnvg__stencilFunc(gl, GL_NOTEQUAL, 0x0, 0xff);
	glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);
	glDrawArrays(GL_TRIANGLE_STRIP, call->triangleOffset, call->triangleCount);

	glDisable(GL_STENCIL_TEST);
}

static void glnvg__convexFill(GLNVGcontext* gl, GLNVGcall* call)
{
	GLNVGpath* paths = &gl->paths[call->pathOffset];
	int i, npaths = call->pathCount;

	glnvg__setUniforms(gl, call->uniformOffset, call->image);
	glnvg__checkError(gl, "convex fill");

	for (i = 0; i < npaths; i++) {
		glDrawArrays(GL_TRIANGLE_FAN, paths[i].fillOffset, paths[i].fillCount);
		// Draw fringes
		if (paths[i].strokeCount > 0) {
			glDrawArrays(GL_TRIANGLE_STRIP, paths[i].strokeOffset, paths[i].strokeCount);
		}
	}
}

static void glnvg__stroke(GLNVGcontext* gl, GLNVGcall* call)
{
	GLNVGpath* paths = &gl->paths[call->pathOffset];
	int npaths = call->pathCount, i;

	if (gl->flags & NVG_STENCIL_STROKES) {

		glEnable(GL_STENCIL_TEST);
		glnvg__stencilMask(gl, 0xff);

    // Draw anti-aliased pixels.
    glnvg__setUniforms(gl, call->uniformOffset, call->image);
    glnvg__stencilFunc(gl, GL_EQUAL, 0x00, 0xff);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    for (i = 0; i < npaths; i++)
      glDrawArrays(GL_TRIANGLE_STRIP, paths[i].strokeOffset, paths[i].strokeCount);

    // Clear stencil buffer.
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glnvg__stencilFunc(gl, GL_ALWAYS, 0x0, 0xff);
    glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);
    glnvg__checkError(gl, "stroke fill 1");
    for (i = 0; i < npaths; i++)
      glDrawArrays(GL_TRIANGLE_STRIP, paths[i].strokeOffset, paths[i].strokeCount);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    glDisable(GL_STENCIL_TEST);

    //    glnvg__convertPaint(gl, nvg__fragUniformPtr(gl, call->uniformOffset + gl->fragSize),
    // paint, scissor, strokeWidth, fringe, 1.0f - 0.5f/255.0f);

  } else {
    glnvg__setUniforms(gl, call->uniformOffset, call->image);
    glnvg__checkError(gl, "stroke fill");
    // Draw Strokes
    for (i = 0; i < npaths; i++)
      glDrawArrays(GL_TRIANGLE_STRIP, paths[i].strokeOffset, paths[i].strokeCount);
  }
}

static void glnvg__triangles(GLNVGcontext* gl, GLNVGcall* call)
{
	glnvg__setUniforms(gl, call->uniformOffset, call->image);
	glnvg__checkError(gl, "triangles fill");

	glDrawArrays(GL_TRIANGLES, call->triangleOffset, call->triangleCount);
}

static void glnvg__renderCancel(void* uptr) {
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	gl->nverts = 0;
	gl->npaths = 0;
	gl->ncalls = 0;
	gl->nuniforms = 0;
}

static GLenum glnvg_convertBlendFuncFactor(int factor)
{
	if (factor == NVG_ZERO)
		return GL_ZERO;
	if (factor == NVG_ONE)
		return GL_ONE;
	if (factor == NVG_SRC_COLOR)
		return GL_SRC_COLOR;
	if (factor == NVG_ONE_MINUS_SRC_COLOR)
		return GL_ONE_MINUS_SRC_COLOR;
	if (factor == NVG_DST_COLOR)
		return GL_DST_COLOR;
	if (factor == NVG_ONE_MINUS_DST_COLOR)
		return GL_ONE_MINUS_DST_COLOR;
	if (factor == NVG_SRC_ALPHA)
		return GL_SRC_ALPHA;
	if (factor == NVG_ONE_MINUS_SRC_ALPHA)
		return GL_ONE_MINUS_SRC_ALPHA;
	if (factor == NVG_DST_ALPHA)
		return GL_DST_ALPHA;
	if (factor == NVG_ONE_MINUS_DST_ALPHA)
		return GL_ONE_MINUS_DST_ALPHA;
	if (factor == NVG_SRC_ALPHA_SATURATE)
		return GL_SRC_ALPHA_SATURATE;
	return GL_INVALID_ENUM;
}

static GLNVGblend glnvg__blendCompositeOperation(NVGcompositeOperationState op)
{
	GLNVGblend blend;
	blend.srcRGB = glnvg_convertBlendFuncFactor(op.srcRGB);
	blend.dstRGB = glnvg_convertBlendFuncFactor(op.dstRGB);
	blend.srcAlpha = glnvg_convertBlendFuncFactor(op.srcAlpha);
	blend.dstAlpha = glnvg_convertBlendFuncFactor(op.dstAlpha);
	if (blend.srcRGB == GL_INVALID_ENUM || blend.dstRGB == GL_INVALID_ENUM || blend.srcAlpha == GL_INVALID_ENUM || blend.dstAlpha == GL_INVALID_ENUM)
	{
		blend.srcRGB = GL_ONE;
		blend.dstRGB = GL_ONE_MINUS_SRC_ALPHA;
		blend.srcAlpha = GL_ONE;
		blend.dstAlpha = GL_ONE_MINUS_SRC_ALPHA;
	}
	return blend;
}

static void glnvg__renderFlush(void* uptr)
{
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	int i;

	if (gl->ncalls > 0) {

		// Setup require GL state.
		glUseProgram(gl->shader.prog);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_SCISSOR_TEST);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glStencilMask(0xffffffff);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilFunc(GL_ALWAYS, 0, 0xffffffff);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
#if NANOVG_GL_USE_STATE_FILTER
    gl->boundTexture = 0;
    gl->stencilMask = 0xffffffff;
    gl->stencilFunc = GL_ALWAYS;
    gl->stencilFuncRef = 0;
    gl->stencilFuncMask = 0xffffffff;
    gl->blendFunc.srcRGB = GL_INVALID_ENUM;
    gl->blendFunc.srcAlpha = GL_INVALID_ENUM;
    gl->blendFunc.dstRGB = GL_INVALID_ENUM;
    gl->blendFunc.dstAlpha = GL_INVALID_ENUM;
#endif

#if NANOVG_GL_USE_UNIFORMBUFFER
		// Upload ubo for frag shaders
		glBindBuffer(GL_UNIFORM_BUFFER, gl->fragBuf);
		glBufferData(GL_UNIFORM_BUFFER, gl->nuniforms * gl->fragSize, gl->uniforms, GL_STREAM_DRAW);
#endif

		// Upload vertex data
#if defined NANOVG_GL3
		glBindVertexArray(gl->vertArr);
#endif
		glBindBuffer(GL_ARRAY_BUFFER, gl->vertBuf);
		glBufferData(GL_ARRAY_BUFFER, gl->nverts * sizeof(NVGvertex), gl->verts, GL_STREAM_DRAW);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(NVGvertex), (const GLvoid*)(size_t)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(NVGvertex), (const GLvoid*)(0 + 2*sizeof(float)));

		// Set view and texture just once per frame.
		glUniform1i(gl->shader.loc[GLNVG_LOC_TEX], 0);
		glUniform2fv(gl->shader.loc[GLNVG_LOC_VIEWSIZE], 1, gl->view);

#if NANOVG_GL_USE_UNIFORMBUFFER
		glBindBuffer(GL_UNIFORM_BUFFER, gl->fragBuf);
#endif

		for (i = 0; i < gl->ncalls; i++) {
			GLNVGcall* call = &gl->calls[i];
			glnvg__blendFuncSeparate(gl,&call->blendFunc);
			if (call->type == GLNVG_FILL)
				glnvg__fill(gl, call);
			else if (call->type == GLNVG_CONVEXFILL)
				glnvg__convexFill(gl, call);
			else if (call->type == GLNVG_STROKE)
				glnvg__stroke(gl, call);
			else if (call->type == GLNVG_TRIANGLES)
				glnvg__triangles(gl, call);
		}

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
#if defined NANOVG_GL3
		glBindVertexArray(0);
#endif
		glDisable(GL_CULL_FACE);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		glUseProgram(0);
		glnvg__bindTexture(gl, 0);
	}

	// Reset calls
	gl->nverts = 0;
	gl->npaths = 0;
	gl->ncalls = 0;
	gl->nuniforms = 0;
}

static int glnvg__maxVertCount(const NVGpath* paths, int npaths)
{
	int i, count = 0;
	for (i = 0; i < npaths; i++) {
		count += paths[i].nfill;
		count += paths[i].nstroke;
	}
	return count;
}

static GLNVGcall* glnvg__allocCall(GLNVGcontext* gl) {
  GLNVGcall* ret = NULL;
  if (gl->ncalls + 1 > gl->ccalls) {
    GLNVGcall* calls;
    int ccalls = glnvg__maxi(gl->ncalls + 1, 128) + gl->ccalls / 2;  // 1.5x Overallocate
    calls = (GLNVGcall*)realloc(gl->calls, sizeof(GLNVGcall) * ccalls);
    if (calls == NULL) return NULL;
    gl->calls = calls;
    gl->ccalls = ccalls;
  }
  ret = &gl->calls[gl->ncalls++];
  memset(ret, 0, sizeof(GLNVGcall));
  return ret;
}

static int glnvg__allocPaths(GLNVGcontext* gl, int n) {
  int ret = 0;
  if (gl->npaths + n > gl->cpaths) {
    GLNVGpath* paths;
    int cpaths = glnvg__maxi(gl->npaths + n, 128) + gl->cpaths / 2;  // 1.5x Overallocate
    paths = (GLNVGpath*)realloc(gl->paths, sizeof(GLNVGpath) * cpaths);
    if (paths == NULL) return -1;
    gl->paths = paths;
    gl->cpaths = cpaths;
  }
  ret = gl->npaths;
  gl->npaths += n;
  return ret;
}

static int glnvg__allocVerts(GLNVGcontext* gl, int n) {
  int ret = 0;
  if (gl->nverts + n > gl->cverts) {
    NVGvertex* verts;
    int cverts = glnvg__maxi(gl->nverts + n, 4096) + gl->cverts / 2;  // 1.5x Overallocate
    verts = (NVGvertex*)realloc(gl->verts, sizeof(NVGvertex) * cverts);
    if (verts == NULL) return -1;
    gl->verts = verts;
    gl->cverts = cverts;
  }
  ret = gl->nverts;
  gl->nverts += n;
  return ret;
}

static int glnvg__allocFragUniforms(GLNVGcontext* gl, int n) {
  int ret = 0, structSize = gl->fragSize;
  if (gl->nuniforms + n > gl->cuniforms) {
    unsigned char* uniforms;
    int cuniforms = glnvg__maxi(gl->nuniforms + n, 128) + gl->cuniforms / 2;  // 1.5x Overallocate
    uniforms = (unsigned char*)realloc(gl->uniforms, structSize * cuniforms);
    if (uniforms == NULL) return -1;
    gl->uniforms = uniforms;
    gl->cuniforms = cuniforms;
  }
  ret = gl->nuniforms * structSize;
  gl->nuniforms += n;
  return ret;
}

static GLNVGfragUniforms* nvg__fragUniformPtr(GLNVGcontext* gl, int i)
{
	return (GLNVGfragUniforms*)&gl->uniforms[i];
}

static void glnvg__vset(NVGvertex* vtx, float x, float y, float u, float v)
{
	vtx->x = x;
	vtx->y = y;
	vtx->u = u;
	vtx->v = v;
}

static int glnvg__pathIsRect(const NVGpath* path) {
  const NVGvertex* verts = path->fill;
  if (path->nfill == 4) {
    int ret1 = (verts[0].y == verts[1].y && verts[1].x == verts[2].x && verts[2].y == verts[3].y &&
                verts[3].x == verts[0].x);

    int ret2 = (verts[0].x == verts[1].x && verts[1].y == verts[2].y && verts[2].x == verts[3].x &&
                verts[3].y == verts[0].y);

    if (ret1 || ret2) {
      return 1;
    }
  }

  return 0;
}

static int glnvg_getSupportFastDraw(GLNVGcontext* gl, NVGscissor* scissor) {
  /* 根据 nanovg 的坐标系，判断坐标系是否已经旋转或者缩放了，如果是就返回不支持快速绘画的方法 */
  if(gl->g_xform[0] == 1.0f && gl->g_xform[1] == 0.0f
  && gl->g_xform[2] == 0.0f && gl->g_xform[3] == 1.0f
  && scissor->xform[0] == 1.0f && scissor->xform[1] == 0.0f
  && scissor->xform[2] == 0.0f && scissor->xform[3] == 1.0f) {
    return 1;
  } else  {
    return 0;
  }
}

static int glnvg__VertsInScissor(const NVGvertex* verts, int nr, NVGscissor* scissor) {
  int32_t i = 0;
  float cx = scissor->xform[4];
  float cy = scissor->xform[5];
  float hw = scissor->extent[0];
  float hh = scissor->extent[1];

  float l = cx - hw;
  float t = cy - hh;
  float r = l + 2 * hw;
  float b = t + 2 * hh;

  for (i = 0; i < nr; i++) {
    const NVGvertex* iter = verts + i;
    int x = iter->x;
    int y = iter->y;
    if (x < l || x > r || y < t || y > b) {
      return 0;
    }
  }

  return 1;
}

static int glnvg__pathInScissor(const NVGpath* path, NVGscissor* scissor) {
  return glnvg__VertsInScissor(path->fill, path->nfill, scissor);
}

static void glnvg__renderFill(void* uptr, NVGpaint* paint,
                              NVGcompositeOperationState compositeOperation, NVGscissor* scissor,
                              float fringe, const float* bounds, const NVGpath* paths, int npaths) {
  int support_fast_draw = 0;
  int is_gradient = memcmp(&(paint->innerColor), &(paint->outerColor), sizeof(paint->outerColor));

  GLNVGcontext* gl = (GLNVGcontext*)uptr;
  GLNVGcall* call = glnvg__allocCall(gl);
  NVGvertex* quad;
  GLNVGfragUniforms* frag = NULL;
  int i, maxverts, offset;

  if (call == NULL) return;

  call->type = GLNVG_FILL;
  call->triangleCount = 4;
  call->pathOffset = glnvg__allocPaths(gl, npaths);
  if (call->pathOffset == -1) goto error;
  call->pathCount = npaths;
  call->image = paint->image;
  call->blendFunc = glnvg__blendCompositeOperation(compositeOperation);

  if (npaths == 1 && paths[0].convex) {
    call->type = GLNVG_CONVEXFILL;
    call->triangleCount = 0;  // Bounding box fill quad not needed for convex fill
  }

  // Allocate vertices for all the paths.
  maxverts = glnvg__maxVertCount(paths, npaths) + call->triangleCount;
  offset = glnvg__allocVerts(gl, maxverts);
  if (offset == -1) goto error;

  for (i = 0; i < npaths; i++) {
    GLNVGpath* copy = &gl->paths[call->pathOffset + i];
    const NVGpath* path = &paths[i];
    memset(copy, 0, sizeof(GLNVGpath));
    if (path->nfill > 0) {
      copy->fillOffset = offset;
      copy->fillCount = path->nfill;
      memcpy(&gl->verts[offset], path->fill, sizeof(NVGvertex) * path->nfill);
      offset += path->nfill;
      if (npaths == 1) {
        support_fast_draw = glnvg__pathIsRect(path) && glnvg__pathInScissor(path, scissor) && glnvg_getSupportFastDraw(gl, scissor);
      }
    }
    if (path->nstroke > 0) {
      copy->strokeOffset = offset;
      copy->strokeCount = path->nstroke;
      memcpy(&gl->verts[offset], path->stroke, sizeof(NVGvertex) * path->nstroke);
      offset += path->nstroke;
    }
  }

  // Setup uniforms for draw calls
  if (call->type == GLNVG_FILL) {
    // Quad
    call->triangleOffset = offset;
    quad = &gl->verts[call->triangleOffset];
    glnvg__vset(&quad[0], bounds[2], bounds[3], 0.5f, 1.0f);
    glnvg__vset(&quad[1], bounds[2], bounds[1], 0.5f, 1.0f);
    glnvg__vset(&quad[2], bounds[0], bounds[3], 0.5f, 1.0f);
    glnvg__vset(&quad[3], bounds[0], bounds[1], 0.5f, 1.0f);

    call->uniformOffset = glnvg__allocFragUniforms(gl, 2);
    if (call->uniformOffset == -1) goto error;
    // Simple shader for stencil
    frag = nvg__fragUniformPtr(gl, call->uniformOffset);
    memset(frag, 0, sizeof(*frag));
    frag->strokeThr = -1.0f;
    frag->type = NSVG_SHADER_SIMPLE;
    // Fill shader
    glnvg__convertPaint(gl, nvg__fragUniformPtr(gl, call->uniformOffset + gl->fragSize), paint,
                        scissor, fringe, fringe, -1.0f);

  } else {
    call->uniformOffset = glnvg__allocFragUniforms(gl, 1);
    if (call->uniformOffset == -1) goto error;
//     Fill shader
    frag = nvg__fragUniformPtr(gl, call->uniformOffset);
    glnvg__convertPaint(gl, nvg__fragUniformPtr(gl, call->uniformOffset), paint, scissor, fringe,
                        fringe, -1.0f);
  }

//  if (support_fast_draw) {
    if (paint->image != 0) {
      frag->type = NSVG_SHADER_FAST_FILLIMG;
    } else if (!is_gradient) {
      frag->type = NSVG_SHADER_FAST_FILLCOLOR;
    }
//  } else {
//    if (paint->image == 0 && !is_gradient) {
//      frag->type = NSVG_SHADER_FILLCOLOR;
//    }
//  }

  return;

error:
	// We get here if call alloc was ok, but something else is not.
	// Roll back the last call to prevent drawing it.
	if (gl->ncalls > 0) gl->ncalls--;
}

static void glnvg__renderStroke(void* uptr, NVGpaint* paint, NVGcompositeOperationState compositeOperation, NVGscissor* scissor, float fringe,
								float strokeWidth, const NVGpath* paths, int npaths)
{
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	GLNVGcall* call = glnvg__allocCall(gl);
	int i, maxverts, offset;

	if (call == NULL) return;

	call->type = GLNVG_STROKE;
	call->pathOffset = glnvg__allocPaths(gl, npaths);
	if (call->pathOffset == -1) goto error;
	call->pathCount = npaths;
	call->image = paint->image;
	call->blendFunc = glnvg__blendCompositeOperation(compositeOperation);

	// Allocate vertices for all the paths.
	maxverts = glnvg__maxVertCount(paths, npaths);
	offset = glnvg__allocVerts(gl, maxverts);
	if (offset == -1) goto error;

	for (i = 0; i < npaths; i++) {
		GLNVGpath* copy = &gl->paths[call->pathOffset + i];
		const NVGpath* path = &paths[i];
		memset(copy, 0, sizeof(GLNVGpath));
		if (path->nstroke) {
			copy->strokeOffset = offset;
			copy->strokeCount = path->nstroke;
			memcpy(&gl->verts[offset], path->stroke, sizeof(NVGvertex) * path->nstroke);
			offset += path->nstroke;
		}
	}

	if (gl->flags & NVG_STENCIL_STROKES) {
		// Fill shader
		call->uniformOffset = glnvg__allocFragUniforms(gl, 2);
		if (call->uniformOffset == -1) goto error;

		glnvg__convertPaint(gl, nvg__fragUniformPtr(gl, call->uniformOffset), paint, scissor, strokeWidth, fringe, -1.0f);
		glnvg__convertPaint(gl, nvg__fragUniformPtr(gl, call->uniformOffset + gl->fragSize), paint, scissor, strokeWidth, fringe, 1.0f - 0.5f/255.0f);

	} else {
		// Fill shader
		call->uniformOffset = glnvg__allocFragUniforms(gl, 1);
		if (call->uniformOffset == -1) goto error;
		glnvg__convertPaint(gl, nvg__fragUniformPtr(gl, call->uniformOffset), paint, scissor, strokeWidth, fringe, -1.0f);
	}

	return;

error:
	// We get here if call alloc was ok, but something else is not.
	// Roll back the last call to prevent drawing it.
	if (gl->ncalls > 0) gl->ncalls--;
}

static void glnvg__renderTriangles(void* uptr, NVGpaint* paint, NVGcompositeOperationState compositeOperation, NVGscissor* scissor,
								   const NVGvertex* verts, int nverts, float fringe)
{
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	GLNVGcall* call = glnvg__allocCall(gl);
	GLNVGfragUniforms* frag;

	if (call == NULL) return;

	call->type = GLNVG_TRIANGLES;
	call->image = paint->image;
	call->blendFunc = glnvg__blendCompositeOperation(compositeOperation);

	// Allocate vertices for all the paths.
	call->triangleOffset = glnvg__allocVerts(gl, nverts);
	if (call->triangleOffset == -1) goto error;
	call->triangleCount = nverts;

	memcpy(&gl->verts[call->triangleOffset], verts, sizeof(NVGvertex) * nverts);

	// Fill shader
	call->uniformOffset = glnvg__allocFragUniforms(gl, 1);
	if (call->uniformOffset == -1) goto error;
	frag = nvg__fragUniformPtr(gl, call->uniformOffset);
	glnvg__convertPaint(gl, frag, paint, scissor, 1.0f, fringe, -1.0f);
	frag->type = NSVG_SHADER_IMG;

	return;

error:
	// We get here if call alloc was ok, but something else is not.
	// Roll back the last call to prevent drawing it.
	if (gl->ncalls > 0) gl->ncalls--;
}

static void glnvg__setStateXfrom(void* uptr, float* xform) {
  GLNVGcontext* gl = (GLNVGcontext*)uptr;
  if(xform != NULL) {
		memcpy(gl->g_xform, xform, sizeof(gl->g_xform));
	}
}

static void glnvg__renderDelete(void* uptr) {
  GLNVGcontext* gl = (GLNVGcontext*)uptr;
  int i;
  if (gl == NULL) return;

  glnvg__deleteShader(gl, &gl->shader);

#if NANOVG_GL3
#if NANOVG_GL_USE_UNIFORMBUFFER
	if (gl->fragBuf != 0)
		glDeleteBuffers(1, &gl->fragBuf);
#endif
	if (gl->vertArr != 0)
		glDeleteVertexArrays(1, &gl->vertArr);
#endif
	if (gl->vertBuf != 0)
		glDeleteBuffers(1, &gl->vertBuf);

	for (i = 0; i < gl->ntextures; i++) {
		if (gl->textures[i].tex != 0 && (gl->textures[i].flags & NVG_IMAGE_NODELETE) == 0)
			glDeleteTextures(1, &gl->textures[i].tex);
	}
	NVG_FREE(gl->textures);

	NVG_FREE(gl->paths);
	NVG_FREE(gl->verts);
	NVG_FREE(gl->uniforms);
	NVG_FREE(gl->calls);

	NVG_FREE(gl);
}


NVGcontext* EXT(nvgCreate)(JNIEnv* env, int flags)
{
	jclass NanoVGGLConfig = (*env)->FindClass(env, "org/lwjgl/nanovg/NanoVGGLConfig");
	jmethodID config = (*env)->GetStaticMethodID(env, NanoVGGLConfig, CONFIG_METHOD, "(J)V");

	NVGparams params;
	NVGcontext* ctx = NULL;
	GLNVGcontext* gl = (GLNVGcontext*)NVG_MALLOC(sizeof(GLNVGcontext));
	if (gl == NULL) goto error;
	memset(gl, 0, sizeof(GLNVGcontext));

	(*env)->CallStaticVoidMethod(env, NanoVGGLConfig, config, (jlong)(uintptr_t)&gl->ActiveTexture);
	if ( (*env)->ExceptionCheck(env) )
		goto error;

	memset(&params, 0, sizeof(params));
	params.renderCreate = glnvg__renderCreate;
	params.renderCreateTexture = glnvg__renderCreateTexture;
	params.renderDeleteTexture = glnvg__renderDeleteTexture;
	params.renderUpdateTexture = glnvg__renderUpdateTexture;
	params.renderGetTextureSize = glnvg__renderGetTextureSize;
	params.renderViewport = glnvg__renderViewport;
	params.renderCancel = glnvg__renderCancel;
	params.renderFlush = glnvg__renderFlush;
	params.renderFill = glnvg__renderFill;
	params.renderStroke = glnvg__renderStroke;
	params.renderTriangles = glnvg__renderTriangles;
	params.renderDelete = glnvg__renderDelete;
	params.userPtr = gl;
	params.edgeAntiAlias = flags & NVG_ANTIALIAS ? 1 : 0;

	gl->flags = flags;

	ctx = nvgCreateInternal(&params);
	if (ctx == NULL) goto error;

	return ctx;

error:
	// 'gl' is freed by nvgDeleteInternal.
	if (ctx != NULL) nvgDeleteInternal(ctx);
	return NULL;
}

void EXT(nvgDelete)(NVGcontext* ctx)
{
	nvgDeleteInternal(ctx);
}

int EXT(nvglCreateImageFromHandle)(NVGcontext* ctx, GLuint textureId, int w, int h, int imageFlags)
{
	GLNVGcontext* gl = (GLNVGcontext*)nvgInternalParams(ctx)->userPtr;
	GLNVGtexture* tex = glnvg__allocTexture(gl);

	if (tex == NULL) return 0;

	tex->type = NVG_TEXTURE_RGBA;
	tex->tex = textureId;
	tex->flags = imageFlags;
	tex->width = w;
	tex->height = h;

	return tex->id;
}

GLuint EXT(nvglImageHandle)(NVGcontext* ctx, int image)
{
	GLNVGcontext* gl = (GLNVGcontext*)nvgInternalParams(ctx)->userPtr;
	GLNVGtexture* tex = glnvg__findTexture(gl, image);
	return tex->tex;
}

#endif /* NANOVG_GL_IMPLEMENTATION */
