project (uapi)
cmake_minimum_required (VERSION 2.6)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
aux_source_directory(${PROJECT_SOURCE_DIR} uapi_srcs)
if (WIN32)
  add_definitions( -DWIN32 -D_WIN32)
endif (WIN32)

set (LINK STATIC)
add_library (uapi ${LINK} ${uapi_srcs})
