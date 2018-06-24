/*
 ***********************************************************************
 *
 *  datafile.cpp - class for re/store settings
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

#include "datafile.h"

// char *getenv(const char *name);

/* ----------------------------------------------------------------------------
 * int datafile::init( void )
 *
 * open settings file
 ------------------------------------------------------------------------------
*/
int datafile::init( void )
{
    int retVal = E_DATAFILE_OK;

    initialized = true;

    return( retVal );
}

/* ----------------------------------------------------------------------------
 * int datafile::use( void )
 *
 * close settings file
 ------------------------------------------------------------------------------
*/
int datafile::use( int year, int month )
{
    int retVal = E_DATAFILE_OK;
    char *pHome;
    int flags = O_RDWR;
    int mode = S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH;

    if( initialized )
    {
        if( dataFd > 0 )
        {
            close( dataFd );
            dataFd = -1;
        }

        if( (pHome = getenv("HOME")) != NULL )
        {
            sprintf(dataFileName, DATA_FILE_NAME, pHome, BOLUS_BASE_DIR, 
                    year, month );

            if( (retVal = access( dataFileName , R_OK | W_OK )) < 0 )
            {
                switch( errno )
                {
                    case EACCES:
                        retVal = E_DATAFILE_ACCESS;
                        break;
                    case ENOENT:
                        flags = O_RDWR|O_CREAT|O_TRUNC;
                        retVal = E_DATAFILE_OK;
                        break;
                    case EROFS:
                        retVal = E_DATAFILE_ROFS;
                        break;
                    case ENOTDIR:
                        retVal = E_DATAFILE_PATH;
                        break;
                    case EINVAL:
                        retVal = E_DATAFILE_FLAGS;
                        break;
                    default:
                        retVal = E_DATAFILE_UNKNOWN;
                        break;
                }
            }
            else
            {
                flags = O_RDWR;
                retVal = E_DATAFILE_OK;
            }
    
            if( retVal == E_DATAFILE_OK )
            {
                if( flags & O_CREAT )
                {
                    if( (dataFd = open(dataFileName, flags, mode)) < 0 )
                    {
                        retVal = E_DATAFILE_UNKNOWN;
                    }
                }
                else
                {
                    if( (dataFd = open(dataFileName, flags)) < 0 )
                    {
                        retVal = E_DATAFILE_UNKNOWN;
                    }
                }
                currYear = year;
                currMonth = month;
            }
        }
        else
        {
            retVal = E_DATAFILE_ENV;
        }
    }
    else
    {
        retVal = E_DATAFILE_INIT;
    }

    return( retVal );
}




int datafile::performRead( struct _record *pData )
{
    int retVal = E_DATAFILE_OK;

    if( pData != NULL )
    {
        int fieldlen, rdlen;
        retVal = E_DATAFILE_OK;

        for( int fieldno = 0; retVal == E_DATAFILE_OK &&
             fieldno < DATA_RECORD_NUM_FIELDS; fieldno++ )
        {
            switch( fieldno )
            {
                case 0:
                    fieldlen = sizeof(pData->timestamp);
                    rdlen = read( dataFd, &pData->timestamp, fieldlen );
                    break;
                case 1:
                    fieldlen = sizeof(pData->recnum);
                    rdlen = read( dataFd, &pData->recnum, fieldlen );
                    break;
                case 2:
                    fieldlen = sizeof(pData->glucose);
                    rdlen = read( dataFd, &pData->glucose, fieldlen );
                    break;
                case 3:
                    fieldlen = sizeof(pData->meal);
                    rdlen = read( dataFd, &pData->meal, fieldlen );
                    break;
                case 4:
                    fieldlen = sizeof(pData->carbon10);
                    rdlen = read( dataFd, &pData->carbon10, fieldlen );
                    break;
                case 5:
                    fieldlen = sizeof(pData->adjust);
                    rdlen = read( dataFd, &pData->adjust, fieldlen );
                    break;
                case 6:
                    fieldlen = sizeof(pData->units);
                    rdlen = read( dataFd, &pData->units, fieldlen );
                    break;
                case 7:
                    fieldlen = sizeof(pData->basalUnits);
                    rdlen = read( dataFd, &pData->basalUnits, fieldlen );
                default:
                    break;
            }

            if( rdlen != fieldlen )
            {
                retVal = E_DATAFILE_READ;
            }
        }
    }
    else
    {
        retVal = E_DATAFILE_NULL;
    }

    return( retVal );
}

