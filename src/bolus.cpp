/*
 ***********************************************************************
 *
 *  bolus.cpp - class implementation
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

#include "bolus.h"

/* ----------------------------------------------------------------------------
 * function:
 *     void bolus::dumpArgs( void )
 * does:
 *     display parameters stored in callerArgs
 * returns:
 *     nothing
 ------------------------------------------------------------------------------
*/
void bolus::dumpArgs( void )
{
    fprintf(stderr, "fail ...............: %s\n", 
            callerArgs.fail == true ? "true" : "false" );
    fprintf(stderr, "offset .............: %d\n", callerArgs.offset );
    fprintf(stderr, "glucose ............: %d\n", callerArgs.glucose );
    fprintf(stderr, "carb ...............: %d\n", callerArgs.carb );
    fprintf(stderr, "bread ..............: %.2f\n", callerArgs.bread );
    fprintf(stderr, "mealType ...........: %c\n", callerArgs.mealType );
    fprintf(stderr, "measureType ........: %c\n", callerArgs.measType );
    fprintf(stderr, "adjustType .........: %c\n", callerArgs.adjustType );
    fprintf(stderr, "adjust .............: %d\n", callerArgs.adjust );
    fprintf(stderr, "last ...............: %s\n", 
            callerArgs.last == true ? "true" : "false" );
    fprintf(stderr, "exportType .........: %c\n", callerArgs.exportType );
    fprintf(stderr, "exportFile .........: %s\n", callerArgs.exportFile );
    fprintf(stderr, "importType .........: %c\n", callerArgs.importType );
    fprintf(stderr, "importFile .........: %s\n", callerArgs.importFile );
    fprintf(stderr, "nostore ............: %s\n", 
            callerArgs.noStore == true ? "true" : "false" );
    fprintf(stderr, "query only .........: %s\n", 
            callerArgs.query == true ? "true" : "false" );
    fprintf(stderr, "queryType ..........: %c\n", callerArgs.queryType );
    fprintf(stderr, "tmblk ..............: %d\n", callerArgs.timeBlockNumber);
    fprintf(stderr, "timeblock count ....: %s\n", 
            callerArgs.timeBlockCount == true ? "true" : "false" );
    fprintf(stderr, "delimiter ..........: %c\n", callerArgs.importDelimiter );
    fprintf(stderr, "1st line extra data : %s\n", 
            callerArgs.import1stLineXtraData == true ? "true" : "false" );
    fprintf(stderr, "debug mode .........: %s\n", 
            callerArgs.debugMode == true ? "true" : "false" );
    fprintf(stderr, "verbose level ......: %d\n", callerArgs.verboseLevel );
    fprintf(stderr, "datafile ...........: %s\n", callerArgs.dataFile );
    fprintf(stderr, "dump record ........: %d\n", callerArgs.dataRecord );
}


/* ----------------------------------------------------------------------------
 * function:
 *     void bolus::resetArgs( void )
 * does:
 *     set program parameters to useable defaults
 * returns:
 *     nothing
 ------------------------------------------------------------------------------
*/
void bolus::resetArgs( void )
{
    callerArgs.fail        = false;
    callerArgs.offset      = 0;
    callerArgs.glucose     = 0;
    callerArgs.carb        = 0;
    callerArgs.bread       = 0.0;
    callerArgs.mealType    = DATA_MEAL_NONE;
    callerArgs.measType    = DATA_NOTHING;
    callerArgs.adjustType  = DATA_NOTHING;
    callerArgs.adjust      = 0;
    callerArgs.last        = false;
    callerArgs.exportType    = DATA_NOTHING;
    callerArgs.exportFile  = NULL;
    callerArgs.importType    = DATA_NOTHING;
    callerArgs.importFile  = NULL;
    callerArgs.noStore = false;
    callerArgs.query = false;
    callerArgs.queryType = DATA_NOTHING;
    callerArgs.timeBlockNumber = -1;
    callerArgs.timeBlockCount = false;
    callerArgs.importDelimiter = DATA_DEFAULT_DELIMITER;
    callerArgs.import1stLineXtraData = false;
    callerArgs.debugMode = false;
    callerArgs.verboseLevel = 0;
    callerArgs.dataFile  = NULL;
    callerArgs.dataRecord = -1;
}

