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
	${OBJECTDIR}/src/dump.o \
	${OBJECTDIR}/src/error.o \
	${OBJECTDIR}/src/hashtable.o \
	${OBJECTDIR}/src/hashtable_seed.o \
	${OBJECTDIR}/src/load.o \
	${OBJECTDIR}/src/memory.o \
	${OBJECTDIR}/src/pack_unpack.o \
	${OBJECTDIR}/src/strbuffer.o \
	${OBJECTDIR}/src/strconv.o \
	${OBJECTDIR}/src/utf.o \
	${OBJECTDIR}/src/value.o


# C Compiler Flags
CFLAGS=-m32

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libjansson-2.6.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libjansson-2.6.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libjansson-2.6.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libjansson-2.6.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libjansson-2.6.a

${OBJECTDIR}/src/dump.o: src/dump.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -DHAVE_FCNTL_H -DHAVE_SCHED_H -DHAVE_STDINT_H -DHAVE_SYS_STAT_H -DHAVE_SYS_TIME_H -DHAVE_SYS_TYPES_H -DHAVE_UNISTD_H -Isrc -std=c99 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/dump.o src/dump.c

${OBJECTDIR}/src/error.o: src/error.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -DHAVE_FCNTL_H -DHAVE_SCHED_H -DHAVE_STDINT_H -DHAVE_SYS_STAT_H -DHAVE_SYS_TIME_H -DHAVE_SYS_TYPES_H -DHAVE_UNISTD_H -Isrc -std=c99 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/error.o src/error.c

${OBJECTDIR}/src/hashtable.o: src/hashtable.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -DHAVE_FCNTL_H -DHAVE_SCHED_H -DHAVE_STDINT_H -DHAVE_SYS_STAT_H -DHAVE_SYS_TIME_H -DHAVE_SYS_TYPES_H -DHAVE_UNISTD_H -Isrc -std=c99 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/hashtable.o src/hashtable.c

${OBJECTDIR}/src/hashtable_seed.o: src/hashtable_seed.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -DHAVE_FCNTL_H -DHAVE_SCHED_H -DHAVE_STDINT_H -DHAVE_SYS_STAT_H -DHAVE_SYS_TIME_H -DHAVE_SYS_TYPES_H -DHAVE_UNISTD_H -Isrc -std=c99 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/hashtable_seed.o src/hashtable_seed.c

${OBJECTDIR}/src/load.o: src/load.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -DHAVE_FCNTL_H -DHAVE_SCHED_H -DHAVE_STDINT_H -DHAVE_SYS_STAT_H -DHAVE_SYS_TIME_H -DHAVE_SYS_TYPES_H -DHAVE_UNISTD_H -Isrc -std=c99 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/load.o src/load.c

${OBJECTDIR}/src/memory.o: src/memory.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -DHAVE_FCNTL_H -DHAVE_SCHED_H -DHAVE_STDINT_H -DHAVE_SYS_STAT_H -DHAVE_SYS_TIME_H -DHAVE_SYS_TYPES_H -DHAVE_UNISTD_H -Isrc -std=c99 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/memory.o src/memory.c

${OBJECTDIR}/src/pack_unpack.o: src/pack_unpack.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -DHAVE_FCNTL_H -DHAVE_SCHED_H -DHAVE_STDINT_H -DHAVE_SYS_STAT_H -DHAVE_SYS_TIME_H -DHAVE_SYS_TYPES_H -DHAVE_UNISTD_H -Isrc -std=c99 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/pack_unpack.o src/pack_unpack.c

${OBJECTDIR}/src/strbuffer.o: src/strbuffer.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -DHAVE_FCNTL_H -DHAVE_SCHED_H -DHAVE_STDINT_H -DHAVE_SYS_STAT_H -DHAVE_SYS_TIME_H -DHAVE_SYS_TYPES_H -DHAVE_UNISTD_H -Isrc -std=c99 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/strbuffer.o src/strbuffer.c

${OBJECTDIR}/src/strconv.o: src/strconv.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -DHAVE_FCNTL_H -DHAVE_SCHED_H -DHAVE_STDINT_H -DHAVE_SYS_STAT_H -DHAVE_SYS_TIME_H -DHAVE_SYS_TYPES_H -DHAVE_UNISTD_H -Isrc -std=c99 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/strconv.o src/strconv.c

${OBJECTDIR}/src/utf.o: src/utf.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -DHAVE_FCNTL_H -DHAVE_SCHED_H -DHAVE_STDINT_H -DHAVE_SYS_STAT_H -DHAVE_SYS_TIME_H -DHAVE_SYS_TYPES_H -DHAVE_UNISTD_H -Isrc -std=c99 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/utf.o src/utf.c

${OBJECTDIR}/src/value.o: src/value.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -DHAVE_FCNTL_H -DHAVE_SCHED_H -DHAVE_STDINT_H -DHAVE_SYS_STAT_H -DHAVE_SYS_TIME_H -DHAVE_SYS_TYPES_H -DHAVE_UNISTD_H -Isrc -std=c99 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/value.o src/value.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libjansson-2.6.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