int datafile::performWrite( struct _record *pData )
{
    int retVal = E_DATAFILE_OK;

    if( pData != NULL )
    {
        int fieldlen, wrlen;
        retVal = E_DATAFILE_OK;

        for( int fieldno = 0; retVal == E_DATAFILE_OK &&
             fieldno < DATA_RECORD_NUM_FIELDS; fieldno++ )
        {
            switch( fieldno )
            {
                case 0:
                    fieldlen = sizeof(pData->timestamp);
                    wrlen = write( dataFd, &pData->timestamp, fieldlen );
                    break;
                case 1:
                    fieldlen = sizeof(pData->recnum);
                    wrlen = write( dataFd, &pData->recnum, fieldlen );
                    break;
                case 2:
                    fieldlen = sizeof(pData->glucose);
                    wrlen = write( dataFd, &pData->glucose, fieldlen );
                    break;
                case 3:
                    fieldlen = sizeof(pData->meal);
                    wrlen = write( dataFd, &pData->meal, fieldlen );
                    break;
                case 4:
                    fieldlen = sizeof(pData->carbon10);
                    wrlen = write( dataFd, &pData->carbon10, fieldlen );
                    break;
                case 5:
                    fieldlen = sizeof(pData->adjust);
                    wrlen = write( dataFd, &pData->adjust, fieldlen );
                    break;
                case 6:
                    fieldlen = sizeof(pData->units);
                    wrlen = write( dataFd, &pData->units, fieldlen );
                    break;
                case 7:
                    fieldlen = sizeof(pData->basalUnits);
                    wrlen = write( dataFd, &pData->basalUnits, fieldlen );
                default:
                    break;
            }

            if( wrlen != fieldlen )
            {
                retVal = E_DATAFILE_WRITE;
            }
        }
    }
    else
    {
        retVal = E_DATAFILE_NULL;
    }

    return( retVal );
}


int datafile::readLastRecord( unsigned long *pRecno, struct _record *pData )
{
    int retVal = E_DATAFILE_OK;
    off_t recPos;
    off_t newPos;

    if( pData != NULL && pRecno != NULL )
    {
        recPos = (-1) * DATA_RECORD_LENGTH;
        newPos = lseek( dataFd, recPos, SEEK_END );

        if( newPos > 0 )
        {
            retVal = performRead( pData );
            *pRecno = newPos / DATA_RECORD_LENGTH;
        }
        else
        {
            retVal = E_DATAFILE_EMPTY;
        }
    }
    else
    {
        retVal = E_DATAFILE_NULL;
    }

    return( retVal );
}



int datafile::readRecord( unsigned long recno, struct _record *pData )
{
    int retVal = E_DATAFILE_OK;
    off_t recPos;
    off_t newPos;

    if( pData != NULL )
    {
        if( recno > 0 )
        {
            recPos = (recno - 1) * DATA_RECORD_LENGTH;
            newPos = lseek( dataFd, recPos, SEEK_SET );

            if( newPos == recPos )
            {
                retVal = performRead( pData );
            }
            else
            {
                retVal = E_DATAFILE_POSITION;
            }
        }
        else
        {
            retVal = E_DATAFILE_RECNO;
        }
    }
    else
    {
        retVal = E_DATAFILE_NULL;
    }

    return( retVal );
}

int datafile::writeRecord( unsigned long recno, struct _record *pData )
{
    int retVal = E_DATAFILE_OK;
    off_t recPos;
    off_t newPos;

    if( pData != NULL )
    {
        if( recno > 0 )
        {
            recPos = (recno - 1) * DATA_RECORD_LENGTH;
            newPos = lseek( dataFd, recPos, SEEK_SET );

            if( newPos == recPos )
            {
                retVal = performWrite( pData );
            }
            else
            {
                retVal = E_DATAFILE_POSITION;
            }
        }
        else
        {
            retVal = E_DATAFILE_RECNO;
        }
    }
    else
    {
        retVal = E_DATAFILE_NULL;
    }

    return( retVal );

}

int datafile::appendRecord( unsigned long *pRecno, struct _record *pData )
{
    int retVal = E_DATAFILE_OK;
    off_t recPos;
    off_t newPos;

    if( pData != NULL && pRecno != NULL )
    {
        newPos = lseek( dataFd, 0, SEEK_END );

        if( newPos > 0 )
        {
            *pRecno = newPos / DATA_RECORD_LENGTH;
            retVal = performWrite( pData );
        }
        else
        {
            retVal = E_DATAFILE_POSITION;
        }
    }
    else
    {
        retVal = E_DATAFILE_NULL;
    }

    return( retVal );
}