/* ----------------------------------------------------------------------------
 * function:
 *     int bolus::checkArgs( )
 * does:
 *     check paramters and their combinations for validity
 * returns:
 *     E_BOLUS_OK on success, otherwise an error code
 * TODO:
 *     perform real checks for given args
 ------------------------------------------------------------------------------
*/
int bolus::checkArgs( )
{
    int retVal = E_BOLUS_OK;

    if( callerArgs.glucose > 0 )
    {
        if( callerArgs.carb > 0 )
        {
            if( callerArgs.bread > 0 )
            {
                callerArgs.fail = true;
                retVal = E_BOLUS_CARB_N_BREAD;
            }
            else
            {
                mode = BOLUS_CALC_CARB_MODE;
            }
        }
        else
        {
            if( callerArgs.bread > 0 )
            {
                mode = BOLUS_CALC_BREAD_MODE;
            }
            else
            {
                callerArgs.fail = true;
                retVal = E_BOLUS_CARB_N_BREAD;
            }
        }

        if( callerArgs.carb == 0 && callerArgs.bread == 0.0 )
        {
            retVal = E_BOLUS_OK;
            mode = BOLUS_CALC_BREAD_MODE;
            callerArgs.fail = false;
        }
    }


    if( !callerArgs.fail )
    {
        if( callerArgs.last )
        {
            mode = BOLUS_LIST_MODE;
        }

        if( callerArgs.exportFile != NULL && 
            callerArgs.exportType != DATA_NOTHING )
        {
            mode = BOLUS_EXPORT_MODE;
        }

        if( callerArgs.importFile != NULL &&
            callerArgs.importType != DATA_NOTHING )
        {
            mode = BOLUS_IMPORT_MODE;
        }

        if( callerArgs.query && callerArgs.queryType != DATA_NOTHING )
        {
            mode = BOLUS_QUERY_MODE;
        }

        if( callerArgs.dataFile != NULL &&
             callerArgs.dataRecord >= 0 )
        {
            mode = BOLUS_DUMP_MODE;
        }
    }

    return( retVal );
}

/* ----------------------------------------------------------------------------
 * function:
 *     void bolus::setArgs( struct _bolus_param *pParam )
 * does:
 *     transfer parameters given e.g. by a CLI call to
 *     the corresponding internal structure
 * returns:
 *     nothing
 ------------------------------------------------------------------------------
*/
void bolus::setArgs( struct _bolus_param *pParam )
{
    if( pParam != NULL )
    {
        callerArgs.fail        = pParam->fail;
        callerArgs.glucose     = pParam->glucose;
        callerArgs.carb        = pParam->carb;
        callerArgs.bread       = pParam->bread;
        callerArgs.mealType    = pParam->mealType;
        callerArgs.measType    = pParam->measType;
        callerArgs.adjustType  = pParam->adjustType;
        callerArgs.adjust      = pParam->adjust;
        callerArgs.last        = pParam->last;
        callerArgs.exportType    = pParam->exportType;
        if( pParam->importFile != NULL )
        {
            callerArgs.importFile  = strdup(pParam->importFile);
        }

        callerArgs.importType = pParam->importType;
        if( pParam->exportFile != NULL )
        {
            callerArgs.exportFile  = strdup(pParam->exportFile);
        }

        callerArgs.noStore     = pParam->noStore;
        callerArgs.query       = pParam->query;
        callerArgs.queryType       = pParam->queryType;
        callerArgs.timeBlockNumber = pParam->timeBlockNumber;
        callerArgs.timeBlockCount = pParam->timeBlockCount;

        callerArgs.importDelimiter = pParam->importDelimiter;
        callerArgs.import1stLineXtraData = pParam->import1stLineXtraData;
        callerArgs.debugMode = pParam->debugMode;
        callerArgs.verboseLevel = pParam->verboseLevel;

        if( pParam->dataFile  != NULL )
        {
            callerArgs.dataFile  = strdup(pParam->dataFile);
        }

        callerArgs.dataRecord = pParam->dataRecord;

        callerArgs.glucose     += pParam->offset;
        if( callerArgs.glucose < 0 )
        {
            callerArgs.glucose = 0;
        }
    }
}

/* ----------------------------------------------------------------------------
 * function:
 *     int bolus::init( struct _bolus_param *pParam )
 * does:
 *     delete possible existing control structures and create them
 *     new. Read settings and check/assign parameters
 * returns:
 *     E_BOLUS_OK on success, otherwise an error code
 ------------------------------------------------------------------------------
*/
int bolus::init( struct _bolus_param *pParam )
{
    int retVal = E_BOLUS_OK;

    if( pSettings != NULL )
    {
        delete pSettings;
        pSettings = NULL;
    }

    if( pDatafile != NULL )
    {
        delete pDatafile;
        pDatafile = NULL;
    }

    if( (pSettings = new settings()) != NULL )
    {
        if( (retVal = pSettings->init()) == E_SETTINGS_OK )
        {
            if( (pDatafile = new datafile() )!= NULL )
            {
                if( (retVal = pDatafile->init()) == E_DATAFILE_OK )
                {
                    setArgs ( pParam );
                    if( (retVal = checkArgs()) == E_BOLUS_OK )
                    {
                        initialized = true;
                    }
                    else
                    {
                        if( pParam->debugMode )
                        {
                            dumpArgs();
                        }
                    }
                }
            }
            else
            {
                retVal = E_BOLUS_DATAFILE;
            }
        }
    }
    else
    {
        retVal = E_BOLUS_SETTINGS;
    }

    return( retVal );
}

