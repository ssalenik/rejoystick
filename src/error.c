/*
   error.c for the RejoysTick program
   Copyright (c) 2007        Samuel Skånberg

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   See the COPYING file for a copy of the GNU General Public License.
*/

#include <stdlib.h>
#include <stdio.h>
#include "error.h"

void error(char *s)
{
	fprintf(stderr, "error occured: %s. Exiting...\n", s);
	exit(EXIT_FAILURE);
}

