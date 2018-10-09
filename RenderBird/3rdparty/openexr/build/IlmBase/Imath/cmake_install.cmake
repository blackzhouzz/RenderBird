# Install script for directory: D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath

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
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/IlmBase/Imath/Debug/Imath-2_3_d.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/IlmBase/Imath/Release/Imath-2_3.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/IlmBase/Imath/MinSizeRel/Imath-2_3.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/IlmBase/Imath/RelWithDebInfo/Imath-2_3.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/IlmBase/Imath/Debug/Imath-2_3_d.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/IlmBase/Imath/Release/Imath-2_3.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/IlmBase/Imath/MinSizeRel/Imath-2_3.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/IlmBase/Imath/RelWithDebInfo/Imath-2_3.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/IlmBase/Imath/Debug/Imath-2_3_s_d.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/IlmBase/Imath/Release/Imath-2_3_s.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/IlmBase/Imath/MinSizeRel/Imath-2_3_s.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/gits/RenderBird/RenderBird/3rdparty/openexr/build/IlmBase/Imath/RelWithDebInfo/Imath-2_3_s.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OpenEXR" TYPE FILE FILES
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathBoxAlgo.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathBox.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathColorAlgo.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathColor.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathEuler.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathExc.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathExport.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathForward.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathFrame.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathFrustum.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathFrustumTest.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathFun.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathGL.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathGLU.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathHalfLimits.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathInt64.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathInterval.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathLimits.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathLineAlgo.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathLine.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathMath.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathMatrixAlgo.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathMatrix.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathNamespace.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathPlane.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathPlatform.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathQuat.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathRandom.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathRoots.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathShear.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathSphere.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathVecAlgo.h"
    "D:/gits/RenderBird/RenderBird/3rdparty/openexr/IlmBase/Imath/ImathVec.h"
    )
endif()