/* ----------------------------------------------------------------------------
 * function:
 *     int bolus::end( void )
 * does:
 *     final cleanup
 * returns:
 *     E_BOLUS_OK on success, otherwise an error code
 ------------------------------------------------------------------------------
*/
int bolus::end( void )
{
    int retVal;

    if( initialized )
    {
        retVal = pSettings->end();
        initialized = false;
    }
    else
    {
        retVal = E_BOLUS_INIT;
    }

    return( retVal );
}

/* ----------------------------------------------------------------------------
 * function:
 *     int bolus::countTimeBlocks( void )
 * does:
 *     count the amount of defined timeblocks
 * returns:
 *     number of timeblocks
 ------------------------------------------------------------------------------
*/
int bolus::countTimeBlocks( void )
{
    int retVal = E_BOLUS_OK;

    retVal = NUM_TIME_BLOCKS;

    return( retVal );
}

/* ----------------------------------------------------------------------------
 * function:
 *     int bolus::runDumpData( void )
 * does:
 *     dump a specific data record
 * returns:
 *     E_BOLUS_OK on success, otherwise an error code
 * TODO: verify 
 ------------------------------------------------------------------------------
*/
int bolus::runDumpData( void )
{
    int retVal = E_BOLUS_OK;
    datafile *pData;
    struct _record dumpData;

    if( callerArgs.dataFile  != NULL )
    {
        if( (pData = new datafile()) != NULL )
        {
            if( (retVal = pData->init()) == E_DATAFILE_OK )
            {
                if( (retVal = pData->directUse( callerArgs.dataFile )) 
                       == E_DATAFILE_OK )
                {
                    if( ( retVal = pData->readRecord(callerArgs.dataRecord, 
                          &dumpData )) == E_DATAFILE_OK )
                    {
                        fprintf(stdout, "%lu:%04d:%c:%04d:%04d:%04d:%04d\n",
                            dumpData.timestamp,
                            dumpData.glucose,
                            dumpData.meal == '\0' ? 'n' : dumpData.meal,
                            dumpData.carboHydrate,
                            dumpData.adjust,
                            dumpData.units,
                            dumpData.basalUnits );
                    }
                }
            }
        }
        else
        {
            retVal = E_BOLUS_NULL;
        }
    }
    else
    {
        retVal = E_BOLUS_NULL;
    }

    return( retVal );
}


/* ----------------------------------------------------------------------------
 * function:
 *     int bolus::runQuery( void )
 * does:
 *     query specific data:
 * returns:
 *     E_BOLUS_OK on success, otherwise an error code
 * TODO: complete and verify 
 ------------------------------------------------------------------------------
*/
int bolus::runQuery( void )
{
    int retVal = E_BOLUS_OK;
    time_t now;
    struct tm *pActual;
    int tmblk4now;
    int timeTmblk;
    int i;

    switch( callerArgs.queryType )
    {
        case QUERY_TIMEBLOCKS:
            if( callerArgs.timeBlockCount )
            {
fprintf( stderr, "query number of timeblocks.\n");
                retVal = countTimeBlocks();
                fprintf( stdout, "%d\n", retVal );
            }
            else
            {
                int numTimeBlocks, tmIndex;

                numTimeBlocks = countTimeBlocks();

                if( callerArgs.timeBlockNumber < numTimeBlocks )
                {
                    tmIndex = callerArgs.timeBlockNumber;
                    if( tmIndex + 1 < numTimeBlocks )
                    {
                        fprintf( stdout, "%02d:%02d:%02d:%02d:%04d:%04d:%04d:%d\n",

                             pSettings->timeblock[tmIndex].time / 60,
                             pSettings->timeblock[tmIndex].time % 60,
                             pSettings->timeblock[tmIndex + 1].time / 60,
                             pSettings->timeblock[tmIndex + 1].time % 60,
                             pSettings->timeblock[tmIndex].rangeFrom,
                             pSettings->timeblock[tmIndex].rangeTo,
                             pSettings->timeblock[tmIndex].uTo10BE,
                             pSettings->timeblock[tmIndex].sens );
                    }
                    else
                    {
                        fprintf( stdout, "%02d:%02d:%02d:%02d:%04d:%04d:%04d:%d\n",
                             pSettings->timeblock[tmIndex].time / 60,
                             pSettings->timeblock[tmIndex].time % 60,
                             pSettings->timeblock[0].time / 60,
                             pSettings->timeblock[0].time % 60,
                             pSettings->timeblock[tmIndex].rangeFrom,
                             pSettings->timeblock[tmIndex].rangeTo,
                             pSettings->timeblock[tmIndex].uTo10BE,
                             pSettings->timeblock[tmIndex].sens );
                    }

                    retVal = 0;
                }
                else
                {
                    retVal = -1;
                }
            }
            break;
        case QUERY_GLUCOSE_STATUS:
            if( callerArgs.glucose > 0 )
            {
fprintf( stderr, "query glucose status for %d.\n", callerArgs.glucose);

                now = time(NULL);
                pActual = localtime(&now);

                tmblk4now = -1;
                timeTmblk = ( pActual->tm_hour * 60 ) + pActual->tm_min;

                for( int i = 0; tmblk4now < 0 && i < MAX_TIME_BLOCKS; i++ )
                {

                    if( timeTmblk <= pSettings->timeblock[i].time )
                    {
                        if( i )
                        {
                            tmblk4now = i-1;
                        }
                        else
                        {
                            tmblk4now = i;
                        }
                    }
                }

                if( tmblk4now < 0 )
                {
                    tmblk4now = MAX_TIME_BLOCKS-1;
                }

                if( tmblk4now >= 0 )
                {
                    if( callerArgs.glucose >= 
                        pSettings->timeblock[tmblk4now].rangeFrom &&
                        callerArgs.glucose <=
                        pSettings->timeblock[tmblk4now].rangeTo )
                    {
                        retVal = 0;           // green - ok
                    }
                    else
                    {
                        if( callerArgs.glucose <
                            pSettings->timeblock[tmblk4now].rangeFrom )
                        {
                            retVal = 2;           // red - too low
                        }
                        else
                        {
                            if( callerArgs.glucose >
                                pSettings->timeblock[tmblk4now].rangeTo )
                            {
                                retVal = 1;           // yellow - too high
                            }
                            else
                            {
                                retVal = -1;           // no idea
                            }
                        }
                    }
                }
                else
                {
                    retVal = -1;           // failed - timeblock not found
                }
            }
            else
            {
                retVal = -1;           // failed - gluco value < 0
            }
            fprintf( stdout, "%d\n", retVal );
            break;

        case QUERY_GLOBALS:
fprintf( stderr, "query global settings.\n" );
            fprintf( stdout, 
                     "%08u:%04d:%04d:%04d:%04d:%04d:%04d:%04d\n",
                     pSettings->globals.version,
                     pSettings->globals.timeBlocksActive,
                     pSettings->globals.increaseLevel,
                     pSettings->globals.snacksize10BE,
                     pSettings->globals.actTime,
                     pSettings->globals.delayTime,
                     pSettings->globals.basalActTime,
                     pSettings->globals.basalDelayTime );
            retVal = 0;
            break;
        case QUERY_ADJUSTMENTS:
fprintf( stderr, "query adjustment settings.\n" );
            fprintf( stdout, "%04d:%04d:%04d:%04d:%04d:%04d:%04d\n",
                     pSettings->adjustments.sober,
                     pSettings->adjustments.sport1,
                     pSettings->adjustments.stress,
                     pSettings->adjustments.illness,
                     pSettings->adjustments.sport2,
                     pSettings->adjustments.menstruation,
                     pSettings->adjustments.other );
                retVal = 0;
            break;
        case QUERY_RECORDS:
fprintf( stderr, "query data records.\n" );
                retVal = runDumpData();
            break;
        case QUERY_DEVICE:
fprintf( stderr, "query device settings.\n" );
                retVal = 0;
            break;
        default:
fprintf( stderr, "UNKNOWN query, yet!\n");
            retVal = -1;
            break;
    }
    return( retVal );
}


