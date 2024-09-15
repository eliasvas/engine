#include "base/base_inc.h"
#include "core/log.h"
#include "ogl.h"
#define OGL_CAST_GLUINTPTR(x) ((GLuint*)((u64)(&x)))
#define OGL_CAST_GLUINT(x) ((GLuint)((u64)(x)))
#define OGL_CAST_PTR(x) ((void*)((u64)(x)))
//TODO -- multiple vertex buffers not supported https://stackoverflow.com/questions/14249634/opengl-vaos-and-multiple-buffers
// maybe this helps too https://vulkan-tutorial.com/Vertex_buffers/Vertex_input_description


////////////////////////////////
// OpenGL function loading
////////////////////////////////

#define GLFunc(type, name) PFNGL##type##PROC name;
GLFunc( GENBUFFERS, glGenBuffers);
GLFunc( BINDBUFFER, glBindBuffer);
GLFunc( DRAWBUFFERS, glDrawBuffers);
GLFunc( USEPROGRAM, glUseProgram);
GLFunc( SHADERSOURCE, glShaderSource);
GLFunc( COMPILESHADER, glCompileShader);
GLFunc( GETSHADERIV, glGetShaderiv);
GLFunc( MAPBUFFERRANGE, glMapBufferRange);
GLFunc( MAPBUFFER, glMapBuffer);
GLFunc( CREATESHADER, glCreateShader);
GLFunc( GETSHADERINFOLOG, glGetShaderInfoLog);
GLFunc( GETPROGRAMINFOLOG, glGetProgramInfoLog);
GLFunc( CREATEPROGRAM, glCreateProgram);
GLFunc( ATTACHSHADER, glAttachShader);
GLFunc( DELETESHADER, glDeleteShader);
GLFunc( DELETEBUFFERS, glDeleteBuffers);
GLFunc( DELETEPROGRAM, glDeleteProgram);
GLFunc( DELETEVERTEXARRAYS, glDeleteVertexArrays);
GLFunc( DISABLEVERTEXATTRIBARRAY, glDisableVertexAttribArray);
GLFunc( LINKPROGRAM, glLinkProgram);
GLFunc( GETPROGRAMIV, glGetProgramiv);
GLFunc( UNIFORM1I, glUniform1i);
GLFunc( UNIFORM1FV, glUniform1fv);
GLFunc( UNIFORM2FV, glUniform2fv);
GLFunc( UNIFORM3FV, glUniform3fv);
GLFunc( UNIFORM4FV, glUniform4fv);
GLFunc( UNIFORM2F, glUniform2f);
GLFunc( UNIFORM3F, glUniform3f);
GLFunc( UNIFORM4F, glUniform4f);
GLFunc( UNIFORM1IV, glUniform1iv);
GLFunc( UNIFORM2FV, glUniform2fv);
GLFunc( UNIFORM1F, glUniform1f);
GLFunc( ACTIVETEXTURE, glActiveTexture_dl);
GLFunc( VERTEXATTRIBDIVISOR, glVertexAttribDivisor);
GLFunc( GETUNIFORMLOCATION, glGetUniformLocation);
GLFunc( GENVERTEXARRAYS, glGenVertexArrays);
GLFunc( DRAWELEMENTSINSTANCED, glDrawElementsInstanced);
GLFunc( DRAWARRAYSINSTANCED, glDrawArraysInstanced);
//GLFunc( DRAWARRAYS, glDrawArrays);
GLFunc( BINDVERTEXARRAY, glBindVertexArray);
GLFunc( UNIFORMMATRIX4FV, glUniformMatrix4fv);
GLFunc( BUFFERDATA, glBufferData);
GLFunc( VERTEXATTRIBPOINTER, glVertexAttribPointer);
GLFunc( VERTEXATTRIBIPOINTER, glVertexAttribIPointer);
GLFunc( ENABLEVERTEXATTRIBARRAY, glEnableVertexAttribArray);
GLFunc( GENERATEMIPMAP, glGenerateMipmap);
GLFunc( GENFRAMEBUFFERS, glGenFramebuffers);
GLFunc( FRAMEBUFFERTEXTURE2D, glFramebufferTexture2D);
GLFunc( BINDFRAMEBUFFER, glBindFramebuffer);
GLFunc( CHECKFRAMEBUFFERSTATUS, glCheckFramebufferStatus);
GLFunc( BINDRENDERBUFFER, glBindRenderbuffer);
GLFunc( RENDERBUFFERSTORAGE, glRenderbufferStorage);
GLFunc( GENRENDERBUFFERS, glGenRenderbuffers);
GLFunc( FRAMEBUFFERRENDERBUFFER, glFramebufferRenderbuffer);
//GLFunc( TEXIMAGE3D, glTexImage3D);
//GLFunc( TEXIMAGE2D, glTexImage2D);
GLFunc( BINDIMAGETEXTURE, glBindImageTexture);
GLFunc( MEMORYBARRIER, glMemoryBarrier);
GLFunc( COPYIMAGESUBDATA, glCopyImageSubData);
GLFunc( BLENDFUNCSEPARATE, glBlendFuncSeparate);
GLFunc( BLENDEQUATIONSEPARATE, glBlendEquationSeparate);
GLFunc( DELETEFRAMEBUFFERS, glDeleteFramebuffers);
GLFunc( BLITFRAMEBUFFER, glBlitFramebuffer);

