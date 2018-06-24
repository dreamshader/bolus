/*
 ***********************************************************************
 *
 *  settings.cpp - class for re/store settings
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

#include "settings.h"

/* ----------------------------------------------------------------------------
 * int settings::open( void )
 *
 * open settings file
 ------------------------------------------------------------------------------
*/
int settings::open( void )
{
    int retVal = E_SETTINGS_OK;
    char *pHome;
    char *pMode;

    if( settingsFile == (FILE*) NULL )
    {
        if( (pHome = getenv("HOME")) != NULL )
        {
            sprintf(settingsFileName, "%s/%s/%s", pHome, BOLUS_BASE_DIR, 
                    SETTINGS_FILE_NAME);

            if( (retVal = access( settingsFileName , R_OK | W_OK )) < 0 )
            {
                switch( errno )
                {
                    case EACCES:
                        retVal = E_SETTINGS_ACCESS;
                        break;
                    case ENOENT:
                        pMode = (char*) "w+";
                        this->defaults();
                        retVal = E_SETTINGS_OK;
                        break;
                    case EROFS:
                        retVal = E_SETTINGS_ROFS;
                        break;
                    case ENOTDIR:
                        retVal = E_SETTINGS_PATH;
                        break;
                    case EINVAL:
                        retVal = E_SETTINGS_FLAGS;
                        break;
                    default:
                        retVal = E_SETTINGS_UNKNOWN;
                        break;
                }
            }
            else
            {
                pMode = (char*) "r+";
                retVal = E_SETTINGS_OK;
            }

            if( retVal == E_SETTINGS_OK )
            {
                if( (settingsFile = fopen(settingsFileName, pMode)) ==
                    (FILE*) NULL )
                {
                    retVal = E_SETTINGS_UNKNOWN;
                }
            }
        }
        else
        {
            retVal = E_SETTINGS_ENV;
        }
    }

    return( retVal );
}

/* ----------------------------------------------------------------------------
 * void settings::close( void )
 *
 * close settings file
 ------------------------------------------------------------------------------
*/
void settings::close( void )
{
    if( settingsFile != (FILE*) NULL )
    {
        fclose( settingsFile );
        settingsFile = (FILE*) NULL;
    }
}

