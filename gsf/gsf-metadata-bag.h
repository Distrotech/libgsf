/* vim: set sw=8: -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * gsf-metadata-bag.h: get, set, remove custom meta properties associated with documents
 *
 * Copyright (C) 2002 Dom Lachowicz (cinamod@hotmail.com)
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

#ifndef GSF_METADATA_BAG_H
#define GSF_METADATA_BAG_H

#include <gsf/gsf.h>
#include <sys/types.h>

G_BEGIN_DECLS

#define GSF_METADATA_BAG_TYPE        (gsf_metadata_bag_get_type ())
#define GSF_METADATA_BAG(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), GSF_METADATA_BAG_TYPE, GsfMetaDataBag))
#define GSF_IS_METADATA_BAG(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), GSF_METADATA_BAG_TYPE))

GType  gsf_metadata_bag_get_type (void);
GQuark gsf_metadata_bag_error (void);

typedef GHFunc GMetaDataBagEnumFunc;

GsfMetaDataBag *gsf_metadata_bag_new (void);
void            gsf_metadata_bag_set_prop	(GsfMetaDataBag *meta, char const *prop, GValue const *value);
void            gsf_metadata_bag_remove_prop	(GsfMetaDataBag *meta, char const *prop);
GValuec const  *gsf_metadata_bag_get_prop	(GsfMetaDataBag *meta, char const *prop);
gboolean        gsf_metadata_bag_contains_prop	(GsfMetaDataBag *meta, char const *prop);
void            gsf_metadata_bag_iterate	(GsfMetaDataBag *meta, GsfMetaDataBagEnumFunc func, gpointer user_data);
gsize_t         gsf_metadata_bag_cardinality	(GsfMetaDataBag *meta);

G_END_DECLS

#endif /* GSF_METADATA_BAG_H */
