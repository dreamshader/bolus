/*
 ***********************************************************************
 *
 *  bolus-cli.cpp - a command line interface to my bolus engine
 *
 ***********************************************************************
 *
 *  Copyright (C) 2018 Dreamshader (aka Dirk Schanz)
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  
 *      http://www.apache.org/licenses/LICENSE-2.0
 *  
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ***********************************************************************
 *
 * Options:
 *
 * -------------- communication port for connection --------------------
 *
 * --com devicename (same as --com=devicename resp. -c devicename)
 *
 *   Default is --com=/dev/ttyUSB0
 *
 * ---------------- baudrate for serial connection ---------------------
 *
 * --baud baudrate (same as --baud=baudrate resp. -b baudrate)
 *
 *   Default is --baud=38400
 *
 * ---------------- databit for serial connection ----------------------
 *
 * --data bits (same as --data=bits resp. -d bits)
 *			may be 5 up to 8
 *
 *   Default is --data=8
 *
 * ---------------- parity for serial connection -----------------------
 *
 * --parity parity (same as --parity=parity resp. -p parity)
 *			may be e/E (even), o/O (odd), n/N (none)
 *
 *   Default is --parity=n
 *
 * --------------- stoppbits for serial connection ---------------------
 *
 * --stop stoppbits (same as --stop=stoppbits resp. -s stoppbits)
 *			may be 1 or 2
 *
 *   Default is --stop=1
 *
 * --------------- handshake for serial connection ---------------------
 *
 * --handshake handshake (same as --handshake=handshake resp. -h handshake)
 *			may be n/N (no handshake), x/X (XON/XOFF)
 *
 *   Default is --handshake=n
 *
 * ----------------------------- help ----------------------------------
 *
 * --help 	(same as -? )
 *
 *   Show options and exit
 *
 ***********************************************************************
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/signal.h>
#include <sys/param.h>
#include <fcntl.h>
#include <termios.h>



#include <iostream>
#include <string>

#include "bolus.h"

#ifdef NEVERDEF
/* ---------------------------------------------------------------------------------
 | void help( struct serial_param_t *ctl_param, short failed )
 |
 | show options. If failed, print an error message, too
 -----------------------------------------------------------------------------------
*/

void help( struct serial_param_t *ctl_param, short failed )
{

	fprintf(stderr, "soehnle printer on RPi\n");
	fprintf(stderr, "valid options are:\n");
	fprintf(stderr, 
		"--com devicename (same as --com=devicename resp. -c devicename)\n");
	fprintf(stderr, "use device devicename\n");
	fprintf(stderr, "Default is --com=/dev/ttyUSB0\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "--baud baudrate (same as --baud=baudrate resp. -b baudrate)\n");
	fprintf(stderr, "set speed to baudrate\n");
	fprintf(stderr, "Default is --baud=38400\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "--data bits (same as --data=bits resp. -d bits)\n");
	fprintf(stderr, "set databits to bits (5 up to 8 )\n");
	fprintf(stderr, "Default is --data=8\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "--parity parity (same as --parity=parity resp. -p parity)\n");
	fprintf(stderr, 
		"which parity to use (e/E for even, o/O for odd, n/N for no parity check)\n");
	fprintf(stderr, "Default is --parity=n\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "--stop stoppbits (same as --stop=stoppbits resp. -s stoppbits)\n");
	fprintf(stderr, "amount of stoppbits ( 1 or 2 )\n");
	fprintf(stderr, "Default is --stop=1\n");
	fprintf(stderr, "\n");
	fprintf(stderr, 
		"--handshake handshake (same as --handshake=handshake resp. -h handshake)\n");
	fprintf(stderr, "handshake for transmission (n/N for none, x/X for ixon/ixoff)\n");
	fprintf(stderr, "Default is --handshake=n\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "--help 	(same as -? )\n");
	fprintf(stderr, "display help info\n");

	exit(failed);
}

/* ---------------------------------------------------------------------------------
 | void get_arguments ( int argc, char **argv, struct serial_param_t *ctl_param, 
 |                      short *myst)
 |
 | scan commandline for arguments an set the corresponding value
 | myst is a short pointer to a secret flag
 -----------------------------------------------------------------------------------
*/

void get_arguments ( int argc, char **argv, struct serial_param_t *ctl_param, short *myst)
{

	int failed = 0;
	int next_option;
	/* valid short options letters */
	const char* const short_options = "c:b:d:p:s:h:m?";

	/* valid long options */
	const struct option long_options[] = {
 		{ "com",		1, NULL, 'c' },
 		{ "baud",		1, NULL, 'b' },
 		{ "data",		1, NULL, 'd' },
 		{ "parity",		1, NULL, 'p' },
 		{ "stop",		1, NULL, 's' },
 		{ "handshake",		1, NULL, 'h' },
 		{ "mystery",		0, NULL, 'm' },
 		{ "help",		0, NULL, '?' },
		{ NULL,			0, NULL,  0  }
	};

	set_defaults( ctl_param );
	if( myst != NULL )
	{
		*myst = 0;
	}
	failed = 0;

	do
	{
		next_option = getopt_long (argc, argv, short_options,
			long_options, NULL);

		switch (next_option) {
			case 'c':
				if( strlen(optarg) )
				{
					ctl_param->device = strdup( optarg );
				}
				else
				{
					help( ctl_param, E_DEVICE);
				}
				break;
			case 'b':
				ctl_param->baud = atoi(optarg);
				break;
			case 'd':
				ctl_param->databit =atoi(optarg);
				break;
			case 'p':
				ctl_param->parity = optarg[0];
				break;
			case 's':
				ctl_param->stoppbits =atoi(optarg);
				break;
			case 'h':
				ctl_param->handshake = optarg[0];
				break;
			case 'm':
				if( myst != NULL )
				{
					*myst = 1;
				}
				break;
			case '?':
				help( ctl_param, 0 );
				break;
			case -1:
				break;
			default:
				fprintf(stderr, "Invalid option %c! \n", next_option);
				help( ctl_param, 0 );
		}
	} while (next_option != -1);
}

#endif // NEVERDEF

int main( int argc, char *argv[] )
{
    int retVal = 0;
    bolus *pNewBolus;

    if( (pNewBolus = new(bolus)) != NULL )
    {
        retVal = pNewBolus->init( );

        pNewBolus->end( );
    }

    return( retVal );
}


