#pragma once

#include "vkObjectTypes.hpp"

namespace my_library
{
   namespace vkm
   {
      namespace debug_utl
      {
         const MesseagSeverityFlagsEXT SeverityFlagsAllBits =
           MessageSeverityFlagBitsEXT::eError | MessageSeverityFlagBitsEXT::eWarning | MessageSeverityFlagBitsEXT::eInfo
           | MessageSeverityFlagBitsEXT::eVerbose;

         const MessageTypeFlagsEXT MessageTypeFlagsAllBits =
           MessageTypeFlagBitsEXT::eDeviceAddressBinding | MessageTypeFlagBitsEXT::ePerformance
           | MessageTypeFlagBitsEXT::eValidation | MessageTypeFlagBitsEXT::eGeneral;

         const PFN_vkDebugUtilsMessengerCallbackEXT callBack();

         const MessengerCreateInfoEXT
         populateCreateInfo();

      }    // namespace debug_utl
   }       // namespace vkm
}    // namespace my_library
