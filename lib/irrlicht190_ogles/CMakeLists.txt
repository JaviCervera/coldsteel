#Minimum version
cmake_minimum_required(VERSION 2.8)

#Define project
project(Irrlicht)

#Options
option(IRRLICHT_M32 "Pass the -m32 flag to the compiler to build 32 bit application" OFF)
option(IRRLICHT_SHARED "Compile Irrlicht as a shared library" OFF)
option(SDL_ENABLED "Use SDL platform" OFF)
set(SDL_PATH "SDL" CACHE PATH "Path to SDL folder (must contain include and lib subfolders)")

#Define file groups
file(GLOB COMMON_FILES
	"source/Irrlicht/*.cpp"
	"source/Irrlicht/aesGladman/aescrypt.cpp" "source/Irrlicht/aesGladman/prng.cpp" "source/Irrlicht/aesGladman/aeskey.cpp" "source/Irrlicht/aesGladman/pwd2key.cpp" "source/Irrlicht/aesGladman/aestab.cpp" "source/Irrlicht/aesGladman/sha1.cpp" "source/Irrlicht/aesGladman/fileenc.cpp" "source/Irrlicht/aesGladman/sha2.cpp" "source/Irrlicht/aesGladman/hmac.cpp"
	"source/Irrlicht/bzip2/blocksort.c" "source/Irrlicht/bzip2/decompress.c" "source/Irrlicht/bzip2/bzcompress.c" "source/Irrlicht/bzip2/huffman.c" "source/Irrlicht/bzip2/bzlib.c" "source/Irrlicht/bzip2/randtable.c" "source/Irrlicht/bzip2/crctable.c"
	"source/Irrlicht/jpeglib/jaricom.c" "source/Irrlicht/jpeglib/jdcolor.c" "source/Irrlicht/jpeglib/jcapimin.c" "source/Irrlicht/jpeglib/jddctmgr.c" "source/Irrlicht/jpeglib/jcapistd.c" "source/Irrlicht/jpeglib/jdhuff.c" "source/Irrlicht/jpeglib/jcarith.c" "source/Irrlicht/jpeglib/jdinput.c" "source/Irrlicht/jpeglib/jccoefct.c" "source/Irrlicht/jpeglib/jdmainct.c" "source/Irrlicht/jpeglib/jccolor.c" "source/Irrlicht/jpeglib/jdmarker.c" "source/Irrlicht/jpeglib/jcdctmgr.c" "source/Irrlicht/jpeglib/jdmaster.c" "source/Irrlicht/jpeglib/jchuff.c" "source/Irrlicht/jpeglib/jdmerge.c" "source/Irrlicht/jpeglib/jcinit.c" "source/Irrlicht/jpeglib/jdpostct.c" "source/Irrlicht/jpeglib/jcmainct.c" "source/Irrlicht/jpeglib/jdsample.c" "source/Irrlicht/jpeglib/jcmarker.c" "source/Irrlicht/jpeglib/jdtrans.c" "source/Irrlicht/jpeglib/jcmaster.c" "source/Irrlicht/jpeglib/jerror.c" "source/Irrlicht/jpeglib/jcomapi.c" "source/Irrlicht/jpeglib/jfdctflt.c" "source/Irrlicht/jpeglib/jcparam.c" "source/Irrlicht/jpeglib/jfdctfst.c" "source/Irrlicht/jpeglib/jcprepct.c" "source/Irrlicht/jpeglib/jfdctint.c" "source/Irrlicht/jpeglib/jcsample.c" "source/Irrlicht/jpeglib/jidctflt.c" "source/Irrlicht/jpeglib/jctrans.c" "source/Irrlicht/jpeglib/jidctfst.c" "source/Irrlicht/jpeglib/jdapimin.c" "source/Irrlicht/jpeglib/jidctint.c" "source/Irrlicht/jpeglib/jdapistd.c" "source/Irrlicht/jpeglib/jmemmgr.c" "source/Irrlicht/jpeglib/jdarith.c" "source/Irrlicht/jpeglib/jmemnobs.c" "source/Irrlicht/jpeglib/jdatadst.c" "source/Irrlicht/jpeglib/jquant1.c" "source/Irrlicht/jpeglib/jdatasrc.c" "source/Irrlicht/jpeglib/jquant2.c" "source/Irrlicht/jpeglib/jdcoefct.c" "source/Irrlicht/jpeglib/jutils.c"
	"source/Irrlicht/libpng/png.c" "source/Irrlicht/libpng/pngrutil.c" "source/Irrlicht/libpng/pngerror.c" "source/Irrlicht/libpng/pngset.c" "source/Irrlicht/libpng/pngget.c" "source/Irrlicht/libpng/pngtrans.c" "source/Irrlicht/libpng/pngmem.c" "source/Irrlicht/libpng/pngwio.c" "source/Irrlicht/libpng/pngpread.c" "source/Irrlicht/libpng/pngwrite.c" "source/Irrlicht/libpng/pngread.c" "source/Irrlicht/libpng/pngwtran.c" "source/Irrlicht/libpng/pngrio.c" "source/Irrlicht/libpng/pngwutil.c" "source/Irrlicht/libpng/pngrtran.c"
	"source/Irrlicht/lzma/LzmaDec.c"
	"source/Irrlicht/zlib/adler32.c" "source/Irrlicht/zlib/infback.c" "source/Irrlicht/zlib/compress.c" "source/Irrlicht/zlib/inffast.c" "source/Irrlicht/zlib/crc32.c" "source/Irrlicht/zlib/inflate.c" "source/Irrlicht/zlib/deflate.c" "source/Irrlicht/zlib/inftrees.c" "source/Irrlicht/zlib/gzclose.c" "source/Irrlicht/zlib/trees.c" "source/Irrlicht/zlib/gzlib.c" "source/Irrlicht/zlib/uncompr.c" "source/Irrlicht/zlib/gzread.c" "source/Irrlicht/zlib/zutil.c" "source/Irrlicht/zlib/gzwrite.c"
)
file(GLOB MAC_FILES "source/Irrlicht/*.mm")

