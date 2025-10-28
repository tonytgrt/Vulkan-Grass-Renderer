#pragma once

#include <glm/glm.hpp>
#include "Device.h"

// Physics parameters for grass simulation
// This structure matches the layout expected by the compute shader
// IMPORTANT: Must match std140 layout (vec3 aligned to 16 bytes)
struct PhysicsParamsData {
    // Gravity
    float gravityStrength = 9.8f;
    float padding1[3];  // Align next vec3 to 16 bytes

    // Wind
    glm::vec3 windDirection = glm::vec3(1.0f, 0.0f, 0.0f);
    float windStrength = 0.0f;

    float windFrequency = 1.0f;
    float turbulenceStrength = 0.0f;

    // Culling parameters
    float orientationThreshold = 0.9f;    // Orientation culling threshold
    float frustumTolerance = 0.1f;         // Frustum culling tolerance

    float maxDistance = 30.0f;             // Max distance for culling
    float numBuckets = 10.0f;              // Number of distance buckets (as float for std140)
};

class PhysicsParams {
private:
    Device* device;
    PhysicsParamsData paramsData;

    VkBuffer buffer;
    VkDeviceMemory bufferMemory;
    void* mappedData;

public:
    PhysicsParams(Device* device);
    ~PhysicsParams();

    VkBuffer GetBuffer() const;
    PhysicsParamsData& GetData();
    void UpdateBuffer();
};