/* ----------------------------------------------------------------------------
 * function:
 *     int bolus::runImport( void )
 * does:
 *     import specified data
 * returns:
 *     E_BOLUS_OK on success, otherwise an error code
 * TODO: implement 
 ------------------------------------------------------------------------------
*/
int bolus::runImport( void )
{
    int retVal = E_BOLUS_OK;

    if( callerArgs.importFile != NULL )
    {
        struct _globals newGlobals;
        struct _adjust  newAdjustments;
        FILE *inFile;
        int csvFields;

#define IMPORT_GLOBALS_NUM_CSV_FIELDS    7
#define IMPORT_ADJUSTMENTS_NUM_CSV_FIELDS    7

        switch( callerArgs.importType )
        {
            case IMPORT_TIMEBLOCKS:
		fprintf(stderr, "import timeblocks\n");
#ifdef NEVERDEF
                if( (inFile = 
                         fopen(callerArgs.importFile, "r")) != (FILE*) NULL )
                {

                    int tmIndex = callerArgs.timeBlockNumber;
                    if( tmIndex + 1 < numTimeBlocks )
                    {
                        fprintf( stdout, "%02d:%02d:%02d:%02d:%04d:%04d:%04d:%d\n",

                             pSettings->timeblock[tmIndex].time / 60,
                             pSettings->timeblock[tmIndex].time % 60,
                             pSettings->timeblock[tmIndex + 1].time / 60,
                             pSettings->timeblock[tmIndex + 1].time % 60,
                             pSettings->timeblock[tmIndex].rangeFrom,
                             pSettings->timeblock[tmIndex].rangeTo,
                             pSettings->timeblock[tmIndex].uTo10BE,
                             pSettings->timeblock[tmIndex].sens );
                    }
                    else
                    {
                        fprintf( stdout, "%02d:%02d:%02d:%02d:%04d:%04d:%04d:%d\n",
                             pSettings->timeblock[tmIndex].time / 60,
                             pSettings->timeblock[tmIndex].time % 60,
                             pSettings->timeblock[0].time / 60,
                             pSettings->timeblock[0].time % 60,
                             pSettings->timeblock[tmIndex].rangeFrom,
                             pSettings->timeblock[tmIndex].rangeTo,
                             pSettings->timeblock[tmIndex].uTo10BE,
                             pSettings->timeblock[tmIndex].sens );
                    }
                }
#endif // NEVERDEF
                break;
            case IMPORT_GLOBALS:
                if( pSettings !=  NULL )
                {
                    if( (inFile = 
                         fopen(callerArgs.importFile, "r")) != (FILE*) NULL )
                    {
                        csvFields = fscanf(inFile, "%d;%d;%d;%d;%d;%d;%d",
                                           &newGlobals.timeBlocksActive,
                                           &newGlobals.increaseLevel,
                                           &newGlobals.snacksize10BE,
                                           &newGlobals.actTime,
                                           &newGlobals.delayTime,
                                           &newGlobals.basalActTime,
                                           &newGlobals.basalDelayTime );

                        if( csvFields == IMPORT_GLOBALS_NUM_CSV_FIELDS )
                        {
                            if( (retVal=pSettings->backup()) == E_SETTINGS_OK )
                            {
                                if( (retVal = pSettings->setNewGlobals(
                                     &newGlobals)) == E_SETTINGS_OK )
                                {
                                    retVal = pSettings->write();
                                }
                            }
                        }
                        else
                        {
                            retVal = E_BOLUS_IMPORT_FIELDS;
                        }
                        fclose(inFile);
                    }
                    else
                    {
                        retVal = E_BOLUS_IMPORT_OPEN;
                    }
                }
                else
                {
                    retVal = E_BOLUS_SETTINGS;
                }
fprintf(stderr, "IMPORT ends with retVal of %d!\n", retVal);
                break;
            case IMPORT_ADJUSTMENTS:
                if( pSettings !=  NULL )
                {
                    if( (inFile = 
                         fopen(callerArgs.importFile, "r")) != (FILE*) NULL )
                    {
                        csvFields = fscanf(inFile, "%d;%d;%d;%d;%d;%d;%d",
                                           &newAdjustments.sober,
                                           &newAdjustments.sport1,
                                           &newAdjustments.stress,
                                           &newAdjustments.illness,
                                           &newAdjustments.sport2,
                                           &newAdjustments.menstruation,
                                           &newAdjustments.other );

                        if( csvFields == IMPORT_ADJUSTMENTS_NUM_CSV_FIELDS )
                        {
                            if( (retVal=pSettings->backup()) == E_SETTINGS_OK )
                            {
                                if( (retVal = pSettings->setNewAdjustments( 
                                     &newAdjustments )) == E_SETTINGS_OK )
                                {
                                    retVal = pSettings->write();
                                }
                            }
                        }
                        else
                        {
                            retVal = E_BOLUS_IMPORT_FIELDS;
                        }
                        fclose(inFile);
                    }
                    else
                    {
                        retVal = E_BOLUS_IMPORT_OPEN;
                    }
                }
                else
                {
                    retVal = E_BOLUS_SETTINGS;
                }
fprintf(stderr, "IMPORT ends with retVal of %d!\n", retVal);
                break;
            // both obsolete
            case IMPORT_RECORDS:
            case IMPORT_DEVICE:
                break;
            default:
                retVal = E_UNKNOWN_IMPORT_TYPE;
                break;
        }
    }
    else
    {
        retVal = E_BOLUS_NULL;
    }

    return( retVal );
}

