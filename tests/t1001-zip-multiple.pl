#!/usr/bin/perl -w
# -----------------------------------------------------------------------------

use strict;
use lib ($0 =~ m|^(.*/)| ? $1 : ".");
use LibGsfTest;

&test_zip ('files' => ['Makefile.am', 'common.supp'],
	   'zip-member-tests' => ['!zip64']);
