#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(LS_Sim)
#include(OpticksBuildOptions NO_POLICY_SCOPE)

#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED True)
option(WITH_G4CXOPTICKS "Build example with OPTICKS" OFF)

#================ Find Optix ===========================#
if(DEFINED ENV{OPTICKS_PREFIX})
   set(Opticks_VERBOSE YES)
   set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "$ENV{JUNOTOP}/opticks/cmake/Modules")
   find_package(Opticks MODULE)
   message(STATUS "${CMAKE_CURRENT_LIST_FILE} : Opticks_FOUND:${Opticks_FOUND}" )
endif()
#=======================================================#

include_directories (${CMAKE_CURRENT_SOURCE_DIR})

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available Vis drivers by default
# You can set WITH_GEANT4_VIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#

option(WITH_GEANT4_VIS "Build example with Geant4 Vis drivers" OFF)
message("WITH_GEANT4_VIS: ${WITH_GEANT4_VIS}")
if(WITH_GEANT4_VIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED )
endif()

#----------------------------------------------------------------------------
##note that for MacOS, no RPATH is allowed for install
##a warning will be given when set INSTALL_RPATH

# for mac
set(CMAKE_MACOSX_RPATH 1)

# the RPATH to be used when building  
set(CMAKE_BUILD_RPATH 
   "./"
   )
#  "/Users/liuqian/Applications/Qt5.6.0/5.6/clang_64/lib")
   
#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Setup ROOT include directories and compile definitions
#

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED)

# check version
if(ROOT_FOUND)
    message("ROOT 6 has been found!")
    message($ENV{ROOTSYS})
  STRING(REGEX MATCH "6.*" VERSION6MATCH ${ROOT_VERSION})
  if(NOT VERSION6MATCH)
    message(FATAL_ERROR "--> ERROR: ROOT 6 is required but not found")
  endif()
endif()

include(${ROOT_USE_FILE})

#----------------------------------------------------------------------------
# Find QT (required package)   
#
#find_package(Qt5Widgets)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${Geant4_INCLUDE_DIR})
include_directories(${ROOT_INCLUDE_DIR})
#include_directories(${OPTICKS_INCLUDE_DIR})

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE sources "${PROJECT_SOURCE_DIR}/src/*.cc")
file(GLOB_RECURSE headers "${PROJECT_SOURCE_DIR}/include/*.hh")

#list(FILTER sources EXCLUDE REGEX "${PROJECT_SOURCE_DIR}/package/*")


#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#

message("Geant4 Libraries: ${Geant4_LIBRARIES}")

add_executable(example example.cc ${sources} ${headers})
target_link_libraries(example 
						PUBLIC
						${Geant4_LIBRARIES}
						PUBLIC 
						${ROOT_LIBRARIES}
						PUBLIC
						$<$<BOOL:${Opticks_FOUND}>:${Opticks_TARGET}>
                     )



#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build My. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(EXAMPLE_SCRIPTS
  example.in
  example.out
  init_vis.mac
  #run1.mac
  #run2.mac
  vis.mac
  #photon.mac
  )

foreach(_script ${EXAMPLE_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()


#----------------------------------------------------------------------------
# Add program to the project targets
# (this avoids the need of typing the program name after make)
#
add_custom_target(LS_Sim DEPENDS example)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS example DESTINATION bin)