/* ----------------------------------------------------------------------------
 * function:
 *     int bolus::runExport( void )
 * does:
 *     export specified data
 * returns:
 *     E_BOLUS_OK on success, otherwise an error code
 * TODO: implement 
 ------------------------------------------------------------------------------
*/
int bolus::runExport( void )
{
    int retVal = E_BOLUS_OK;

    return( retVal );
}


/* ----------------------------------------------------------------------------
 * function:
 *     int bolus::runListLast( void )
 * does:
 *     dump last data record
 * returns:
 *     E_BOLUS_OK on success, otherwise an error code
 * TODO: verify 
 ------------------------------------------------------------------------------
*/
int bolus::runListLast( void )
{
    int retVal = E_BOLUS_OK;
    unsigned int lastRecno;
    struct _record lastData;
    time_t now;
    struct tm *pActual;

    if( pSettings != NULL && pDatafile != NULL )
    {
        now = time(NULL);
        pActual = localtime(&now);

        retVal = use( pActual->tm_year+1900, pActual->tm_mon+1 );

        if( retVal == E_BOLUS_OK )
        {
            pDatafile->resetRec( &lastData );
            retVal = pDatafile->readLastRecord( &lastRecno, &lastData );
        }

        if( lastRecno >= 0 && retVal == E_DATAFILE_OK )
        {
            pDatafile->dumpRec( &lastData );
        }
    }

    return( retVal );
}

// fprintf(stderr, "(%s[%d]) offUnits is %f\n", __FILE__, __LINE__, offUnits);

