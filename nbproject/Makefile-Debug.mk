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
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/asciiwar.o \
	${OBJECTDIR}/src/main.o \
	${OBJECTDIR}/src/moba.o \
	${OBJECTDIR}/src/parsers.o


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
LDLIBSOPTIONS=-Ldep/SDL-1.2.15/lib -Ldep/libtcod-1.5.2/lib -Ldep/jansson-2.6/dist/Release/MinGW-Windows -Ldep/enet-1.3.11/dist/Release/MinGW-Windows -Ldep/zlib-1.2.8/dist/Release/MinGW-Windows -Ldep/SDL_mixer-1.2.12/lib

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk bin/ASCIIWar.exe

bin/ASCIIWar.exe: ${OBJECTFILES}
	${MKDIR} -p bin
	${LINK.cc} -o bin/ASCIIWar ${OBJECTFILES} ${LDLIBSOPTIONS} -lmingw32 -mwindows -llibtcod -lenet-1.3.11 -lSDLmain -lSDL -lsdlmixer -lws2_32 -lwinmm -ljansson-2.6 -lzlib-1.2.8 -static-libgcc -static-libstdc++ -mthreads

${OBJECTDIR}/src/asciiwar.o: src/asciiwar.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -Werror -DDEBUG -DLOAD_PNG -Idep/libtcod-1.5.2/include/gui -Idep/libtcod-1.5.2/include -Idep/jansson-2.6/src -Idep/enet-1.3.11/include -Idep/zlib-1.2.8 -Idep/SDL-1.2.15/include -Idep/SDL_mixer-1.2.12 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/asciiwar.o src/asciiwar.cpp

${OBJECTDIR}/src/main.o: src/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -Werror -DDEBUG -DLOAD_PNG -Idep/libtcod-1.5.2/include/gui -Idep/libtcod-1.5.2/include -Idep/jansson-2.6/src -Idep/enet-1.3.11/include -Idep/zlib-1.2.8 -Idep/SDL-1.2.15/include -Idep/SDL_mixer-1.2.12 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/main.o src/main.cpp

${OBJECTDIR}/src/moba.o: src/moba.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -Werror -DDEBUG -DLOAD_PNG -Idep/libtcod-1.5.2/include/gui -Idep/libtcod-1.5.2/include -Idep/jansson-2.6/src -Idep/enet-1.3.11/include -Idep/zlib-1.2.8 -Idep/SDL-1.2.15/include -Idep/SDL_mixer-1.2.12 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/moba.o src/moba.cpp

${OBJECTDIR}/src/parsers.o: src/parsers.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -Werror -DDEBUG -DLOAD_PNG -Idep/libtcod-1.5.2/include/gui -Idep/libtcod-1.5.2/include -Idep/jansson-2.6/src -Idep/enet-1.3.11/include -Idep/zlib-1.2.8 -Idep/SDL-1.2.15/include -Idep/SDL_mixer-1.2.12 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/parsers.o src/parsers.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} bin/ASCIIWar.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
