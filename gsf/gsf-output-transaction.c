/* vim: set sw=8: -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * gsf-output-transaction.c:
 *
 * Copyright (C) 2004-2006 Dom Lachowicz (cinamod@hotmail.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
 * USA
 */

#include <gsf-config.h>
#include <gsf/gsf-output-transaction.h>
#include <gsf/gsf.h>

struct _GsfOutputTransaction
{
	GsfOutput   parent;

	GsfOutput * proxy;
	GsfOutput * wrapped;

	gboolean    aborted;
};

struct _GsfOutputTransactionClass
{
	GsfOutputClass parent;

	/* signals */
	void (* updated)   (GsfOutputTransaction * me);
	void (* committed) (GsfOutputTransaction * me);
	void (* aborted)   (GsfOutputTransaction * me);
};

enum {
	UPDATED,
	COMMITTED,
	ABORTED,
	LAST_SIGNAL
};

static guint transaction_signals [LAST_SIGNAL];
static GsfOutputClass *proxy_class;
static GsfOutputClass *parent_class;

/********************************************************************************/
/********************************************************************************/

/**
 * gsf_output_transaction_commit:
 * @output: the transaction you wish to commit
 *
 * "Atomically" commits your data changes. After committing, it closes the
 * output. No more writes, seeks, etc... are valid on this object.
 *
 * Emits "committed" signal on success, "aborted" signal on failure.
 *
 * Returns: Commit success or failure
 */
gboolean
gsf_output_transaction_commit (GsfOutputTransaction * output)
{
	gboolean result;

	g_return_val_if_fail (output != NULL, FALSE);
	g_return_val_if_fail (GSF_IS_OUTPUT_TRANSACTION (output), FALSE);

	if (output->aborted) {
		g_warning ("Attempting to commit to an aborted transaction");
		return FALSE;
	}

	/* write the proxy's bytes to the wrapped class */
	result = gsf_output_write (output->wrapped,
				   gsf_output_size (GSF_OUTPUT (output->proxy)),
				   gsf_output_memory_get_bytes (GSF_OUTPUT_MEMORY (output->proxy)));

	if (result) {
		/* close the underlying proxy - we can no longer take place in any transactions */
		gsf_output_close (GSF_OUTPUT (output));
		g_signal_emit (G_OBJECT (output), transaction_signals[COMMITTED], 0);
	}
	else
		gsf_output_transaction_abort (output);

	return result;
}

/**
 * gsf_output_transaction_commit:
 * @output: the transaction you wish to abort. After aborting, it closes the
 * output. No more writes, seeks, etc... are valid on this object.
 *
 * Emits "aborted" signal
 */
void
gsf_output_transaction_abort (GsfOutputTransaction * output)
{
	g_return_if_fail (output);
	g_return_if_fail (GSF_IS_OUTPUT_TRANSACTION (output));

	output->aborted = TRUE;

	/* close the underlying proxy - we can no longer take place in any transactions */
	gsf_output_close (GSF_OUTPUT (output));

	g_signal_emit (G_OBJECT (output), transaction_signals[ABORTED], 0);
}

/**
 * gsf_output_transaction_new_named:
 * @wrapped: The sink you want to write to
 * @name: The non-null name for this transaction
 *
 * Returns: The new transaction output object
 */
GsfOutput *
gsf_output_transaction_new_named (GsfOutput *wrapped, char const *name)
{
	GsfOutputTransaction * trans = NULL;

	g_return_val_if_fail (wrapped != NULL, NULL);
	g_return_val_if_fail (GSF_IS_OUTPUT (wrapped), NULL);
	g_return_val_if_fail (name != NULL, NULL);

	trans = g_object_new (GSF_OUTPUT_TRANSACTION_TYPE, NULL);

	gsf_output_set_name (GSF_OUTPUT (trans), name);

	trans->proxy = gsf_output_memory_new ();

	/* TODO: connect signals if wrapped is a GsfOutputTransaction for cascade-effect?? */

	trans->wrapped = g_object_ref  (wrapped);

	return GSF_OUTPUT (trans);
}

/**
 * gsf_output_transaction_new:
 * @wrapped: The sink you want to write to
 *
 * Returns: The new anonymous transaction output object
 */
GsfOutput *
gsf_output_transaction_new (GsfOutput *wrapped)
{
	/* All error checking done in new_named() */
	return gsf_output_transaction_new_named (wrapped, "Anonymous Transaction");
}

/********************************************************************************/
/********************************************************************************/

/* closes the underlying proxy */
static gboolean
gsf_output_trans_close (GsfOutput * output)
{
	g_return_val_if_fail (output != NULL, FALSE);
	g_return_val_if_fail (GSF_IS_OUTPUT_TRANSACTION (output), FALSE);

	if (!gsf_output_is_closed (output))
		return gsf_output_close (GSF_OUTPUT_TRANSACTION (output)->proxy);
	return TRUE;
}

