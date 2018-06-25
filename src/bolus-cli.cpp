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
 *            may be 5 up to 8
 *
 *   Default is --data=8
 *
 * ---------------- parity for serial connection -----------------------
 *
 * --parity parity (same as --parity=parity resp. -p parity)
 *            may be e/E (even), o/O (odd), n/N (none)
 *
 *   Default is --parity=n
 *
 * --------------- stoppbits for serial connection ---------------------
 *
 * --stop stoppbits (same as --stop=stoppbits resp. -s stoppbits)
 *            may be 1 or 2
 *
 *   Default is --stop=1
 *
 * --------------- handshake for serial connection ---------------------
 *
 * --handshake handshake (same as --handshake=handshake resp. -h handshake)
 *            may be n/N (no handshake), x/X (XON/XOFF)
 *
 *   Default is --handshake=n
 *
 * ----------------------------- help ----------------------------------
 *
 * --help     (same as -? )
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

/* ---------------------------------------------------------------------------------
 | void help( struct serial_param_t *ctl_param, short failed )
 |
 | show options. If failed, print an error message, too
 -----------------------------------------------------------------------------------
*/

void help( void )
{
    fprintf(stderr, "HELP!\n");
    exit(0);
}

void dumpArgs( struct _bolus_param *pParam )
{
    if( pParam != NULL )
    {
        fprintf(stderr, "fail ...: %s\n", 
                pParam->fail == true ? "true" : "false" );
        fprintf(stderr, "glucose.: %d\n", pParam->glucose );
        fprintf(stderr, "carb ...: %d\n", pParam->carb );
        fprintf(stderr, "bread ..: %d\n", pParam->bread );
        fprintf(stderr, "meal ...: %c\n", pParam->mealType );
        fprintf(stderr, "measure.: %c\n", pParam->measType );
        fprintf(stderr, "adjust .: %c\n", pParam->adjust );
        fprintf(stderr, "last ...: %s\n", 
                pParam->last == true ? "true" : "false" );
        fprintf(stderr, "edit ...: %c\n", pParam->editType );
        fprintf(stderr, "export .: %c\n", pParam->exportType );
        fprintf(stderr, "import .: %s\n", pParam->importFile );
        fprintf(stderr, "interact: %s\n", 
                pParam->interactive == true ? "true" : "false" );
        fprintf(stderr, "nostore : %s\n", 
                pParam->noStore == true ? "true" : "false" );
    }
}


void resetArgs( struct _bolus_param *pParam )
{
    if( pParam != NULL )
    {
        pParam->fail        = false;
        pParam->glucose     = 0;
        pParam->carb        = 0;
        pParam->bread       = 0;
        pParam->mealType    = '\0';
        pParam->measType    = '\0';
        pParam->adjust      = 0;
        pParam->last        = false;
        pParam->editType    = '\0';
        pParam->exportType  = '\0';
        pParam->importFile  = NULL;
        pParam->interactive = false;
        pParam->noStore = false;
    }
}

/* ---------------------------------------------------------------------------------
 | void get_arguments ( int argc, char **argv, struct serial_param_t *ctl_param, 
 |                      short *myst)
 |
 | scan commandline for arguments an set the corresponding value
 | myst is a short pointer to a secret flag
 -----------------------------------------------------------------------------------
*/