/* ----------------------------------------------------------------------------
 * void settings::defaults( void )
 *
 * reset all settings to their defaults
 ------------------------------------------------------------------------------
*/
void settings::defaults( void )
{

//    uuid_generate_time(uuid_t devId);
    uuid_generate_time(devId);

    timeblock[0].num = 0;
    timeblock[0].hour = TMBLCK0_HOUR;
    timeblock[0].minute = TMBLCK0_MINUTE;
    timeblock[0].rangeFrom = TMBLCK0_TGT_FROM;
    timeblock[0].rangeTo = TMBLCK0_TGT_TO;
    timeblock[0].uTo10BE = TMBLCK0_U210BE;
    timeblock[0].sens = TMBLCK0_SENSITIVITY;

    timeblock[1].num = 1;
    timeblock[1].hour = TMBLCK1_HOUR;
    timeblock[1].minute = TMBLCK1_MINUTE;
    timeblock[1].rangeFrom = TMBLCK1_TGT_FROM;
    timeblock[1].rangeTo = TMBLCK1_TGT_TO;
    timeblock[1].uTo10BE = TMBLCK1_U210BE;
    timeblock[1].sens = TMBLCK1_SENSITIVITY;

    timeblock[2].num = 2;
    timeblock[2].hour = TMBLCK2_HOUR;
    timeblock[2].minute = TMBLCK2_MINUTE;
    timeblock[2].rangeFrom = TMBLCK2_TGT_FROM;
    timeblock[2].rangeTo = TMBLCK2_TGT_TO;
    timeblock[2].uTo10BE = TMBLCK2_U210BE;
    timeblock[2].sens = TMBLCK2_SENSITIVITY;

    timeblock[3].num = 3;
    timeblock[3].hour = TMBLCK3_HOUR;
    timeblock[3].minute = TMBLCK3_MINUTE;
    timeblock[3].rangeFrom = TMBLCK3_TGT_FROM;
    timeblock[3].rangeTo = TMBLCK3_TGT_TO;
    timeblock[3].uTo10BE = TMBLCK3_U210BE;
    timeblock[3].sens = TMBLCK3_SENSITIVITY;

    timeblock[4].num = 4;
    timeblock[4].hour = TMBLCK4_HOUR;
    timeblock[4].minute = TMBLCK4_MINUTE;
    timeblock[4].rangeFrom = TMBLCK4_TGT_FROM;
    timeblock[4].rangeTo = TMBLCK4_TGT_TO;
    timeblock[4].uTo10BE = TMBLCK4_U210BE;
    timeblock[4].sens = TMBLCK4_SENSITIVITY;


    globals.version = MAJOR_VERSION;
    globals.version = globals.version << 8;
    globals.version |= MINOR_VERSION;
    globals.version = globals.version << 8;
    globals.version |= PATCHLEVEL;

    globals.timeBlocksActive = ACTIVE_TIMEBLOCKS;
    globals.increaseLevel = INCREASE_LEVEL;
    globals.snacksize10BE = SNACKSIZE_10_BE;
    globals.actHours = ACT_HOURS;
    globals.actMinutes = ACT_MINUTES;
    globals.delayHours = DELAY_HOURS;
    globals.delayMinutes = DELAY_MINUTES;
    globals.basalActHours = BASAL_ACT_HOURS;
    globals.basalActMinutes = BASAL_ACT_MINUTES;
    globals.basalDelayHours = BASAL_DELAY_HOURS;
    globals.basalDelayMinutes = BASAL_DELAY_MINUTES;

    adjustments.sports1 = ADJUST_SPORTS_1;
    adjustments.sports2 = ADJUST_SPORTS_2;
    adjustments.stress = ADJUST_STRESS;
    adjustments.ill = ADJUST_ILL;
    adjustments.female = ADJUST_FEMALE;

}

/* ----------------------------------------------------------------------------
 * int settings::writeTimeblocks( void )
 *
 * store settings to file
 ------------------------------------------------------------------------------
*/
int settings::writeTimeblocks( void )
{
    int retVal;
    uint32_t recCrc = 0;

    if( (retVal = this->open()) == E_SETTINGS_OK )
    {
        for( int i = 0; i < MAX_TIME_BLOCKS; i++ )
        {
            recCrc = 0;
            sprintf( settingsBuffer, TIME_BLOCK_REC_FMT, TIME_BLOCK_REC_ID,
                 timeblock[i].num,
                 timeblock[i].hour,
                 timeblock[i].minute,
                 timeblock[i].rangeFrom,
                 timeblock[i].rangeTo,
                 timeblock[i].uTo10BE,
                 timeblock[i].sens,
                 recCrc );

            // calc crc with crc field = 0
            recCrc = this->crc( settingsBuffer, TIME_BLOCK_REC_LEN );

            fprintf( settingsFile, TIME_BLOCK_REC_FMT, TIME_BLOCK_REC_ID,
                 timeblock[i].num,
                 timeblock[i].hour,
                 timeblock[i].minute,
                 timeblock[i].rangeFrom,
                 timeblock[i].rangeTo,
                 timeblock[i].uTo10BE,
                 timeblock[i].sens,
                 recCrc );
        }
    }

    return( retVal );
}

