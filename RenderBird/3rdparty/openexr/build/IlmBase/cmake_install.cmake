# Install script for directory: D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OpenEXR" TYPE FILE FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/IlmBase/config/IlmBaseConfig.h")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/IlmBase/Half/cmake_install.cmake")
  include("D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/IlmBase/Iex/cmake_install.cmake")
  include("D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/IlmBase/IexMath/cmake_install.cmake")
  include("D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/IlmBase/Imath/cmake_install.cmake")
  include("D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/IlmBase/IlmThread/cmake_install.cmake")
  include("D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/IlmBase/HalfTest/cmake_install.cmake")
  include("D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/IlmBase/IexTest/cmake_install.cmake")
  include("D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/IlmBase/ImathTest/cmake_install.cmake")

endif()

