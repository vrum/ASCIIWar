#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=MinGW-Windows
CND_DLIB_EXT=dll
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/bresenham_c.o \
	${OBJECTDIR}/src/bsp_c.o \
	${OBJECTDIR}/src/color_c.o \
	${OBJECTDIR}/src/console_c.o \
	${OBJECTDIR}/src/fov_c.o \
	${OBJECTDIR}/src/fov_circular_raycasting.o \
	${OBJECTDIR}/src/fov_diamond_raycasting.o \
	${OBJECTDIR}/src/fov_permissive2.o \
	${OBJECTDIR}/src/fov_recursive_shadowcasting.o \
	${OBJECTDIR}/src/fov_restrictive.o \
	${OBJECTDIR}/src/heightmap_c.o \
	${OBJECTDIR}/src/image_c.o \
	${OBJECTDIR}/src/lex_c.o \
	${OBJECTDIR}/src/list_c.o \
	${OBJECTDIR}/src/mersenne_c.o \
	${OBJECTDIR}/src/namegen_c.o \
	${OBJECTDIR}/src/noise_c.o \
	${OBJECTDIR}/src/parser_c.o \
	${OBJECTDIR}/src/path_c.o \
	${OBJECTDIR}/src/png/lodepng.o \
	${OBJECTDIR}/src/sys_c.o \
	${OBJECTDIR}/src/sys_opengl_c.o \
	${OBJECTDIR}/src/sys_sdl_c.o \
	${OBJECTDIR}/src/sys_sdl_img_bmp.o \
	${OBJECTDIR}/src/sys_sdl_img_png.o \
	${OBJECTDIR}/src/tree_c.o \
	${OBJECTDIR}/src/txtfield_c.o \
	${OBJECTDIR}/src/wrappers.o \
	${OBJECTDIR}/src/zip_c.o


# C Compiler Flags
CFLAGS=-ffloat-store -march=i686 -msse -mfpmath=sse

# CC Compiler Flags
CCFLAGS=-ffloat-store -march=i686 -msse -mfpmath=sse
CXXFLAGS=-ffloat-store -march=i686 -msse -mfpmath=sse

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../SDL-1.2.15/lib -L../zlib-1.2.8/dist/Release/MinGW-Windows

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liblibtcod-1.5.2.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liblibtcod-1.5.2.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liblibtcod-1.5.2.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -lmingw32 -mwindows -lws2_32 -lwinmm -lzlib-1.2.8 -lSDLmain -lSDL -static-libgcc -static-libstdc++ -mthreads -Wl,--out-implib,liblibtcod.a -shared

${OBJECTDIR}/src/bresenham_c.o: src/bresenham_c.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/bresenham_c.o src/bresenham_c.c

${OBJECTDIR}/src/bsp_c.o: src/bsp_c.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/bsp_c.o src/bsp_c.c

${OBJECTDIR}/src/color_c.o: src/color_c.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/color_c.o src/color_c.c

${OBJECTDIR}/src/console_c.o: src/console_c.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/console_c.o src/console_c.c

${OBJECTDIR}/src/fov_c.o: src/fov_c.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/fov_c.o src/fov_c.c

${OBJECTDIR}/src/fov_circular_raycasting.o: src/fov_circular_raycasting.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/fov_circular_raycasting.o src/fov_circular_raycasting.c

${OBJECTDIR}/src/fov_diamond_raycasting.o: src/fov_diamond_raycasting.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/fov_diamond_raycasting.o src/fov_diamond_raycasting.c

${OBJECTDIR}/src/fov_permissive2.o: src/fov_permissive2.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/fov_permissive2.o src/fov_permissive2.c

${OBJECTDIR}/src/fov_recursive_shadowcasting.o: src/fov_recursive_shadowcasting.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/fov_recursive_shadowcasting.o src/fov_recursive_shadowcasting.c

${OBJECTDIR}/src/fov_restrictive.o: src/fov_restrictive.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/fov_restrictive.o src/fov_restrictive.c

${OBJECTDIR}/src/heightmap_c.o: src/heightmap_c.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/heightmap_c.o src/heightmap_c.c

${OBJECTDIR}/src/image_c.o: src/image_c.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/image_c.o src/image_c.c

${OBJECTDIR}/src/lex_c.o: src/lex_c.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/lex_c.o src/lex_c.c

${OBJECTDIR}/src/list_c.o: src/list_c.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/list_c.o src/list_c.c

${OBJECTDIR}/src/mersenne_c.o: src/mersenne_c.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/mersenne_c.o src/mersenne_c.c

${OBJECTDIR}/src/namegen_c.o: src/namegen_c.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/namegen_c.o src/namegen_c.c

${OBJECTDIR}/src/noise_c.o: src/noise_c.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/noise_c.o src/noise_c.c

${OBJECTDIR}/src/parser_c.o: src/parser_c.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/parser_c.o src/parser_c.c

${OBJECTDIR}/src/path_c.o: src/path_c.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/path_c.o src/path_c.c

${OBJECTDIR}/src/png/lodepng.o: src/png/lodepng.c 
	${MKDIR} -p ${OBJECTDIR}/src/png
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/png/lodepng.o src/png/lodepng.c

${OBJECTDIR}/src/sys_c.o: src/sys_c.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/sys_c.o src/sys_c.c

${OBJECTDIR}/src/sys_opengl_c.o: src/sys_opengl_c.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/sys_opengl_c.o src/sys_opengl_c.c

${OBJECTDIR}/src/sys_sdl_c.o: src/sys_sdl_c.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/sys_sdl_c.o src/sys_sdl_c.c

${OBJECTDIR}/src/sys_sdl_img_bmp.o: src/sys_sdl_img_bmp.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/sys_sdl_img_bmp.o src/sys_sdl_img_bmp.c

${OBJECTDIR}/src/sys_sdl_img_png.o: src/sys_sdl_img_png.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/sys_sdl_img_png.o src/sys_sdl_img_png.c

${OBJECTDIR}/src/tree_c.o: src/tree_c.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tree_c.o src/tree_c.c

${OBJECTDIR}/src/txtfield_c.o: src/txtfield_c.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/txtfield_c.o src/txtfield_c.c

${OBJECTDIR}/src/wrappers.o: src/wrappers.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/wrappers.o src/wrappers.c

${OBJECTDIR}/src/zip_c.o: src/zip_c.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -DLIBTCOD_EXPORTS -DNO_OPENGL -Iinclude -I../zlib-1.2.8 -I../SDL-1.2.15/include  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/zip_c.o src/zip_c.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liblibtcod-1.5.2.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