/* NB: seeks within your transacted data, not the underlying wrapped sink. emits "updated" or "aborted" */
static gboolean
gsf_output_trans_seek (GsfOutput *output, gsf_off_t offset,
		       GSeekType whence)
{
	GsfOutputTransaction * trans = (GsfOutputTransaction *)output;
	gboolean result;

	g_return_val_if_fail (output != NULL, FALSE);
	g_return_val_if_fail (GSF_IS_OUTPUT_TRANSACTION (output), FALSE);

	if (trans->aborted) {
		g_warning ("Attempting to seek an aborted transaction");
		return FALSE;
	}

	result = gsf_output_seek (trans->proxy, offset, whence);
	if (result)
		g_signal_emit (G_OBJECT (trans), transaction_signals[UPDATED], 0);
	else
		gsf_output_transaction_abort (trans);

	return result;
}

/* write data to the transaction. emits "updated" or "aborted" */
static gboolean
gsf_output_trans_write (GsfOutput *output,
			size_t num_bytes,
			guint8 const *buffer)
{
	GsfOutputTransaction * trans = (GsfOutputTransaction *)output;
	gboolean result;

	g_return_val_if_fail (output != NULL, FALSE);
	g_return_val_if_fail (GSF_IS_OUTPUT_TRANSACTION (output), FALSE);

	if (trans->aborted) {
		g_warning ("Attempting to write to an aborted transaction");
		return FALSE;
	}

	result = gsf_output_write (trans->proxy, num_bytes, buffer);
	if (result)
		g_signal_emit (G_OBJECT (trans), transaction_signals[UPDATED], 0);
	else
		gsf_output_transaction_abort (trans);

	return result;
}

/* vprintf function. emits "updated" or "aborted" */
static gsf_off_t gsf_output_trans_vprintf (GsfOutput *output,
	char const *format, va_list args) G_GNUC_PRINTF (2, 0);

static gsf_off_t
gsf_output_trans_vprintf (GsfOutput *output, char const *format, va_list args)
{
	GsfOutputTransaction * trans = (GsfOutputTransaction *)output;
	gsf_off_t result;

	g_return_val_if_fail (output != NULL, FALSE);
	g_return_val_if_fail (GSF_IS_OUTPUT_TRANSACTION (output), FALSE);

	if (trans->aborted) {
		g_warning ("Attempting to write to an aborted transaction");
		return FALSE;
	}

	result = proxy_class->Vprintf (trans->proxy, format, args);
	if (result >= 0)
		g_signal_emit (G_OBJECT (trans), transaction_signals[UPDATED], 0);
	else
		gsf_output_transaction_abort (trans);

	return result;
}

/********************************************************************************/
/********************************************************************************/

static void
gsf_output_transaction_init (GsfOutputTransaction *me,
			     G_GNUC_UNUSED GsfOutputTransactionClass *klass)
{
	me->proxy   = NULL;
	me->wrapped = NULL;
	me->aborted = FALSE;
}

static void
gsf_output_transaction_finalize (GObject *object)
{
	GsfOutputTransaction *me = (GsfOutputTransaction *) object;

	g_return_if_fail (GSF_IS_OUTPUT_TRANSACTION (object));

	g_object_unref (me->proxy);
	g_object_unref (me->wrapped);

	parent_class->finalize (object);
}

static void
gsf_output_transaction_class_init (GObjectClass *klass)
{
	GsfOutputClass *output_class = GSF_OUTPUT_CLASS (klass);

	klass->finalize         = gsf_output_transaction_finalize;
	output_class->Close     = gsf_output_trans_close;
	output_class->Seek      = gsf_output_trans_seek;
	output_class->Write     = gsf_output_trans_write;
	output_class->Vprintf   = gsf_output_trans_vprintf;

	/* class signals */
	transaction_signals[UPDATED] =
		g_signal_new ("updated",
			      G_TYPE_FROM_CLASS (klass),
			      G_SIGNAL_RUN_LAST,
			      G_STRUCT_OFFSET (GsfOutputTransactionClass, updated),
			      NULL, NULL,
			      g_cclosure_marshal_VOID__VOID,
			      G_TYPE_NONE, 0);

	transaction_signals[COMMITTED] =
		g_signal_new ("committed",
			      G_TYPE_FROM_CLASS (klass),
			      G_SIGNAL_RUN_LAST,
			      G_STRUCT_OFFSET (GsfOutputTransactionClass, committed),
			      NULL, NULL,
			      g_cclosure_marshal_VOID__VOID,
			      G_TYPE_NONE, 0);

	transaction_signals[ABORTED] =
		g_signal_new ("aborted",
			      G_TYPE_FROM_CLASS (klass),
			      G_SIGNAL_RUN_LAST,
			      G_STRUCT_OFFSET (GsfOutputTransactionClass, aborted),
			      NULL, NULL,
			      g_cclosure_marshal_VOID__VOID,
			      G_TYPE_NONE, 0);

	/* global variables */
	proxy_class = g_type_class_peek (GSF_OUTPUT_MEMORY_TYPE);
	parent_class = g_type_class_peek_parent (klass);
}

GSF_CLASS (GsfOutputTransaction, gsf_output_transaction,
	   gsf_output_transaction_class_init, gsf_output_transaction_init,
	   GSF_OUTPUT_TYPE)
