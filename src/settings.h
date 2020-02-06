/*
 ***********************************************************************
 *
 *  settings.h - class definition
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

#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <uuid/uuid.h> // needs uuid-dev

#ifdef __cplusplus
extern "C" {
#endif
 
using namespace std;

#define SETTINGS_FILE_NAME     ".bolus.settings.bin"
#define SETTINGS_BACKUP_NAME   "bolus.settings.backup"
#define BOLUS_BASE_DIR         "bolus"

#define MAX_TIME_BLOCKS         5
#define NUM_TIME_BLOCKS         5


#define TIME_BLOCK_REC_FMT      "%c:%02d:%04d:%03d:%03d:%02d:%03d:%u\n"
#define TIME_BLOCK_REC_ID       't'
#define TIME_BLOCK_REC_LEN     24
#define TIME_BLOCK_REC_ITEMS    8

#define ADJUSTMENTS_REC_FMT     "%c:%d:%d:%d:%d:%d:%d:%d:%u\n"
#define ADJUSTMENTS_REC_ID      'a'
#define ADJUSTMENTS_REC_ITEMS   9

#define GLOBALS_REC_FMT         "%c:%u:%d:%d:%d:%d:%d:%d:%d:%u\n"
#define GLOBALS_REC_ID          'g'
#define GLOBALS_REC_ITEMS      10

#define DEVICE_ID_REC_FMT       "%c:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%u\n"
#define DEVICE_ID_REC_ID        'd'
#define DEVICE_ID_REC_ITEMS    18
//
// timeblock defaults
//
#define TMBLCK0_TIME            0
#define TMBLCK0_TGT_FROM      100
#define TMBLCK0_TGT_TO        180
#define TMBLCK0_U210BE         12
#define TMBLCK0_SENSITIVITY    70
//
#define TMBLCK1_TIME          360
#define TMBLCK1_TGT_FROM       80
#define TMBLCK1_TGT_TO        160
#define TMBLCK1_U210BE         13
#define TMBLCK1_SENSITIVITY    70
//
#define TMBLCK2_TIME          660
#define TMBLCK2_TGT_FROM       80
#define TMBLCK2_TGT_TO        160
#define TMBLCK2_U210BE         13
#define TMBLCK2_SENSITIVITY    70
//
#define TMBLCK3_TIME          960
#define TMBLCK3_TGT_FROM       80
#define TMBLCK3_TGT_TO        160
#define TMBLCK3_U210BE         15
#define TMBLCK3_SENSITIVITY    75
//
#define TMBLCK4_TIME         1320
#define TMBLCK4_TGT_FROM      100
#define TMBLCK4_TGT_TO        180
#define TMBLCK4_U210BE         14
#define TMBLCK4_SENSITIVITY    75
//
#define DEFAULT_ADJUST_NO_ENTRY      0
#define DEFAULT_ADJUST_SOBER         0
#define DEFAULT_ADJUST_SPORT1       -10
#define DEFAULT_ADJUST_STRESS        25
#define DEFAULT_ADJUST_ILLNESS       10
#define DEFAULT_ADJUST_SPORT2       -30
#define DEFAULT_ADJUST_MENSTRUATION  0
#define DEFAULT_ADJUST_OTHER         0
//
#define MAJOR_VERSION           0
#define MINOR_VERSION           0
#define PATCHLEVEL              0
#define ACTIVE_TIMEBLOCKS       NUM_TIME_BLOCKS
#define INCREASE_LEVEL         50
#define SNACKSIZE_10_BE        10

#define ACT_TIME              180
#define DELAY_TIME             90
#define BASAL_ACT_TIME       1380
#define BASAL_DELAY_TIME       30

#define E_SETTINGS_OK           0
#define E_SETTINGS_ENV         -1
#define E_SETTINGS_ACCESS      -2
#define E_SETTINGS_ROFS        -3
#define E_SETTINGS_PATH        -4
#define E_SETTINGS_FLAGS       -5
#define E_SETTINGS_UNKNOWN     -6
#define E_SETTINGS_CRC         -7
#define E_SETTINGS_INV_REC     -8

#define E_SETTINGS_GETENV     -10
#define E_SETTINGS_INVAL      -11
#define E_SETTINGS_FILE       -12

#define MAX_SETTINGS_RECLEN   128
#define DEV_ID_LEN             16
            
struct _timeblk {
    int num;
    int time;
    int rangeFrom;
    int rangeTo;
    int uTo10BE;
    int sens;
};

struct _adjust {
    int sober;
    int sport1;
    int stress;
    int illness;
    int sport2;
    int menstruation;
    int other;
};


struct _globals {
    unsigned int version;
    int timeBlocksActive;
    int increaseLevel;
    int snacksize10BE;
    int actTime;
    int delayTime;
    int basalActTime;
    int basalDelayTime;
};

class settings {

  protected:
      // **********************************************************************
      // CRC lookup table
      // **********************************************************************
      //
      uint32_t crc_table[16] = {
          0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
          0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
          0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
          0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
      };


      FILE *settingsFile;
      char settingsFileName[PATH_MAX];
      char settingsBuffer[MAX_SETTINGS_RECLEN];
      uint8_t devId[DEV_ID_LEN];
      bool _modified = false;
      bool _new = false;
      bool _cancel = false;

  public:
    struct _timeblk timeblock[MAX_TIME_BLOCKS];
    struct _adjust  adjustments;
    struct _globals globals;

  private:
      //
      // **********************************************************************
      // CRC calculation e.g. over a buffer
      // **********************************************************************
      //
      uint32_t crc( char buf[], int length )
      {
          uint32_t crc = ~0L;

          for (int index = 0; index < length; index++) 
          {
              crc = crc_table[(crc ^ buf[index]) & 0x0f] ^ (crc >> 4);
              crc = crc_table[((crc ^ buf[index]) >> 4) & 0x0f] ^ (crc >> 4);
              crc = ~crc;
            }
            return crc;
      }

      void defaults( void );
      int open( void );
      void close( void );
      int writeTimeblocks( void );
      int readTimeblocks( void );
      int writeGlobals( void );
      int readGlobals( void );
      int writeAdjustments( void );
      int readAdjustments( void );
      int writeDeviceId( void );
      int readDeviceId( void );
      void dumpDeviceId( void );
      void dumpAdjustments( void );
      void dumpGlobals( void );
      void dumpTimeblocks( void );
      void dump( void );
      int read( void );

  public:
      settings( void ) { settingsFile = (FILE*) NULL; };
      int init( void );
      int end( void );
      int write( void );
      int backup( void );
      bool newAdjustmentsValid( struct _adjust *pNewAdjustments );
      bool newGlobalsValid( struct _globals *pNewGlobals );
      int setNewAdjustments( struct _adjust *pNewAdjustments );
      int setNewGlobals( struct _globals *pNewGlobals );


};


#ifdef __cplusplus
}
#endif

#endif // _SETTINGS_H_

