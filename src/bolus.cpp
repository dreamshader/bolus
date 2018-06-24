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

/* ----------------------------------------------------------------------------
 * int bolus::init( void )
 *
 * open settings file
 ------------------------------------------------------------------------------
*/
int bolus::init( void )
{
    int retVal = E_BOLUS_OK;

    if( pSettings != NULL )
    {
        if( (retVal = pSettings->read()) == E_SETTINGS_OK )
        {
            if( pDatafile != NULL )
            {
                if( (retVal = pDatafile->init()) == E_DATAFILE_OK )
                {
                    initialized = true;
                    retVal = E_BOLUS_OK;
//
                    pSettings->dump();
                    retVal = pSettings->write();
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
        retVal = pSettings->write();
        if( retVal == E_SETTINGS_OK )
        {
            if( pDatafile != NULL )
            {
                if( (retVal = pDatafile->end()) == E_DATAFILE_OK )
                {
                    retVal = E_BOLUS_OK;
                }
            }
            else
            {
                retVal = E_BOLUS_DATAFILE;
            }
        }
        initialized = false;
    }
    else
    {
        retVal = E_BOLUS_INIT;
    }

    return( retVal );
}

