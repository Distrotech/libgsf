/* vim: set sw=8: -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * gsf-shared-bonobo-stream.h: helper class for gsf-input-bonobo
 *
 * Copyright (C) 2002 Jon K Hellan (hellan@acm.org)
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

#ifndef GSF_SHARED_BONOBO_STREAM_H
#define GSF_SHARED_BONOBO_STREAM_H

#include <gsf/gsf.h>
#include <glib-object.h>
#include <bonobo/bonobo-stream.h>

G_BEGIN_DECLS

#define GSF_SHARED_BONOBO_STREAM_TYPE	(gsf_shared_bonobo_stream_get_type ())
#define GSF_SHARED_BONOBO_STREAM(o)	(G_TYPE_CHECK_INSTANCE_CAST ((o), GSF_SHARED_BONOBO_STREAM_TYPE, GsfSharedBonoboStream))
#define GSF_IS_SHARED_BONOBO_STREAM(o)	(G_TYPE_CHECK_INSTANCE_TYPE ((o), GSF_SHARED_BONOBO_STREAM_TYPE))

typedef struct _GsfSharedBonoboStream GsfSharedBonoboStream;
struct _GsfSharedBonoboStream {
	GObject g_object;
	Bonobo_Stream stream;
	CORBA_long pos;
};

GType gsf_shared_bonobo_stream_get_type (void);
GsfSharedBonoboStream *gsf_shared_bonobo_stream_new (Bonobo_Stream stream);

G_END_DECLS

#endif /* GSF_SHARED_BONOBO_STREAM_H */
