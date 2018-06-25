/*
 ***********************************************************************
 *
 *  datafile.h - class for re/store settings
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

#ifndef _DATAFILE_H_
#define _DATAFILE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>

#include "settings.h"

#ifdef __cplusplus
extern "C" {
#endif
 
using namespace std;

#define DATA_FILE_NAME         "%s/%s/bolus.data.%04d.%02d"

#define E_DATAFILE_OK           0
#define E_DATAFILE_SETTINGS    -1
#define E_DATAFILE_INIT        -2
#define E_DATAFILE_ENV         -3
#define E_DATAFILE_ACCESS      -4
#define E_DATAFILE_ROFS        -5
#define E_DATAFILE_PATH        -6
#define E_DATAFILE_FLAGS       -7
#define E_DATAFILE_UNKNOWN     -8
#define E_DATAFILE_INV_REC     -9
#define E_DATAFILE_POSITION   -10
#define E_DATAFILE_NULL       -11
#define E_DATAFILE_EMPTY      -12
#define E_DATAFILE_RECNO      -13
#define E_DATAFILE_READ       -14
#define E_DATAFILE_WRITE      -15


#define DATA_RECORD_LENGTH     41

#define DATA_RECORD_NUM_FIELDS  9


struct _record {
    time_t timestamp;
    unsigned long recnum;
    int glucose;
    char meal;
    int carbon10; 
    int adjust;
    int units;
    int basalUnits;
    int type;
};


class datafile {

  protected:
      int dataFd = -1;
      char dataFileName[PATH_MAX];
      int currYear;
      int currMonth;

  public:

  private:
      bool initialized = false;
      int performRead( struct _record *pData );
      int performWrite( struct _record *pData );

  public:
      datafile( void ) { };
      int init( void );
      int use( int year, int month );
      int readLastRecord( unsigned long *recno, struct _record *pData );
      int readRecord( unsigned long recno, struct _record *pData );
      int writeRecord( unsigned long recno, struct _record *pData );
      int appendRecord( unsigned long *recno, struct _record *pData );

};


#ifdef __cplusplus
}
#endif

#endif // _DATAFILE_H_

