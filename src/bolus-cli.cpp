/*
 ***********************************************************************
 *
 *  bolus-cli.cpp - command line Bolus-Rechner
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
 * Kommandozeilen-Argumente:
 *
 * ---------------------- Korrektur-Faktor -----------------------------
 *
 * --adjustType <Korrektur-Faktor> (oder -a <Korrektur-Faktor>)
 *
 * default: 0
 *
 * ----------------------- Blutzucker-Wert -----------------------------
 *
 * --glucose <BZ-Wert>    (oder -g <BZ-Wert>)
 *
 * default: --
 *

--carb <Kohlenhydrate>       (oder -c <Kohlenhydrate>)
--bread <Broteinheiten>      (oder -b <Broteinheiten>)
--meal <Essenszeit>       (oder -m <Essenszeit>)
--type <Messgerät>       (oder -t <Messgerät>)
--last (oder -l)
--offset <Wert> (oder -o <Wert>)

--adjustType <wert> (oder -a <wert>)
#define DATA_MEAL_BEFORE           'b'
#define DATA_MEAL_U_BEFORE         'B'
#define DATA_MEAL_AFTER            'a'
#define DATA_MEAL_U_AFTER          'A'
#define DATA_MEAL_NONE             'n'
#define DATA_MEAL_U_NONE           'N'
#define DATA_MEAL_SLEEPTIME        's'
#define DATA_MEAL_U_SLEEPTIME      'S'
#define DATA_MEAL_EXTRA            'x'
#define DATA_MEAL_U_EXTRA          'X'

--export <Dateiname> (oder -E <Dateiname>)
--exporttype <type> (oder -e <type> )
#define DATA_TIMEBLOCKS     'T'
#define DATA_GLOBALS        'G'
#define DATA_ADJUSTMENTS    'A'
#define DATA_RECORDS        'R'
#define DATA_DEVICE         'D'

--import <Dateiname> (oder -I <Dateiname>)
--importtype <type> (oder -i <type>)
#define DATA_TIMEBLOCKS     'T'
#define DATA_GLOBALS        'G'
#define DATA_ADJUSTMENTS    'A'
#define DATA_RECORDS        'R'
#define DATA_DEVICE         'D'



--query <type> (oder -q <type> )
type is
#define DATA_TIMEBLOCKS     'T'
#define DATA_GLOBALS        'G'
#define DATA_ADJUSTMENTS    'A'
#define DATA_RECORDS        'R'
#define DATA_DEVICE         'D'
#define QUERY_GLUCOSE_STATUS 'g'

--timeblock <# oder Nummer> (oder -T <# oder Nummer>)
--recordfile <filename> (oder -R <filename> )
--record <Recordnummer> (oder -r <Recordnummer> )

--xtra (oder -x)
--delim <Begrenzer> (oder -d <Begrenzer>)

--nostore (oder -n)
--help (oder -h)
--debug (oder -D)
--verbose <Level> (oder -v <Level>)



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

/* ----------------------------------------------------------------------------
 * function:
 *     void help( void )
 * does:
 *     display a help screen
 * returns:
 *     nothing
 ------------------------------------------------------------------------------
*/
void help( void )
{
    fprintf(stderr, "HELP!\n");
    exit(0);
}

/* ----------------------------------------------------------------------------
 * function:
 *     void dumpArgs( struct _bolus_param *pParam )
 * does:
 *     display options given in pointer
 * returns:
 *     nothing
 ------------------------------------------------------------------------------
*/
void dumpArgs( struct _bolus_param *pParam )
{
    if( pParam != NULL )
    {
        fprintf(stderr, "fail ...............: %s\n", 
                pParam->fail == true ? "true" : "false" );
        fprintf(stderr, "offset .............: %d\n", pParam->offset );
        fprintf(stderr, "glucose ............: %d\n", pParam->glucose );
        fprintf(stderr, "carb ...............: %d\n", pParam->carb );
        fprintf(stderr, "bread ..............: %.2f\n", pParam->bread );
        fprintf(stderr, "mealType ...........: %c\n", pParam->mealType );
        fprintf(stderr, "measureType ........: %c\n", pParam->measType );
        fprintf(stderr, "adjustType .........: %c\n", pParam->adjustType );
        fprintf(stderr, "adjust .............: %d\n", pParam->adjust );
        fprintf(stderr, "last ...............: %s\n", 
                pParam->last == true ? "true" : "false" );
        fprintf(stderr, "exportType .........: %c\n", pParam->exportType );
        fprintf(stderr, "exportFile .........: %s\n", pParam->exportFile );
        fprintf(stderr, "importType .........: %c\n", pParam->importType );
        fprintf(stderr, "importFile .........: %s\n", pParam->importFile );
        fprintf(stderr, "nostore ............: %s\n", 
                pParam->noStore == true ? "true" : "false" );
        fprintf(stderr, "query only .........: %s\n", 
                pParam->query == true ? "true" : "false" );
        fprintf(stderr, "queryType ..........: %c\n", pParam->queryType );
        fprintf(stderr, "tmblk ..............: %d\n", pParam->timeBlockNumber);
        fprintf(stderr, "timeblock count ....: %s\n", 
                pParam->timeBlockCount == true ? "true" : "false" );
        fprintf(stderr, "delimiter ..........: %c\n", pParam->importDelimiter );
        fprintf(stderr, "1st line extra data : %s\n", 
                pParam->import1stLineXtraData == true ? "true" : "false" );
        fprintf(stderr, "debug mode .........: %s\n", 
                pParam->debugMode == true ? "true" : "false" );
        fprintf(stderr, "verbose level ......: %d\n", pParam->verboseLevel );
        fprintf(stderr, "datafile ...........: %s\n", pParam->dataFile );
        fprintf(stderr, "dump record ........: %d\n", pParam->dataRecord );
    }
}