#Add header and lib directories
include_directories(${INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/source/Irrlicht ${CMAKE_CURRENT_SOURCE_DIR}/source/Irrlicht/zlib)
if (SDL_ENABLED)
	include_directories(${INCLUDE_DIRECTORIES} ${SDL_PATH}/include)
	link_directories(${LINK_DIRECTORIES} ${SDL_PATH}/lib)
	add_definitions(-D_IRR_COMPILE_WITH_SDL_DEVICE_ -DNO_IRR_COMPILE_WITH_WINDOWS_DEVICE_ -DNO_IRR_COMPILE_WITH_OSX_DEVICE_ -DNO_IRR_COMPILE_WITH_X11_DEVICE_)
endif (SDL_ENABLED)

#Add executable targets
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	if (IRRLICHT_SHARED)
		add_library(Irrlicht SHARED ${COMMON_FILES} ${MAC_FILES})
	else (IRRLICHT_SHARED)
		add_library(Irrlicht STATIC ${COMMON_FILES} ${MAC_FILES})
	endif (IRRLICHT_SHARED)
else (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	if (IRRLICHT_SHARED)
		add_library(Irrlicht SHARED ${COMMON_FILES})
	else (IRRLICHT_SHARED)
		add_library(Irrlicht STATIC ${COMMON_FILES})
	endif (IRRLICHT_SHARED)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

#Add common options
if (IRRLICHT_SHARED)
	add_definitions(-D_USRDLL -DIRRLICHT_EXPORTS)
else (IRRLICHT_SHARED)
	add_definitions(-D_IRR_STATIC_LIB_)
endif (IRRLICHT_SHARED)
add_definitions(-U__STRICT_ANSI__)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-parameter -Wno-implicit-function-declaration")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98 -fno-rtti -ffast-math -Wno-unused-parameter")
if (IRRLICHT_M32)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
endif (IRRLICHT_M32)

#Add platform specific options
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	message("Platform is Windows")
	add_definitions(-D__GNUWIN32__ -DWIN32 -D_WINDOWS -DNDEBUG -D_MBCS -D_CRT_SECURE_NO_DEPRECATE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
	if (IRRLICHT_SHARED)
		set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s -static")
		target_link_libraries(Irrlicht -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lwinmm -lopengl32 ${TARGET_LINK_LIBRARIES})
	endif (IRRLICHT_SHARED)
	if (SDL_ENABLED AND IRRLICHT_SHARED)
		target_link_libraries(Irrlicht -lSDL.dll ${TARGET_LINK_LIBRARIES})
	endif (SDL_ENABLED AND IRRLICHT_SHARED)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	message("Platform is macOS")
	add_definitions(-DGL_SILENCE_DEPRECATION)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -x objective-c++")
	if (IRRLICHT_SHARED)
		set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s")
		target_link_libraries(Irrlicht "-framework AppKit" "-framework Carbon" "-framework IOKit" "-framework OpenGL" ${TARGET_LINK_LIBRARIES})
	endif (IRRLICHT_SHARED)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	message("Platform is Linux")
	add_definitions(-DPNG_ARM_NEON_IMPLEMENTATION=0)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
	if (IRRLICHT_SHARED)
		set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s")
		target_link_libraries(Irrlicht -lGL -lXxf86vm ${TARGET_LINK_LIBRARIES})
	endif (IRRLICHT_SHARED)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
	message("Platform is Emscripten")
else (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	message("Unrecognized platform " ${CMAKE_SYSTEM_NAME})
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