/* ----------------------------------------------------------------------------
 * int settings::readTimeblocks( void )
 *
 * read stored settings from file
 ------------------------------------------------------------------------------
*/
int settings::readTimeblocks( void )
{
    int retVal = E_SETTINGS_OK;
    char recId;
    uint32_t recCrc = 0;
    uint32_t calcCrc = 0;

    if( (retVal = this->open()) == E_SETTINGS_OK )
    {
        for( int i = 0; retVal == E_SETTINGS_OK && i < MAX_TIME_BLOCKS; i++ )
        {
            retVal = fscanf( settingsFile, TIME_BLOCK_REC_FMT, &recId,
                 &timeblock[i].num,
                 &timeblock[i].hour,
                 &timeblock[i].minute,
                 &timeblock[i].rangeFrom,
                 &timeblock[i].rangeTo,
                 &timeblock[i].uTo10BE,
                 &timeblock[i].sens,
                 &recCrc );

            if( retVal == TIME_BLOCK_REC_ITEMS )
            {
                sprintf( settingsBuffer, TIME_BLOCK_REC_FMT, TIME_BLOCK_REC_ID,
                    timeblock[i].num,
                    timeblock[i].hour,
                    timeblock[i].minute,
                    timeblock[i].rangeFrom,
                    timeblock[i].rangeTo,
                    timeblock[i].uTo10BE,
                    timeblock[i].sens,
                    recCrc );

                calcCrc = this->crc( settingsBuffer, TIME_BLOCK_REC_LEN );

                if( calcCrc == recCrc )
                {
                    retVal = E_SETTINGS_OK;
                }
                else
                {
                    retVal = E_SETTINGS_CRC;
                }
            }
            else
            {
                retVal = E_SETTINGS_INV_REC;
            }
        }
    }

    return( retVal );
}

/* ----------------------------------------------------------------------------
 * int settings::writeGlobals( void )
 *
 * store settings to file
 ------------------------------------------------------------------------------
*/
int settings::writeGlobals( void )
{
    int retVal;
    uint32_t recCrc = 0;

    if( (retVal = this->open()) == E_SETTINGS_OK )
    {
        recCrc = 0;
        sprintf( settingsBuffer, GLOBALS_REC_FMT, GLOBALS_REC_ID,
                 globals.version,
                 globals.timeBlocksActive,
                 globals.increaseLevel,
                 globals.snacksize10BE,
                 globals.actHours,
                 globals.actMinutes,
                 globals.delayHours,
                 globals.delayMinutes,
                 globals.basalActHours,
                 globals.basalActMinutes,
                 globals.basalDelayHours,
                 globals.basalDelayMinutes,
                 recCrc );

        // calc crc with crc field = 0
        recCrc = this->crc( settingsBuffer, strlen(settingsBuffer) );

        fprintf( settingsFile, GLOBALS_REC_FMT, GLOBALS_REC_ID,
                 globals.version,
                 globals.timeBlocksActive,
                 globals.increaseLevel,
                 globals.snacksize10BE,
                 globals.actHours,
                 globals.actMinutes,
                 globals.delayHours,
                 globals.delayMinutes,
                 globals.basalActHours,
                 globals.basalActMinutes,
                 globals.basalDelayHours,
                 globals.basalDelayMinutes,
                 recCrc );
    }

    return( retVal );
}

/* ----------------------------------------------------------------------------
 * int settings::readGlobals( void )
 *
 * read stored settings from file
 ------------------------------------------------------------------------------
*/
int settings::readGlobals( void )
{
    int retVal = E_SETTINGS_OK;
    char recId;
    uint32_t recCrc = 0;
    uint32_t calcCrc = 0;

    if( (retVal = this->open()) == E_SETTINGS_OK )
    {
        retVal = fscanf( settingsFile, GLOBALS_REC_FMT, &recId,
                &globals.version,
                &globals.timeBlocksActive,
                &globals.increaseLevel,
                &globals.snacksize10BE,
                &globals.actHours,
                &globals.actMinutes,
                &globals.delayHours,
                &globals.delayMinutes,
                &globals.basalActHours,
                &globals.basalActMinutes,
                &globals.basalDelayHours,
                &globals.basalDelayMinutes,
                &recCrc );

        if( retVal == GLOBALS_REC_ITEMS )
        {
            sprintf( settingsBuffer, GLOBALS_REC_FMT, GLOBALS_REC_ID,
                 globals.version,
                 globals.timeBlocksActive,
                 globals.increaseLevel,
                 globals.snacksize10BE,
                 globals.actHours,
                 globals.actMinutes,
                 globals.delayHours,
                 globals.delayMinutes,
                 globals.basalActHours,
                 globals.basalActMinutes,
                 globals.basalDelayHours,
                 globals.basalDelayMinutes,
                 (unsigned int) 0 );

            // calc crc with crc field = 0
            calcCrc = this->crc( settingsBuffer, strlen(settingsBuffer) );
 
            if( calcCrc == recCrc )
            {
                retVal = E_SETTINGS_OK;
            }
            else
            {
                retVal = E_SETTINGS_CRC;
            }
        }
        else
        {
            retVal = E_SETTINGS_INV_REC;
        }
    }

    return( retVal );
}

