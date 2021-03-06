cmake_minimum_required(VERSION 2.8.4)
project(musicplayer.openmptplugin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++11")

file(GLOB SOUNDLIB_FILES soundlib/*.cpp)
file(GLOB MPT_FILES common/mpt*.cpp)

set(SOURCE_FILES
	openmpt_plugin.cpp
	#common/AudioCriticalSection.cpp
	#common/stdafx.cpp
	${MPT_FILES}
	common/Logging.cpp
	common/misc_util.cpp
	#common/Profiler.cpp
	common/serialization_utils.cpp
	common/typedefs.cpp
	common/version.cpp
	common/ComponentManager.cpp
	libopenmpt/libopenmpt_c.cpp
	libopenmpt/libopenmpt_cxx.cpp
	libopenmpt/libopenmpt_impl.cpp
	libopenmpt/libopenmpt_ext.cpp
	${SOUNDLIB_FILES}
)

include_directories(common common/svn_version_default .)
add_definitions(-DLIBOPENMPT_BUILD -DMPT_CHARSET_CUSTOMUTF8 -DMPT_WITH_ZLIB -DNO_PLUGINS)

find_package(ZLIB)
include_directories(${ZLIB_INCLUDE_DIRS})

add_library(openmptplugin SHARED ${SOURCE_FILES})
target_link_libraries(openmptplugin ${ZLIB_LIBRARIES})
