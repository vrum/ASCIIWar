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
	${OBJECTDIR}/dynamic_flac.o \
	${OBJECTDIR}/dynamic_fluidsynth.o \
	${OBJECTDIR}/dynamic_mod.o \
	${OBJECTDIR}/dynamic_mp3.o \
	${OBJECTDIR}/dynamic_ogg.o \
	${OBJECTDIR}/effect_position.o \
	${OBJECTDIR}/effect_stereoreverse.o \
	${OBJECTDIR}/effects_internal.o \
	${OBJECTDIR}/fluidsynth.o \
	${OBJECTDIR}/load_aiff.o \
	${OBJECTDIR}/load_flac.o \
	${OBJECTDIR}/load_ogg.o \
	${OBJECTDIR}/load_voc.o \
	${OBJECTDIR}/mixer.o \
	${OBJECTDIR}/music.o \
	${OBJECTDIR}/music_cmd.o \
	${OBJECTDIR}/music_flac.o \
	${OBJECTDIR}/music_mad.o \
	${OBJECTDIR}/music_mod.o \
	${OBJECTDIR}/music_modplug.o \
	${OBJECTDIR}/music_ogg.o \
	${OBJECTDIR}/native_midi/native_midi_common.o \
	${OBJECTDIR}/native_midi/native_midi_haiku.o \
	${OBJECTDIR}/native_midi/native_midi_mac.o \
	${OBJECTDIR}/native_midi/native_midi_macosx.o \
	${OBJECTDIR}/native_midi/native_midi_win32.o \
	${OBJECTDIR}/wavestream.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../SDL-1.2.15/lib

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libSDL_mixer-1.2.12.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libSDL_mixer-1.2.12.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libSDL_mixer-1.2.12.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -lmingw32 -mwindows -lws2_32 -lwinmm -lSDLmain -lSDL -static-libgcc -static-libstdc++ -mthreads -Wl,--out-implib,libsdlmixer.a -shared

${OBJECTDIR}/dynamic_flac.o: dynamic_flac.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DMP3_DYNAMIC=\"smpeg.dll\" -DMP3_MUSIC -DOGG_DYNAMIC=\"libvorbisfile-3.dll\" -DOGG_MUSIC -DWAV_MUSIC -IVisualC/external/include -I../SDL-1.2.15/include -I. -Inative_midi  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dynamic_flac.o dynamic_flac.c

${OBJECTDIR}/dynamic_fluidsynth.o: dynamic_fluidsynth.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DMP3_DYNAMIC=\"smpeg.dll\" -DMP3_MUSIC -DOGG_DYNAMIC=\"libvorbisfile-3.dll\" -DOGG_MUSIC -DWAV_MUSIC -IVisualC/external/include -I../SDL-1.2.15/include -I. -Inative_midi  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dynamic_fluidsynth.o dynamic_fluidsynth.c

${OBJECTDIR}/dynamic_mod.o: dynamic_mod.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DMP3_DYNAMIC=\"smpeg.dll\" -DMP3_MUSIC -DOGG_DYNAMIC=\"libvorbisfile-3.dll\" -DOGG_MUSIC -DWAV_MUSIC -IVisualC/external/include -I../SDL-1.2.15/include -I. -Inative_midi  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dynamic_mod.o dynamic_mod.c

${OBJECTDIR}/dynamic_mp3.o: dynamic_mp3.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DMP3_DYNAMIC=\"smpeg.dll\" -DMP3_MUSIC -DOGG_DYNAMIC=\"libvorbisfile-3.dll\" -DOGG_MUSIC -DWAV_MUSIC -IVisualC/external/include -I../SDL-1.2.15/include -I. -Inative_midi  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dynamic_mp3.o dynamic_mp3.c

${OBJECTDIR}/dynamic_ogg.o: dynamic_ogg.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DMP3_DYNAMIC=\"smpeg.dll\" -DMP3_MUSIC -DOGG_DYNAMIC=\"libvorbisfile-3.dll\" -DOGG_MUSIC -DWAV_MUSIC -IVisualC/external/include -I../SDL-1.2.15/include -I. -Inative_midi  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dynamic_ogg.o dynamic_ogg.c

${OBJECTDIR}/effect_position.o: effect_position.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DMP3_DYNAMIC=\"smpeg.dll\" -DMP3_MUSIC -DOGG_DYNAMIC=\"libvorbisfile-3.dll\" -DOGG_MUSIC -DWAV_MUSIC -IVisualC/external/include -I../SDL-1.2.15/include -I. -Inative_midi  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/effect_position.o effect_position.c

