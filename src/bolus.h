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

#define E_BOLUS_OK              0
#define E_BOLUS_SETTINGS       -1
#define E_BOLUS_INIT           -2
#define E_BOLUS_DATAFILE       -3



class bolus {

  protected:

  public:
    settings *pSettings;
    datafile *pDatafile;

  private:
      bool initialized = false;

  public:
      bolus( void ) { 
             pSettings = new settings(); 
             pDatafile = new datafile();
      };
      int init( void );
      int end( void );
};


#ifdef __cplusplus
}
#endif

#endif // _BOLUS_H_