GLFunc( BINDBUFFERBASE, glBindBufferBase);
GLFunc( UNMAPBUFFER, glUnmapBuffer);
GLFunc( GETBUFFERSUBDATA, glGetBufferSubData);
GLFunc( CLEARBUFFERDATA, glClearBufferData);
GLFunc( TEXSTORAGE2D, glTexStorage2D);
GLFunc( BUFFERSUBDATA, glBufferSubData);
GLFunc( CLEARTEXIMAGE, glClearTexImage);
GLFunc( DISPATCHCOMPUTE, glDispatchCompute);
GLFunc( MEMORYBARRIER, glMemoryBarrier);
void ogl_load_gl_functions(void *(*load_func)()) {
    glActiveTexture_dl = (PFNGLACTIVETEXTUREPROC)load_func("glActiveTexture");
    glGenBuffers = (PFNGLGENBUFFERSPROC)load_func("glGenBuffers");
    glBindBuffer = (PFNGLBINDBUFFERPROC)load_func("glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC)load_func("glBufferData");
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)load_func("glDeleteBuffers");
    glDeleteShader = (PFNGLDELETESHADERPROC)load_func("glDeleteShader");

    glCreateShader = (PFNGLCREATESHADERPROC)load_func("glCreateShader");
    glShaderSource = (PFNGLSHADERSOURCEPROC)load_func("glShaderSource");
    glCompileShader = (PFNGLCOMPILESHADERPROC)load_func("glCompileShader");
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)load_func("glCreateProgram");
    glAttachShader = (PFNGLATTACHSHADERPROC)load_func("glAttachShader");
    glLinkProgram = (PFNGLLINKPROGRAMPROC)load_func("glLinkProgram");
    glUseProgram = (PFNGLUSEPROGRAMPROC)load_func("glUseProgram");
    glGetShaderiv = (PFNGLGETSHADERIVPROC)load_func("glGetShaderiv");
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)load_func("glGetShaderInfoLog");
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)load_func("glGetProgramiv");
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)load_func("glGetProgramInfoLog");

    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)load_func("glGetUniformLocation");
    glUniform1i = (PFNGLUNIFORM1IPROC)load_func("glUniform1i");
    glUniform1f = (PFNGLUNIFORM1FPROC)load_func("glUniform1f");
    glUniform2f = (PFNGLUNIFORM2FPROC)load_func("glUniform2f");
    glUniform3f = (PFNGLUNIFORM3FPROC)load_func("glUniform3f");
    glUniform4f = (PFNGLUNIFORM4FPROC)load_func("glUniform4f");
    glUniform1fv = (PFNGLUNIFORM1FVPROC)load_func("glUniform1fv");
    glUniform2fv = (PFNGLUNIFORM2FVPROC)load_func("glUniform2fv");
    glUniform3fv = (PFNGLUNIFORM3FVPROC)load_func("glUniform3fv");
    glUniform4fv = (PFNGLUNIFORM4FVPROC)load_func("glUniform4fv");
    glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)load_func("glUniformMatrix4fv");

    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)load_func("glVertexAttribPointer");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)load_func("glEnableVertexAttribArray");
    glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)load_func("glDisableVertexAttribArray");
    glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC)load_func("glVertexAttribDivisor");

    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)load_func("glGenVertexArrays");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)load_func("glBindVertexArray");
    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)load_func("glDeleteVertexArrays");

    //glActiveTexture = (PFNGLACTIVETEXTUREPROC)load_func("glActiveTexture");
    glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)load_func("glGenerateMipmap");
    glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)load_func("glGenFrameBuffers");
    glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)load_func("glBindFramebuffer");
    glBlendFuncSeparate= (PFNGLBLENDFUNCSEPARATEPROC)load_func("glBlendFuncSeparate");
    glBlendEquationSeparate= (PFNGLBLENDEQUATIONSEPARATEPROC)load_func("glBlendEquationSeparate");

    //glDrawArrays = (PFNGLDRAWARRAYSPROC)load_func("glDrawArrays");
    //glDrawElements = (PFNGLDRAWELEMENTSPROC)load_func("glDrawElements");
    glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC)load_func("glDrawArraysInstanced");
}

