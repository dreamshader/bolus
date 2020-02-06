/*
 ***********************************************************************
 *
 *  bolus.h - class for bolus calculator
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
 */

#ifndef _BOLUS_H_
#define _BOLUS_H_

#ifndef IS_FRONTEND // set by the GUI source

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <ctype.h>

#include "settings.h"
#include "datafile.h"

#endif // IS_FRONTEND

#ifdef __cplusplus
extern "C" {
#endif
 
using namespace std;

#ifndef IS_FRONTEND // set by the GUI source

#define W_BOLUS_IDLE            1
#define E_BOLUS_OK              0
#define E_BOLUS_SETTINGS       -1
#define E_BOLUS_INIT           -2
#define E_BOLUS_DATAFILE       -3
#define E_BOLUS_CARB_N_BREAD   -4
#define E_BOLUS_RUN_MODE       -5
#define E_BOLUS_NULL           -6
#define E_BOLUS_TIMEBLK        -7

#define E_UNKNOWN_ADJUSTMENT  -10
#define E_UNKNOWN_IMPORT_TYPE -11
#define E_UNKNOWN_EXPORT_TYPE -12
#define E_UNKNOWN_QUERY_TYPE  -13

#define E_BOLUS_IMPORT_OPEN   -14
#define E_BOLUS_IMPORT_FIELDS -15

#define BOLUS_NO_MODE           0
#define BOLUS_IMPORT_MODE       1
#define BOLUS_EXPORT_MODE       2
#define BOLUS_LIST_MODE         3
#define BOLUS_CALC_BREAD_MODE   4
#define BOLUS_CALC_CARB_MODE    5
#define BOLUS_QUERY_MODE        6
#define BOLUS_DUMP_MODE         7

#define DATA_TIMEBLOCKS     'T'
#define DATA_GLOBALS        'G'
#define DATA_ADJUSTMENTS    'A'
#define DATA_RECORDS        'R'
#define DATA_DEVICE         'D'

#define IMPORT_TIMEBLOCKS    DATA_TIMEBLOCKS
#define IMPORT_GLOBALS       DATA_GLOBALS
#define IMPORT_ADJUSTMENTS   DATA_ADJUSTMENTS
#define IMPORT_RECORDS       DATA_RECORDS
#define IMPORT_DEVICE        DATA_DEVICE

#define EXPORT_TIMEBLOCKS    DATA_TIMEBLOCKS
#define EXPORT_GLOBALS       DATA_GLOBALS
#define EXPORT_ADJUSTMENTS   DATA_ADJUSTMENTS
#define EXPORT_RECORDS       DATA_RECORDS
#define EXPORT_DEVICE        DATA_DEVICE

#define QUERY_TIMEBLOCKS     DATA_TIMEBLOCKS
#define QUERY_GLOBALS        DATA_GLOBALS
#define QUERY_ADJUSTMENTS    DATA_ADJUSTMENTS
#define QUERY_RECORDS        DATA_RECORDS
#define QUERY_DEVICE         DATA_DEVICE
#define QUERY_GLUCOSE_STATUS 's'

#define SECONDS_A_MINUTE       60
#define SECONDS_A_HOUR         (60 * SECONDS_A_MINUTE)
#define SECONDS_A_DAY          (24 * SECONDS_A_HOUR)

#define BOLUS_FACTOR_CARB2BREAD 12

#endif // IS_FRONTEND

struct _bolus_param {
    bool fail;
    int offset;
    int  glucose;
    int  carb;
    double  bread;
    char mealType;
    char measType;
    char adjustType;
    int adjust;
    bool last;
    char exportType;
    char *exportFile;
    char importType;
    char *importFile;
    bool noStore;
    bool query;
    char queryType;
    int timeBlockNumber;
    bool timeBlockCount;
    char importDelimiter;
    bool import1stLineXtraData;
    bool debugMode;
    int verboseLevel;
    char *dataFile;
    int dataRecord;
};

#ifndef IS_FRONTEND // set by the GUI source

class bolus {

  private:
      bool initialized = false;
      int mode = BOLUS_NO_MODE;

  public:
    settings *pSettings;
    datafile *pDatafile;
    struct _bolus_param callerArgs;

  protected:
      void dumpArgs( void );
      void resetArgs( void );
      void setArgs( struct _bolus_param *pParam );
      int checkArgs( void );
      int runImport( void );
      int runExport( void );
      int runInteractive( FILE *pIn, FILE *pOut );
      int runListLast( void );
      int runCalcBread( void );
      int runCalcCarb( void );
      int runEditor( void );
      int runQuery( void );
      int runDumpData( void );
      int runCalibrate( void );

      int calcBolus( int timeBlk, struct _record *pLastData, struct _record *pNewData );

  public:
      bolus( void ) { 
             callerArgs.importFile = NULL;
             mode = BOLUS_NO_MODE;
       };

      ~bolus( void ) { 
             if( pSettings != NULL )
             {
                 pSettings->end();
                 delete pSettings;
             }
             if( pDatafile != NULL )
             {
                 delete pDatafile;
             }
             if( callerArgs.importFile != NULL )
             {
                 free( callerArgs.importFile );
                 callerArgs.importFile = NULL;
             }
      };

      int init( struct _bolus_param *pParam );
      int end( void );
      int run( void );
      int use( int year, int month );
      int getMode( void ) {
                  return mode;
      };
      int countTimeBlocks( void );


};

#endif // IS_FRONTEND

#ifdef __cplusplus
}
#endif

#endif // _BOLUS_H_

