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
    int flags = O_RDWR; // |O_SYNC;
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
                        flags = O_RDWR|O_CREAT|O_TRUNC; // |O_SYNC;
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
                flags = O_RDWR; // |O_SYNC;
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


void datafile::resetRec( struct _record *pData )
{
    if( pData != NULL )
    {
        pData->timestamp = (time_t) 0;
        pData->recnum = 0;
        pData->glucose = 0;
        pData->meal = 0;
        pData->carbon10  = 0;
        pData->adjust = 0;
        pData->units = 0;
        pData->basalUnits = 0;
        pData->type = 0;
        pData->actUnits = 0;
        pData->actBasunits = 0;

    }
}

void datafile::dumpRec( struct _record *pData )
{
    if( pData != NULL )
    {
        fprintf(stderr, "timestamp : %lu\n", pData->timestamp );
        fprintf(stderr, "recno ....: %d\n", pData->recnum );
        fprintf(stderr, "glucose ..: %d\n", pData->glucose );
        fprintf(stderr, "meal .....: %d\n", pData->meal );
        fprintf(stderr, "carbon10 .: %d\n", pData->carbon10 );
        fprintf(stderr, "adjust ...: %d\n", pData->adjust );
        fprintf(stderr, "units ....: %d\n", pData->units );
        fprintf(stderr, "basalUnits: %d\n", pData->basalUnits );
        fprintf(stderr, "type .....: %d\n", pData->type );
        fprintf(stderr, "act Units : %d\n", pData->actUnits );
        fprintf(stderr, "bact Units: %d\n", pData->actBasunits );
    }
}



int datafile::performRead( struct _record *pData )
{
    int retVal = E_DATAFILE_OK;
    int fsize = 0;

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
                    break;
                case 8:
                    fieldlen = sizeof(pData->type);
                    rdlen = read( dataFd, &pData->type, fieldlen );
                    break;
                case 9:
                    fieldlen = sizeof(pData->actUnits);
                    rdlen = read( dataFd, &pData->actUnits, fieldlen );
                    break;
                case 10:
                    fieldlen = sizeof(pData->actBasunits);
                    rdlen = read( dataFd, &pData->actBasunits, fieldlen );
                    break;
                default:
                    retVal = E_DATAFILE_FIELDS;
                    break;
            }

            fsize += rdlen;
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
    int fsize = 0;

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
                    break;
                case 8:
                    fieldlen = sizeof(pData->type);
                    wrlen = write( dataFd, &pData->type, fieldlen );
                    break;
                case 9:
                    fieldlen = sizeof(pData->actUnits);
                    wrlen = write( dataFd, &pData->actUnits, fieldlen );
                    break;
                case 10:
                    fieldlen = sizeof(pData->actBasunits);
                    wrlen = write( dataFd, &pData->actBasunits, fieldlen );
                    break;
                default:
                    retVal = E_DATAFILE_FIELDS;
                    break;
            }

            fsize += wrlen;
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


int datafile::readLastRecord( unsigned int *pRecno, struct _record *pData )
{
    int retVal = E_DATAFILE_OK;
    off_t datafileSize;
    off_t readPos;
    off_t newPos;

    if( pData != NULL && pRecno != NULL )
    {
        // go to end of file
        datafileSize = lseek( dataFd, 0L, SEEK_END );

        // last record position is size minus record length
        readPos = datafileSize - DATA_RECORD_LENGTH;

        // if file is empty, pos of last record is less than 0
        if( readPos < 0 )
        {
            readPos = 0;
        }

        // set position in file to begin of last record
        newPos = lseek( dataFd, readPos, SEEK_SET );

        // if file is not empty
        if( datafileSize > 0 )
        {
            if( readPos >= 0 )
            {
                retVal = performRead( pData );

                if( retVal == E_DATAFILE_OK )
                {
                    newPos = lseek( dataFd, 0L, SEEK_CUR );
                    *pRecno = newPos / DATA_RECORD_LENGTH;
                }
                else
                {
                    *pRecno = -1;
                }
            }
            else
            {
                newPos = 0;
                *pRecno = 0;
            }
        }
        else
        {
            //
            *pRecno = 0;
            retVal = E_DATAFILE_EMPTY;
        }
    }
    else
    {
        retVal = E_DATAFILE_NULL;
        *pRecno = -1;
    }

    return( retVal );
}



int datafile::readRecord( unsigned int recno, struct _record *pData )
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

int datafile::writeRecord( unsigned int recno, struct _record *pData )
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

int datafile::appendRecord( unsigned int *pRecno, struct _record *pData )
{
    int retVal = E_DATAFILE_OK;
    off_t recPos;
    off_t newPos;

    if( pData != NULL && pRecno != NULL )
    {
        newPos = lseek( dataFd, 0L, SEEK_END );

        if( newPos >= 0 )
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

