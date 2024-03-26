cmake_minimum_required(VERSION 3.5...3.16)

project(ufoutility
	VERSION 2.0.0
	DESCRIPTION "The UFO utility library"
	LANGUAGES CXX
)

add_library(ufoutility SHARED 
	src/timer.cpp
	src/timing.cpp
	src/io/read_buffer.cpp
	src/io/write_buffer.cpp
	src/io/buffer.cpp
)
add_library(UFO::Utility ALIAS ufoutility)

set_target_properties(ufoutility PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	CXX_STANDARD 17
	CXX_EXTENSIONS OFF
)

target_include_directories(ufoutility PUBLIC
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

target_compile_features(ufoutility PUBLIC cxx_std_17)

include(GNUInstallDirs)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Only enable tests when we're the root project
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  add_subdirectory(tests)
endif()

install(TARGETS ufoutility
	EXPORT ufoutilityTargets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(EXPORT ufoutilityTargets
	FILE "ufoutilityTargets.cmake"
	NAMESPACE UFO::
	DESTINATION lib/cmake/${PROJECT_NAME}
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
	"${PROJECT_SOURCE_DIR}/cmake/ufoutilityConfig.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/ufoutilityConfig.cmake"
	INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
)

write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/ufoutilityConfigVersion.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion
)

INSTALL(
	FILES
	"${CMAKE_CURRENT_BINARY_DIR}/ufoutilityConfig.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/ufoutilityConfigVersion.cmake"
	DESTINATION lib/cmake/${PROJECT_NAME}
)

install(
	DIRECTORY ${PROJECT_SOURCE_DIR}/include/
	DESTINATION include
)