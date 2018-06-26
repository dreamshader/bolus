/*
 ***********************************************************************
 *
 *  bolus.h - class for re/store settings
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

#ifdef __cplusplus
extern "C" {
#endif
 
using namespace std;

#define W_BOLUS_IDLE            1
#define E_BOLUS_OK              0
#define E_BOLUS_SETTINGS       -1
#define E_BOLUS_INIT           -2
#define E_BOLUS_DATAFILE       -3
#define E_BOLUS_CARB_N_BREAD   -4
#define E_BOLUS_RUN_MODE       -5
#define E_BOLUS_NULL           -6
#define E_BOLUS_TIMEBLK        -7


#define BOLUS_NO_MODE           0
#define BOLUS_IMPORT_MODE       1
#define BOLUS_EXPORT_MODE       2
#define BOLUS_INTERACTIVE_MODE  3
#define BOLUS_LIST_MODE         4
#define BOLUS_CALC_BREAD_MODE   5
#define BOLUS_CALC_CARB_MODE    6
#define BOLUS_EDIT_MODE         7

#define SECONDS_A_MINUTE       60
#define SECONDS_A_HOUR         (60 * SECONDS_A_MINUTE)
#define SECONDS_A_DAY          (24 * SECONDS_A_HOUR)

#define BOLUS_FACTOR_CARB2BREAD 12

#define BOLUS_EDIT_CMD_AA       'a'
#define BOLUS_EDIT_CMD_BB       'b'
#define BOLUS_EDIT_CMD_ENDEDIT  'x'
#define BOLUS_EDIT_CMD_WRITE    'w'
#define BOLUS_EDIT_CMD_READ     'r'
#define BOLUS_EDIT_CMD_REJECT   'c'
#define BOLUS_EDIT_CMD_INVAL    '!'
#define BOLUS_EDIT_CMD_HELP     '?'

#define BOLUS_INTERACT_NONE     '\0'
#define BOLUS_INTERACT_SKIP     's'
#define BOLUS_INTERACT_BACK     'b'
#define BOLUS_INTERACT_REJECT   'c'
#define BOLUS_INTERACT_INVAL    '!'
#define BOLUS_INTERACT_HELP     '?'
#define BOLUS_INTERACT_YES_DE   'j'
#define BOLUS_INTERACT_NO       'n'
#define BOLUS_INTERACT_YES_EN   'y'

#define BOLUS_INTERACT_U_YES_DE 'J'
#define BOLUS_INTERACT_U_NO     'N'
#define BOLUS_INTERACT_U_YES_EN 'Y'

#define BOLUS_FIELD_TIMESTAMP   0
#define BOLUS_FIELD_RECNUM      1
#define BOLUS_FIELD_GLUCOSE     2
#define BOLUS_FIELD_MEAL        3
#define BOLUS_FIELD_CARBON      4
#define BOLUS_FIELD_ADJUST      5
#define BOLUS_FIELD_UNITS       6
#define BOLUS_FIELD_BASALUNITS  7
#define BOLUS_FIELD_TYPE        8
#define BOLUS_FIELD_ACTUNITS    9
#define BOLUS_FIELD_ACTBASUNITS 10
#define BOLUS_FIELD_ASK_YESNO   11

#define BOLUS_COMMAND_BUFSIZE 128
#define BOLUS_DIALOG_BUFSIZE  128

struct _bolus_param {
    bool fail;
    int  glucose;
    int  carb;
    int  bread;
    char mealType;
    char measType;
    int adjust;
    bool last;
    char editType;
    char exportType;
    char *importFile;
    bool interactive;
    bool noStore;
    bool offset;
};


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

};


#ifdef __cplusplus
}
#endif

#endif // _BOLUS_H_

