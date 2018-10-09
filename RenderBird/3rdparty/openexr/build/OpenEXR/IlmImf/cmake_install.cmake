# Install script for directory: D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "${LOCAL}")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImf/Debug/IlmImf-2_3_d.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImf/Release/IlmImf-2_3.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImf/MinSizeRel/IlmImf-2_3.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImf/RelWithDebInfo/IlmImf-2_3.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "/bin/IlmImf-2_3_d.dll")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "/bin" TYPE SHARED_LIBRARY FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImf/Debug/IlmImf-2_3_d.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "/bin/IlmImf-2_3.dll")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "/bin" TYPE SHARED_LIBRARY FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImf/Release/IlmImf-2_3.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "/bin/IlmImf-2_3.dll")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "/bin" TYPE SHARED_LIBRARY FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImf/MinSizeRel/IlmImf-2_3.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "/bin/IlmImf-2_3.dll")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "/bin" TYPE SHARED_LIBRARY FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImf/RelWithDebInfo/IlmImf-2_3.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImf/Debug/IlmImf-2_3_s_d.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImf/Release/IlmImf-2_3_s.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImf/MinSizeRel/IlmImf-2_3_s.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImf/RelWithDebInfo/IlmImf-2_3_s.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OpenEXR" TYPE FILE FILES
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfForward.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfExport.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfAttribute.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfBoxAttribute.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfCRgbaFile.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfChannelList.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfChannelListAttribute.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfCompressionAttribute.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfDoubleAttribute.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfFloatAttribute.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfFrameBuffer.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfHeader.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfIO.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfInputFile.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfIntAttribute.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfLineOrderAttribute.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfMatrixAttribute.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfOpaqueAttribute.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfOutputFile.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfRgbaFile.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfStringAttribute.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfVecAttribute.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfHuf.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfWav.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfLut.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfArray.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfCompression.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfLineOrder.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfName.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfPixelType.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfVersion.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfXdr.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfConvert.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfPreviewImage.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfPreviewImageAttribute.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfChromaticities.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfChromaticitiesAttribute.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfKeyCode.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfKeyCodeAttribute.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfTimeCode.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfTimeCodeAttribute.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfRational.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfRationalAttribute.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfFramesPerSecond.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfStandardAttributes.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfStdIO.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfEnvmap.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfEnvmapAttribute.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfInt64.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfRgba.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfTileDescription.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfTileDescriptionAttribute.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfTiledInputFile.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfTiledOutputFile.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfTiledRgbaFile.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfRgbaYca.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfTestFile.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfThreading.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfB44Compressor.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfStringVectorAttribute.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfMultiView.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfAcesFile.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfMultiPartOutputFile.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfGenericOutputFile.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfMultiPartInputFile.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfGenericInputFile.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfPartType.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfPartHelper.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfOutputPart.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfTiledOutputPart.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfInputPart.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfTiledInputPart.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfDeepScanLineOutputFile.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfDeepScanLineOutputPart.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfDeepScanLineInputFile.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfDeepScanLineInputPart.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfDeepTiledInputFile.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfDeepTiledInputPart.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfDeepTiledOutputFile.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfDeepTiledOutputPart.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfDeepFrameBuffer.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfDeepCompositing.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfCompositeDeepScanLine.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfNamespace.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfMisc.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfDeepImageState.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfDeepImageStateAttribute.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImf/ImfFloatVectorAttribute.h"
    )
endif()

