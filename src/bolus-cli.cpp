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
--edit (oder -e)
--offset <Wert> (oder -o <Wert>)
--timeblock <# oder Nummer> (oder -T <# oder Nummer>)

--calibrate (oder -C)
--acucheck <Messwert> (oder -A <Messwert>)
--freestyle <Messwert> (oder -F <Messwert>)

--interactive (oder -i  no func
--nostore (oder -n)
--help (oder -h)

--export <Dateiname> (oder -X <Dateiname>)
--import <Dateiname> (oder -I <Dateiname>)
--query (oder -q)
--globals (oder -G)
--factors (oder -f)
--blocks (oder -B)
--device (oder -D)

--xtra (oder -x)
--delim <Begrenzer> (oder -d <Begrenzer>)

--debug (oder -V)
--verbose <Level> (oder -v <Level>)

--record <Recordnummer> (oder -R <Recordnummer> )


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
        fprintf(stderr, "fail ...............: %s\n", 
                pParam->fail == true ? "true" : "false" );
        fprintf(stderr, "glucose ............: %d\n", pParam->glucose );
        fprintf(stderr, "carb ...............: %d\n", pParam->carb );
        fprintf(stderr, "bread ..............: %.2f\n", pParam->bread );
        fprintf(stderr, "meal ...............: %c\n", pParam->mealType );
        fprintf(stderr, "measure ............: %c\n", pParam->measType );
        fprintf(stderr, "adjustType .........: %c\n", pParam->adjustType );
        fprintf(stderr, "adjust .............: %d\n", pParam->adjust );
        fprintf(stderr, "last ...............: %s\n", 
                pParam->last == true ? "true" : "false" );
        fprintf(stderr, "edit ...............: %c\n", pParam->editType );
        fprintf(stderr, "export .............: %s\n", pParam->exportFile );
        fprintf(stderr, "import .............: %s\n", pParam->importFile );
        fprintf(stderr, "interact ...........: %s\n", 
                pParam->interactive == true ? "true" : "false" );
        fprintf(stderr, "nostore ............: %s\n", 
                pParam->noStore == true ? "true" : "false" );
        fprintf(stderr, "offset .............: %d\n", pParam->offset );

        fprintf(stderr, "query only .........: %s\n", 
                pParam->query == true ? "true" : "false" );
        fprintf(stderr, "timeblock count ....: %s\n", 
                pParam->timeBlockCount == true ? "true" : "false" );
        fprintf(stderr, "tmblk ..............: %d\n", pParam->timeBlockNumber);

        fprintf(stderr, "calibrate ..........: %s\n", 
                pParam->calibrate == true ? "true" : "false" );
        fprintf(stderr, "acucheck ...........: %d\n", pParam->acucheckValue );
        fprintf(stderr, "freestyle ..........: %d\n", pParam->freestyleValue );

        fprintf(stderr, "query factors ......: %s\n", 
                pParam->qFactors == true ? "true" : "false" );
        fprintf(stderr, "query globals ......: %s\n", 
                pParam->qGlobals == true ? "true" : "false" );
        fprintf(stderr, "blocks flag ........: %s\n", 
                pParam->qBlocks == true ? "true" : "false" );
        fprintf(stderr, "device flag ........: %s\n", 
                pParam->device == true ? "true" : "false" );

        fprintf(stderr, "delimiter ..........: %c\n", pParam->importDelimiter );
        fprintf(stderr, "1st line extra data : %s\n", 
                pParam->import1stLineXtraData == true ? "true" : "false" );

        fprintf(stderr, "debug mode .........: %s\n", 
                pParam->debugMode == true ? "true" : "false" );
        fprintf(stderr, "verbose level ......: %d\n", pParam->verboseLevel );

        fprintf(stderr, "dump record ........: %d\n", pParam->dataRecord );

        fprintf(stderr, "datafile ...........: %s\n", pParam->dataFile );
    }
}


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
        pParam->measType    = '\0';
        pParam->adjustType  = '\0';
        pParam->adjust      = 0;
        pParam->last        = false;
        pParam->editType    = '\0';
        pParam->exportFile  = NULL;
        pParam->importFile  = NULL;
        pParam->interactive = false;
        pParam->noStore = false;
        pParam->query = false;
        pParam->timeBlockNumber = -1;
        pParam->timeBlockCount = false;
        pParam->calibrate = false;
        pParam->acucheckValue = -1;
        pParam->freestyleValue = -1;
        pParam->qFactors = false;
        pParam->qGlobals = false;
        pParam->qBlocks = false;
        pParam->device = false;
        pParam->importDelimiter = ';';
        pParam->import1stLineXtraData = false;
        pParam->debugMode = false;
        pParam->verboseLevel = 0;
        pParam->dataFile  = NULL;
        pParam->dataRecord = -1;

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
    const char* const short_options = "a:g:c:b:m:t:le:X:I:o:T:F:A:d:v:R:r:VxDBGfCinhq?";

    if( pParam != NULL )
    {

        /* valid long options */
        const struct option long_options[] = {
             { "adjustType",  1, NULL, 'a' },
             { "glucose",     1, NULL, 'g' },
             { "carb",        1, NULL, 'c' },
             { "bread",       1, NULL, 'b' },
             { "meal",        1, NULL, 'm' },
             { "type",        1, NULL, 't' },
             { "last",        0, NULL, 'l' },
             { "edit",        1, NULL, 'e' },
             { "export",      1, NULL, 'X' },
             { "import",      1, NULL, 'I' },
             { "offset",      1, NULL, 'o' },
             { "timeblock",   1, NULL, 'T' },

             { "calibrate",   1, NULL, 'C' },
             { "acucheck",    1, NULL, 'A' },
             { "freestyle",   1, NULL, 'F' },

             { "interactive", 0, NULL, 'i' },
             { "nostore",     0, NULL, 'n' },
             { "help",        0, NULL, 'h' },
             { "query",       0, NULL, 'q' },

             { "globals",     0, NULL, 'G' },
             { "factors",     0, NULL, 'f' },
             { "blocks",      0, NULL, 'B' },
             { "device",      0, NULL, 'D' },

             { "xtra",        0, NULL, 'x' },
             { "delim",       1, NULL, 'd' },

             { "debug",       0, NULL, 'V' },
             { "verbose",     1, NULL, 'v' },

             { "datafile",    1, NULL, 'R' },
             { "record",      1, NULL, 'r' },

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
                    pParam->bread = atof(optarg);
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
                    switch( pParam->adjustType = optarg[0] )
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
// gParam.adjustType = '-';
// gParam.adjustType = 'n';
// gParam.adjustType = '1';
// gParam.adjustType = 's';
// gParam.adjustType = 'i';
// gParam.adjustType = '2';
// gParam.adjustType = 'f';
// gParam.adjustType = 'o';
// ("Kein Eintrag");
// ("Nüchtern");
// ("Sport 1");
// ("Stress");
// ("Krankheit");
// ("Sport 2");
// ("Menstruation");
// ("Andere");

                    }
                    break;
                case 'l':
                    pParam->last = true;
                    break;
                case 'e':
                    switch( optarg[0] )
                    {
                        case DATA_EDIT_TYPE_TIMEBLOCKS:
                        case DATA_EDIT_TYPE_GLOBALS:
                        case DATA_EDIT_TYPE_ADJUSTMENTS:
                            pParam->editType = optarg[0];
                            break;
                        default:
                            pParam->editType = DATA_EDIT_TYPE_NO_TYPE;
                            break;
                    }
                    break;
                case 'X':
                    if( strlen( optarg ) )
                    {
                        pParam->exportFile = strdup(optarg);
                    }
                    break;
                case 'I':
                    if( strlen( optarg ) )
                    {
                        pParam->importFile = strdup(optarg);
                    }
                    break;
                case 'o':
                    pParam->offset = atoi(optarg);
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
                case 'i':
                    pParam->interactive = true;
                    break;
                case 'n':
                    pParam->noStore = true;
                    break;
                case '?':
                    help();
                    break;
                case 'q':
                    pParam->query = true;
                    break;

                case 'C':
                    pParam->calibrate = true;
                    break;
                case 'A':
                    pParam->acucheckValue = atoi(optarg);
                    break;
                case 'F':
                    pParam->freestyleValue = atoi(optarg);
                    break;

                case 'f':
                    pParam->qFactors = true;
                    break;
                case 'G':
                    pParam->qGlobals = true;
                    break;
                case 'B':
                    pParam->qBlocks = true;
                    break;
                case 'D':
                    pParam->device = true;
                    break;

                case 'd':
                    pParam->importDelimiter = optarg[0];
                    break;
                case 'x':
                    pParam->import1stLineXtraData = true;
                    break;
                case 'V':
                    pParam->debugMode = true;
                    break;
                case 'v':
                    pParam->verboseLevel = atoi(optarg);
                    break;

                case 'r':
                    pParam->dataRecord = atoi(optarg);
                    break;
                case 'R':
                    if( strlen( optarg ) )
                    {
                        pParam->dataFile = strdup(optarg);
                    }
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

#ifdef NODEF
QString str = "a,,b,c";

QStringList list1 = str.split(',');
// list1: [ "a", "", "b", "c" ]

QStringList list2 = str.split(',', QString::SkipEmptyParts);
// list2: [ "a", "b", "c" ]

#include <QList>
#include <QStringList>
#include <QDir>
#include <QDebug>

#include "qtcsv/stringdata.h"
#include "qtcsv/reader.h"
#include "qtcsv/writer.h"

int main()
{
    // prepare data that you want to save to csv-file
    QStringList strList;
    strList << "one" << "two" << "three";

    QtCSV::StringData strData;
    strData.addRow(strList);
    strData.addEmptyRow();
    strData << strList << "this is the last row";

    // write to file
    QString filePath = QDir::currentPath() + "/test.csv";
    QtCSV::Writer::write(filePath, strData);

    // read data from file
    QList<QStringList> readData = QtCSV::Reader::readToList(filePath);
    for ( int i = 0; i < readData.size(); ++i )
    {
        qDebug() << readData.at(i).join(",");
    }

    return 0;
}





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

// -t a --type=(a)cucheck
// -t f --type=(f)reestyle

// -a 1  sport 1
// -a 2  sport 2
// -a s  stress
// -a S  stress
// -a i  krank
// -a I  krank
// -a f   mens
// -a F   mens


#endif // NODEF