/* ----------------------------------------------------------------------------
 * function:
 * int bolus::calcBolus( int timeBlk, struct _record *pLastData, 
 *                struct _record *pNewData )
 * does:
 *     calculate bolus
 * returns:
 *     E_BOLUS_OK on success, otherwise an error code
 * TODO: verify 
 ------------------------------------------------------------------------------
*/
int bolus::calcBolus( int timeBlk, struct _record *pLastData, struct _record *pNewData )
{
    int retVal = E_BOLUS_OK;

    int gOffset = 0;
    float offUnits = 0.0;
    float insUnits = 0.0;
    float diffUnits = 0.0;
    float basUnits = 0.0;
    float diffBasUnits = 0.0;
    float adjustment = 0.0;
    time_t differenceSeconds;
    time_t differenceMinutes;
    time_t differenceHours;
    int differenceDays;
    float insFactor;
    float basFactor;

    if( callerArgs.debugMode )
    {
fprintf(stderr, "(%s[%d]) timeBlk is %d\n", __FILE__, __LINE__, timeBlk);
    }

    if( pNewData != NULL )
    {

        gOffset = 0;

        if( pNewData->glucose > pSettings->timeblock[timeBlk].rangeTo )
        {
            gOffset = pNewData->glucose - ((pSettings->timeblock[timeBlk].rangeTo +
                         pSettings->timeblock[timeBlk].rangeFrom ) / 2);

        }

        if( pNewData->glucose < pSettings->timeblock[timeBlk].rangeFrom )
        {
            gOffset = pNewData->glucose - ((pSettings->timeblock[timeBlk].rangeTo +
                         pSettings->timeblock[timeBlk].rangeFrom ) / 2);
        }

        if( callerArgs.debugMode )
        {
fprintf(stderr, "(%s[%d]) gOffset is %d\n", __FILE__, __LINE__, gOffset);
        }

        insUnits = (pNewData->carboHydrate / 12.0) * (pSettings->timeblock[timeBlk].uTo10BE / 10.0);

        if( callerArgs.debugMode )
        {
fprintf(stderr, "(%s[%d]) insUnits is %f\n", __FILE__, __LINE__, insUnits);
        }

        if( pSettings->timeblock[timeBlk].sens > 0 )
        {
            offUnits = gOffset / pSettings->timeblock[timeBlk].sens;
        }
        else
        {
            offUnits = 0;
        }

        if( callerArgs.debugMode )
        {
fprintf(stderr, "(%s[%d]) offUnits is %f\n", __FILE__, __LINE__, offUnits);
        }

        adjustment = ( (float) pNewData->adjust / 100.0) * insUnits;

        if( callerArgs.debugMode )
        {
fprintf(stderr, "(%s[%d]) adjustment is %f\n", __FILE__, __LINE__, adjustment);
        }

        insUnits += offUnits;
        insUnits += adjustment;

        pNewData->units = (int) insUnits;
        pNewData->actUnits = pNewData->units;

        pNewData->basalUnits = 0;
        pNewData->actBasunits = pNewData->basalUnits;

        if( callerArgs.debugMode )
        {
fprintf(stderr, "(%s[%d]) pNewData->units is %d\n", __FILE__, __LINE__, pNewData->units);
        }

        if( pLastData != NULL )
        {
            if( callerArgs.debugMode )
            {
fprintf( stderr, "PREVIOUS RECORD:\n" );
fprintf( stderr, "----------------\n" );
            pDatafile->dumpRec( pLastData );
fprintf( stderr, "----------------\n" );
            }

            differenceSeconds = pNewData->timestamp - pLastData->timestamp;
            differenceMinutes = differenceSeconds / SECONDS_A_MINUTE;
            differenceHours = differenceSeconds / SECONDS_A_HOUR;
            differenceDays = differenceSeconds / SECONDS_A_DAY;

            if( callerArgs.debugMode )
            {
fprintf(stderr, "(%s[%d]) differenceMinutes is %ld\n", __FILE__, __LINE__, differenceMinutes);
            }

            if( pNewData->units > 0 || pLastData->units > 0 )
            {
                // active time since last record has not gone
                if( differenceMinutes < 
                    (pSettings->globals.actTime + 
                     pSettings->globals.delayTime) )
                {

                    insFactor = 1.0 / (pSettings->globals.actTime + 
                                 pSettings->globals.delayTime);

                    insFactor *= differenceMinutes;

                    // diffUnits = insFactor * pLastData active units
                    diffUnits = insFactor * pLastData->actUnits;

                    if( callerArgs.debugMode )
                    {
fprintf(stderr, "(%s[%d]) diffUnits is %f\n", __FILE__, __LINE__, diffUnits);
                    }

                    pLastData->actUnits -= diffUnits;

                    pNewData->units -= pLastData->actUnits;
                }
            }

            if( pNewData->basalUnits > 0 || pLastData->basalUnits > 0 )
            {
                if( differenceMinutes < 
                    (pSettings->globals.basalActTime + 
                     pSettings->globals.basalDelayTime) )
                {
                    basFactor = 1.0 / (pSettings->globals.basalActTime + 
                                 pSettings->globals.basalDelayTime);
    
                    basFactor *= differenceMinutes;
    
                    diffBasUnits = basFactor * pLastData->actBasunits;
    
                    pNewData->actBasunits -= diffBasUnits;
    
                    pNewData->actBasunits -= pNewData->actBasunits;
                }
            }
        }
    }

    return( retVal );
}

