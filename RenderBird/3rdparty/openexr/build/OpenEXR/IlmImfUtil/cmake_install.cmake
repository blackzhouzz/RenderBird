# Install script for directory: D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfUtil

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
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImfUtil/Debug/IlmImfUtil-2_3_d.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImfUtil/Release/IlmImfUtil-2_3.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImfUtil/MinSizeRel/IlmImfUtil-2_3.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImfUtil/RelWithDebInfo/IlmImfUtil-2_3.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImfUtil/Debug/IlmImfUtil-2_3_d.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImfUtil/Release/IlmImfUtil-2_3.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImfUtil/MinSizeRel/IlmImfUtil-2_3.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImfUtil/RelWithDebInfo/IlmImfUtil-2_3.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImfUtil/Debug/IlmImfUtil-2_3_s_d.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImfUtil/Release/IlmImfUtil-2_3_s.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImfUtil/MinSizeRel/IlmImfUtil-2_3_s.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImfUtil/RelWithDebInfo/IlmImfUtil-2_3_s.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/${LOCAL}/include/OpenEXR" TYPE FILE FILES
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfUtil/ImfImageChannel.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfUtil/ImfFlatImageChannel.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfUtil/ImfDeepImageChannel.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfUtil/ImfSampleCountChannel.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfUtil/ImfImageLevel.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfUtil/ImfFlatImageLevel.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfUtil/ImfDeepImageLevel.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfUtil/ImfImage.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfUtil/ImfFlatImage.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfUtil/ImfDeepImage.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfUtil/ImfImageIO.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfUtil/ImfFlatImageIO.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfUtil/ImfDeepImageIO.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfUtil/ImfImageDataWindow.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfUtil/ImfImageChannelRenaming.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfUtil/ImfUtilExport.h"
    )
endif()

