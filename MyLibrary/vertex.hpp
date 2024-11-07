#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
namespace my_library
{
   namespace vertex
   {
      enum class Layout
      {
         BASIC,
         TEXTURE,
         COUNT,
      };
      struct BasicLayout
      {
         glm::vec2 position;
         glm::vec3 color;
      };
   }    // namespace vertex
}    // namespace my_library