${OBJECTDIR}/effect_stereoreverse.o: effect_stereoreverse.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DMP3_DYNAMIC=\"smpeg.dll\" -DMP3_MUSIC -DOGG_DYNAMIC=\"libvorbisfile-3.dll\" -DOGG_MUSIC -DWAV_MUSIC -IVisualC/external/include -I../SDL-1.2.15/include -I. -Inative_midi  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/effect_stereoreverse.o effect_stereoreverse.c

${OBJECTDIR}/effects_internal.o: effects_internal.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DMP3_DYNAMIC=\"smpeg.dll\" -DMP3_MUSIC -DOGG_DYNAMIC=\"libvorbisfile-3.dll\" -DOGG_MUSIC -DWAV_MUSIC -IVisualC/external/include -I../SDL-1.2.15/include -I. -Inative_midi  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/effects_internal.o effects_internal.c

${OBJECTDIR}/fluidsynth.o: fluidsynth.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DMP3_DYNAMIC=\"smpeg.dll\" -DMP3_MUSIC -DOGG_DYNAMIC=\"libvorbisfile-3.dll\" -DOGG_MUSIC -DWAV_MUSIC -IVisualC/external/include -I../SDL-1.2.15/include -I. -Inative_midi  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/fluidsynth.o fluidsynth.c

${OBJECTDIR}/load_aiff.o: load_aiff.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DMP3_DYNAMIC=\"smpeg.dll\" -DMP3_MUSIC -DOGG_DYNAMIC=\"libvorbisfile-3.dll\" -DOGG_MUSIC -DWAV_MUSIC -IVisualC/external/include -I../SDL-1.2.15/include -I. -Inative_midi  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/load_aiff.o load_aiff.c

${OBJECTDIR}/load_flac.o: load_flac.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DMP3_DYNAMIC=\"smpeg.dll\" -DMP3_MUSIC -DOGG_DYNAMIC=\"libvorbisfile-3.dll\" -DOGG_MUSIC -DWAV_MUSIC -IVisualC/external/include -I../SDL-1.2.15/include -I. -Inative_midi  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/load_flac.o load_flac.c

${OBJECTDIR}/load_ogg.o: load_ogg.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DMP3_DYNAMIC=\"smpeg.dll\" -DMP3_MUSIC -DOGG_DYNAMIC=\"libvorbisfile-3.dll\" -DOGG_MUSIC -DWAV_MUSIC -IVisualC/external/include -I../SDL-1.2.15/include -I. -Inative_midi  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/load_ogg.o load_ogg.c

${OBJECTDIR}/load_voc.o: load_voc.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DMP3_DYNAMIC=\"smpeg.dll\" -DMP3_MUSIC -DOGG_DYNAMIC=\"libvorbisfile-3.dll\" -DOGG_MUSIC -DWAV_MUSIC -IVisualC/external/include -I../SDL-1.2.15/include -I. -Inative_midi  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/load_voc.o load_voc.c

${OBJECTDIR}/mixer.o: mixer.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DMP3_DYNAMIC=\"smpeg.dll\" -DMP3_MUSIC -DOGG_DYNAMIC=\"libvorbisfile-3.dll\" -DOGG_MUSIC -DWAV_MUSIC -IVisualC/external/include -I../SDL-1.2.15/include -I. -Inative_midi  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/mixer.o mixer.c

${OBJECTDIR}/music.o: music.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DMP3_DYNAMIC=\"smpeg.dll\" -DMP3_MUSIC -DOGG_DYNAMIC=\"libvorbisfile-3.dll\" -DOGG_MUSIC -DWAV_MUSIC -IVisualC/external/include -I../SDL-1.2.15/include -I. -Inative_midi  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/music.o music.c

${OBJECTDIR}/music_cmd.o: music_cmd.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DMP3_DYNAMIC=\"smpeg.dll\" -DMP3_MUSIC -DOGG_DYNAMIC=\"libvorbisfile-3.dll\" -DOGG_MUSIC -DWAV_MUSIC -IVisualC/external/include -I../SDL-1.2.15/include -I. -Inative_midi  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/music_cmd.o music_cmd.c

${OBJECTDIR}/music_flac.o: music_flac.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DMP3_DYNAMIC=\"smpeg.dll\" -DMP3_MUSIC -DOGG_DYNAMIC=\"libvorbisfile-3.dll\" -DOGG_MUSIC -DWAV_MUSIC -IVisualC/external/include -I../SDL-1.2.15/include -I. -Inative_midi  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/music_flac.o music_flac.c

${OBJECTDIR}/music_mad.o: music_mad.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DMP3_DYNAMIC=\"smpeg.dll\" -DMP3_MUSIC -DOGG_DYNAMIC=\"libvorbisfile-3.dll\" -DOGG_MUSIC -DWAV_MUSIC -IVisualC/external/include -I../SDL-1.2.15/include -I. -Inative_midi  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/music_mad.o music_mad.c

