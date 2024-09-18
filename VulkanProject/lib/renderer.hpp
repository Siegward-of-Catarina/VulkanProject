#pragma once

#include "./enumExtensions.hpp"
#include "./utilities.hpp"
#include "./vulkan/vkObjectTypes.hpp"

#include <memory>

namespace my_library
{
   namespace window
   {
      class GLwindow;
      class GLwindowForVk;
   }
   namespace vkm
   {
      class Vulkan;
   }
   namespace renderer
   {
      enum Platform
      {
         DirectX,
         Vulkan
      };
      enum class DebugSeverityFlagBits : uint32_t
      {
         Warning = 1,
         Error   = 1 << 1,
         Verbose = 1 << 2,
         MaxCount = 1 << 3
      };
      enum class DebugMessageTypeFlagBits : uint32_t
      {
         General = 1,
         Performance = 1 << 1,
         Validation = 1 << 2,
         MaxCount = 1 << 3
      };
      struct CreateInfo
      {
         Platform                                       render_platform { Vulkan };
         std::unique_ptr<window::GLwindow>&             gl_window;
         std::string                                    app_name;
         bool                                           enable_debug { false };
         enumExtensions::BitFlag<DebugSeverityFlagBits> debug_severity_flags;
         enumExtensions::BitFlag<DebugMessageTypeFlagBits> debug_msg_type_flags;
      };
   }    // namespace renderer
   class Renderer
   {
   public:
      void
      init( const renderer::CreateInfo& create_info );

      Renderer();
      ~Renderer();

   private:
      std::unique_ptr<vkm::Vulkan> _vulkan;
   };
}    // namespace my_library