void get_arguments ( int argc, char **argv, struct _bolus_param *pParam )
{

    int failed = 0;
    int next_option;
    /* valid short options letters */
    const char* const short_options = "g:c:b:m:t:le:X:I:inh?";

    if( pParam != NULL )
    {

        /* valid long options */
        const struct option long_options[] = {
             { "glucose",     1, NULL, 'g' },
             { "carb",        1, NULL, 'c' },
             { "bread",       1, NULL, 'b' },
             { "meal",        1, NULL, 'm' },
             { "type",        1, NULL, 't' },
             { "last",        0, NULL, 'l' },
             { "edit",        1, NULL, 'e' },
             { "export",      1, NULL, 'X' },
             { "import",      1, NULL, 'I' },
             { "interactive", 0, NULL, 'i' },
             { "nostore",     0, NULL, 'n' },
             { "help",        0, NULL, 'h' },
            { NULL,           0, NULL,  0  }
        };
    
        resetArgs( pParam );
    
        do
        {
            next_option = getopt_long (argc, argv, short_options,
                long_options, NULL);
    
            switch (next_option) {
                case 'g':
                    pParam->glucose = atoi(optarg);
                    break;
                case 'c':
                    pParam->carb = atoi(optarg);
                    break;
                case 'b':
                    pParam->bread = atoi(optarg);
                    break;
                case 'm':
                    pParam->mealType = optarg[0];
                    // -m b --meal=b(efore)
                    // -m B --meal=B(efore)
                    // -m a --meal=a(fter)
                    // -m A --meal=A(fter)
                    // -m n --meal=n(one)
                    // -m N --meal=N(one)
                    // -m s --meal=s(leeptime)
                    // -m s --meal=s(leeptime)
                    // -m x --meal=x(tra)
                    // -m S --meal=X(tra)
                    break;
                case 't':
                    pParam->measType = optarg[0];
                    // -t a --type=(a)cucheck
                    // -t f --type=(f)reestyle
                    break;
                case 'a':
                    switch( optarg[0] )
                    {
                        case '1':
                            pParam->adjust = ADJUST_SPORTS_1;
                            break;
                        case '2':
                            pParam->adjust = ADJUST_SPORTS_2;
                            break;
                        case 's':
                        case 'S':
                            pParam->adjust = ADJUST_STRESS;
                            break;
                        case 'i':
                        case 'I':
                            pParam->adjust = ADJUST_ILL;
                            break;
                        case 'f':
                        case 'F':
                            pParam->adjust = ADJUST_FEMALE;
                            break;
                        default:
                            pParam->adjust = 0;
                            break;
                    }
                    break;
                case 'l':
                    pParam->last = true;
                    break;
                case 'e':
                    pParam->editType = optarg[0];
                    // -e --edit=T(imeblocks)
                    // -e --edit=G(lobals)
                    // -e --edit=A(djustments)
                    // -e T
                    // -e G
                    // -e A
                     break;
                case 'X':
                    pParam->exportType = optarg[0];
                    // -X --export=T(imeblocks)
                    // -X --export=G(lobals)
                    // -X --export=A(djustments)
                    // -X --export=J(ason)
                    // -X T
                    // -X G
                    // -X A
                    // -X J
                    break;
                case 'I':
                    if( strlen( optarg ) )
                    {
                        pParam->importFile = strdup(optarg);
                    }
                    break;
                case 'i':
                    pParam->interactive = true;
                    break;
                case 'n':
                    pParam->noStore = true;
                    break;
                case '?':
                    help();
                    break;
                case -1:
                    break;
                default:
                    fprintf(stderr, "Invalid option %c! \n", next_option);
                    help();
            }
        } while (next_option != -1);
    }

}


int main( int argc, char *argv[] )
{
    int retVal = 0;
    bolus *pNewBolus;
    time_t now;
    struct tm *pActual;
    struct _bolus_param bParam;

    if( (pNewBolus = new(bolus)) != NULL )
    {
        now = time(NULL);
        pActual = localtime(&now);

        get_arguments ( argc, argv, &bParam );

        if( (retVal = pNewBolus->init( &bParam )) == E_BOLUS_OK )
        {
            if( pNewBolus->getMode() != BOLUS_NO_MODE )
            {
                retVal = pNewBolus->run();
            }
            pNewBolus->end( );
        }

//        retVal = pNewBolus->use( pActual->tm_year+1900, pActual->tm_mon+1 );

    }

    return( retVal );
}


