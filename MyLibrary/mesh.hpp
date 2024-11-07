#pragma once
#include "vertex.hpp"
#include "vulkan/vertexBuffer.hpp"
namespace my_library
{
    namespace ctr
    {
       class Container;
   }
   class Mesh
   {
   public:
      Mesh(const ctr::Container& c);
      virtual ~Mesh();
   protected:
      std::vector<vertex::BasicLayout> _verties;
      std::vector<uint32_t> _indices;
   };
}    // namespace my_library
