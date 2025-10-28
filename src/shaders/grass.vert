
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 1, binding = 0) uniform ModelBufferObject {
    mat4 model;
};

layout(location = 0) in vec4 in_v0;
layout(location = 1) in vec4 in_v1;
layout(location = 2) in vec4 in_v2;
layout(location = 3) in vec4 in_up;

layout(location = 0) out vec4 out_v0;
layout(location = 1) out vec4 out_v1;
layout(location = 2) out vec4 out_v2;
layout(location = 3) out vec4 out_up;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    out_v0 = model * in_v0;
    out_v1 = model * in_v1;
    out_v2 = model * in_v2;
    out_up = model * in_up;

    out_v0.w = in_v0.w;
    out_v1.w = in_v1.w;
    out_v2.w = in_v2.w;

    gl_Position = out_v0;
}
