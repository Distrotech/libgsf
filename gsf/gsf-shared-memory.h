/*
 * gsf-shared-file.h: interface for used by the ole layer to read raw data
 *
 * Copyright (C) 2002 Morten Welinder (terra@diku.dk)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */

#ifndef GSF_SHARED_MEMORY_H
#define GSF_SHARED_MEMORY_H

#include <gsf/gsf.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define GSF_SHARED_MEMORY_TYPE	(gsf_shared_memory_get_type ())
#define GSF_SHARED_MEMORY(o)	(G_TYPE_CHECK_INSTANCE_CAST ((o), GSF_SHARED_MEMORY_TYPE, GsfSharedMemory))
#define IS_GSF_SHARED_MEMORY(o)	(G_TYPE_CHECK_INSTANCE_TYPE ((o), GSF_SHARED_MEMORY_TYPE))

typedef struct _GsfSharedMemory GsfSharedMemory;
struct _GsfSharedMemory {
	GObject g_object;
	void *buf;
	size_t size;

	gboolean needs_free;
	gboolean needs_unmap;
};

GType gsf_shared_memory_get_type (void);
GsfSharedMemory *gsf_shared_memory_new (void *buf, size_t size, gboolean needs_free);
GsfSharedMemory *gsf_shared_memory_mmapped_new (void *buf, size_t size);

G_END_DECLS

#endif /* GSF_SHARED_MEMORY_H */
