cmake_minimum_required (VERSION 2.8)
project (VTKWriter)

# Pass options to GCC
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpic")

option(DEBUG_PRINT "Adds printf for debug output" OFF)
if (DEBUG_PRINT)
  add_definitions(-DDEBUG_PRINT)
endif()

set(PRECISION "double" CACHE STRING "Set precision (double/float)")
set_property(CACHE PRECISION PROPERTY STRINGS "double" "float")
add_definitions(-DDTYPE=${PRECISION})

find_package( ZLIB REQUIRED )
if ( ZLIB_FOUND )
    include_directories( ${ZLIB_INCLUDE_DIRS} )
endif( ZLIB_FOUND )

add_subdirectory(pugixml)
include_directories(pugixml)

add_library(vtkWriter VTKWriter.cpp)
add_library(dataArray DataArray.cpp)
add_library(base64 base64.cpp)
target_link_libraries(vtkWriter dataArray base64 pugixml ${ZLIB_LIBRARIES})

# Now build python interface
FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

if(MSVC_IDE)
  option(SWIG_DEBUG "Swig outputs to debug/ (default is release/)" OFF)
  if(SWIG_DEBUG)
    SET(CMAKE_SWIG_OUTDIR ${PROJECT_BINARY_DIR}/Debug)
  else()
    SET(CMAKE_SWIG_OUTDIR ${PROJECT_BINARY_DIR}/Release)
  endif()
endif()

FIND_PACKAGE(PythonLibs)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(CMAKE_SWIG_FLAGS "-DDTYPE=${PRECISION}")

SET_SOURCE_FILES_PROPERTIES(VTKWriter.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(VTKWriter.i PROPERTIES SWIG_FLAGS "-includeall")
SWIG_ADD_MODULE(VTKWriter python VTKWriter.i)
SWIG_LINK_LIBRARIES(VTKWriter ${PYTHON_LIBRARIES})
SWIG_LINK_LIBRARIES(VTKWriter vtkWriter)

## Add type specific writers here
add_library(vtiWriter VTIWriter.cpp)
add_library(vtpWriter VTPWriter.cpp)
target_link_libraries(vtkWriter vtiWriter vtpWriter)

set(CPACK_PACKAGE_VERSION "0.1.0")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_VENDOR "MIT Geonumerics")

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")

option(USE_WIX_TOOLSET "Use WIX" true)

if (WIN32)
    if (USE_WIX_TOOLSET)
        set(CPACK_GENERATOR "WIX") # this need WiX Tooset installed and a path to candle.exe
        set(CPACK_WIX_TEMPLATE "${CMAKE_CURRENT_SOURCE_DIR}/install/windows/vtkWriter.template.in")
        SET(CPACK_WIX_PRODUCT_GUID AF05A6B3-5B62-497B-90FF-6B4FD927CBFD)
        SET(CPACK_WIX_UPGRADE_GUID AF05A6B3-5B62-497B-90FF-6B4FD927CBFD)
        # SET(CPACK_WIX_PRODUCT_ICON ../rsc/app.ico)
        SET(CPACK_WIX_UI_BANNER  "${CMAKE_CURRENT_SOURCE_DIR}/install/windows/banner_493x58.bmp")
        SET(CPACK_WIX_UI_DIALOG  "${CMAKE_CURRENT_SOURCE_DIR}/install/windows/dialog_493x312.bmp")
    else ()
        set(CPACK_GENERATOR "NSIS") # this needs NSIS installed, and available
    endif ()
    set(CPACK_SOURCE_GENERATOR "ZIP")
    set(INSTALL_DEST .)
elseif ( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
    set(CPACK_GENERATOR "PackageMake")
else ()
  set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION "/usr/local")
  set(INSTALL_DEST local/VTKWriter)
  install(FILES ${CMAKE_SOURCE_DIR}/install/unix/set_vtk_writer_env.sh DESTINATION ${INSTALL_DEST})
    
  option(BUILD_RPM "Builds an RPM instead of a DEB" OFF)
  if(BUILD_RPM)
    set(CPACK_RPM_PACKAGE_AUTOREQ OFF) # should probably use CPACK_RPM_PACKAGE_CONFLICTS instead
    set(CPACK_GENERATOR "RPM")
  else()
    set(CPACK_GENERATOR "DEB")
    set(CPACK_SOURCE_GENERATOR "TGZ")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Bruce Jones") #required
  endif()
endif ()

# INSTALL(TARGETS ${SWIG_MODULE_swig_test_REAL_NAME} DESTINATION test)
install(TARGETS _VTKWriter DESTINATION ${INSTALL_DEST})
install(FILES ${CMAKE_BINARY_DIR}/VTKWriter.py DESTINATION ${INSTALL_DEST})
install(FILES ${ZLIB_LIBRARY_RELEASE} DESTINATION ${INSTALL_DEST})

include(CPack)
