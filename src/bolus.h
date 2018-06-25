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


#define BOLUS_NO_MODE           0
#define BOLUS_IMPORT_MODE       1
#define BOLUS_EXPORT_MODE       2
#define BOLUS_INTERACTIVE_MODE  3
#define BOLUS_LIST_MODE         4
#define BOLUS_CALC_BREAD_MODE   5
#define BOLUS_CALC_CARB_MODE    6
#define BOLUS_EDIT_MODE         7


struct _bolus_param {
    bool fail;
    int  glucose;
    int  carb;
    int  bread;
    char mealType;
    char measType;
    bool last;
    char editType;
    char exportType;
    char *importFile;
    bool interactive;
    bool noStore;
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
      int runEsport( void );
      int runInteractive( void );
      int runList( void );
      int runCalcBread( void );
      int runCalcCarb( void );
      int runEdit( void );

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