/* ----------------------------------------------------------------------------
 * function:
 *     void resetArgs( struct _bolus_param *pParam )
 * does:
 *     set param pointed by pParm to their defaults
 * returns:
 *     nothing
 ------------------------------------------------------------------------------
*/
void resetArgs( struct _bolus_param *pParam )
{
    if( pParam != NULL )
    {
        pParam->fail        = false;
        pParam->offset      = 0;
        pParam->glucose     = 0;
        pParam->carb        = 0;
        pParam->bread       = 0.0;
        pParam->mealType    = DATA_MEAL_NONE;
        pParam->measType    = DATA_NOTHING;
        pParam->adjustType  = '-';
        pParam->adjust      = 0;
        pParam->last        = false;
        pParam->exportType    = DATA_NOTHING;
        pParam->exportFile  = NULL;
        pParam->importType    = DATA_NOTHING;
        pParam->importFile  = NULL;
        pParam->noStore = false;
        pParam->query = false;
        pParam->queryType = DATA_NOTHING;
        pParam->timeBlockNumber = -1;
        pParam->timeBlockCount = false;
        pParam->importDelimiter = DATA_DEFAULT_DELIMITER;
        pParam->import1stLineXtraData = false;
        pParam->debugMode = false;
        pParam->verboseLevel = 0;
        pParam->dataFile  = NULL;
        pParam->dataRecord = -1;

    }
}

