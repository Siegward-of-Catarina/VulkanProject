#pragma once

#include <vulkan/vulkan.hpp>

namespace my_library::vkm
{
   template<typename... ChainElements>
   using StructureChain        = vk::StructureChain<ChainElements...>;
   using ApplicationInfo       = vk::ApplicationInfo;
   using LayerProperties       = vk::LayerProperties;
   using ExtensionProperties   = vk::ExtensionProperties;
   using DispatchLoaderDynamic = vk::DispatchLoaderDynamic;
   namespace instance
   {
      using UniqueHandle = vk::UniqueHandle<vk::Instance, DispatchLoaderDynamic>;
      using CreateInfo   = vk::InstanceCreateInfo;
   }    // namespace instance
   namespace device
   {
      using Gpu                              = vk::PhysicalDevice;
      using GpuFeatures                      = vk::PhysicalDeviceFeatures;
      using GpuExtendedDynamicState3Features = vk::PhysicalDeviceExtendedDynamicState3FeaturesEXT;
      using GpuProperties                    = vk::PhysicalDeviceProperties;
      using Size                             = vk::DeviceSize;
      using CreateInfo                       = vk::DeviceCreateInfo;
      using UniqueHandle                     = vk::UniqueHandle<vk::Device, DispatchLoaderDynamic>;

      using MemoryPropertyFlags    = vk::MemoryPropertyFlags;
      using MemoryPropertyFlagBits = vk::MemoryPropertyFlagBits;
      using MemoryProperties       = vk::PhysicalDeviceMemoryProperties;
      using MemoryRequirements     = vk::MemoryRequirements;
      using MemoryAllocateInfo     = vk::MemoryAllocateInfo;
      using UniqueMemory           = vk::UniqueHandle<vk::DeviceMemory, DispatchLoaderDynamic>;
   }    // namespace device
   namespace queue
   {
      using vkQueue     = vk::Queue;
      using CreateInfo  = vk::DeviceQueueCreateInfo;
      using SubmitInfo  = vk::SubmitInfo;
      using PresentInfo = vk::PresentInfoKHR;
      using SharingMode = vk::SharingMode;
   }    // namespace queue
   namespace debug_utl_msgr
   {
      using UniqueHandle        = vk::UniqueHandle<vk::DebugUtilsMessengerEXT, DispatchLoaderDynamic>;
      using SeverityFlagBitsEXT = vk::DebugUtilsMessageSeverityFlagBitsEXT;
      using TypeFlagBitsEXT     = vk::DebugUtilsMessageTypeFlagBitsEXT;
      using SeverityFlagsEXT    = vk::DebugUtilsMessageSeverityFlagsEXT;
      using TypeFlagsEXT        = vk::DebugUtilsMessageTypeFlagsEXT;
      using CallbackDataEXT     = vk::DebugUtilsMessengerCallbackDataEXT;
      using CreateInfoEXT       = vk::DebugUtilsMessengerCreateInfoEXT;
   }    // namespace debug_utl_msgr
   namespace surface
   {
      using UniqueHandle = vk::UniqueSurfaceKHR;
      using Capabilities = vk::SurfaceCapabilitiesKHR;
      using Format       = vk::SurfaceFormatKHR;
   }    // namespace surface
   namespace swapchain
   {
      using CreateInfo             = vk::SwapchainCreateInfoKHR;
      using UniqueHandle           = vk::UniqueHandle<vk::SwapchainKHR, vk::DispatchLoaderDynamic>;
      using PresentMode            = vk::PresentModeKHR;
      using ImageUsageFlagBits     = vk::ImageUsageFlagBits;
      using ImageUsageFlags        = vk::ImageUsageFlags;
      using CompositeAlphaFlagBits = vk::CompositeAlphaFlagBitsKHR;
      using CompositeAlphaFlags    = vk::CompositeAlphaFlagsKHR;
      using Image                  = vk::Image;
   }    // namespace swapchain
   namespace image_view
   {
      using UniqueHandle = vk::UniqueHandle<vk::ImageView, vk::DispatchLoaderDynamic>;
      using CreateInfo   = vk::ImageViewCreateInfo;
      using Type         = vk::ImageViewType;

      using ComponentMapping = vk::ComponentMapping;
      using ComponentSwizzle = vk::ComponentSwizzle;

      using ImageSubresourceRange = vk::ImageSubresourceRange;

      using ImageAspectFlags    = vk::ImageAspectFlags;
      using ImageAspectFlagBits = vk::ImageAspectFlagBits;
   }    // namespace image_view
   namespace image
   {
      using Format     = vk::Format;
      using ColorSpace = vk::ColorSpaceKHR;
      using Offset2D   = vk::Offset2D;
      using Extent2D   = vk::Extent2D;
   }    // namespace image
   namespace shader_module
   {
      using CreateInfo   = vk::ShaderModuleCreateInfo;
      using UniqueHandle = vk::UniqueHandle<vk::ShaderModule, vk::DispatchLoaderDynamic>;
   }    // namespace shader_module
   namespace shader_stage
   {
      using CreateInfo = vk::PipelineShaderStageCreateInfo;
      using FlagBits   = vk::ShaderStageFlagBits;
      using Flags      = vk::ShaderStageFlags;
   }    // namespace shader_stage
   namespace input_stage
   {
      using VertexInputStateCreateInfo = vk::PipelineVertexInputStateCreateInfo;
      using BindingDescription         = vk::VertexInputBindingDescription;
      using AttributeDescription       = vk::VertexInputAttributeDescription;
      using AssemblyStateCreateInfo    = vk::PipelineInputAssemblyStateCreateInfo;
      using VertexInputRate            = vk::VertexInputRate;
   }    // namespace input_stage
   namespace pipeline
   {
      using RasterrizationStateCreateInfo = vk::PipelineRasterizationStateCreateInfo;

