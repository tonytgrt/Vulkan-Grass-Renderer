
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 1, binding = 0) uniform ModelBufferObject {
    mat4 model;
};

layout(location = 0) in vec4 v0;
layout(location = 1) in vec4 v1;
layout(location = 2) in vec4 v2;
layout(location = 3) in vec4 up;

layout(location = 0) out vec4 vert_v0;
layout(location = 1) out vec4 vert_v1;
layout(location = 2) out vec4 vert_v2;
layout(location = 3) out vec4 vert_up;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    float orientation = v0.w;
    float height = v1.w;
    float width = v2.w;

    vert_v0 = model * v0;
    vert_v1 = model * v1;
    vert_v2 = model * v2;
    vert_up = model * up;

    vert_v0.w = orientation;
    vert_v1.w = height;
    vert_v2.w = width;

    gl_Position = vert_v0;
}