// fprintf(stderr, "(%s[%d]) retVal is %d\n", __FILE__, __LINE__, retVal);

/* ----------------------------------------------------------------------------
 * function:
 *     int bolus::runCalcBread( void )
 * does:
 *     d
 * returns:
 *     E_BOLUS_OK on success, otherwise an error code
 * TODO: verify 
 ------------------------------------------------------------------------------
*/
int bolus::runCalcBread( void )
{
    int retVal = E_BOLUS_OK;
    time_t now;
    struct tm *pActual;
    int tmblk4now;
    int timeTmblk;
    unsigned int lastRecno;
    unsigned int currRecno;
    struct _record lastData;
    struct _record newData;


    if( pSettings != NULL && pDatafile != NULL )
    {
        now = time(NULL);
        pActual = localtime(&now);

        tmblk4now = -1;
        timeTmblk = ( pActual->tm_hour * 60 ) + pActual->tm_min;

        for( int i = 0; tmblk4now < 0 && i < MAX_TIME_BLOCKS; i++ )
        {

// fprintf(stderr, "(%s[%d]) now %d - setting %d\n",
// __FILE__, __LINE__, timeTmblk, pSettings->timeblock[i].time );

            if( timeTmblk <= pSettings->timeblock[i].time )
            {
                if( i )
                {
                    tmblk4now = i-1;
                }
                else
                {
                    tmblk4now = i;
                }
            }
        }

        if( tmblk4now < 0 )
        {
            tmblk4now = MAX_TIME_BLOCKS-1;
        }


// fprintf(stderr, "(%s[%d]) timeblk4now is %d\n", __FILE__, __LINE__, tmblk4now);

        if( tmblk4now >= 0 )
        {
            retVal = use( pActual->tm_year+1900, pActual->tm_mon+1 );

            if( retVal == E_BOLUS_OK )
            {

                pDatafile->resetRec( &lastData );
                retVal = pDatafile->readLastRecord( &lastRecno, &lastData );

                if( lastRecno >= 0 &&
                    (retVal == E_DATAFILE_OK || retVal == E_DATAFILE_EMPTY) )
                {
                    pDatafile->resetRec( &newData );

                    newData.timestamp = time(NULL);
                    newData.recnum = lastRecno;

                    if( callerArgs.glucose < DATA_GLUCOSE_MIN )
                    {
                        newData.glucose = DATA_GLUCOSE_MIN;
                    }
                    else
                    {
                        if( callerArgs.glucose > DATA_GLUCOSE_MAX )
                        {
                            newData.glucose = DATA_GLUCOSE_MAX;
                        }
                        else
                        {
                            newData.glucose = callerArgs.glucose;
                        }
                    }
                    callerArgs.glucose = newData.glucose;

                    if( callerArgs.mealType != '\0' )
                    {
                        switch( callerArgs.mealType )
                        {
                            case DATA_MEAL_BEFORE:
                            case DATA_MEAL_U_BEFORE:
                            case DATA_MEAL_AFTER:
                            case DATA_MEAL_U_AFTER:
                            case DATA_MEAL_NONE:
                            case DATA_MEAL_U_NONE:
                            case DATA_MEAL_SLEEPTIME:
                            case DATA_MEAL_U_SLEEPTIME:
                            case DATA_MEAL_EXTRA:
                            case DATA_MEAL_U_EXTRA:
                                newData.meal = callerArgs.mealType;
                                break;
                            default:
                                newData.meal = DATA_MEAL_NONE;
                                break;
                        }
                    }
                    callerArgs.mealType = newData.meal;

                    if( callerArgs.carb == 0 )
                    {
                        newData.carboHydrate = callerArgs.bread * 
                                        BOLUS_FACTOR_CARB2BREAD;
                    }

                    if( callerArgs.carb != 0 )
                    {
                        if( callerArgs.carb < DATA_CARBON10_MIN )
                        {
                            newData.carboHydrate = DATA_CARBON10_MIN;
                        }
                        else
                        {
                            if( callerArgs.carb > DATA_CARBON10_MAX )
                            {
                                newData.carboHydrate = DATA_CARBON10_MAX;
                            }
                            else
                            {
                                newData.carboHydrate = callerArgs.carb;
                            }
                        }
                    }
                    callerArgs.carb = newData.carboHydrate;

                    if( callerArgs.bread == 0.0 )
                    {
                        callerArgs.bread = callerArgs.carb / 
                                           BOLUS_FACTOR_CARB2BREAD;
                    }

                    if( callerArgs.bread != 0.0 )
                    {
                        if( callerArgs.bread < DATA_BREAD_MIN )
                        {
                            callerArgs.bread = DATA_BREAD_MIN;
                        }
                        else
                        {
                            if( callerArgs.bread > DATA_BREAD_MAX )
                            {
                                callerArgs.bread = DATA_BREAD_MAX;
                            }
                            else
                            {
                                callerArgs.bread = callerArgs.bread;
                            }
                        }
                    }

                    if( callerArgs.carb == 0 )
                    {
                        newData.carboHydrate = callerArgs.bread * 
                                 BOLUS_FACTOR_CARB2BREAD;
                    }

                    if( callerArgs.carb != 0 )
                    {
                        if( callerArgs.carb < DATA_CARBON10_MIN )
                        {
                            newData.carboHydrate = DATA_CARBON10_MIN;
                        }
                        else
                        {
                            if( callerArgs.carb > DATA_CARBON10_MAX )
                            {
                                newData.carboHydrate = DATA_CARBON10_MAX;
                            }
                            else
                            {
                                newData.carboHydrate = callerArgs.carb;
                            }
                        }
                    }
                    callerArgs.carb = newData.carboHydrate;

                    if( callerArgs.measType != '\0' )
                    {
                        switch( callerArgs.measType )
                        {
                            case DATA_MEASURE_ACUCHECK:
                            case DATA_MEASURE_U_ACUCHECK:
                            case DATA_MEASURE_FREESTYLE:
                            case DATA_MEASURE_U_FREESTYLE:
                                newData.type = callerArgs.measType;
                            default:
                                newData.type = DATA_MEASURE_NONE;
                                break;
                        }
                    }

                    callerArgs.measType = newData.type;

                    if( (retVal = calcBolus( tmblk4now, &lastData, 
                         &newData )) == E_BOLUS_OK )
                    {
                        if( callerArgs.debugMode )
                        {
fprintf( stderr, "NEW RECORD:\n" );
fprintf( stderr, "-----------\n" );
                        pDatafile->dumpRec( &newData );
fprintf( stderr, "-----------\n" );
                        }

                        if( callerArgs.noStore )
                        {
                            if( callerArgs.verboseLevel >= 1 )
                            {
fprintf( stderr, "WARN: data will not be save becaus of argument!\n");
                            }
                            retVal = newData.units;
                            // pNewData.units
                            // retVal = 0;
                            if( callerArgs.debugMode )
                            {
fprintf( stderr, "setting retVal to: %d!\n", retVal);
                            }
                        }
                        else
                        {
                            retVal = pDatafile->appendRecord( &currRecno, 
                                                              &newData );
                        }
                    }
                }
            }
        }
        else
        {
            retVal = E_BOLUS_TIMEBLK;
        }
    }
    else
    {
        retVal = E_BOLUS_NULL;
    }

    return( retVal );
}

