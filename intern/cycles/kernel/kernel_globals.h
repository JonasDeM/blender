/*
 * Copyright 2011-2013 Blender Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Constant Globals */

CCL_NAMESPACE_BEGIN

/* On the CPU, we pass along the struct KernelGlobals to nearly everywhere in
 * the kernel, to access constant data. These are all stored as "textures", but
 * these are really just standard arrays. We can't use actually globals because
 * multiple renders may be running inside the same process. */

#ifdef __KERNEL_CPU__

#  ifdef __OSL__
struct OSLGlobals;
struct OSLThreadData;
struct OSLShadingSystem;
#  endif

struct Intersection;
struct VolumeStep;

typedef struct KernelGlobals {
	texture_image_uchar4 texture_byte4_images[TEX_NUM_BYTE4_CPU];
	texture_image_float4 texture_float4_images[TEX_NUM_FLOAT4_CPU];
	texture_image_half4 texture_half4_images[TEX_NUM_HALF4_CPU];
	texture_image_float texture_float_images[TEX_NUM_FLOAT_CPU];
	texture_image_uchar texture_byte_images[TEX_NUM_BYTE_CPU];
	texture_image_half texture_half_images[TEX_NUM_HALF_CPU];

#  define KERNEL_TEX(type, ttype, name) ttype name;
#  define KERNEL_IMAGE_TEX(type, ttype, name)
#  include "kernel_textures.h"

	KernelData __data;

#  ifdef __OSL__
	/* On the CPU, we also have the OSL globals here. Most data structures are shared
	 * with SVM, the difference is in the shaders and object/mesh attributes. */
	OSLGlobals *osl;
	OSLShadingSystem *osl_ss;
	OSLThreadData *osl_tdata;
#  endif

	/* **** Run-time data ****  */

	/* Heap-allocated storage for transparent shadows intersections. */
	Intersection *transparent_shadow_intersections;

	/* Storage for decoupled volume steps. */
	VolumeStep *decoupled_volume_steps[2];
	int decoupled_volume_steps_index;

	/* split kernel */
	ShaderData *sd_input;
	Intersection *isect_shadow;
	SplitData split_data;
	SplitParams split_param_data;
} KernelGlobals;

#endif  /* __KERNEL_CPU__ */

/* For CUDA, constant memory textures must be globals, so we can't put them
 * into a struct. As a result we don't actually use this struct and use actual
 * globals and simply pass along a NULL pointer everywhere, which we hope gets
 * optimized out. */

#ifdef __KERNEL_CUDA__

__constant__ KernelData __data;
typedef struct KernelGlobals {} KernelGlobals;

#  ifdef __KERNEL_CUDA_TEX_STORAGE__
#    define KERNEL_TEX(type, ttype, name) ttype name;
#  else
#    define KERNEL_TEX(type, ttype, name) const __constant__ __device__ type *name;
#  endif
#  define KERNEL_IMAGE_TEX(type, ttype, name) ttype name;
#  include "kernel_textures.h"

#endif  /* __KERNEL_CUDA__ */

/* OpenCL */

#ifdef __KERNEL_OPENCL__

typedef ccl_addr_space struct KernelGlobals {
	ccl_constant KernelData *data;

#  define KERNEL_TEX(type, ttype, name) \
	ccl_global type *name;
#  include "kernel_textures.h"

#  ifdef __SPLIT_KERNEL__
	ShaderData *sd_input;
	Intersection *isect_shadow;
	SplitData split_data;
	SplitParams split_param_data;
#  endif
} KernelGlobals;

#endif  /* __KERNEL_OPENCL__ */

/* Interpolated lookup table access */

ccl_device float lookup_table_read(KernelGlobals *kg, float x, int offset, int size)
{
	x = saturate(x)*(size-1);

	int index = min(float_to_int(x), size-1);
	int nindex = min(index+1, size-1);
	float t = x - index;

	float data0 = kernel_tex_fetch(__lookup_table, index + offset);
	if(t == 0.0f)
		return data0;

	float data1 = kernel_tex_fetch(__lookup_table, nindex + offset);
	return (1.0f - t)*data0 + t*data1;
}

ccl_device float lookup_table_read_2D(KernelGlobals *kg, float x, float y, int offset, int xsize, int ysize)
{
	y = saturate(y)*(ysize-1);

	int index = min(float_to_int(y), ysize-1);
	int nindex = min(index+1, ysize-1);
	float t = y - index;

	float data0 = lookup_table_read(kg, x, offset + xsize*index, xsize);
	if(t == 0.0f)
		return data0;

	float data1 = lookup_table_read(kg, x, offset + xsize*nindex, xsize);
	return (1.0f - t)*data0 + t*data1;
}

CCL_NAMESPACE_END

