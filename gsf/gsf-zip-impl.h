/* vim: set sw=8: -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * gsf-zip-impl.h: 
 *
 * Copyright (C) 2002 Tambet Ingo (tambet@ximian.com)
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

#ifndef GSF_ZIP_IMPL_H
#define GSF_ZIP_IMPL_H

G_BEGIN_DECLS

#define ZIP_HEADER_SIZE 		30
#define ZIP_HEADER_VERSION 		 4
#define ZIP_HEADER_OS	 		 5
#define ZIP_HEADER_FLAGS 	         6
#define ZIP_HEADER_COMP_METHOD           8
#define ZIP_HEADER_TIME                 10
#define ZIP_HEADER_CRC 			14
#define ZIP_HEADER_COMP_SIZE		18
#define ZIP_HEADER_UNCOMP_SIZE          22
#define ZIP_HEADER_NAME_LEN		26
#define ZIP_HEADER_EXTRA_LEN		28

#define ZIP_TRAILER_SIZE 		22
#define ZIP_TRAILER_DISK 		4
#define ZIP_TRAILER_DIR_DISK 		6
#define ZIP_TRAILER_ENTRIES 		8
#define ZIP_TRAILER_TOTAL_ENTRIES 	10
#define ZIP_TRAILER_DIR_SIZE 		12
#define ZIP_TRAILER_DIR_POS 		16
#define ZIP_TRAILER_COMMENT_SIZE	20

#define ZIP_DIRENT_SIZE                 46
#define ZIP_DIRENT_ENCODER              4
#define ZIP_DIRENT_EXTRACT              6
#define ZIP_DIRENT_FLAGS                8
#define ZIP_DIRENT_COMPR_METHOD         10
#define ZIP_DIRENT_DOSTIME              12
#define ZIP_DIRENT_CRC32                16
#define ZIP_DIRENT_CSIZE                20
#define ZIP_DIRENT_USIZE                24
#define ZIP_DIRENT_NAME_SIZE            28
#define ZIP_DIRENT_EXTRAS_SIZE          30
#define ZIP_DIRENT_COMMENT_SIZE         32
#define ZIP_DIRENT_DISKSTART            34
#define ZIP_DIRENT_FILE_TYPE            36
#define ZIP_DIRENT_FILE_MODE            38
#define ZIP_DIRENT_OFFSET               42

#define ZIP_FILE_HEADER_SIZE            30
#define ZIP_FILE_HEADER_EXTRACT          4
#define ZIP_FILE_HEADER_FLAGS            6
#define ZIP_FILE_HEADER_COMPR_METHOD     8
#define ZIP_FILE_HEADER_DOSTIME         10
#define ZIP_FILE_HEADER_CRC32           14
#define ZIP_FILE_HEADER_CSIZE           18
#define ZIP_FILE_HEADER_USIZE           22
#define ZIP_FILE_HEADER_NAME_SIZE       26
#define ZIP_FILE_HEADER_EXTRAS_SIZE     28

#define ZIP_NAME_SEPARATOR    '/'

#define ZIP_BLOCK_SIZE 32768
#define ZIP_BUF_SIZE 512


/* z_flags */
#define ZZIP_IS_ENCRYPTED(p)    ((*(unsigned char*)p)&1)
#define ZZIP_IS_COMPRLEVEL(p)  (((*(unsigned char*)p)>>1)&3)
#define ZZIP_IS_STREAMED(p)    (((*(unsigned char*)p)>>3)&1)

typedef enum {
	ZIP_STORED =          0,
	ZIP_SHRUNK =          1,
	ZIP_REDUCEDx1 =       2,
	ZIP_REDUCEDx2 =       3,
	ZIP_REDUCEDx3 =       4,
	ZIP_REDUCEDx4 =       5,
	ZIP_IMPLODED  =       6,
	ZIP_TOKENIZED =       7,
	ZIP_DEFLATED =        8,
	ZIP_DEFLATED_BETTER = 9,
	ZIP_IMPLODED_BETTER = 10
} ZipCompressionMethod;

typedef struct {	
	char                 *name;
	ZipCompressionMethod  compr_method;
	guint32               crc32;
	size_t                csize;
	size_t                usize;
	off_t                 offset;
	off_t                 data_offset;
	guint32               dostime;
} ZipDirent;

typedef struct {
	char *name;
	gboolean is_directory;
	ZipDirent *dirent;
	GSList *children;
} ZipVDir;

ZipDirent* zip_dirent_new (void);
void       zip_dirent_free (ZipDirent *dirent);

ZipVDir* vdir_new (char const *name, gboolean is_directory, ZipDirent *dirent);
void     vdir_free (ZipVDir *vdir, gboolean free_dirent);
void     vdir_add_child (ZipVDir *vdir, ZipVDir *child);

G_END_DECLS

#endif /* GSF_ZIP_IMPL_H */