/* ----------------------------------------------------------------------------
 * function:
 *     int bolus::runCalcCarb( void )
 * does:
 *     d
 * returns:
 *     E_BOLUS_OK on success, otherwise an error code
 ------------------------------------------------------------------------------
*/
int bolus::runCalcCarb( void )
{
    int retVal = E_BOLUS_OK;

    callerArgs.bread = callerArgs.carb / BOLUS_FACTOR_CARB2BREAD;
    retVal = runCalcBread();

    return( retVal );
}

/* ----------------------------------------------------------------------------
 * function:
 *     int bolus::run( void )
 * does:
 *     perform action
 * returns:
 *     E_BOLUS_OK on success, otherwise an error code
 * TODO: verify 
 ------------------------------------------------------------------------------
*/
int bolus::run( void )
{
    int retVal;

    if( initialized )
    {
        switch( mode )
        {
            case BOLUS_NO_MODE:
                retVal = W_BOLUS_IDLE;
                break;
            case BOLUS_IMPORT_MODE:
                retVal = runImport();
                break;
            case BOLUS_EXPORT_MODE:
                retVal = runExport();
                break;
            case BOLUS_LIST_MODE:
                retVal = runListLast();
                break;
            case BOLUS_CALC_BREAD_MODE:
                retVal = runCalcBread();
                break;
            case BOLUS_CALC_CARB_MODE:
                retVal = runCalcCarb();
                break;
            case BOLUS_QUERY_MODE:
                retVal = runQuery();
                break;
            case BOLUS_DUMP_MODE:
                retVal = runDumpData();
                break;
            default:
                retVal = E_BOLUS_RUN_MODE;
                break;
        }
    }
    else
    {
        retVal = E_BOLUS_INIT;
    }

    return( retVal );
}


/* ----------------------------------------------------------------------------
 * function:
 *     int bolus::use( int year, int month )
 * does:
 *     specify data file
 * returns:
 *     E_BOLUS_OK on success, otherwise an error code
 ------------------------------------------------------------------------------
*/
int bolus::use( int year, int month )
{
    int retVal;

    if( initialized )
    {
        if( pDatafile != NULL )
        {
            retVal = pDatafile->use( year, month );
        }
        else
        {
            retVal = E_BOLUS_DATAFILE;
        }
    }
    else
    {
        retVal = E_BOLUS_INIT;
    }

    return( retVal );
}

