/*
 ***********************************************************************
 *
 *  bolus.cpp - class for re/store settings
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
 *     display parameters given e.g. by a CLI call
 * returns:
 *     nothing
 ------------------------------------------------------------------------------
*/
void bolus::dumpArgs( void )
{
        fprintf(stderr, "fail ...............: %s\n", 
                callerArgs.fail == true ? "true" : "false" );
        fprintf(stderr, "glucose ............: %d\n", callerArgs.glucose );
        fprintf(stderr, "carb ...............: %d\n", callerArgs.carb );
        fprintf(stderr, "bread ..............: %.2f\n", callerArgs.bread );
        fprintf(stderr, "meal ...............: %c\n", callerArgs.mealType );
        fprintf(stderr, "measure ............: %c\n", callerArgs.measType );
        fprintf(stderr, "adjustType .........: %c\n", callerArgs.adjustType );
        fprintf(stderr, "adjust .............: %d\n", callerArgs.adjust );
        fprintf(stderr, "last ...............: %s\n", 
                callerArgs.last == true ? "true" : "false" );
        fprintf(stderr, "edit ...............: %c\n", callerArgs.editType );
        fprintf(stderr, "export .............: %s\n", callerArgs.exportFile );
        fprintf(stderr, "import .............: %s\n", callerArgs.importFile );
        fprintf(stderr, "interact ...........: %s\n", 
                callerArgs.interactive == true ? "true" : "false" );
        fprintf(stderr, "nostore ............: %s\n", 
                callerArgs.noStore == true ? "true" : "false" );
        fprintf(stderr, "offset .............: %d\n", callerArgs.offset );
        fprintf(stderr, "query only .........: %s\n", 
                callerArgs.query == true ? "true" : "false" );
        fprintf(stderr, "timeblock count ....: %s\n", 
                callerArgs.timeBlockCount == true ? "true" : "false" );
        fprintf(stderr, "tmblk ..............: %d\n", 
                callerArgs.timeBlockNumber);
        fprintf(stderr, "calibrate ..........: %s\n", 
                callerArgs.calibrate == true ? "true" : "false" );
        fprintf(stderr, "acucheck ...........: %d\n", 
                callerArgs.acucheckValue );
        fprintf(stderr, "freestyle ..........: %d\n", 
                callerArgs.freestyleValue );
        fprintf(stderr, "query factors ......: %s\n", 
                callerArgs.qFactors == true ? "true" : "false" );
        fprintf(stderr, "query globals ......: %s\n", 
                callerArgs.qGlobals == true ? "true" : "false" );
        fprintf(stderr, "blocks flag ........: %s\n", 
                callerArgs.qBlocks == true ? "true" : "false" );
        fprintf(stderr, "device flag ........: %s\n", 
                callerArgs.device == true ? "true" : "false" );
        fprintf(stderr, "delimiter ..........: %c\n", 
                callerArgs.importDelimiter );
        fprintf(stderr, "1st line extra data : %s\n", 
                callerArgs.import1stLineXtraData == true ? "true" : "false" );

        fprintf(stderr, "debug mode .........: %s\n", 
                callerArgs.debugMode == true ? "true" : "false" );
        fprintf(stderr, "verbose level ......: %d\n", callerArgs.verboseLevel );

        fprintf(stderr, "dump record ........: %d\n", callerArgs.dataRecord );
        fprintf(stderr, "datafile ...........: %s\n", callerArgs.dataFile );

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
    callerArgs.glucose     = 0;
    callerArgs.carb        = 0;
    callerArgs.bread       = 0.0;
    callerArgs.mealType    = DATA_MEAL_NONE;
    callerArgs.measType    = '\0';
    callerArgs.adjustType  = '\0';
    callerArgs.adjust      = 0;
    callerArgs.last        = false;
    callerArgs.editType    = '\0';
    callerArgs.exportFile  = NULL;
    callerArgs.importFile  = NULL;
    callerArgs.interactive = false;
    callerArgs.noStore     = false;
    callerArgs.offset      = 0;
    callerArgs.query       = false;
    callerArgs.timeBlockCount = false;
    callerArgs.timeBlockNumber = -1;
    callerArgs.calibrate        = false;
    callerArgs.acucheckValue   = -1;
    callerArgs.freestyleValue   = -1;
    callerArgs.qFactors = false;
    callerArgs.qGlobals = false;
    callerArgs.qBlocks = false;
    callerArgs.device = false;
    callerArgs.importDelimiter = ';';
    callerArgs.import1stLineXtraData = false;
    callerArgs.debugMode = false;
    callerArgs.verboseLevel = 0;
    callerArgs.dataRecord = -1;
    callerArgs.dataFile  = NULL;

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

        if( callerArgs.editType != '\0' )
        {
            mode = BOLUS_EDIT_MODE;
        }

        if( callerArgs.exportFile != NULL )
        {
            mode = BOLUS_EXPORT_MODE;
        }

        if( callerArgs.importFile != NULL )
        {
            mode = BOLUS_IMPORT_MODE;
        }

        if( callerArgs.interactive )
        {
            mode = BOLUS_INTERACTIVE_MODE;
        }

        if( callerArgs.query )
        {
            mode = BOLUS_QUERY_MODE;
        }

        if( callerArgs.dataFile != 0 )
        {
            mode = BOLUS_DUMP_MODE;
        }

        if( callerArgs.calibrate )
        {
            mode = BOLUS_CALIBRATE_MODE;
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
        callerArgs.editType    = pParam->editType;

        if( pParam->importFile != NULL )
        {
            callerArgs.importFile  = strdup(pParam->importFile);
        }

        if( pParam->exportFile != NULL )
        {
            callerArgs.exportFile  = strdup(pParam->exportFile);
        }

        callerArgs.interactive = pParam->interactive;
        callerArgs.noStore     = pParam->noStore;
        callerArgs.query       = pParam->query;
        callerArgs.timeBlockCount = pParam->timeBlockCount;
        callerArgs.timeBlockNumber = pParam->timeBlockNumber;

        callerArgs.calibrate = pParam->calibrate;
        callerArgs.acucheckValue = pParam->acucheckValue;
        callerArgs.freestyleValue = pParam->freestyleValue;

        callerArgs.qFactors = pParam->qFactors;
        callerArgs.qGlobals = pParam->qGlobals;
        callerArgs.qBlocks = pParam->qBlocks;
        callerArgs.device = pParam->device;
        callerArgs.importDelimiter = pParam->importDelimiter;
        callerArgs.import1stLineXtraData = pParam->import1stLineXtraData;
        callerArgs.debugMode = pParam->debugMode;
        callerArgs.verboseLevel = pParam->verboseLevel;

        callerArgs.dataRecord = pParam->dataRecord;
        callerArgs.dataFile  = pParam->dataFile;

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
 * int bolus::end( void )
 *
 * close settings file
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

int bolus::countTimeBlocks( void )
{
    int retVal = E_BOLUS_OK;

    retVal = NUM_TIME_BLOCKS;

    return( retVal );
}

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
fprintf(stdout, "%04d:%c:%04d:%04d:%04d:%04d",
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


int bolus::runQuery( void )
{
    int retVal = E_BOLUS_OK;
    time_t now;
    struct tm *pActual;
    int tmblk4now;
    int timeTmblk;
    int i;

    if( callerArgs.timeBlockCount )
    {
fprintf( stderr, "query number of timeblocks.\n");
        retVal = countTimeBlocks();
        fprintf( stdout, "%d\n", retVal );
    }
    else
    {
        if( callerArgs.timeBlockNumber >= 0 )
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
        else
        {
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

                fprintf( stdout, "%d\n", retVal );
            }
            else
            {
                if( callerArgs.qFactors )
                {
fprintf( stderr, "query factor settings.\n" );
                    fprintf( stdout, "%04d:%04d:%04d:%04d:%04d",
                             pSettings->adjustments.sports1,
                             pSettings->adjustments.sports2,
                             pSettings->adjustments.stress,
                             pSettings->adjustments.ill,
                             pSettings->adjustments.female );
                    retVal = 0;
                }
                else
                {
                    if( callerArgs.qGlobals )
                    {
fprintf( stderr, "query global settings.\n" );
                        fprintf( stdout, 
                                 "%08u:%04d:%04d:%04d:%04d:%04d:%04d:%04d",
                                 pSettings->globals.version,
                                 pSettings->globals.timeBlocksActive,
                                 pSettings->globals.increaseLevel,
                                 pSettings->globals.snacksize10BE,
                                 pSettings->globals.actTime,
                                 pSettings->globals.delayTime,
                                 pSettings->globals.basalActTime,
                                 pSettings->globals.basalDelayTime );
                        retVal = 0;
                    }
                    else
                    {
                        if( callerArgs.qBlocks )
                        {
fprintf( stderr, "query timeblocks.\n" );

                        }
                        else
                        {
                            if( callerArgs.device )
                            {
fprintf( stderr, "query device settings.\n" );
                            }
                            else
                            {
fprintf( stderr, "UNKNOWN query, yet!\n");
                                retVal = -1;
                            }
                        }
                    }
                }
            }
        }
    }


#ifdef NEVERDEF

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

struct _adjust {
    int sports1;
    int sports2;
    int stress;
    int ill;
    int female;
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

pSettings->adjustments;
pSettings->globals;


#endif // NEVERDEF




    return( retVal );
}

int bolus::runCalibrate( void )
{
    int retVal = E_BOLUS_OK;

fprintf(stderr, "Calibrating for sensor values\n");
//    bool callerArgs.calibrate;
//    callerArgs.freestyleValue;
//    callerArgs.acucheckValue;

    if( callerArgs.freestyleValue > 0 && callerArgs.acucheckValue > 0 )
    {
fprintf( stderr, "real value of %d is equivalent to freestyle value %d\n",
         callerArgs.acucheckValue, callerArgs.freestyleValue );

    }

    return( retVal );
}


int bolus::runImport( void )
{
    int retVal = E_BOLUS_OK;

    if( callerArgs.importFile != NULL )
    {
        if( callerArgs.qBlocks )
        {
fprintf(stderr, "Import timeblocks, file is -> %s\n", callerArgs.importFile );
        }
        else
        {
            if( callerArgs.qGlobals )
            {
fprintf(stderr, "Import globals, file is -> %s\n", callerArgs.importFile );
            }
            else
            {
                if( callerArgs.qFactors )
                {
fprintf(stderr, "Import adjustments, file is -> %s\n", callerArgs.importFile );
                }
                else
                {
                    if( callerArgs.device )
                    {
fprintf(stderr, "Import device settings, file is -> %s\n", callerArgs.importFile );
//        callerArgs.importDelimiter = pParam->importDelimiter;
//        callerArgs.import1stLineXtraData = pParam->import1stLineXtraData;

                    }
                }
            }
        }

// case 'B':
// callerArgs.qBlocks
// case 'G':
// callerArgs.qGlobals
// case 'f':
// callerArgs.qFactors

    }

    return( retVal );
}

int bolus::runExport( void )
{
    int retVal = E_BOLUS_OK;

// -X --export=T(imeblocks)
// -X --export=G(lobals)
// -X --export=A(djustments)
// -X --export=J(ason)
// -X T
// -X G
// -X A
// -X J

    return( retVal );
}

int bolus::runInteractive( FILE *pIn, FILE *pOut )
{
    char inputBuffer[BOLUS_DIALOG_BUFSIZE];
    int retVal = E_BOLUS_OK;
    char command;
    int inpPos;
    bool inputComplete;
    bool skip;
    int currFieldNo;
    struct _record newData;
    int intInput;

// fprintf(stderr, "edit ...: %c\n", callerArgs.editType );

    if( pIn != (FILE*) NULL && pOut != (FILE*) NULL )
    {
        pDatafile->resetRec( &newData );
        inputComplete = false;
        currFieldNo = 0;
        command = BOLUS_INTERACT_NONE;
        do
        {
            skip = false;
            switch(currFieldNo)
            {
                case BOLUS_FIELD_TIMESTAMP:
                    fprintf(pOut, "timestamp  (= %lu): ", newData.timestamp );
                    fprintf(pOut, "skipped.\n");
                    currFieldNo++;
                    skip = true;
                    break;
                case BOLUS_FIELD_RECNUM:
                    fprintf(pOut, "recno      (= %d): ", newData.recnum );
                    fprintf(pOut, "skipped.\n");
                    currFieldNo++;
                    skip = true;
                    break;
                case BOLUS_FIELD_GLUCOSE:
                    fprintf(pOut, "glucose ( %d to %d ): ",
                              DATA_GLUCOSE_MIN, DATA_GLUCOSE_MAX );
                    break;
                case BOLUS_FIELD_MEAL:
                    fprintf(pOut, "meal (%c,%c,%c,%c,%c,%c,%c,%c,%c,%c): ",
                              DATA_MEAL_BEFORE, DATA_MEAL_U_BEFORE,
                              DATA_MEAL_AFTER, DATA_MEAL_U_AFTER,
                              DATA_MEAL_NONE, DATA_MEAL_U_NONE,
                              DATA_MEAL_SLEEPTIME, DATA_MEAL_U_SLEEPTIME,
                              DATA_MEAL_EXTRA, DATA_MEAL_U_EXTRA );
                    break;
                case BOLUS_FIELD_CARBON:
                    fprintf(pOut, "carboHydrate ( %d to %d ): ",
                              DATA_CARBON10_MIN, DATA_CARBON10_MAX );
                    break;
                case BOLUS_FIELD_ADJUST:
                    fprintf(pOut, "adjust     (= %d): ", newData.adjust );
                    fprintf(pOut, "skipped.\n");
                    currFieldNo++;
                    skip = true;
                    break;
                case BOLUS_FIELD_UNITS:
                    fprintf(pOut, "units      (= %d): ", newData.units );
                    fprintf(pOut, "skipped.\n");
                    currFieldNo++;
                    skip = true;
                    break;
                case BOLUS_FIELD_BASALUNITS:
                    fprintf(pOut, "basalUnits ( %d to %d): ",
                              DATA_BASAL_UNITS_MIN, DATA_BASAL_UNITS_MAX );
                    break;
                case BOLUS_FIELD_TYPE:
                    fprintf(pOut, "type (%d,%c,%c,%c,%c): ", 0,
                              DATA_MEASURE_ACUCHECK, DATA_MEASURE_U_ACUCHECK,
                              DATA_MEASURE_FREESTYLE, DATA_MEASURE_U_FREESTYLE);

                    break;
                case BOLUS_FIELD_ACTUNITS:
                    fprintf(pOut, "act Units  (= %d): ", newData.actUnits );
                    fprintf(pOut, "skipped.\n");
                    currFieldNo++;
                    skip = true;
                    break;
                case BOLUS_FIELD_ACTBASUNITS:
                    fprintf(pOut, "bact Units (= %d): ", newData.actBasunits );
                    fprintf(pOut, "skipped.\n");
                    currFieldNo++;
                    skip = true;
                    break;
                case BOLUS_FIELD_ASK_YESNO:
                    pDatafile->dumpRec( &newData );
                    fprintf(pOut, "save (y)es/(n)o: " );
                    break;
                default:
                    break;
            }

           
            if( !skip )
            {
                fgets( inputBuffer, BOLUS_DIALOG_BUFSIZE-1, pIn );

                for( inpPos = 0; inpPos < strlen(inputBuffer) && 
                        isspace( inputBuffer[inpPos] ) ; inpPos++ )
                   ;
    
                switch( inputBuffer[inpPos] )
                {
                    case BOLUS_INTERACT_SKIP:
                        command = inputBuffer[inpPos];
                        break;
                    case BOLUS_INTERACT_BACK:
                        command = inputBuffer[inpPos];
                        break;
                    case BOLUS_INTERACT_REJECT:
                        command = inputBuffer[inpPos];
                        break;
                    case BOLUS_INTERACT_HELP:
                        command = inputBuffer[inpPos];
                        break;
                    case BOLUS_INTERACT_YES_DE:
                    case BOLUS_INTERACT_YES_EN:
                    case BOLUS_INTERACT_U_YES_DE:
                    case BOLUS_INTERACT_U_YES_EN:
                    case BOLUS_INTERACT_NO:
                    case BOLUS_INTERACT_U_NO:
                        command = inputBuffer[inpPos];
                        break;
                    default:
                        command = BOLUS_INTERACT_INVAL;
                        break;
                }


                switch(currFieldNo)
                {
                    case BOLUS_FIELD_GLUCOSE:
                        intInput = atoi(inputBuffer);
                        newData.glucose = intInput;
                        currFieldNo++;
                        break;
                    case BOLUS_FIELD_MEAL:
                        intInput = atoi(inputBuffer);
                        newData.meal = intInput;
                        currFieldNo++;
                        break;
                    case BOLUS_FIELD_CARBON:
                        intInput = atoi(inputBuffer);
                        newData.carboHydrate = intInput;
                        currFieldNo++;
                        break;
                    case BOLUS_FIELD_BASALUNITS:
                        intInput = atoi(inputBuffer);
                        newData.basalUnits = intInput;
                        currFieldNo++;
                        break;
                    case BOLUS_FIELD_TYPE:
                        intInput = atoi(inputBuffer);
                        newData.type = intInput;
                        currFieldNo++;
                        break;
                    case BOLUS_FIELD_ASK_YESNO:
                        switch( command )
                        {
                            case BOLUS_INTERACT_YES_DE:
                            case BOLUS_INTERACT_YES_EN:
                            case BOLUS_INTERACT_U_YES_DE:
                            case BOLUS_INTERACT_U_YES_EN:
fprintf(stderr, "(%s[%d]) save data!\n", __FILE__, __LINE__ );
                                break;
                            case BOLUS_INTERACT_NO:
                            case BOLUS_INTERACT_U_NO:
fprintf(stderr, "(%s[%d]) DISCARD data!\n", __FILE__, __LINE__ );
                                break;
                            default:
fprintf(stderr, "(%s[%d]) input was %c!\n", __FILE__, __LINE__, command );
                                break;
                        }
                        inputComplete = true;
                        break;
                    default:
                        currFieldNo++;
                        break;
                }
            }
fprintf(stderr, "(%s[%d]) inputComplete %s\n", __FILE__, __LINE__, 
          inputComplete?"DONE!":"MORE");
        } while( command != BOLUS_EDIT_CMD_REJECT && !inputComplete );
    }
}

// fprintf(stderr, "(%s[%d]) command %c\n", __FILE__, __LINE__, command);

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

int bolus::runCalcCarb( void )
{
    int retVal = E_BOLUS_OK;

    callerArgs.bread = callerArgs.carb / BOLUS_FACTOR_CARB2BREAD;
    retVal = runCalcBread();

    return( retVal );
}

int bolus::runEditor( void )
{
    char commandBuffer[BOLUS_COMMAND_BUFSIZE];
    int retVal = E_BOLUS_OK;
    int editorCommand;
    int cmdPos;

// fprintf(stderr, "edit ...: %c\n", callerArgs.editType );
// -e --edit=T(imeblocks)
// -e --edit=G(lobals)
// -e --edit=A(djustments)
// -e T
// -e G
// -e A

    do
    {
        printf("\n>");
        if( fgets( commandBuffer, BOLUS_COMMAND_BUFSIZE-1, stdin ) != NULL )
        {
            for( cmdPos = 0; cmdPos < strlen(commandBuffer) && 
                    isspace( commandBuffer[cmdPos] ) ; cmdPos++ )
               ;

            switch( commandBuffer[cmdPos] )
            {
                case BOLUS_EDIT_CMD_AA:
                    editorCommand = commandBuffer[cmdPos];
fprintf(stderr, "(%s[%d]) BOLUS_EDIT_CMD_AA %c\n", __FILE__, __LINE__, editorCommand);
                    break;
                case BOLUS_EDIT_CMD_BB:
                    editorCommand = commandBuffer[cmdPos];
fprintf(stderr, "(%s[%d]) BOLUS_EDIT_CMD_BB %c\n", __FILE__, __LINE__, editorCommand);
                    break;
                case BOLUS_EDIT_CMD_ENDEDIT:
                    editorCommand = commandBuffer[cmdPos];
fprintf(stderr, "(%s[%d]) BOLUS_EDIT_CMD_ENDEDIT %c\n", __FILE__, __LINE__, editorCommand);
                    break;
                case BOLUS_EDIT_CMD_WRITE:
                    editorCommand = commandBuffer[cmdPos];
fprintf(stderr, "(%s[%d]) BOLUS_EDIT_CMD_WRITE %c\n", __FILE__, __LINE__, editorCommand);
                    break;
                case BOLUS_EDIT_CMD_READ:
                    editorCommand = commandBuffer[cmdPos];
fprintf(stderr, "(%s[%d]) BOLUS_EDIT_CMD_READ %c\n", __FILE__, __LINE__, editorCommand);
                    break;
                case BOLUS_EDIT_CMD_REJECT:
                    editorCommand = commandBuffer[cmdPos];
fprintf(stderr, "(%s[%d]) BOLUS_EDIT_CMD_REJECT %c\n", __FILE__, __LINE__, editorCommand);
                    break;
                case BOLUS_EDIT_CMD_HELP:
                    editorCommand = commandBuffer[cmdPos];
fprintf(stderr, "(%s[%d]) BOLUS_EDIT_CMD_HELP %c\n", __FILE__, __LINE__, editorCommand);
                    break;
                default:
                    editorCommand = BOLUS_EDIT_CMD_INVAL;
fprintf(stderr, "(%s[%d]) BOLUS_EDIT_CMD_INVAL %c\n", __FILE__, __LINE__, editorCommand);
                    break;
            }
        }

    } while( editorCommand != BOLUS_EDIT_CMD_ENDEDIT &&
           editorCommand != BOLUS_EDIT_CMD_REJECT );
}


/* ----------------------------------------------------------------------------
 * int bolus::run( void )
 *
 * close settings file
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
            case BOLUS_INTERACTIVE_MODE:
                retVal = runInteractive( stdin, stdout );
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
            case BOLUS_EDIT_MODE:
                retVal = runEditor();
                break;
            case BOLUS_QUERY_MODE:
                retVal = runQuery();
                break;
            case BOLUS_DUMP_MODE:
                retVal = runDumpData();
                break;
            case BOLUS_CALIBRATE_MODE:
                retVal = runCalibrate();
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
 * int bolus::use( int year, int month )
 *
 * close settings file
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

