#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(vertices = 1) out;

layout(set = 0, binding = 0) uniform CameraBufferObject {
    mat4 view;
    mat4 proj;
} camera;

in gl_PerVertex {
    vec4 gl_Position;
} gl_in[];

out gl_PerVertex {
    vec4 gl_Position;
} gl_out[];

layout(location = 0) in vec4[] in_v0;
layout(location = 1) in vec4[] in_v1;
layout(location = 2) in vec4[] in_v2;
layout(location = 3) in vec4[] in_up;

layout(location = 0) out vec4[] out_v0;
layout(location = 1) out vec4[] out_v1;
layout(location = 2) out vec4[] out_v2;

void main() {
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    out_v0[gl_InvocationID] = in_v0[gl_InvocationID];
    out_v1[gl_InvocationID] = in_v1[gl_InvocationID];
    out_v2[gl_InvocationID] = in_v2[gl_InvocationID];

    vec3 camera_position = inverse(camera.view)[3].xyz;
    vec3 blade_root = in_v0[gl_InvocationID].xyz;
    vec3 up_direction = normalize(in_up[gl_InvocationID].xyz);

    vec3 to_camera = blade_root - camera_position;
    float ground_distance = length(to_camera - up_direction * dot(to_camera, up_direction));

    float max_tess_level = 10.0;
    float min_tess_level = 2.0;
    float falloff_rate = 0.15;

    float tess_level = max_tess_level * exp(-falloff_rate * ground_distance);
    tess_level = clamp(tess_level, min_tess_level, max_tess_level);

    gl_TessLevelInner[0] = tess_level;
    gl_TessLevelInner[1] = tess_level;
    gl_TessLevelOuter[0] = tess_level;
    gl_TessLevelOuter[1] = tess_level;
    gl_TessLevelOuter[2] = tess_level;
    gl_TessLevelOuter[3] = tess_level;
}
