# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/Users/carter/Pac-man/PTSD/lib/glm")
  file(MAKE_DIRECTORY "/Users/carter/Pac-man/PTSD/lib/glm")
endif()
file(MAKE_DIRECTORY
  "/Users/carter/Pac-man/build_local/_deps/glm-build"
  "/Users/carter/Pac-man/build_local/_deps/glm-subbuild/glm-populate-prefix"
  "/Users/carter/Pac-man/build_local/_deps/glm-subbuild/glm-populate-prefix/tmp"
  "/Users/carter/Pac-man/build_local/_deps/glm-subbuild/glm-populate-prefix/src/glm-populate-stamp"
  "/Users/carter/Pac-man/build_local/_deps/glm-subbuild/glm-populate-prefix/src"
  "/Users/carter/Pac-man/build_local/_deps/glm-subbuild/glm-populate-prefix/src/glm-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/carter/Pac-man/build_local/_deps/glm-subbuild/glm-populate-prefix/src/glm-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/carter/Pac-man/build_local/_deps/glm-subbuild/glm-populate-prefix/src/glm-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
