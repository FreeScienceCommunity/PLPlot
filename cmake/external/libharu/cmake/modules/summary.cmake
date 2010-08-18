# cmake/modules/summary.cmake
#
# Copyright (C) 2008  Werner Smekal
#
# This software is provided 'as-is', without any express or implied warranty.
#
# In no event will the authors be held liable for any damages arising from the
# use of this software.
# 
# Permission is granted to anyone to use this software for any purpose,including
# commercial applications, and to alter it and redistribute it freely, subject
# to the following restrictions:
# 
#  1. The origin of this software must not be misrepresented; you must not claim
#     that you wrote the original software. If you use this software in a
#     product, an acknowledgment in the product documentation would be
#     appreciated but is not required.
#  2. Altered source versions must be plainly marked as such, and must not be
#     misrepresented as being the original software.
#  3. This notice may not be removed or altered from any source distribution. 
#
# Macro for outputting all the most important CMake variables for haru

# =======================================================================
# print summary of configuration to screen
# =======================================================================

macro(summary)
set(_output_results "
Summary of CMake build system results for the haru library

Install location variables which can be set by the user:
CMAKE_INSTALL_PREFIX:      ${CMAKE_INSTALL_PREFIX}
CMAKE_INSTALL_EXEC_PREFIX  ${CMAKE_INSTALL_EXEC_PREFIX}
CMAKE_INSTALL_BINDIR 	   ${CMAKE_INSTALL_BINDIR}
CMAKE_INSTALL_LIBDIR 	   ${CMAKE_INSTALL_LIBDIR}
CMAKE_INSTALL_INCLUDEDIR   ${CMAKE_INSTALL_INCLUDEDIR}

Other important CMake variables:

CMAKE_SYSTEM_NAME:	${CMAKE_SYSTEM_NAME}
UNIX:			${UNIX}
WIN32:			${WIN32}
APPLE:			${APPLE}
MSVC:			${MSVC}	(MSVC_VERSION:	${MSVC_VERSION})
MINGW:			${MINGW}
MSYS:			${MSYS}
CYGWIN:			${CYGWIN}
BORLAND:		${BORLAND}
WATCOM:		  ${WATCOM}

CMAKE_BUILD_TYPE:	${CMAKE_BUILD_TYPE}
CMAKE_C_COMPILER CMAKE_C_FLAGS:			${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS}

Library options:
LIBHARU_SHARED:		${LIBHARU_SHARED}
LIBHARU_STATIC:		${LIBHARU_STATIC}
LIBHARU_EXAMPLES:	${LIBHARU_EXAMPLES}
DEVPAK:			${DEVPAK}

Optional libraries:
HAVE_LIBZ:		${HAVE_LIBZ}
HAVE_LIBPNG:		${HAVE_LIBPNG}
")
message("${_output_results}")
endmacro(summary)