////////////////////////////////
// OGL context
////////////////////////////////

b32 ogl_ctx_init(oglContext *ctx) {
    //ogl_load_gl_functions(SDL_GL_GetProcAddress);
    assert(glGenVertexArrays);
    glGenVertexArrays(1, &ctx->vao);
    ctx->initialized = 1;
    return (ctx->vao != 0);
}

b32 ogl_ctx_deinit(oglContext *ctx) {
    glDeleteVertexArrays(1, &ctx->vao);
    return 1;
}

void ogl_set_viewport(f32 x, f32 y, f32 w, f32 h) {
    glViewport(x,y,w,h);
}

void ogl_clear_all_state(oglContext *ctx) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindVertexArray(0);
    glBindVertexArray(ctx->vao);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    for(u32 i = 0; i < OGL_CTX_MAX_ATTRIBS; i+=1){
        glDisableVertexAttribArray(i);
    }
    for (u32 i = 0; i < OGL_CTX_MAX_TEX_SLOTS; i+=1) {
        glActiveTexture_dl(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    //...
    //...
    //...
    glBindVertexArray(ctx->vao);
    //glEnable(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // This is Fine ONLY for the UI i think
    //maybe we just need to write UI to non-glEnable(GL_FRAMEBUFFER_SRGB);
    //glDisable(GL_BLEND);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

}

GLenum gl_check_err(const char *file, int line) {
    GLenum last_ec = GL_NO_ERROR;
    GLenum ec;
    while ((ec = glGetError()) != GL_NO_ERROR)
    {
        char *error;
        switch (ec)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            //case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            //case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
            default: break;
        }
        NLOG_ERR("GL ERROR: %s | %s | %d\n", error, file, line);
        last_ec = ec;
    }
    return last_ec;
}
#define ogl_check_error() gl_check_err(__FILE__, __LINE__)



////////////////////////////////
// OGL buffers
////////////////////////////////


oglBuf ogl_buf_make(oglBufKind kind, void *data, u32 data_count, u32 data_size) {
    oglBuf buf;
    buf.kind = kind;
    buf.count = data_count;
    buf.size = data_size * data_count;

    GLuint buffer_kind = (kind == OGL_BUF_KIND_VERTEX) ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER;

    glGenBuffers(1, OGL_CAST_GLUINTPTR(buf.impl_state));
    glBindBuffer(buffer_kind, OGL_CAST_GLUINT(buf.impl_state));
    ogl_buf_update(&buf, data,data_count,data_size);
    glBindBuffer(buffer_kind, 0);
    ogl_check_error();
    return buf;
}

b32 ogl_buf_deinit(oglBuf *b) {
    glDeleteBuffers(1, OGL_CAST_GLUINTPTR(b->impl_state));
    return 1;
}
void ogl_buf_update(oglBuf *buf, void *data, u32 data_count, u32 data_size) {
    ogl_bind_vertex_buffer(buf);
    buf->count = data_count;
    buf->size = data_size * data_count;
    // TODO -- do we need something here for UBO?
    GLuint buffer_kind = (buf->kind == OGL_BUF_KIND_VERTEX) ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER;
    if (data) {
        glBufferData(buffer_kind, buf->size, data, GL_STREAM_DRAW); //TODO: make an arg for DYNAMIC/STATIC/STREAM?
    }
}

void ogl_bind_index_buffer(oglBuf *b) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OGL_CAST_GLUINT(b->impl_state));
}

void ogl_bind_vertex_buffer(oglBuf *b) {
    glBindBuffer(GL_ARRAY_BUFFER, OGL_CAST_GLUINT(b->impl_state));
}



