# Install script for directory: D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/${LOCAL}/include/OpenEXR" TYPE FILE FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/config/OpenEXRConfig.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/${LOCAL}/share/doc/OpenEXR-2.3.0" TYPE FILE FILES
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/doc/TechnicalIntroduction.pdf"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/doc/ReadingAndWritingImageFiles.pdf"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/doc/OpenEXRFileLayout.pdf"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/doc/MultiViewOpenEXR.pdf"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/doc/InterpretingDeepPixels.pdf"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/doc/TheoryDeepPixels.pdf"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/${LOCAL}/share/doc/OpenEXR-2.3.0/examples" TYPE FILE FILES
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfExamples/main.cpp"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfExamples/drawImage.cpp"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfExamples/rgbaInterfaceExamples.cpp"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfExamples/rgbaInterfaceTiledExamples.cpp"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfExamples/generalInterfaceExamples.cpp"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfExamples/lowLevelIoExamples.cpp"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfExamples/previewImageExamples.cpp"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfExamples/generalInterfaceTiledExamples.cpp"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfExamples/generalInterfaceTiledExamples.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfExamples/drawImage.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfExamples/rgbaInterfaceExamples.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfExamples/generalInterfaceExamples.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfExamples/rgbaInterfaceTiledExamples.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfExamples/lowLevelIoExamples.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfExamples/previewImageExamples.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/OpenEXR/IlmImfExamples/namespaceAlias.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImf/cmake_install.cmake")
  include("D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImfUtil/cmake_install.cmake")
  include("D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImfExamples/cmake_install.cmake")
  include("D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImfTest/cmake_install.cmake")
  include("D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImfUtilTest/cmake_install.cmake")
  include("D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/IlmImfFuzzTest/cmake_install.cmake")
  include("D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/exrheader/cmake_install.cmake")
  include("D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/exrmaketiled/cmake_install.cmake")
  include("D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/exrstdattr/cmake_install.cmake")
  include("D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/exrmakepreview/cmake_install.cmake")
  include("D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/exrenvmap/cmake_install.cmake")
  include("D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/exrmultiview/cmake_install.cmake")
  include("D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/OpenEXR/exrmultipart/cmake_install.cmake")

endif()