/* ----------------------------------------------------------------------------
 * int settings::writeAdjustments( void )
 *
 * store settings to file
 ------------------------------------------------------------------------------
*/
int settings::writeAdjustments( void )
{
    int retVal;
    uint32_t recCrc = 0;

    if( (retVal = this->open()) == E_SETTINGS_OK )
    {
        recCrc = 0;
        sprintf( settingsBuffer, ADJUSTMENTS_REC_FMT, ADJUSTMENTS_REC_ID,
                 adjustments.sports1,
                 adjustments.sports2,
                 adjustments.stress,
                 adjustments.ill,
                 adjustments.female,
                 recCrc );

        // calc crc with crc field = 0
        recCrc = this->crc( settingsBuffer, strlen(settingsBuffer) );

        fprintf( settingsFile, ADJUSTMENTS_REC_FMT, ADJUSTMENTS_REC_ID,
                 adjustments.sports1,
                 adjustments.sports2,
                 adjustments.stress,
                 adjustments.ill,
                 adjustments.female,
                 recCrc );
    }

    return( retVal );
}

/* ----------------------------------------------------------------------------
 * int settings::readAdjustments( void )
 *
 * read stored settings from file
 ------------------------------------------------------------------------------
*/
int settings::readAdjustments( void )
{
    int retVal = E_SETTINGS_OK;
    char recId;
    uint32_t recCrc = 0;
    uint32_t calcCrc = 0;

    if( (retVal = this->open()) == E_SETTINGS_OK )
    {
        retVal = fscanf( settingsFile, ADJUSTMENTS_REC_FMT, &recId,
                &adjustments.sports1,
                &adjustments.sports2,
                &adjustments.stress,
                &adjustments.ill,
                &adjustments.female,
                &recCrc );

        if( retVal == ADJUSTMENTS_REC_ITEMS )
        {
            sprintf( settingsBuffer, ADJUSTMENTS_REC_FMT, ADJUSTMENTS_REC_ID,
                 adjustments.sports1,
                 adjustments.sports2,
                 adjustments.stress,
                 adjustments.ill,
                 adjustments.female,
                 (unsigned int) 0 );

            // calc crc with crc field = 0
            calcCrc = this->crc( settingsBuffer, strlen(settingsBuffer) );

            if( calcCrc == recCrc )
            {
                retVal = E_SETTINGS_OK;
            }
            else
            {
                retVal = E_SETTINGS_CRC;
            }
        }
        else
        {
            retVal = E_SETTINGS_INV_REC;
        }
    }

    return( retVal );
}

