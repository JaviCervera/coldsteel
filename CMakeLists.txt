#Minimum version
cmake_minimum_required(VERSION 2.8)

#Define project
project(coldsteel)

#Define file groups
file(
	GLOB
	COLDSTEEL_EXE_FILES
	"src/*.cpp"
	"src/runtime.rc"
	"lib/lua/*.c"
	"lib/soloud/src/audiosource/wav/*.cpp"
	"lib/soloud/src/audiosource/wav/stb_vorbis.c"
	"lib/soloud/src/backend/coreaudio/soloud_coreaudio.cpp"
	"lib/soloud/src/backend/winmm/soloud_winmm.cpp"
	"lib/soloud/src/core/*.cpp")

#Add header and lib directories
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	include_directories(
		${INCLUDE_DIRECTORIES}
		${CMAKE_CURRENT_SOURCE_DIR}/lib/irrlicht185/include
		${CMAKE_CURRENT_SOURCE_DIR}/lib/lua
		${CMAKE_CURRENT_SOURCE_DIR}/lib/soloud/include)
	link_directories(
		${LINK_DIRECTORIES}
		${CMAKE_CURRENT_SOURCE_DIR}/_CMAKE/_IRRLICHT)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
	include_directories(
		${INCLUDE_DIRECTORIES}
		${CMAKE_CURRENT_SOURCE_DIR}/lib/irrlicht190_ogles/include
		${CMAKE_CURRENT_SOURCE_DIR}/lib/lua
		${CMAKE_CURRENT_SOURCE_DIR}/lib/soloud/include)
	link_directories(
		${LINK_DIRECTORIES}
		${CMAKE_CURRENT_SOURCE_DIR}/_CMAKE/_IRRLICHT_EMSCRIPTEN)
else (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	include_directories(
		${INCLUDE_DIRECTORIES}
		${CMAKE_CURRENT_SOURCE_DIR}/lib/irrlicht190/include
		${CMAKE_CURRENT_SOURCE_DIR}/lib/lua
		${CMAKE_CURRENT_SOURCE_DIR}/lib/soloud/include)
	link_directories(
		${LINK_DIRECTORIES}
		${CMAKE_CURRENT_SOURCE_DIR}/_CMAKE/_IRRLICHT)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

#Add executable targets
add_executable(coldsteel ${COLDSTEEL_EXE_FILES})

#Add platform specific options
add_definitions(-D_IRR_STATIC_LIB_)
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	add_definitions(-DWIN32 -DWITH_WINMM -DDISABLE_SIMD)
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
	target_link_libraries(coldsteel -lIrrlicht -lopengl32 -lwinmm ${TARGET_LINK_LIBRARIES})
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	message("Platform is macOS")
	add_definitions(-DWITH_COREAUDIO)
	#add_definitions(-std=c++11 -stdlib=libc++)
	#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c90 -pedantic-errors -Wall")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98 -fno-exceptions -fno-rtti")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s")
	target_link_libraries(coldsteel -lIrrlicht "-framework AudioToolbox" "-framework Cocoa" "-framework IOKit" "-framework OpenGL" ${TARGET_LINK_LIBRARIES})
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	message("Platform is Linux")
	#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c90 -pedantic-errors -Wall")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98 -fno-exceptions -fno-rtti -fPIC")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s")
	target_link_libraries(coldsteel -lIrrlicht -lGL -lX11 -lXxf86vm ${TARGET_LINK_LIBRARIES})
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
	message("Platform is Emscripten")
	target_compile_definitions(coldsteel PRIVATE ${COMPILE_DEFINITIONS} -DLUA_USE_LONGJMP)
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fno-exceptions -fno-rtti -fstrict-aliasing	-U__STRICT_ANSI__")
	set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-o coldsteel.html -lIrrlicht -lGL --preload-file ../../data.bin@/ -s ALLOW_MEMORY_GROWTH=1 -s NO_EXIT_RUNTIME=1")
else (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	message("Unrecognized platform " ${CMAKE_SYSTEM_NAME})
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