${OBJECTDIR}/music_mod.o: music_mod.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DMP3_DYNAMIC=\"smpeg.dll\" -DMP3_MUSIC -DOGG_DYNAMIC=\"libvorbisfile-3.dll\" -DOGG_MUSIC -DWAV_MUSIC -IVisualC/external/include -I../SDL-1.2.15/include -I. -Inative_midi  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/music_mod.o music_mod.c

${OBJECTDIR}/music_modplug.o: music_modplug.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DMP3_DYNAMIC=\"smpeg.dll\" -DMP3_MUSIC -DOGG_DYNAMIC=\"libvorbisfile-3.dll\" -DOGG_MUSIC -DWAV_MUSIC -IVisualC/external/include -I../SDL-1.2.15/include -I. -Inative_midi  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/music_modplug.o music_modplug.c

${OBJECTDIR}/music_ogg.o: music_ogg.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DMP3_DYNAMIC=\"smpeg.dll\" -DMP3_MUSIC -DOGG_DYNAMIC=\"libvorbisfile-3.dll\" -DOGG_MUSIC -DWAV_MUSIC -IVisualC/external/include -I../SDL-1.2.15/include -I. -Inative_midi  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/music_ogg.o music_ogg.c

${OBJECTDIR}/native_midi/native_midi_common.o: native_midi/native_midi_common.c 
	${MKDIR} -p ${OBJECTDIR}/native_midi
	${RM} "$@.d"
	$(COMPILE.c) -g -DMP3_DYNAMIC=\"smpeg.dll\" -DMP3_MUSIC -DOGG_DYNAMIC=\"libvorbisfile-3.dll\" -DOGG_MUSIC -DWAV_MUSIC -IVisualC/external/include -I../SDL-1.2.15/include -I. -Inative_midi  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/native_midi/native_midi_common.o native_midi/native_midi_common.c

${OBJECTDIR}/native_midi/native_midi_haiku.o: native_midi/native_midi_haiku.cpp 
	${MKDIR} -p ${OBJECTDIR}/native_midi
	${RM} "$@.d"
	$(COMPILE.cc) -g -DMP3_DYNAMIC=\"smpeg.dll\" -DMP3_MUSIC -DOGG_DYNAMIC=\"libvorbisfile-3.dll\" -DOGG_MUSIC -DWAV_MUSIC -IVisualC/external/include -I../SDL-1.2.15/include -I. -Inative_midi  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/native_midi/native_midi_haiku.o native_midi/native_midi_haiku.cpp

${OBJECTDIR}/native_midi/native_midi_mac.o: native_midi/native_midi_mac.c 
	${MKDIR} -p ${OBJECTDIR}/native_midi
	${RM} "$@.d"
	$(COMPILE.c) -g -DMP3_DYNAMIC=\"smpeg.dll\" -DMP3_MUSIC -DOGG_DYNAMIC=\"libvorbisfile-3.dll\" -DOGG_MUSIC -DWAV_MUSIC -IVisualC/external/include -I../SDL-1.2.15/include -I. -Inative_midi  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/native_midi/native_midi_mac.o native_midi/native_midi_mac.c

${OBJECTDIR}/native_midi/native_midi_macosx.o: native_midi/native_midi_macosx.c 
	${MKDIR} -p ${OBJECTDIR}/native_midi
	${RM} "$@.d"
	$(COMPILE.c) -g -DMP3_DYNAMIC=\"smpeg.dll\" -DMP3_MUSIC -DOGG_DYNAMIC=\"libvorbisfile-3.dll\" -DOGG_MUSIC -DWAV_MUSIC -IVisualC/external/include -I../SDL-1.2.15/include -I. -Inative_midi  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/native_midi/native_midi_macosx.o native_midi/native_midi_macosx.c

${OBJECTDIR}/native_midi/native_midi_win32.o: native_midi/native_midi_win32.c 
	${MKDIR} -p ${OBJECTDIR}/native_midi
	${RM} "$@.d"
	$(COMPILE.c) -g -DMP3_DYNAMIC=\"smpeg.dll\" -DMP3_MUSIC -DOGG_DYNAMIC=\"libvorbisfile-3.dll\" -DOGG_MUSIC -DWAV_MUSIC -IVisualC/external/include -I../SDL-1.2.15/include -I. -Inative_midi  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/native_midi/native_midi_win32.o native_midi/native_midi_win32.c

${OBJECTDIR}/wavestream.o: wavestream.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -DMP3_DYNAMIC=\"smpeg.dll\" -DMP3_MUSIC -DOGG_DYNAMIC=\"libvorbisfile-3.dll\" -DOGG_MUSIC -DWAV_MUSIC -IVisualC/external/include -I../SDL-1.2.15/include -I. -Inative_midi  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/wavestream.o wavestream.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libSDL_mixer-1.2.12.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