////////////////////////////////
// OGL shaders
////////////////////////////////

b32 gl_check_gl_shader_link_errors(GLuint sp_handle) {
    s32 success;
    glGetProgramiv(sp_handle, GL_LINK_STATUS, &success);
    if (!success) {
        s32 info_len;
        glGetProgramiv (sp_handle, GL_INFO_LOG_LENGTH, &info_len);
        //info_data := cast (*u8) alloc (info_len);
        //defer free(info_data);
        u8 info_data[256] = {0};
        glGetProgramInfoLog (sp_handle, info_len, &info_len, info_data);
        NLOG_ERR("Shader link error: %s\n", info_data);
    }
    return (success == 0);
}

b32 gl_check_gl_shader_compile_errors(GLuint sp_handle) {
    s32 success;
    glGetShaderiv(sp_handle, GL_COMPILE_STATUS, &success);
    if (!success) {
        s32 info_len;
        glGetShaderiv (sp_handle, GL_INFO_LOG_LENGTH, &info_len);
        //info_data := cast (*u8) alloc (info_len);
        //defer free(info_data);
        u8 info_data[256] = {0};
        glGetShaderInfoLog (sp_handle, info_len, &info_len, info_data);
        NLOG_ERR("Shader compilation error: %s\n", info_data);
    }
    return (success == 0);
}


GLuint gl_make_sp(const char *vs_source, const char *fs_source) {
    GLuint sp = 0;

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_source, NULL);
    glCompileShader(vs);
    assert(vs);
    assert(0 == gl_check_gl_shader_compile_errors(vs));
    ogl_check_error();

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_source, NULL);
    glCompileShader(fs);
    assert(fs);
    assert(0 == gl_check_gl_shader_compile_errors(fs));
    ogl_check_error();

    sp = glCreateProgram();
    glAttachShader(sp, vs);
    glAttachShader(sp, fs);
    glLinkProgram(sp);
    assert(0 == gl_check_gl_shader_link_errors(sp));
    ogl_check_error();

    glDeleteShader(vs);
    glDeleteShader(fs);

    return sp;
}

u32 ogl_get_component_num_for_data_type(oglShaderDataType type) {
    u32 comp_count = 0;
    switch(type) {
        case (OGL_SHADER_DATA_TYPE_FLOAT):
            comp_count = 1;
            break;
        case (OGL_SHADER_DATA_TYPE_INT):
            comp_count = 1;
            break;
        case (OGL_SHADER_DATA_TYPE_VEC2):
            comp_count = 2;
            break;
        case (OGL_SHADER_DATA_TYPE_VEC3):
            comp_count = 3;
            break;
        case (OGL_SHADER_DATA_TYPE_VEC4):
            comp_count = 4;
            break;
         case (OGL_SHADER_DATA_TYPE_MAT4):
            comp_count = 16;
            break;
        default:
            comp_count = 0;
            assert(0 && "Invalid OGL_SHADER_DATA_TYPE");
            break;
    }
    return comp_count;
}

oglShaderAttrib ogl_make_attrib(u32 vbo_idx, oglShaderDataType type, u32 stride, u32 offset, b32 instanced) {
    oglShaderAttrib sa;
    sa.vbo_idx = vbo_idx;
    sa.stride = stride;
    sa.offset = offset;
    sa.type = type;
    sa.instanced = instanced;
    return sa;
}



void ogl_bind_attrib(oglShaderAttrib *attrib) {
    glEnableVertexAttribArray(attrib->vbo_idx);
    if (attrib->type == OGL_SHADER_DATA_TYPE_INT) {
        glVertexAttribIPointer(attrib->vbo_idx, ogl_get_component_num_for_data_type(attrib->type), GL_FLOAT, attrib->stride, (void*)((u64)attrib->offset));
    } else {
        glVertexAttribPointer(attrib->vbo_idx, ogl_get_component_num_for_data_type(attrib->type), GL_FLOAT, GL_FALSE, attrib->stride, (void*)((u64)attrib->offset));
    }
    glVertexAttribDivisor(attrib->vbo_idx, attrib->instanced);
}

b32 ogl_sp_init(oglSP *shader, const char *vs_source, const char *fs_source) {
    shader->impl_state = OGL_CAST_PTR(gl_make_sp(vs_source, fs_source));
    shader->attrib_count = 0;
    ogl_check_error();
    return (OGL_CAST_GLUINT(shader->impl_state) != 0);
}

