#Minimum version
cmake_minimum_required(VERSION 3.0)

#Define project
project(astro)

#Define file groups
file(GLOB ASTRO_EXE_FILES "src/*.cc" "src/runtime.rc" "src/*.cpp" "src/script_ext/*.cpp" "lib/angelscript/source/*.cpp" "lib/angelscript/add_on/scriptbuilder/scriptbuilder.cpp" "lib/tinyfiledialogs/tinyfiledialogs.c")

#Add header and lib directories
include_directories(${INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR}/lib/angelscript/include ${CMAKE_CURRENT_SOURCE_DIR}/lib/sdl/include)
link_directories(${LINK_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR}/_CMAKE/_IRRLICHT ${CMAKE_CURRENT_SOURCE_DIR}/lib/sdl/lib)

#Add executable targets
add_executable(astro ${ASTRO_EXE_FILES})

#Add platform specific options
add_definitions(-D_IRR_STATIC_LIB_)
target_compile_definitions(astro PRIVATE ${COMPILE_DEFINITIONS} -DAS_MAX_PORTABILITY -DAS_NO_EXCEPTIONS -DAS_NO_THREADS -DAS_PROCESS_METADATA=0)
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	target_compile_definitions(astro PRIVATE ${COMPILE_DEFINITIONS} -DWIN32)
	if (MSVC)
		message("Platform is Windows (MSVC)")
		#add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	else (MSVC)
		message("Platform is Windows (MinGW)")
		#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c90 -pedantic-errors -Wall")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98 -fno-exceptions -fno-rtti")
		set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s -static")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s -static")
	endif (MSVC)
	target_link_libraries(astro -lIrrlicht -lSDL.win32 -lSDL_mixer.win32 -lopengl32 -lwinmm ${TARGET_LINK_LIBRARIES})
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	message("Platform is macOS")
	#add_definitions(-std=c++11 -stdlib=libc++)
	#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c90 -pedantic-errors -Wall")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98 -fno-exceptions -fno-rtti")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s")
	target_link_libraries(astro -lIrrlicht ${TARGET_LINK_LIBRARIES})
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	message("Platform is Linux")
	#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c90 -pedantic-errors -Wall")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti -fPIC")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s")
	target_link_libraries(astro -lIrrlicht -lGL -lX11 -lXxf86vm ${TARGET_LINK_LIBRARIES})
else (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	message("Unrecognized platform " ${CMAKE_SYSTEM_NAME})
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
