# ***** BEGIN GPL LICENSE BLOCK *****
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
# ***** END GPL LICENSE BLOCK *****

set(INC
  ../include
  ../../blenfont
  ../../blenkernel
  ../../blenlib
  ../../blentranslation
  ../../bmesh
  ../../draw
  ../../gpu
  ../../imbuf
  ../../makesdna
  ../../makesrna
  ../../render
  ../../depsgraph
  ../../windowmanager
  ../../../../intern/glew-mx
  ../../../../intern/guardedalloc
  ../../../../intern/mantaflow/extern

  # dna_type_offsets.h
  ${CMAKE_CURRENT_BINARY_DIR}/../../makesdna/intern
)


set(SRC
  space_forgegameview.c
  forgegameview_header.c

  forgegameview_intern.h
)

set(LIB
  bf_editor_lattice
  bf_editor_mesh
)

if(WITH_PYTHON)
  blender_include_dirs(../../python)
  add_definitions(-DWITH_PYTHON)
endif()


if(WITH_INTERNATIONAL)
  add_definitions(-DWITH_INTERNATIONAL)
endif()

if(WITH_FREESTYLE)
  add_definitions(-DWITH_FREESTYLE)
endif()

if(WITH_XR_OPENXR)
  add_definitions(-DWITH_XR_OPENXR)
endif()

blender_add_lib(bf_editor_space_forgegameview "${SRC}" "${INC}" "${INC_SYS}" "${LIB}")

# Needed so we can use dna_type_offsets.h for defaults initialization.
add_dependencies(bf_editor_space_forgegameview bf_dna)
