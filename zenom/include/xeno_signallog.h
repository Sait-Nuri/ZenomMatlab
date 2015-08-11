/*
 * File: capi_BIOSignalLog.h
 * $Revision: 1.1.6.1 $
 * Copyright 1994-2008 The MathWorks, Inc.
 *
 * Abstract:
 *   Header file for Block Signal logging routines using buffers 
 *   of fixed size. See capi_BIOSignalLoh.c for function definitions
 *
 */

#ifndef _XENO_SIGNALLOG_H_
# define _XENO_SIGNALLOG_H_

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "rtw_modelmap.h"

#define MAX_SIGNAL_NUM   20
#define MAX_LOG_POINTS 1000000

/*Signal çok boyutlu ise*/

typedef struct SignalAloneIsArray_tag{

void* reparts[100];
void* imparts[100];

}SignalAloneIsArray;

    
typedef struct rtModelName{
char modelName[81];

}XrtModelName;


/* SignalBuffer structure - Structure to log a Signal */
/* Will be used to store the value of a Block signal as the code executes */
typedef struct SignalBuffer_tag {
    char_T     sigName[81]; /* Signal name                                */
    uint_T     sigIdx;      /* Index in the C-API Array                   */
    //void* im;
    //SignalAloneIsArray*   sigAloneArray; 
    real_T     slope;       /* slope if stored as an integer              */
    real_T     bias;        /* bias if stored as integer                  */
    uint8_T    slDataType;  /* Data Type of the Signal (Enumerated value) */
    double   logFrequency;/* signal logging frequency */ 
    double     logDuration; /* signal logging duration  */
    double     logStartTime;/*  signal log start time           */ 
     uint8_T dataType;
     boolean_T isComplex;
     rtwCAPI_Orientation orientation;
    uint_T  numRows;
    uint_T childId;
    unsigned int updatedIndex;
    int logLoop;
}SignalBuffer;





/* SignalBufferInfo - Structure to log time and Signal Buffers */
/* Will be used to store a number of Block IO signals and book keeeping */
typedef struct SignalBufferInfo_tag {
    //SignalBuffer *sigBufs;      /* Pointer to logged Signal Buffers        */
    //time_T       *time;         /* Time buffer                             */
    uint_T        numSigBufs;   /* Number of Signal Buffers                */
    uint_T        numDataPoints; /* Number of data points logged per buffer */
    uint_T       absNumSigBufs;
}SignalBufferList;



extern void xeno_CreateSignalBuffer(SignalBuffer   *sigBuf,
                                    char_T const   *varName,
                                    uint8_T         slDataType, 
                                    uint_T         *dims, 
                                    uint16_T        dataSize,
                                    boolean_T       isComplex,
                                    int_T           maxSize,
                                    real_T          fSlope,
                                    real_T          fBias,
                                    int8_T          fExp);

extern void xeno_UpdateSignalBuffer(SignalBuffer *sigBuf, 
                                    const void   *data,
                                    uint_T        numDataPoints,
                                    boolean_T     isComplex,uint32_T maxSize);

int xeno_UpdateBlockIOLogging(rtwCAPI_ModelMappingInfo* mmi, time_T *tPtr);

extern void xeno_StartBlockIOLogging(rtwCAPI_ModelMappingInfo *mmi, 
                                     uint32_T         maxSize);
void xeno_StopBlockIOLogging(const char_T *file);
                                     
void xeno_ContinuousLogging(const char_T * file,int i);                                      



#endif /* _CAPI_BIOSIGNALLOG_H */

/* EOF rtwdemo_capi_signallog.h */
