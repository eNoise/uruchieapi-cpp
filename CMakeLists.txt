project(uapi-test)
cmake_minimum_required(VERSION 2.6)

find_package(CURL REQUIRED)
find_package(Boost REQUIRED)
find_package(OpenSSL REQUIRED)

include_directories(
			${CMAKE_CURRENT_BINARY_DIR}
			${CURL_INCLUDE_DIR}
			${Boost_INCLUDE_DIRS}
			${OPENSSL_INCLUDE_DIR}
)

set(uapitest_SRCS 
		uapi_test.cpp
)

add_subdirectory(uapi)
include_directories ("${PROJECT_SOURCE_DIR}/uapi")
set (UAPI_LIBRARIES "uapi")

add_executable(uapi-test ${uapitest_SRCS})
target_link_libraries(uapi-test
			${CURL_LIBRARY}
			${Boost_LIBRARIES}
			${OPENSSL_LIBRARIES}

			${UAPI_LIBRARIES}
)