b32 ogl_sp_deinit(oglSP *shader) {
    glDeleteProgram(OGL_CAST_GLUINT(shader->impl_state));
    return 1;
}

void ogl_sp_add_attrib(oglSP *shader, oglShaderAttrib attrib) {
    shader->attribs[shader->attrib_count] = attrib;
    shader->attrib_count += 1;
    assert(shader->attrib_count < OGL_CTX_MAX_ATTRIBS);
}

void ogl_sp_bind_attribs(oglSP *sp){
    for (u32 i = 0; i < sp->attrib_count; i+=1) {
        ogl_bind_attrib(&sp->attribs[i]);
    }
}

void ogl_bind_sp(oglSP *sp){
    glUseProgram(OGL_CAST_GLUINT(sp->impl_state));
    ogl_sp_bind_attribs(sp);
}

void ogl_sp_set_uniform(oglSP *sp, const char *uniform_name, oglShaderDataType type, void * val){
    switch (type) {
        case(OGL_SHADER_DATA_TYPE_FLOAT):
            glUniform1fv(glGetUniformLocation(OGL_CAST_GLUINT(sp->impl_state), uniform_name), 1, (f32*)val);
            break;
        case(OGL_SHADER_DATA_TYPE_VEC2):
            glUniform2fv(glGetUniformLocation(OGL_CAST_GLUINT(sp->impl_state), uniform_name), 1, (f32*)val);
            break;
        case(OGL_SHADER_DATA_TYPE_VEC3):
            glUniform3fv(glGetUniformLocation(OGL_CAST_GLUINT(sp->impl_state), uniform_name), 1, (f32*)val);
            break;
        case(OGL_SHADER_DATA_TYPE_VEC4):
            glUniform4fv(glGetUniformLocation(OGL_CAST_GLUINT(sp->impl_state), uniform_name), 1, (f32*)val);
            break;
        case(OGL_SHADER_DATA_TYPE_MAT4):
            glUniformMatrix4fv(glGetUniformLocation(OGL_CAST_GLUINT(sp->impl_state), uniform_name), 1, GL_FALSE, (f32*)val);
            break;
        default:
            assert(0);
            break;
    }
}


////////////////////////////////
// OGL draw API
////////////////////////////////

GLuint ogl_prim_to_gl_prim(oglPrimitive prim) {
    GLuint gl_prim = GL_TRIANGLES;
    switch (prim) {
        case OGL_PRIM_TRIANGLES:
            gl_prim = GL_TRIANGLES;
            break;
        case OGL_PRIM_TRIANGLE_STRIP:
            gl_prim = GL_TRIANGLE_STRIP;
            break;
        case OGL_PRIM_TRIANGLE_FAN:
            gl_prim = GL_TRIANGLE_FAN;
            break;
        case OGL_PRIM_LINES:
            gl_prim = GL_LINES;
            break;
        case OGL_PRIM_LINE_STRIP:
            gl_prim = GL_LINE_STRIP;
            break;
        default:
            gl_prim = GL_TRIANGLES;
            break;
    }
    return gl_prim;
}

void ogl_draw(oglPrimitive prim, u32 first, u32 count) {
    glDrawArrays(ogl_prim_to_gl_prim(prim), first, count);
}

void ogl_draw_instanced(oglPrimitive prim, u32 first, s32 count, u32 instance_count) {
    glDrawArraysInstanced(ogl_prim_to_gl_prim(prim), first, count, instance_count);
}

void ogl_draw_indexed(oglPrimitive prim, u32 count) {
    glDrawElements(ogl_prim_to_gl_prim(prim), count, GL_UNSIGNED_INT, 0);
}

////////////////////////////////
// OGL image API
////////////////////////////////

void ogl_rt_bind(oglImage *img) {
    if (img == NULL) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    } else {
        glBindFramebuffer(GL_FRAMEBUFFER, OGL_CAST_GLUINT(img->impl_state));
    }
}

//TODO: in case of RT, should we bind it first
void ogl_image_clear(oglImage *img) {
    ogl_rt_bind(img);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    //glClearColor(0.2 * cos(get_current_timestamp()/4000.0),0.2 * sin(get_current_timestamp()/3000.0),0.3 * cos(get_current_timestamp()/1000.0),1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ogl_rt_bind(NULL);
}

