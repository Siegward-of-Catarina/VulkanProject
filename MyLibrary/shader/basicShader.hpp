#pragma once
#include "../vulkan/vkObjectTypes.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace my_library
{
   namespace shader
   {
      class ShaderForVK;
   }

   class BasicShaders
   {
   public:
      BasicShaders();
      ~BasicShaders();
      void
      createModules( const vkm::device::UniqueHandle& device, const vkm::DispatchLoaderDynamic& dld );
      const std::array<vkm::shader_stage::CreateInfo, 2>
      getShaderStages();
      const vkm::input_stage::BindingDescription
      getVertInBindDesc();
      const std::array<vkm::input_stage::AttributeDescription, 2>
      getVertInAttribDesc();

   private:
      std::unique_ptr<shader::ShaderForVK> _vs;
      std::unique_ptr<shader::ShaderForVK> _ps;
   };
}    // namespace my_library
