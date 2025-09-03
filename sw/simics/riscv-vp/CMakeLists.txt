#
# Basic/minimal setup with GCC and Ninja:
#
#   cmake -S . -B bt -G Ninja -DCMAKE_BUILD_TYPE=Debug
#   cmake --build bt
#
# To learn more, see official documentation:
#   https://cmake.org/cmake/help/latest/index.html
# and this nice tutorial:
#   https://cliutils.gitlab.io/modern-cmake/
# and this list of do's and dont's
#   https://gist.github.com/mbinna/c61dbb39bca0e4fb7d1f73b0d66a4fd1
#

# NOTE: This file is a copy of `cmake/CMakeLists-template.txt` from the Simics
#       Base package. Re-run project-setup with `--force` flag to replace it
#       with a new copy from Base.

# In general, newer is better but a minimum of 3.22 is required
cmake_minimum_required(VERSION 3.22)

# Setup project; customize to your liking
project(Simics_Project LANGUAGES C CXX ASM)

# The idea behind project-setup.cmake is that it can define three different
# functions:
# - setup_compiler_flags
# - setup_modules_pre
# - setup_modules_post
# and one variable:
# - WHERE_TO_GLOB_FOR_MODULES
#
# You can set WHERE_TO_GLOB_FOR_MODULES to either IN_PROJECT or IN_PACKAGES to
# restrict the searching for modules to one of the two scopes. If you do not
# set it (the default) both scopes will be searched, with project taking
# precedence over packages.
#
# This CMakeLists.txt will call the aforementioned functions if they exist.
#
# If "setup_compiler_flags" is available it will be called and then no C and
# CXX standard settings will be applied.  The expectation is that you then do
# this in setup_compiler_flags.  You can use it to do all sorts of similar
# settings, like policy settings and the likes.
#
# If "setup_modules_pre" is available, it will be called before any Simics
# modules are searched. This allows to include or add_subdirectory other things
# that modules might depend on.
#
# If "setup_modules_post" is available, it will be called after the Simics
# modules are searched. This allows to include or add_subdirectory additional
# things that you may want to build as well.

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/project-setup.cmake)
  include(${CMAKE_CURRENT_SOURCE_DIR}/project-setup.cmake)
endif()

if(COMMAND setup_compiler_flags)
  setup_compiler_flags()
else()
  set(CMAKE_C_STANDARD 11)
  set(CMAKE_C_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED OFF)
endif()

# In general, the Windows/MinGW setup does support longer paths. Setting MAX to
# 1000 suppresses the warnings CMake emits if the config ends up with paths
# that are too long.
set(CMAKE_OBJECT_PATH_MAX 1000)

# Enable CTest support early so BUILD_TESTING is defined for all modules
include(CTest)

# Load Simics from the Base package associated with the current project
find_file(project_paths_file .project-properties/project-paths
          REQUIRED NO_DEFAULT_PATH HINTS ${CMAKE_CURRENT_SOURCE_DIR})
file(STRINGS "${project_paths_file}" project_paths_lines)
foreach(line ${project_paths_lines})
  if(line MATCHES "^simics-root: (.*)")
    cmake_path(CONVERT "${CMAKE_MATCH_1}"
      TO_CMAKE_PATH_LIST _simics_base NORMALIZE)
    set(SIMICS_BASE "${_simics_base}")
    break()
  endif()
endforeach()
find_package(Simics REQUIRED CONFIG NO_DEFAULT_PATH HINTS ${SIMICS_BASE}/cmake)

if(COMMAND setup_modules_pre)
  setup_modules_pre()
endif()

# By default, we search in the project and then Simics Base and all add-on
# packages; but you can opt-out of this and only search in the projec or only
# search in the packages by passing `IN_PROJECT` or `IN_PACKAGES` to this
# function. To opt out of any type of globbing; just remove/comment-out the
# next line
simics_find_and_add_modules(${WHERE_TO_GLOB_FOR_MODULES})

if(COMMAND setup_modules_post)
  setup_modules_post()
endif()

# Put anything you want below OR into the config-user.cmake. The benefit of
# using the config-user.cmake is that you can pass --force flag to
# project-setup when upgrading Simics Base and not risk overwriting local
# configuration.
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/config-user.cmake)
  include(${CMAKE_CURRENT_SOURCE_DIR}/config-user.cmake)
endif()

# Add support for DML language server
generate_dml_compile_commands_json()
