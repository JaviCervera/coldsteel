#Minimum version
cmake_minimum_required(VERSION 3.5)

#Define project
project(coldsteel)

#Options
option(LUAJIT "Use LuaJIT instead of Lua" OFF)

#LuaJIT is not compatible with Emscripten
if (LUAJIT AND ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
	message(FATAL_ERROR "LuaJIT is not compatible with Emscripten.")
endif()

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
	"src/*.cpp"
	"src/internal/*.cpp")
file(
	GLOB
	COLDSTEEL_EXE_FILES
	"src/*.cc"
	"src/internal/*.cc"
	"src/runtime.rc")
file(
	GLOB
	BUILDER_BASE_FILES
	"lib/zip/zip.c"
	"src_builder/builder.cpp"
	"src/win95_fix.cpp")
file(
	GLOB
	EDITOR_FILES
	"src_editor/editor.cpp"
	"src/win95_fix.cpp")

#Handle Lua sources (LuaJIT uses a prebuilt static library instead)
if (LUAJIT)
	set(COLDSTEEL_LUA_SOURCES "")
	set(BUILDER_LUA_SOURCES "")
else()
	file(GLOB COLDSTEEL_LUA_SOURCES "lib/lua/*.c")
	file(GLOB BUILDER_LUA_SOURCES "lib/lua/*.c")
endif()

#Add header and lib directories
add_definitions(-DZIP_ENABLE_SHARABLE_FILE_OPEN)
include_directories(
	${INCLUDE_DIRECTORIES}
	${CMAKE_CURRENT_SOURCE_DIR}/lib/irrlicht190_ogles/include
	${CMAKE_CURRENT_SOURCE_DIR}/lib/stb
	${CMAKE_CURRENT_SOURCE_DIR}/lib/soloud/include)
if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
	link_directories(${LINK_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR}/_CMAKE/_IRRLICHT_EMSCRIPTEN)
else (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
	link_directories(${LINK_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR}/_CMAKE/_IRRLICHT)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")

#Add executable targets
add_executable(coldsteel ${COLDSTEEL_FILES} ${COLDSTEEL_EXE_FILES} ${COLDSTEEL_LUA_SOURCES})
add_library(builder SHARED ${BUILDER_BASE_FILES} ${BUILDER_LUA_SOURCES})
add_library(editor SHARED ${EDITOR_FILES})

#Lua include directories (target-specific)
if (LUAJIT)
	target_include_directories(coldsteel PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/luajit-2.1/src)
	target_include_directories(builder PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/luajit-2.1/src)
else()
	target_include_directories(coldsteel PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/lua)
	target_include_directories(builder PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/lua)
endif()
target_include_directories(editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/_build/sdk)

#Link LuaJIT static library (prebuilt by build script)
if (LUAJIT)
	set(LUAJIT_LIB "${CMAKE_CURRENT_SOURCE_DIR}/_CMAKE/_LUAJIT/libluajit.a")
	if (NOT EXISTS ${LUAJIT_LIB})
		message("Run build script with --luajit first.")
		message(FATAL_ERROR "LuaJIT library not found at ${LUAJIT_LIB}.")
	endif()
	target_link_libraries(coldsteel ${LUAJIT_LIB})
	target_link_libraries(builder ${LUAJIT_LIB})
endif()

#Add platform specific options
add_definitions(-D_IRR_STATIC_LIB_)
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	add_definitions(-DWIN32 -DWITH_WINMM -DDISABLE_SIMD -DWINVER=0x0400 -D_WIN32_WINNT=0x0400)
	if (MSVC)
		message("Platform is Windows (MSVC)")
		#add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	else (MSVC)
		message("Platform is Windows (MinGW)")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=i586")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=i586 -std=c++98 -fno-exceptions -fno-rtti")
		set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s -static -Wl,--major-subsystem-version,4 -Wl,--minor-subsystem-version,0")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s -static -Wl,--major-subsystem-version,4 -Wl,--minor-subsystem-version,0")
	endif (MSVC)
	target_link_libraries(coldsteel -lIrrlicht -lopengl32 -lwinmm ${TARGET_LINK_LIBRARIES})
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	message("Platform is macOS")
	add_definitions(-DWITH_COREAUDIO)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98 -fno-exceptions -fno-rtti")
	target_link_libraries(coldsteel -lIrrlicht "-framework AudioToolbox" "-framework Cocoa" "-framework IOKit" "-framework OpenGL" ${TARGET_LINK_LIBRARIES})
	target_link_libraries(builder -ldl ${TARGET_LINK_LIBRARIES})
	target_link_libraries(editor -ldl ${TARGET_LINK_LIBRARIES})
	add_custom_command(TARGET coldsteel POST_BUILD COMMAND codesign --force --sign - $<TARGET_FILE:coldsteel>)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	message("Platform is Linux")
	add_definitions(-DWITH_ALSA)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98 -fno-exceptions -fno-rtti -fPIC")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s")
	target_link_libraries(coldsteel -lIrrlicht -lasound -lGL -lX11 -lXxf86vm -lEGL ${TARGET_LINK_LIBRARIES})
	target_link_libraries(builder -ldl ${TARGET_LINK_LIBRARIES})
	target_link_libraries(editor -ldl ${TARGET_LINK_LIBRARIES})
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
	message("Platform is Emscripten")
	add_definitions(-DWITH_OPENAL -DSOLOUD_STATIC_OPENAL)
	target_sources(coldsteel PRIVATE
		lib/soloud/src/backend/openal/soloud_openal.cpp
		lib/soloud/src/backend/openal/soloud_openal_dll.c)
	set_source_files_properties(lib/soloud/src/backend/openal/soloud_openal_dll.c PROPERTIES LANGUAGE CXX)
	if (NOT LUAJIT)
		target_compile_definitions(coldsteel PRIVATE ${COMPILE_DEFINITIONS} -DLUA_USE_LONGJMP)
	endif()
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fno-exceptions -fno-rtti -fstrict-aliasing	-U__STRICT_ANSI__")
	set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-lIrrlicht -lGL --preload-file ../../data.bin@/ -s MAIN_MODULE=1 -s ALLOW_MEMORY_GROWTH=1 -s NO_EXIT_RUNTIME=1 -s USE_SDL=1 -s ASYNCIFY -s MINIFY_HTML=0")
else (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	message("Unrecognized platform " ${CMAKE_SYSTEM_NAME})
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
