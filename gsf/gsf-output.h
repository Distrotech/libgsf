/* vim: set sw=8: -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * gsf-output.h: interface for storing data
 *
 * Copyright (C) 2002 Jody Goldberg (jody@gnome.org)
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

#ifndef GSF_OUTPUT_H
#define GSF_OUTPUT_H

#include <gsf/gsf.h>
#include <sys/types.h>

G_BEGIN_DECLS

#define GSF_OUTPUT_TYPE        (gsf_output_get_type ())
#define GSF_OUTPUT(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), GSF_OUTPUT_TYPE, GsfOutput))
#define GSF_IS_OUTPUT(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), GSF_OUTPUT_TYPE))

GType gsf_output_get_type (void);

char const   *gsf_output_name	   (GsfOutput const *output);
GsfOutfile   *gsf_output_container (GsfOutput const *output);

ssize_t	      gsf_output_size      (GsfOutput *output);
gboolean      gsf_output_close     (GsfOutput *output);
gboolean      gsf_output_is_closed (GsfOutput const *output);
int           gsf_output_tell      (GsfOutput *output);
gboolean      gsf_output_seek      (GsfOutput *output,
				    off_t offset, GsfOff_t whence);
gboolean      gsf_output_write     (GsfOutput *output,
				    size_t num_bytes, guint8 const *data);

gboolean gsf_output_wrap   (GsfOutput *wrapper, GsfOutput *wrapee);
gboolean gsf_output_unwrap (GsfOutput *wrapper, GsfOutput *wrapee);

GQuark gsf_output_error (void);

G_END_DECLS

#endif /* GSF_OUTPUT_H */
