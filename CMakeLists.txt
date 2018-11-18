cmake_minimum_required(VERSION 3.1)

set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
		STRING "Choose the type of build." FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
		"Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

project(WatchMe)
add_executable(watchme watchme.cc)
find_library(FAM_LIBRARY fam)
if(FAM_LIBRARY)
	target_link_libraries(watchme PUBLIC ${FAM_LIBRARY})
endif()

install(TARGETS watchme
	RUNTIME DESTINATION bin
	)
