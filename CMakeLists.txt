#Minimum version
cmake_minimum_required(VERSION 3.0)

#Define project
project(astro)

#Define file groups
file(GLOB ASTRO_EXE_FILES "src/*.cc" "src/runtime.rc" "src/runtime.cpp" "src/as_wrapper.cpp" "lib/angelscript/source/*.cpp" "lib/angelscript/add_on/scriptarray/scriptarray.cpp" "lib/angelscript/add_on/scriptbuilder/scriptbuilder.cpp" "lib/angelscript/add_on/scriptstdstring/scriptstdstring.cpp")

#Add header and lib directories
include_directories(${INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR}/lib/angelscript/include)
link_directories(${LINK_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR}/lib/irrlicht ${CMAKE_CURRENT_SOURCE_DIR}/lib/sdl)

#Add executable targets
add_executable(astro_exe ${ASTRO_EXE_FILES})

#Add platform specific options
add_definitions(-D_IRR_STATIC_LIB_)
target_compile_definitions(astro_exe PRIVATE ${COMPILE_DEFINITIONS} -DAS_MAX_PORTABILITY -DAS_NO_EXCEPTIONS -DAS_NO_THREADS -DAS_PROCESS_METADATA=0)
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	target_compile_definitions(astro_exe PRIVATE ${COMPILE_DEFINITIONS} -DWIN32)
	if (MSVC)
		message("Platform is Windows (MSVC)")
		#add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	else (MSVC)
		message("Platform is Windows (MinGW)")
		#set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c90 -pedantic-errors -Wall")
		set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++98 -fno-exceptions -fno-rtti")
		set(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} "-s -static")
		set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-s -static")
	endif (MSVC)
	target_link_libraries(astro_exe Irrlicht.win32 SDL.win32 SDL_mixer.win32 opengl32 winmm ${TARGET_LINK_LIBRARIES})
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	message("Platform is macOS")
	#add_definitions(-std=c++11 -stdlib=libc++)
	#set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c90 -pedantic-errors -Wall")
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++98 -fno-exceptions -fno-rtti")
	set(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} "-s")
	set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-s")
	target_link_libraries(astro_exe Irrlicht.mac ${TARGET_LINK_LIBRARIES})
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	message("Platform is Linux")
	#set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c90 -pedantic-errors -Wall")
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fno-exceptions -fno-rtti -fPIC")
	set(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} "-s")
	set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-s")
	target_link_libraries(astro_exe Irrlicht.linux -lGL -lX11 -lXxf86vm ${TARGET_LINK_LIBRARIES})
else (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	message("Unrecognized platform " ${CMAKE_SYSTEM_NAME})
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
