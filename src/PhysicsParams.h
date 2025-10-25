#pragma once

#include <glm/glm.hpp>
#include "Device.h"

// Physics parameters for grass simulation
// This structure matches the layout expected by the compute shader
// IMPORTANT: Must match std140 layout (vec3 aligned to 16 bytes)
struct PhysicsParamsData {
    // Gravity
    float gravityStrength = 2.45f;
    float padding1[3];  // Align next vec3 to 16 bytes

    // Wind
    glm::vec3 windDirection = glm::vec3(1.0f, 0.0f, 0.0f);
    float windStrength = 5.0f;

    float windFrequency = 1.0f;
    float turbulenceStrength = 3.0f;
    float padding2[2];  // Padding for alignment
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
