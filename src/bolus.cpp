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
                        dumpArgs();
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

int bolus::runCalcBread( void )
{
    int retVal = E_BOLUS_OK;
    time_t now;
    struct tm *pActual;
    int tmblk4now;

    now = time(NULL);
    pActual = localtime(&now);

    tmblk4now = -1;
    for( int i = 0; tmblk4now < 0 && i < MAX_TIME_BLOCKS; i++ )
    {

    pActual->tm_hour
    pActual->tm_min
    tmbl4now;

                 timeblock[i].num,
                 timeblock[i].hour,
                 timeblock[i].minute,
                 timeblock[i].rangeFrom,
                 timeblock[i].rangeTo,
                 timeblock[i].uTo10BE,
                 timeblock[i].sens,



    retVal = use( pActual->tm_year+1900, pActual->tm_mon+1 );
fprintf(stderr, "retVal %s -> %d is %d\n", __FILE__, __LINE__, retVal);

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
fprintf(stderr, "Use datafile year %d, month %d\n", year, month);
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