/* ----------------------------------------------------------------------------
 * int settings::writeDeviceId( void )
 *
 * store settings to file
 ------------------------------------------------------------------------------
*/
int settings::writeDeviceId( void )
{
    int retVal;
    uint32_t recCrc = 0;

    if( (retVal = this->open()) == E_SETTINGS_OK )
    {
        recCrc = 0;
        sprintf( settingsBuffer, DEVICE_ID_REC_FMT, DEVICE_ID_REC_ID,
                 devId[ 0],
                 devId[ 1],
                 devId[ 2],
                 devId[ 3],
                 devId[ 4],
                 devId[ 5],
                 devId[ 6],
                 devId[ 7],
                 devId[ 8],
                 devId[ 9],
                 devId[10],
                 devId[11],
                 devId[12],
                 devId[13],
                 devId[14],
                 devId[15],
                 recCrc );

        // calc crc with crc field = 0
        recCrc = this->crc( settingsBuffer, strlen(settingsBuffer) );

        fprintf( settingsFile, DEVICE_ID_REC_FMT, DEVICE_ID_REC_ID,
                 devId[ 0],
                 devId[ 1],
                 devId[ 2],
                 devId[ 3],
                 devId[ 4],
                 devId[ 5],
                 devId[ 6],
                 devId[ 7],
                 devId[ 8],
                 devId[ 9],
                 devId[10],
                 devId[11],
                 devId[12],
                 devId[13],
                 devId[14],
                 devId[15],
                 recCrc );
    }

    return( retVal );
}

/* ----------------------------------------------------------------------------
 * int settings::readDeviceId( void )
 *
 * read stored settings from file
 ------------------------------------------------------------------------------
*/
int settings::readDeviceId( void )
{
    int retVal = E_SETTINGS_OK;
    char recId;
    uint32_t recCrc = 0;
    uint32_t calcCrc = 0;

    if( (retVal = this->open()) == E_SETTINGS_OK )
    {
        retVal = fscanf( settingsFile, DEVICE_ID_REC_FMT, &recId,
                (int*) &devId[ 0],
                (int*) &devId[ 1],
                (int*) &devId[ 2],
                (int*) &devId[ 3],
                (int*) &devId[ 4],
                (int*) &devId[ 5],
                (int*) &devId[ 6],
                (int*) &devId[ 7],
                (int*) &devId[ 8],
                (int*) &devId[ 9],
                (int*) &devId[10],
                (int*) &devId[11],
                (int*) &devId[12],
                (int*) &devId[13],
                (int*) &devId[14],
                (int*) &devId[15],
                &recCrc );

        if( retVal == DEVICE_ID_REC_ITEMS )
        {
            sprintf( settingsBuffer, DEVICE_ID_REC_FMT, DEVICE_ID_REC_ID,
                 devId[ 0],
                 devId[ 1],
                 devId[ 2],
                 devId[ 3],
                 devId[ 4],
                 devId[ 5],
                 devId[ 6],
                 devId[ 7],
                 devId[ 8],
                 devId[ 9],
                 devId[10],
                 devId[11],
                 devId[12],
                 devId[13],
                 devId[14],
                 devId[15],
                 (unsigned int) 0 );

            calcCrc = this->crc( settingsBuffer, strlen(settingsBuffer) );

            if( calcCrc == recCrc )
            {
                retVal = E_SETTINGS_OK;
            }
            else
            {
                retVal = E_SETTINGS_CRC;
            }
        }
        else
        {
            retVal = E_SETTINGS_INV_REC;
        }
    }

    return( retVal );
}

/* ----------------------------------------------------------------------------
 * int settings::write( void )
 *
 * store settings to file
 ------------------------------------------------------------------------------
*/
int settings::write( void )
{
    int retVal;

    if( (retVal = writeDeviceId()) == E_SETTINGS_OK )
    {
        if( (retVal = writeGlobals()) == E_SETTINGS_OK )
        {
            if( (retVal = writeAdjustments()) == E_SETTINGS_OK )
            {
                retVal = writeTimeblocks();
            }
        }
    }

    this->close();

    return( retVal );
 }

/* ----------------------------------------------------------------------------
 * int settings::write( void )
 *
 * store settings to file
 ------------------------------------------------------------------------------
*/
int settings::read( void )
{
    int retVal;
    uint32_t recCrc = 0;

    if( (retVal = readDeviceId()) == E_SETTINGS_OK )
    {
        if( (retVal = readGlobals()) == E_SETTINGS_OK )
        {
            if( (retVal = readAdjustments()) == E_SETTINGS_OK )
            {
                retVal = readTimeblocks();

                if( retVal != E_SETTINGS_OK )
                {
fprintf(stderr, "read timeblock records failed with code %d\n", retVal );
                }
            }
            else
            {
fprintf(stderr, "read adjustmend record failed with code %d\n", retVal );
            }
        }
        else
        {
fprintf(stderr, "read global rec failed with code %d\n", retVal );
        }
    }
    else
    {
fprintf(stderr, "read dev Id failed with code %d\n", retVal );
    }

    this->close();

    return( retVal );
 }



