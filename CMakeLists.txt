#Minimum version
cmake_minimum_required(VERSION 3.5)

#Define project
project(coldsteel)

#Options
option(LEGACY_IRRLICHT "Compile with an old version of Irrlicht" OFF)

if (LEGACY_IRRLICHT)
  set(CMAKE_RC_COMPILER_INIT windres)
  ENABLE_LANGUAGE(RC)
  SET(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> <DEFINES> -i <SOURCE> -o <OBJECT>")
endif (LEGACY_IRRLICHT)

#Define file groups
file(
	GLOB
	COLDSTEEL_FILES
	"lib/soloud/src/audiosource/wav/*.cpp"
	"lib/soloud/src/audiosource/wav/stb_vorbis.c"
	"lib/soloud/src/backend/alsa/soloud_alsa.cpp"
	"lib/soloud/src/backend/coreaudio/soloud_coreaudio.cpp"
	"lib/soloud/src/backend/winmm/soloud_winmm.cpp"
	"lib/soloud/src/core/*.cpp"
	"lib/zip/zip.c"
	"src/*.cpp"
	"src/internal/*.cpp")
file(
	GLOB
	COLDSTEEL_EXE_FILES
	"lib/lua/*.c"
	"src/*.cc"
	"src/internal/*.cc"
	"src/runtime.rc")

#Add header and lib directories
if (LEGACY_IRRLICHT)
add_definitions(-DLEGACY_IRRLICHT -DZIP_ENABLE_SHARABLE_FILE_OPEN)
include_directories(
	${INCLUDE_DIRECTORIES}
	${CMAKE_CURRENT_SOURCE_DIR}/lib/irrlicht185/include
	${CMAKE_CURRENT_SOURCE_DIR}/lib/lua
	${CMAKE_CURRENT_SOURCE_DIR}/lib/soloud/include)
else (LEGACY_IRRLICHT)
include_directories(
	${INCLUDE_DIRECTORIES}
	${CMAKE_CURRENT_SOURCE_DIR}/lib/irrlicht190_ogles/include
	${CMAKE_CURRENT_SOURCE_DIR}/lib/lua
	${CMAKE_CURRENT_SOURCE_DIR}/lib/soloud/include)
endif (LEGACY_IRRLICHT)
if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
	link_directories(${LINK_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR}/_CMAKE/_IRRLICHT_EMSCRIPTEN)
else (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
	link_directories(${LINK_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR}/_CMAKE/_IRRLICHT)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")

#Add executable targets
add_executable(coldsteel ${COLDSTEEL_FILES} ${COLDSTEEL_EXE_FILES})

#Add platform specific options
add_definitions(-D_IRR_STATIC_LIB_)
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	add_definitions(-DWIN32 -DWITH_WINMM -DDISABLE_SIMD)
	if (MSVC)
		message("Platform is Windows (MSVC)")
		#add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	else (MSVC)
		message("Platform is Windows (MinGW)")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98 -fno-exceptions -fno-rtti")
		set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s -static")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s -static")
	endif (MSVC)
	target_link_libraries(coldsteel -lIrrlicht -lopengl32 -lwinmm ${TARGET_LINK_LIBRARIES})
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	message("Platform is macOS")
	add_definitions(-DWITH_COREAUDIO)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98 -fno-exceptions -fno-rtti")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s")
	target_link_libraries(coldsteel -lIrrlicht "-framework AudioToolbox" "-framework Cocoa" "-framework IOKit" "-framework OpenGL" ${TARGET_LINK_LIBRARIES})
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	message("Platform is Linux")
	add_definitions(-DWITH_ALSA)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98 -fno-exceptions -fno-rtti -fPIC")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s")
	target_link_libraries(coldsteel -lIrrlicht -lasound -lGL -lX11 -lXxf86vm ${TARGET_LINK_LIBRARIES})
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
	message("Platform is Emscripten")
	add_definitions(-DWITH_OPENAL)
	target_compile_definitions(coldsteel PRIVATE ${COMPILE_DEFINITIONS} -DLUA_USE_LONGJMP)
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fno-exceptions -fno-rtti -fstrict-aliasing	-U__STRICT_ANSI__")
	set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-lIrrlicht -lGL --preload-file ../../data.bin@/ -s ALLOW_MEMORY_GROWTH=1 -s NO_EXIT_RUNTIME=1 -s ASYNCIFY")
else (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	message("Unrecognized platform " ${CMAKE_SYSTEM_NAME})
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
