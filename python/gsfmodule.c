/*
 * gsfmodule.c
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

/* 
 * Python bindings for libgsf.
 */

#include <pygobject.h>

extern PyMethodDef pygsf_functions[];

DL_EXPORT(void)
initgsf (void)
{
	PyObject *m, *d;

	init_pygobject ();

	m = Py_InitModule ("gsf", pygsf_functions);
	d = PyModule_GetDict (m);

	pygsf_register_classes (d);
	
	if (PyErr_Occurred ()) {
		Py_FatalError ("can't initialise module gsf");
	}
}
