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
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/common/strutils.o \
	${OBJECTDIR}/fmt/format.o \
	${OBJECTDIR}/fmt/os.o \
	${OBJECTDIR}/font.o \
	${OBJECTDIR}/fsmgr.o \
	${OBJECTDIR}/game.o \
	${OBJECTDIR}/game_character.o \
	${OBJECTDIR}/game_data.o \
	${OBJECTDIR}/game_input.o \
	${OBJECTDIR}/game_item.o \
	${OBJECTDIR}/game_locale.o \
	${OBJECTDIR}/game_loot.o \
	${OBJECTDIR}/game_mixture_info.o \
	${OBJECTDIR}/game_newgame.o \
	${OBJECTDIR}/game_pathfind.o \
	${OBJECTDIR}/game_placeoff.o \
	${OBJECTDIR}/game_play.o \
	${OBJECTDIR}/game_quest.o \
	${OBJECTDIR}/game_soundres.o \
	${OBJECTDIR}/game_state.o \
	${OBJECTDIR}/game_ui.o \
	${OBJECTDIR}/game_village.o \
	${OBJECTDIR}/gfx.o \
	${OBJECTDIR}/glfuncs.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/resources.o \
	${OBJECTDIR}/system.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-g
CXXFLAGS=-g

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=`pkg-config --libs sdl2` `pkg-config --libs opengl` `pkg-config --libs SDL2_image`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/slavik

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/slavik: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/slavik ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/common/strutils.o: common/strutils.cpp
	${MKDIR} -p ${OBJECTDIR}/common
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `pkg-config --cflags sdl2` `pkg-config --cflags opengl` `pkg-config --cflags SDL2_image`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/common/strutils.o common/strutils.cpp

${OBJECTDIR}/fmt/format.o: fmt/format.cc
	${MKDIR} -p ${OBJECTDIR}/fmt
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `pkg-config --cflags sdl2` `pkg-config --cflags opengl` `pkg-config --cflags SDL2_image`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/fmt/format.o fmt/format.cc

${OBJECTDIR}/fmt/os.o: fmt/os.cc
	${MKDIR} -p ${OBJECTDIR}/fmt
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `pkg-config --cflags sdl2` `pkg-config --cflags opengl` `pkg-config --cflags SDL2_image`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/fmt/os.o fmt/os.cc

${OBJECTDIR}/font.o: font.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `pkg-config --cflags sdl2` `pkg-config --cflags opengl` `pkg-config --cflags SDL2_image`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/font.o font.cpp

${OBJECTDIR}/fsmgr.o: fsmgr.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `pkg-config --cflags sdl2` `pkg-config --cflags opengl` `pkg-config --cflags SDL2_image`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/fsmgr.o fsmgr.cpp

${OBJECTDIR}/game.o: game.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `pkg-config --cflags sdl2` `pkg-config --cflags opengl` `pkg-config --cflags SDL2_image`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/game.o game.cpp

${OBJECTDIR}/game_character.o: game_character.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `pkg-config --cflags sdl2` `pkg-config --cflags opengl` `pkg-config --cflags SDL2_image`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/game_character.o game_character.cpp

${OBJECTDIR}/game_data.o: game_data.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `pkg-config --cflags sdl2` `pkg-config --cflags opengl` `pkg-config --cflags SDL2_image`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/game_data.o game_data.cpp

${OBJECTDIR}/game_input.o: game_input.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `pkg-config --cflags sdl2` `pkg-config --cflags opengl` `pkg-config --cflags SDL2_image`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/game_input.o game_input.cpp

${OBJECTDIR}/game_item.o: game_item.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `pkg-config --cflags sdl2` `pkg-config --cflags opengl` `pkg-config --cflags SDL2_image`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/game_item.o game_item.cpp

${OBJECTDIR}/game_locale.o: game_locale.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `pkg-config --cflags sdl2` `pkg-config --cflags opengl` `pkg-config --cflags SDL2_image`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/game_locale.o game_locale.cpp

${OBJECTDIR}/game_loot.o: game_loot.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `pkg-config --cflags sdl2` `pkg-config --cflags opengl` `pkg-config --cflags SDL2_image`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/game_loot.o game_loot.cpp

${OBJECTDIR}/game_mixture_info.o: game_mixture_info.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `pkg-config --cflags sdl2` `pkg-config --cflags opengl` `pkg-config --cflags SDL2_image`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/game_mixture_info.o game_mixture_info.cpp

${OBJECTDIR}/game_newgame.o: game_newgame.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `pkg-config --cflags sdl2` `pkg-config --cflags opengl` `pkg-config --cflags SDL2_image`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/game_newgame.o game_newgame.cpp

${OBJECTDIR}/game_pathfind.o: game_pathfind.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `pkg-config --cflags sdl2` `pkg-config --cflags opengl` `pkg-config --cflags SDL2_image`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/game_pathfind.o game_pathfind.cpp

${OBJECTDIR}/game_placeoff.o: game_placeoff.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `pkg-config --cflags sdl2` `pkg-config --cflags opengl` `pkg-config --cflags SDL2_image`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/game_placeoff.o game_placeoff.cpp

${OBJECTDIR}/game_play.o: game_play.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `pkg-config --cflags sdl2` `pkg-config --cflags opengl` `pkg-config --cflags SDL2_image`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/game_play.o game_play.cpp

${OBJECTDIR}/game_quest.o: game_quest.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `pkg-config --cflags sdl2` `pkg-config --cflags opengl` `pkg-config --cflags SDL2_image`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/game_quest.o game_quest.cpp

${OBJECTDIR}/game_soundres.o: game_soundres.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `pkg-config --cflags sdl2` `pkg-config --cflags opengl` `pkg-config --cflags SDL2_image`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/game_soundres.o game_soundres.cpp

${OBJECTDIR}/game_state.o: game_state.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `pkg-config --cflags sdl2` `pkg-config --cflags opengl` `pkg-config --cflags SDL2_image`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/game_state.o game_state.cpp

${OBJECTDIR}/game_ui.o: game_ui.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `pkg-config --cflags sdl2` `pkg-config --cflags opengl` `pkg-config --cflags SDL2_image`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/game_ui.o game_ui.cpp

${OBJECTDIR}/game_village.o: game_village.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `pkg-config --cflags sdl2` `pkg-config --cflags opengl` `pkg-config --cflags SDL2_image`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/game_village.o game_village.cpp

${OBJECTDIR}/gfx.o: gfx.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `pkg-config --cflags sdl2` `pkg-config --cflags opengl` `pkg-config --cflags SDL2_image`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gfx.o gfx.cpp

${OBJECTDIR}/glfuncs.o: glfuncs.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `pkg-config --cflags sdl2` `pkg-config --cflags opengl` `pkg-config --cflags SDL2_image`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/glfuncs.o glfuncs.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `pkg-config --cflags sdl2` `pkg-config --cflags opengl` `pkg-config --cflags SDL2_image`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/resources.o: resources.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `pkg-config --cflags sdl2` `pkg-config --cflags opengl` `pkg-config --cflags SDL2_image`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/resources.o resources.cpp

${OBJECTDIR}/system.o: system.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `pkg-config --cflags sdl2` `pkg-config --cflags opengl` `pkg-config --cflags SDL2_image`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/system.o system.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
