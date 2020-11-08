/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2008 Blender Foundation.
 * All rights reserved.
 */

/** \file
 * \ingroup spforgegameview
 */

#include <stdio.h>
#include <string.h>

#include "DNA_defaults.h"
#include "DNA_gpencil_types.h"
#include "DNA_lightprobe_types.h"
#include "DNA_material_types.h"
#include "DNA_object_types.h"
#include "DNA_scene_types.h"

#include "MEM_guardedalloc.h"

#include "BLI_blenlib.h"
#include "BLI_math.h"
#include "BLI_utildefines.h"

#include "BLT_translation.h"

#include "BKE_context.h"
#include "BKE_curve.h"
#include "BKE_global.h"
#include "BKE_icons.h"
#include "BKE_idprop.h"
#include "BKE_lattice.h"
#include "BKE_main.h"
#include "BKE_mball.h"
#include "BKE_mesh.h"
#include "BKE_object.h"
#include "BKE_scene.h"
#include "BKE_screen.h"
#include "BKE_workspace.h"

#include "ED_screen.h"
#include "ED_space_api.h"
#include "ED_transform.h"

#include "GPU_matrix.h"

#include "DRW_engine.h"

#include "WM_api.h"
#include "WM_message.h"
#include "WM_toolsystem.h"
#include "WM_types.h"

#include "RE_engine.h"
#include "RE_pipeline.h"

#include "RNA_access.h"

#include "UI_interface.h"
#include "UI_resources.h"

#ifdef WITH_PYTHON
#  include "BPY_extern.h"
#endif

#include "DEG_depsgraph.h"

#include "forgegameview_intern.h" /* own include */


// copied from view3d