// Example usage to make a 2x2 image holding RED,GREEN,BLUE,WHITE colors
// oglImage rgba_tex = {0};
// u8 rgba_tex[] =
//     {
//         0xFF,0x00,0x00,0xFF,
//         0x00,0xFF,0x00,0xFF,
//         0x00,0x00,0xFF,0xFF,
//         0xFF,0xFF,0xFF,0xFF,
//     };
//     ogl_image_init(&rgba_tex, (u8*)white, 2, 2, OGL_IMAGE_FORMAT_RGBA8U)
b32 ogl_image_init(oglImage *img, u8 *tex_data, u32 tex_w, u32 tex_h, oglImageFormat fmt) {
    img->width = tex_w;
    img->height = tex_h;
    img->format = fmt;
    GLuint tex_format;
    switch (img->format) {
        case (OGL_IMAGE_FORMAT_RGBA8U): // normal textures
        case (OGL_IMAGE_FORMAT_RGB8U):
            tex_format = (img->format == OGL_IMAGE_FORMAT_RGBA8U) ? GL_RGBA : GL_RGB;
            img->kind = OGL_IMAGE_KIND_TEXTURE;
            glGenTextures(1, OGL_CAST_GLUINTPTR(img->impl_state));
            glBindTexture(GL_TEXTURE_2D, OGL_CAST_GLUINT(img->impl_state));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, tex_w, tex_h, 0, tex_format, GL_UNSIGNED_BYTE, tex_data);
            glGenerateMipmap(GL_TEXTURE_2D);
            break;
        case (OGL_IMAGE_FORMAT_R8U):
            img->kind = OGL_IMAGE_KIND_TEXTURE;
            glGenTextures(1, OGL_CAST_GLUINTPTR(img->impl_state));
            glBindTexture(GL_TEXTURE_2D, OGL_CAST_GLUINT(img->impl_state));
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // NOTE: is this really needed? its used only for font rendering random access
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            //glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, tex_w, tex_h, 0, GL_RED, GL_UNSIGNED_BYTE, tex_data);
            #if ARCH_WASM64 || ARCH_WASM32
                glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, tex_w, tex_h, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, tex_data);
            #else
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, tex_w, tex_h, 0, GL_RED, GL_UNSIGNED_BYTE, tex_data);
            #endif
            break;
        case (OGL_IMAGE_FORMAT_RGBA32F): // framebuffers
            img->kind = OGL_IMAGE_KIND_RT;
            glGenFramebuffers(1, OGL_CAST_GLUINTPTR(img->impl_state));
            glBindFramebuffer(GL_FRAMEBUFFER, OGL_CAST_GLUINT(img->impl_state));

            // - position color buffer
            glGenTextures(1, &img->attachments[0]);
            glBindTexture(GL_TEXTURE_2D, img->attachments[0]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tex_w, tex_h, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, img->attachments[0], 0);

            // - normal color buffer
            glGenTextures(1, &img->attachments[1]);
            glBindTexture(GL_TEXTURE_2D, img->attachments[1]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tex_w, tex_h, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, img->attachments[1], 0);

            // - color + specular color buffer
            glGenTextures(1, &img->attachments[2]);
            glBindTexture(GL_TEXTURE_2D, img->attachments[2]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tex_w, tex_h, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, img->attachments[2], 0);

            u32 attachments_gl[3]= { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
            glDrawBuffers(3, &attachments_gl[0]);

            GLuint rbo;
            glGenRenderbuffers(1, &rbo);
            glBindRenderbuffer(GL_RENDERBUFFER, rbo);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, tex_w, tex_h);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

            assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
        default:
            assert(0);
            break;
    }
    return (OGL_CAST_GLUINT(img->impl_state) != 0);
}

void ogl_image_deinit(oglImage *img) {
    if (img->kind == OGL_IMAGE_KIND_RT){
        glDeleteFramebuffers(1,OGL_CAST_GLUINTPTR(img->impl_state));
    }else {
        glDeleteTextures(1,OGL_CAST_GLUINTPTR(img->impl_state));
    }
}


void ogl_bind_image_to_texture_slot(oglImage *img, u32 tex_slot, u32 attachment) {
    glActiveTexture_dl(GL_TEXTURE0 + tex_slot);
    if (img->kind == OGL_IMAGE_KIND_RT) {
        glBindTexture(GL_TEXTURE_2D, img->attachments[attachment]);
    } else {
        glBindTexture(GL_TEXTURE_2D, OGL_CAST_GLUINT(img->impl_state));
    }
}