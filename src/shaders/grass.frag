#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform CameraBufferObject {
    mat4 view;
    mat4 proj;
} camera;

layout(location = 0) in vec3 fs_nor;
layout(location = 1) in float fs_v;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 n = normalize(fs_nor);

    vec3 light = normalize(vec3(1.0, 1.0, 1.0));

    float diffuse = max(0.0, dot(n, light));

    vec3 grass_base = vec3(0.15, 0.4, 0.2);
    vec3 grass_tip = vec3(0.55, 0.75, 0.4);

    vec3 base_color = mix(grass_base, grass_tip, fs_v);

    float ambient_strength = 0.25;
    vec3 ambient = ambient_strength * base_color;

    vec3 final_color = ambient + diffuse * base_color * 0.75;

    outColor = vec4(final_color, 1.0);
}
