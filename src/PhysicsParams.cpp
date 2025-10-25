#include "PhysicsParams.h"
#include "BufferUtils.h"
#include <cstring>

PhysicsParams::PhysicsParams(Device* device) : device(device) {
    // Create uniform buffer for physics parameters
    BufferUtils::CreateBuffer(
        device,
        sizeof(PhysicsParamsData),
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        buffer,
        bufferMemory
    );

    // Map buffer memory persistently
    vkMapMemory(device->GetVkDevice(), bufferMemory, 0, sizeof(PhysicsParamsData), 0, &mappedData);

    // Copy initial data
    memcpy(mappedData, &paramsData, sizeof(PhysicsParamsData));
}

PhysicsParams::~PhysicsParams() {
    vkUnmapMemory(device->GetVkDevice(), bufferMemory);
    vkDestroyBuffer(device->GetVkDevice(), buffer, nullptr);
    vkFreeMemory(device->GetVkDevice(), bufferMemory, nullptr);
}

VkBuffer PhysicsParams::GetBuffer() const {
    return buffer;
}

PhysicsParamsData& PhysicsParams::GetData() {
    return paramsData;
}

void PhysicsParams::UpdateBuffer() {
    memcpy(mappedData, &paramsData, sizeof(PhysicsParamsData));
}