/* ----------------------------------------------------------------------------
 * function:
 *     void get_arguments ( int argc, char **argv, struct _bolus_param *pParam )
 * does:
 *     scan command line arguments an set corresponding parameters pointed
 *     by pParam
 * returns:
 *     nothing
 ------------------------------------------------------------------------------
*/
void get_arguments ( int argc, char **argv, struct _bolus_param *pParam )
{

    int fail = 0;
    int next_option;
    /* valid short options letters */
    const char* const short_options = "g:c:b:m:t:lo:E:e:I:i:q:T:R:r:a:xd:nhDv:";

    if( pParam != NULL )
    {
        /* valid long options */
        const struct option long_options[] = {
             { "glucose",     1, NULL, 'g' },
             { "carb",        1, NULL, 'c' },
             { "bread",       1, NULL, 'b' },
             { "meal",        1, NULL, 'm' },
             { "type",        1, NULL, 't' },
             { "adjustType",  1, NULL, 'a' },
             { "last",        0, NULL, 'l' },
             { "offset",      1, NULL, 'o' },
             { "export",      1, NULL, 'E' },
             { "exporttype",  1, NULL, 'e' },
             { "import",      1, NULL, 'I' },
             { "importtype",  1, NULL, 'i' },
             { "query",       1, NULL, 'q' },
             { "timeblock",   1, NULL, 'T' },
             { "recordfile",  1, NULL, 'R' },
             { "record",      1, NULL, 'r' },
             { "xtra",        0, NULL, 'x' },
             { "delim",       1, NULL, 'd' },
             { "nostore",     0, NULL, 'n' },
             { "help",        0, NULL, 'h' },
             { "debug",       0, NULL, 'D' },
             { "verbose",     1, NULL, 'v' },
             { NULL,          0, NULL,  0  }
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
                    pParam->bread = atof(optarg);
                    break;
                case 'm':
                    pParam->mealType = optarg[0];
                    break;
                case 't':
                    pParam->measType = optarg[0];
                    break;
                case 'a':
                    switch( optarg[0] )
                    {
                        case DATA_ADJUST_NO_ENTRY:
                        case DATA_ADJUST_SOBER:
                        case DATA_ADJUST_U_SOBER:
                        case DATA_ADJUST_SPORT1:
                        case DATA_ADJUST_STRESS:
                        case DATA_ADJUST_U_STRESS:
                        case DATA_ADJUST_ILLNESS:
                        case DATA_ADJUST_U_ILLNESS:
                        case DATA_ADJUST_SPORT2:
                        case DATA_ADJUST_MENSTRUATION:
                        case DATA_ADJUST_U_MENSTRUATION:
                        case DATA_ADJUST_OTHER:
                        case DATA_ADJUST_U_OTHER:
                            pParam->adjustType = optarg[0];
                            break;
                        default:
                            fail = E_UNKNOWN_ADJUSTMENT;
                            fprintf(stderr, "Unknown adjustment %c!\n", optarg[0] );
                            break;
                    }
                    break;
                case 'l':
                    pParam->last = true;
                    break;
                case 'o':
                    pParam->offset = atoi(optarg);
                    break;
                case 'E':
                    if( strlen( optarg ) )
                    {
                        pParam->exportFile = strdup(optarg);
                    }
                    break;
                case 'e':
                    switch( optarg[0] )
                    {
                        case EXPORT_TIMEBLOCKS:
                        case EXPORT_GLOBALS:
                        case EXPORT_ADJUSTMENTS:
                        case EXPORT_RECORDS:
                        case EXPORT_DEVICE:
                            pParam->exportType = optarg[0];
                            break;
                        default:
                            fail = E_UNKNOWN_EXPORT_TYPE;
                            fprintf(stderr, "Unknown export %c!\n", optarg[0] );
                            break;
                    }
                    break;
                case 'I':
                    if( strlen( optarg ) )
                    {
                        pParam->importFile = strdup(optarg);
                    }
                    break;
                case 'i':
                    switch( optarg[0] )
                    {
                        case IMPORT_TIMEBLOCKS:
                        case IMPORT_GLOBALS:
                        case IMPORT_ADJUSTMENTS:
                        case IMPORT_RECORDS:
                        case IMPORT_DEVICE:
                            pParam->importType = optarg[0];
                            break;
                        default:
                            fail = E_UNKNOWN_IMPORT_TYPE;
                            fprintf(stderr, "Unknown import %c!\n", optarg[0] );
                            break;
                    }
                    break;
                case 'q':
                    switch( optarg[0] )
                    {
                        case QUERY_TIMEBLOCKS:
                        case QUERY_GLOBALS:
                        case QUERY_ADJUSTMENTS:
                        case QUERY_RECORDS:
                        case QUERY_DEVICE:
                        case QUERY_GLUCOSE_STATUS:
                            pParam->query = true;
                            pParam->queryType = optarg[0];
                            break;
                        default:
                            fail = E_UNKNOWN_QUERY_TYPE;
                            fprintf(stderr, "Unknown query %c!\n", optarg[0] );
                            break;
                    }
                    break;
                case 'T':
                    if( optarg[0] == '#' )
                    {
                        pParam->timeBlockCount = true;
                        pParam->timeBlockNumber = -1;
                    }
                    else
                    {
                        pParam->timeBlockCount = false;
                        pParam->timeBlockNumber = atoi(optarg);
                    }
                    break;
                case 'R':
                    if( strlen( optarg ) )
                    {
                        pParam->dataFile = strdup(optarg);
                    }
                    break;
                case 'r':
                    pParam->dataRecord = atoi(optarg);
                    break;
                case 'x':
                    pParam->import1stLineXtraData = true;
                    break;
                case 'd':
                    pParam->importDelimiter = optarg[0];
                    break;
                case 'n':
                    pParam->noStore = true;
                    break;
                case 'h':
                    help();
                    break;
                case 'D':
                    pParam->debugMode = true;
                    break;
                case 'v':
                    pParam->verboseLevel = atoi(optarg);
                    break;
                case -1:
                    break;
                default:
                    fprintf(stderr, "Invalid option %c! \n", next_option);
                    help();
            }
        } while( !fail && next_option != -1);
    }

}


/* ----------------------------------------------------------------------------
 * function:
 *     int main( int argc, char *argv[] )
 * does:
 *     main function of the CLI
 * returns:
 *     an error code if an error occured, otherwise, depending on the
 *     executed function, 0 or an appropriate value
 ------------------------------------------------------------------------------
*/
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

        if( bParam.debugMode )
        {
  	    dumpArgs( &bParam );
        }

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


