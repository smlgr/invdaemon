/*
 *   SolarMax S3000 DB logger
 *   Copyright (C) 2013  Luca Cireddu
 *   sardylan@gmail.com
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, write to the Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */


#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "invdaemon.h"
#include "ui.h"
#include "cfg.h"

char* smlgr_program_name;
cfg* conf;



int main(int argc, char** argv)
{
    smlgr_program_name = argv[0];

    cfgInit();

    if(cfgParse(argc, argv)) {
        cfgPrint();
        invdaemon();
    }

    cfgFree();

    return 0;
}



/**
 * Main program function
 */

void invdaemon()
{

}