/* ----------------------------------------------------------------------------
 * void settings::dumpDeviceId( void )
 *
 * reset all settings to their defaults
 ------------------------------------------------------------------------------
*/
void settings::dumpDeviceId( void )
{
    fprintf(stderr, "Dev-Id is: %x%x%x%x-%x%x%x%x-%x%x%x%x-%x%x%x%x\n",
                 devId[ 0], devId[ 1], devId[ 2], devId[ 3],
                 devId[ 4], devId[ 5], devId[ 6], devId[ 7],
                 devId[ 8], devId[ 9], devId[10], devId[11],
                 devId[12], devId[13], devId[14], devId[15] );
}

/* ----------------------------------------------------------------------------
 * void settings::dumpAdjustments( void )
 *
 * reset all settings to their defaults
 ------------------------------------------------------------------------------
*/
void settings::dumpAdjustments( void )
{
    fprintf(stderr, "Adjustment sports 1: %d\n", adjustments.sports1);
    fprintf(stderr, "Adjustment sports 2: %d\n", adjustments.sports2);
    fprintf(stderr, "Adjustment stress .: %d\n", adjustments.stress);
    fprintf(stderr, "Adjustment illness : %d\n", adjustments.ill);
    fprintf(stderr, "Adjustment female .: %d\n", adjustments.female);
}

/* ----------------------------------------------------------------------------
 * void settings::dumpGlobals( void )
 *
 * reset all settings to their defaults
 ------------------------------------------------------------------------------
*/
void settings::dumpGlobals( void )
{
    fprintf( stderr, "Version ........: %u\n", globals.version );
    fprintf( stderr, "Act. timeblocks : %d\n", globals.timeBlocksActive );
    fprintf( stderr, "Increase .......: %d\n", globals.increaseLevel );
    fprintf( stderr, "Snack size .....: %.1f\n", globals.snacksize10BE/10.0 );
    fprintf( stderr, "active .........: %d:%d\n", globals.actHours, 
             globals.actMinutes );
    fprintf( stderr, "delay ..........: %d:%d\n", globals.delayHours, 
             globals.delayMinutes );
    fprintf( stderr, "basal active ...: %d:%d\n", globals.basalActHours, 
             globals.basalActMinutes );
    fprintf( stderr, "basal delay ....: %d:%d\n", globals.basalDelayHours, 
             globals.basalDelayMinutes );
}

/* ----------------------------------------------------------------------------
 * void settings::dumpTimeblocks( void )
 *
 * reset all settings to their defaults
 ------------------------------------------------------------------------------
*/
void settings::dumpTimeblocks( void )
{
    for( int i = 0; i < MAX_TIME_BLOCKS; i++ )
    {
       fprintf( stderr, "timeblock ........: %d\n", timeblock[i].num );
       fprintf( stderr, "Time .......: %02d:%02d\n", timeblock[i].hour,
                 timeblock[i].minute );
       fprintf( stderr, "Range ......: %02d-%02d\n", timeblock[i].rangeFrom,
                 timeblock[i].rangeTo );
       fprintf( stderr, "Factor .....: %02d\n", timeblock[i].uTo10BE );
       fprintf( stderr, "Sensitivity : %02d\n", timeblock[i].sens );
    }
}

/* ----------------------------------------------------------------------------
 * void settings::dump( void )
 *
 * reset all settings to their defaults
 ------------------------------------------------------------------------------
*/
void settings::dump( void )
{
    dumpDeviceId();
    dumpAdjustments();
    dumpGlobals();
    dumpTimeblocks();
}

