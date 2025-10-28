#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(quads, equal_spacing, ccw) in;

layout(set = 0, binding = 0) uniform CameraBufferObject {
    mat4 view;
    mat4 proj;
} camera;

layout(location = 0) in vec4[] v0;
layout(location = 1) in vec4[] v1;
layout(location = 2) in vec4[] v2;

layout(location = 0) out vec3 fs_nor;
layout(location = 1) out float fs_v;

void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec3 p0 = v0[0].xyz;
    vec3 p1 = v1[0].xyz;
    vec3 p2 = v2[0].xyz;

    float orientation = v0[0].w;
    float height = v1[0].w;
    float width = v2[0].w;

    vec3 theta = normalize(vec3(-cos(orientation), 0.0, sin(orientation)));

    //deCasteljau
    vec3 a = mix(p0, p1, v);
    vec3 b = mix(p1, p2, v);
    vec3 c = mix(a, b, v);

    vec3 c0 = c - width * theta;
    vec3 c1 = c + width * theta;

    vec3 tangent = normalize(mix(p1 - p0, p2 - p1, v));

    vec3 n = normalize(cross(tangent, theta));
    fs_nor = n;
    fs_v = v;

    float t = u + 0.5 * v - u * v;
    vec3 pos = mix(c0, c1, t);

    gl_Position = camera.proj * camera.view * vec4(pos, 1.0);
}
