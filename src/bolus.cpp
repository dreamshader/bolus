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

// char *getenv(const char *name);
void bolus::dumpArgs( void )
{
    fprintf(stderr, "fail ...: %s\n", 
            callerArgs.fail == true ? "true" : "false" );
    fprintf(stderr, "glucose.: %d\n", callerArgs.glucose );
    fprintf(stderr, "carb ...: %d\n", callerArgs.carb );
    fprintf(stderr, "bread ..: %d\n", callerArgs.bread );
    fprintf(stderr, "meal ...: %c\n", callerArgs.mealType );
    fprintf(stderr, "measure.: %c\n", callerArgs.measType );
    fprintf(stderr, "adjust .: %c\n", callerArgs.adjust );
    fprintf(stderr, "last ...: %s\n", 
            callerArgs.last == true ? "true" : "false" );
    fprintf(stderr, "edit ...: %c\n", callerArgs.editType );
    fprintf(stderr, "export .: %c\n", callerArgs.exportType );
    fprintf(stderr, "import .: %s\n", 
         callerArgs.importFile == NULL ? "NULL" : callerArgs.importFile );
    fprintf(stderr, "interact: %s\n", 
            callerArgs.interactive == true ? "true" : "false" );
    fprintf(stderr, "noStore : %s\n", 
            callerArgs.noStore == true ? "true" : "false" );
}


void bolus::resetArgs( void )
{
    callerArgs.fail        = false;
    callerArgs.glucose     = 0;
    callerArgs.carb        = 0;
    callerArgs.bread       = 0;
    callerArgs.mealType    = '\0';
    callerArgs.measType    = '\0';
    callerArgs.adjust      = 0;
    callerArgs.last        = false;
    callerArgs.editType    = '\0';
    callerArgs.exportType  = '\0';
    callerArgs.importFile  = NULL;
    callerArgs.interactive = false;
    callerArgs.noStore     = false;
}

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

        if( callerArgs.exportType != '\0' )
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
    }

    return( retVal );
}

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
        callerArgs.adjust      = pParam->adjust;
        callerArgs.last        = pParam->last;
        callerArgs.editType    = pParam->editType;
        callerArgs.exportType  = pParam->exportType;

        if( pParam->importFile != NULL )
        {
            callerArgs.importFile  = strdup(pParam->importFile);
        }

        callerArgs.interactive = pParam->interactive;
        callerArgs.noStore     = pParam->noStore;
    }
}

/* ----------------------------------------------------------------------------
 * int bolus::init( void )
 *
 * open settings file
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
//                        dumpArgs();
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




int bolus::runImport( void )
{
    int retVal = E_BOLUS_OK;
}

int bolus::runEsport( void )
{
    int retVal = E_BOLUS_OK;
}

int bolus::runInteractive( void )
{
    int retVal = E_BOLUS_OK;
}

int bolus::runList( void )
{
    int retVal = E_BOLUS_OK;
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

fprintf(stderr, "(%s[%d]) timeBlk is %d\n", __FILE__, __LINE__, timeBlk);

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

fprintf(stderr, "(%s[%d]) gOffset is %d\n", __FILE__, __LINE__, gOffset);

        insUnits = (pNewData->carbon10 / 12.0) * (pSettings->timeblock[timeBlk].uTo10BE / 10.0);

fprintf(stderr, "(%s[%d]) insUnits is %f\n", __FILE__, __LINE__, insUnits);

        if( pSettings->timeblock[timeBlk].sens > 0 )
        {
            offUnits = gOffset / pSettings->timeblock[timeBlk].sens;
        }
        else
        {
            offUnits = 0;
        }

fprintf(stderr, "(%s[%d]) offUnits is %f\n", __FILE__, __LINE__, offUnits);

        adjustment = ( (float) pNewData->adjust / 100.0) * insUnits;

fprintf(stderr, "(%s[%d]) adjustment is %f\n", __FILE__, __LINE__, adjustment);

        insUnits += offUnits;
        insUnits += adjustment;

        pNewData->units = (int) insUnits;
        pNewData->actUnits = pNewData->units;

        pNewData->basalUnits = 0;
        pNewData->actBasunits = pNewData->basalUnits;

fprintf(stderr, "(%s[%d]) pNewData->units is %d\n", __FILE__, __LINE__, pNewData->units);

        if( pLastData != NULL )
        {
fprintf( stderr, "PREVIOUS RECORD:\n" );
fprintf( stderr, "----------------\n" );
            pDatafile->dumpRec( pLastData );
fprintf( stderr, "----------------\n" );

            differenceSeconds = pNewData->timestamp - pLastData->timestamp;
            differenceMinutes = differenceSeconds / SECONDS_A_MINUTE;
            differenceHours = differenceSeconds / SECONDS_A_HOUR;
            differenceDays = differenceSeconds / SECONDS_A_DAY;

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

fprintf(stderr, "(%s[%d]) diffUnits is %f\n", __FILE__, __LINE__, diffUnits);

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
                    newData.glucose = callerArgs.glucose;

                    if( callerArgs.mealType != '\0' )
                    {
                        switch( callerArgs.mealType )
                        {
                            case 'b':
                            case 'B':
                                newData.meal = DATA_MEAL_BEFORE;
                                break;
                            case 'a':
                            case 'A':
                                newData.meal = DATA_MEAL_AFTER;
                                break;
                            case 'n':
                            case 'N':
                                newData.meal = DATA_MEAL_NONE;
                                break;
                            case 's':
                            case 'S':
                                newData.meal = DATA_MEAL_SLEEPTIME;
                                break;
                            case 'x':
                            case 'X':
                                newData.meal = DATA_MEAL_EXTRA;
                                break;
                            default:
                                break;
                        }
                    }

                    if( callerArgs.carb > 0 )
                    {
                        newData.carbon10 = callerArgs.carb;
                    }
                    else
                    {
                        newData.carbon10 = callerArgs.bread * 12;
                    }

                    if( callerArgs.measType != '\0' )
                    {
                        switch( callerArgs.measType )
                        {
                            case 'f':
                            case 'F':
                                newData.type = DATA_MEASURE_FREESTYLE;
                                break;
                            case 'a':
                            case 'A':
                                newData.type = DATA_MEASURE_ACUCHECK;
                                break;
                            default:
                                break;
                        }
                    }

                    if( (retVal = calcBolus( tmblk4now, &lastData, 
                         &newData )) == E_BOLUS_OK )
                    {
fprintf( stderr, "NEW RECORD:\n" );
fprintf( stderr, "-----------\n" );
                        pDatafile->dumpRec( &newData );
fprintf( stderr, "-----------\n" );
                        retVal = pDatafile->appendRecord( &currRecno, 
                                                          &newData );
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
    time_t now;
    struct tm *pActual;

    now = time(NULL);
    pActual = localtime(&now);
    retVal = use( pActual->tm_year+1900, pActual->tm_mon+1 );

    return( retVal );
}

int bolus::runEdit( void )
{
    int retVal = E_BOLUS_OK;
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
                retVal = runEsport();
                break;
            case BOLUS_INTERACTIVE_MODE:
                retVal = runInteractive();
                break;
            case BOLUS_LIST_MODE:
                retVal = runList();
                break;
            case BOLUS_CALC_BREAD_MODE:
                retVal = runCalcBread();
                break;
            case BOLUS_CALC_CARB_MODE:
                retVal = runCalcCarb();
                break;
            case BOLUS_EDIT_MODE:
                retVal = runEdit();
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