      using DynamicStateCreateInfo      = vk::PipelineDynamicStateCreateInfo;
      using MultisampleStateCreateInfo  = vk::PipelineMultisampleStateCreateInfo;
      using DepthStencilStateCreateInfo = vk::PipelineDepthStencilStateCreateInfo;
      using ColorBlendStateCreateInfo   = vk::PipelineColorBlendStateCreateInfo;
      using LayoutCreateInfo            = vk::PipelineLayoutCreateInfo;
      using CreateInfo                  = vk::GraphicsPipelineCreateInfo;

      using ColorBlendAttachmentState = vk::PipelineColorBlendAttachmentState;
      using UniqueLayout              = vk::UniqueHandle<vk::PipelineLayout, DispatchLoaderDynamic>;
      using UniqueHandle              = vk::UniqueHandle<vk::Pipeline, DispatchLoaderDynamic>;

      using StageFlags = vk::PipelineStageFlags;

      using DynamicState           = vk::DynamicState;
      using SampleCountFlagBits    = vk::SampleCountFlagBits;
      using PrimitiveTopology      = vk::PrimitiveTopology;
      using PolygonMode            = vk::PolygonMode;
      using CullModeFlagBits       = vk::CullModeFlagBits;
      using FrontFace              = vk::FrontFace;
      using ColorComponentFlagBits = vk::ColorComponentFlagBits;
      using BlendFactor            = vk::BlendFactor;
      using BlendOp                = vk::BlendOp;
      using LogicOp                = vk::LogicOp;
      using CreateFlagBits         = vk::PipelineCreateFlagBits;
      using BindPoint              = vk::PipelineBindPoint;
      using StageFlagBits          = vk::PipelineStageFlagBits;
      using BindPoint              = vk::PipelineBindPoint;
   }    // namespace pipeline
   namespace render_pass
   {
      using AttachmentDescription = vk::AttachmentDescription;
      using AttachmentReference   = vk::AttachmentReference;
      using SubpassDescription    = vk::SubpassDescription;
      using SubpassDependency     = vk::SubpassDependency;
      using CreateInfo            = vk::RenderPassCreateInfo;
      using BeginInfo             = vk::RenderPassBeginInfo;

      using UniqueHandle = vk::UniqueHandle<vk::RenderPass, DispatchLoaderDynamic>;

      using AttachmentLoadOp  = vk::AttachmentLoadOp;
      using AttachmentStoreOp = vk::AttachmentStoreOp;
      using ImageLayout       = vk::ImageLayout;
      using SubpassContents   = vk::SubpassContents;
      using AccessFlagBits    = vk::AccessFlagBits;
   }    // namespace render_pass
   namespace framebuffer
   {
      using CreateInfo   = vk::FramebufferCreateInfo;
      using UniqueHandle = vk::UniqueHandle<vk::Framebuffer, DispatchLoaderDynamic>;
      using ClearValue   = vk::ClearValue;

      using ViewportStateCreateInfo = vk::PipelineViewportStateCreateInfo;
      using Viewport                = vk::Viewport;
      using Rect2D                  = vk::Rect2D;
   }    // namespace framebuffer
   namespace command
   {
      using PoolCreateInfo     = vk::CommandPoolCreateInfo;
      using BufferAllocateInfo = vk::CommandBufferAllocateInfo;
      using BufferBeginInfo    = vk::CommandBufferBeginInfo;

      using UniquePool   = vk::UniqueHandle<vk::CommandPool, DispatchLoaderDynamic>;
      using UniqueBuffer = vk::UniqueHandle<vk::CommandBuffer, DispatchLoaderDynamic>;

      using PoolResetFlagBits   = vk::CommandPoolResetFlagBits;
      using PoolCreateFlagBits  = vk::CommandPoolCreateFlagBits;
      using BufferLevel         = vk::CommandBufferLevel;
      using BufferUsageFlagBits = vk::CommandBufferUsageFlagBits;
   }    // namespace command
   namespace sync
   {
      using SemaphoreCreateInfo = vk::SemaphoreCreateInfo;
      using FenceCreateInfo     = vk::FenceCreateInfo;

      using UniqueSemaphore = vk::UniqueHandle<vk::Semaphore, DispatchLoaderDynamic>;
      using UniqueFence     = vk::UniqueHandle<vk::Fence, DispatchLoaderDynamic>;

      using FenceCreateFlagBits = vk::FenceCreateFlagBits;
   }    // namespace sync
   namespace buffer
   {
      using CreateInfo    = vk::BufferCreateInfo;
      using vkBuffer      = vk::Buffer;
      using UniqueHandle  = vk::UniqueHandle<vk::Buffer, DispatchLoaderDynamic>;
      using UsageFlagBits = vk::BufferUsageFlagBits;
      using UsageFlags    = vk::BufferUsageFlags;
      using CopyRegion    = vk::BufferCopy;

   }    // namespace buffer
}    // namespace my_library::vkm
