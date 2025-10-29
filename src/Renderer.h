#pragma once

#include "Device.h"
#include "SwapChain.h"
#include "Scene.h"
#include "Camera.h"
#include <fstream>
#include <vector>
#include <string>

class Renderer {
public:
    Renderer() = delete;
    Renderer(Device* device, SwapChain* swapChain, Scene* scene, Camera* camera);
    ~Renderer();

    void CreateCommandPools();

    void CreateRenderPass();

    void CreateCameraDescriptorSetLayout();
    void CreateModelDescriptorSetLayout();
    void CreateTimeDescriptorSetLayout();
    void CreateComputeDescriptorSetLayout();
    void CreatePhysicsDescriptorSetLayout();

    void CreateDescriptorPool();

    void CreateCameraDescriptorSet();
    void CreateModelDescriptorSets();
    void CreateGrassDescriptorSets();
    void CreateTimeDescriptorSet();
    void CreateComputeDescriptorSets();
    void CreatePhysicsDescriptorSet();

    void CreateGraphicsPipeline();
    void CreateGrassPipeline();
    void CreateComputePipeline();

    void CreateFrameResources();
    void DestroyFrameResources();
    void RecreateFrameResources();

    void RecordCommandBuffers();
    void RecordComputeCommandBuffer();

    void InitImGui();
    void CleanupImGui();
    void RenderImGui();

    void Frame();

private:
    Device* device;
    VkDevice logicalDevice;
    SwapChain* swapChain;
    Scene* scene;
    Camera* camera;

    VkCommandPool graphicsCommandPool;
    VkCommandPool computeCommandPool;

    VkRenderPass renderPass;

    VkDescriptorSetLayout cameraDescriptorSetLayout;
    VkDescriptorSetLayout modelDescriptorSetLayout;
    VkDescriptorSetLayout timeDescriptorSetLayout;
    VkDescriptorSetLayout computeDescriptorSetLayout;
    VkDescriptorSetLayout physicsDescriptorSetLayout;

    VkDescriptorPool descriptorPool;
    VkDescriptorPool imguiDescriptorPool;

    VkDescriptorSet cameraDescriptorSet;
    std::vector<VkDescriptorSet> modelDescriptorSets;
    std::vector<VkDescriptorSet> grassDescriptorSets;
    VkDescriptorSet timeDescriptorSet;
    std::vector<VkDescriptorSet> computeDescriptorSets;
    VkDescriptorSet physicsDescriptorSet;

    VkPipelineLayout graphicsPipelineLayout;
    VkPipelineLayout grassPipelineLayout;
    VkPipelineLayout computePipelineLayout;

    VkPipeline graphicsPipeline;
    VkPipeline grassPipeline;
    VkPipeline computePipeline;

    std::vector<VkImageView> imageViews;
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;
    std::vector<VkFramebuffer> framebuffers;

    std::vector<VkCommandBuffer> commandBuffers;
    VkCommandBuffer computeCommandBuffer;

    uint32_t lastBladeCount = 1 << 20; 

    // FPS and frame time tracking
    static constexpr int FRAME_TIME_HISTORY_SIZE = 1000;
    std::vector<float> frameTimeHistory;
    std::vector<float> fpsHistory;
    int frameTimeIndex = 0;
    bool frameTimeHistoryFilled = false;

    // ImGui update throttling
    static constexpr float IMGUI_UPDATE_INTERVAL = 0.2f;  // Update every 0.2 seconds
    float timeSinceLastImGuiUpdate = 0.0f;
    float cachedCurrentFps = 0.0f;
    float cachedAvgFps = 0.0f;
    float cachedOnePercentLowFps = 0.0f;
    int cachedSampleCount = 0;
    float cachedCurrentFrameTime = 0.0f;
    float cachedMaxFrameTime = 0.0f;

    void ResetPerformanceMetrics();
    void UpdatePerformanceMetrics(float deltaTime);

    // Automated performance testing
    enum class TestState {
        Idle,
        Running,
        ChangingConfig,
        Measuring
    };

    struct TestConfig {
        uint32_t bladeCount;
        bool orientationCulling;
        bool viewFrustumCulling;
        bool distanceCulling;
    };

    TestState testState = TestState::Idle;
    std::vector<TestConfig> testConfigs;
    size_t currentTestIndex = 0;
    float testTimer = 0.0f;
    float testMeasurementDuration = 5.0f;  // 5 second measurement window
    std::vector<float> testFpsSamples;
    std::ofstream testCsvFile;
    std::string testCsvFilePath;

    void StartPerformanceTesting();
    void UpdatePerformanceTesting(float deltaTime);
    void WriteTestResultToCSV(const TestConfig& config, float avgFps);
    std::string GetCPUName();
    std::string GetGPUName();
};
