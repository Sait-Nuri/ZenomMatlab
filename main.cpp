/**
 * Zenom - Hard Real-Time Simulation Enviroment
 * @author
 *
 * ZenomMatlab
 *
 *
 */

/*ANSI Headers*/
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>

/*Linux Headers*/
#include <sys/mman.h>
#include <rtdk.h>
#include <signal.h>
#include <sys/io.h>

/** 
 *  ZENOM INCLUDE.
 =================================================*/
#include <controlbase.h>

/** 
 *  MATLAB INCLUDE.
 =================================================*/
#include "rt_nonfinite.h"
#include "rtmodel.h"/*model_name.h dosyası include,(RT_MODEL)*/
#include "rtwtypes.h"/*real_T*/ 
#include "rtw_modelmap.h"/*rtwCAPI_ModelMappingInfo,mdl_info.h yerine*/
#include "rtw_capi.h"/*bio_sig.h ve pt_info.h yerine*/
#include "rt_sim.h"
#include "Q8Lib.h"
#include <cmath>
#include <string>

#define QUOTE1(name) #name
#define QUOTE(name) QUOTE1(name)    /* need to expand name    */

#define EXPAND_CONCAT(name1,name2) name1 ## name2
#define CONCAT(name1,name2) EXPAND_CONCAT(name1,name2)
#define RT_MODEL            CONCAT(MODEL,_rtModel)
#define NAMELENGTH 81

#define XSTR(x)    #x
#define STR(x)     XSTR(x)

extern "C" {

extern RT_MODEL *MODEL();
extern void MdlInitializeSizes(void);
extern void MdlInitializeSampleTimes(void);
extern void MdlStart(void);
extern void MdlOutputs(int_T tid);
extern void MdlUpdate(int_T tid);
extern void MdlTerminate(void);

extern int fd;

};

#define INITIALIZE_SIZES(M)     MdlInitializeSizes()
#define INITIALIZE_SAMPLE_TIMES(M)  MdlInitializeSampleTimes()
#define START(M)            MdlStart()
#define OUTPUTS(M,tid)          MdlOutputs(tid)
#define UPDATED(M,tid)          MdlUpdate(tid)
#define TERMINATE(M)            MdlTerminate()
#define MAX_NAMES_SIZE          81
#define MAX_DATA_SIZE            256
#define NAMELENGTH 81

#define __MATLAB__

extern "C" {

#if NCSTATES > 0
  extern void rt_ODECreateIntegrationData(RTWSolverInfo *si);
  extern void rt_ODEUpdateContinuousStates(RTWSolverInfo *si);

};

# define rt_CreateIntegrationData(S) \
    rt_ODECreateIntegrationData(rtmGetRTWSolverInfo(S));
# define rt_UpdateContinuousStates(S) \
    rt_ODEUpdateContinuousStates(rtmGetRTWSolverInfo(S));
# else
# define rt_CreateIntegrationData(S)  \
      rtsiSetSolverName(rtmGetRTWSolverInfo(S),"FixedStepDiscrete");
# define rt_UpdateContinuousStates(S) \
      rtmSetT(S, rtsiGetSolverStopTime(rtmGetRTWSolverInfo(S)));
#endif      

typedef struct rtTargetParamInfo {
  char modelName[MAX_NAMES_SIZE];
  char blockName[MAX_NAMES_SIZE];
  char paramName[MAX_NAMES_SIZE];
  unsigned int numRows;
  unsigned int numColumns;
  unsigned int dataType;
  unsigned short isComplex;
  rtwCAPI_Orientation dataOrientation;/*scalar,vector,matrix*/
  double dataValue[MAX_DATA_SIZE];
} XrtTargetParamInfo;

typedef struct rtTargetSignalInfo{
    char blockName[MAX_NAMES_SIZE];
    char signalName[MAX_NAMES_SIZE];
    unsigned int numRows;
    unsigned int numColumns;
    unsigned int dataType;   
    unsigned short isComplex; 
    rtwCAPI_Orientation dataOrientation;/*scalar,vector,matrix*/
    double* dataValue;
}XrtTargetSignalInfo;

class ZenomMatlab : public ControlBase
{
public:

    // ----- User Functions -----
    // This functions need to be implemented by the user.
    int initialize();
    int start();
    int doloop();
    int stop();
    int terminate();

private:
    rtwCAPI_ModelMappingInfo* mmi;
    RT_MODEL *rtM;
    rtwCAPI_Signals const *sig;
    rtwCAPI_BlockParameters const *bp;
    rtwCAPI_ModelParameters const *mp;
    XrtTargetParamInfo *xrtpi;
    XrtTargetSignalInfo *xtsi;
    real_T tnext;

    void strncpyz(char *dest, const char *src, size_t n);
    void init_xenomai();
    char* concatParamName(char * blockName, char * paramName);
    int_T Xrt_GetParameterInfo(void *rtw_mpi);
    int Xrt_GetSignalInfo(rtwCAPI_ModelMappingInfo* mmi);
    int_T Xrt_SetParameterInfo(rtwCAPI_ModelMappingInfo* mmi);

    // ----- Log Variables -----


    // ----- Control Variables -----

 };

/**
 * This function is called when the control program is loaded to zenom.
 * Use this function to register control parameters, to register log variables
 * and to initialize control parameters.
 *
 * @return Return non-zero to indicate an error.
 */
int ZenomMatlab::initialize()
{
    
    int paramIdx, sigIdx;
    int nBlockParams, nSignals;
    int fd;
    const char* status;
    
    
    /* Here is where Q8 dirver is  loaded to kernel space */
    /* 
    system(STR(sudo insmod DQ8));
    fd = rt_dev_open(DEV_NAME, O_RDWR);

    if(fd < 0)
        fprintf(stderr, "target:Q8 device open error!\n");
    */
    
    init_xenomai();

    rtM = MODEL();

    if (rtmGetErrorStatus(rtM) != NULL) {
        (void)fprintf(stderr,"Error during model registration: %s\n",
                      rtmGetErrorStatus(rtM));
        exit(EXIT_FAILURE);
    }
    
    MdlInitializeSizes();
    MdlInitializeSampleTimes();

    status = rt_SimInitTimingEngine(rtmGetNumSampleTimes(rtM),
                                    rtmGetStepSize(rtM),
                                    rtmGetSampleTimePtr(rtM),
                                    rtmGetOffsetTimePtr(rtM),
                                    rtmGetSampleHitPtr(rtM),
                                    rtmGetSampleTimeTaskIDPtr(rtM),
                                    rtmGetTStart(rtM),
                                    &rtmGetSimTimeStep(rtM),
                                    &rtmGetTimingData(rtM));


    if (status != NULL) {
        (void)fprintf(stderr, "Failed to initialize sample time engine: %s\n", status);
        exit(EXIT_FAILURE);
    }

    rt_CreateIntegrationData(rtM);

    mmi = &(rtmGetDataMapInfo(rtM).mmi);

    if (mmi!=NULL){
        //exception here 
    }    

    bp = rtwCAPI_GetBlockParameters(mmi);
    sig = rtwCAPI_GetSignals(mmi);

    nBlockParams = rtwCAPI_GetNumBlockParameters(mmi);
    nSignals = rtwCAPI_GetNumSignals(mmi);

    xrtpi = new XrtTargetParamInfo[nBlockParams];
    xtsi = new XrtTargetSignalInfo[nSignals];

    /** Get parameter and register */      
    
    Xrt_GetParameterInfo(mmi);
    Xrt_GetSignalInfo(mmi);

    /**************** ZENOM PART  ***************/
    
    for(paramIdx = 0; paramIdx < rtwCAPI_GetNumBlockParameters(mmi); paramIdx++){
        
        std::string paramName(xrtpi[paramIdx].paramName);

        for (std::size_t found = paramName.find_first_of(" "); 
            found != std::string::npos ; 
            found = paramName.find_first_of(" "))
        {
            paramName.erase (found);
        }

        registerControlVariable( xrtpi[paramIdx].dataValue, 
                                 paramName, 
                                 xrtpi[paramIdx].numRows, 
                                 xrtpi[paramIdx].numColumns );
    }

    for (sigIdx = 0; sigIdx < rtwCAPI_GetNumSignals(mmi); ++sigIdx){

        std::string sigName(xtsi[sigIdx].signalName);  

        for (std::size_t found = sigName.find_first_of(" "); 
            found != std::string::npos ; 
            found = sigName.find_first_of(" "))
        {
            sigName.erase (found);
        }

        registerLogVariable(xtsi[sigIdx].dataValue,
                        sigName,
                        xtsi[sigIdx].numRows,
                        xtsi[sigIdx].numColumns);
    }

    setFrequency( (double)rtmGetStepSize(rtM) );
    setDuration ( (double)rtmGetTFinal(rtM) );

    fprintf(stderr, "init done!");
    
    return 0;
}

/**
 * This function is called when the START button is pushed from zenom.
 *
 * @return If you return 0, the control starts and the doloop() function is
 * called periodically. If you return nonzero, the control will not start.
 */
int ZenomMatlab::start()
{
    
    fprintf(stderr, "\n** starting the model **\n");

    START(rtM);

    if (rtmGetErrorStatus(rtM) != NULL) {
        fprintf(stderr, "Failed in target initialization.\n");
        TERMINATE(rtM); 
        return 0;
    }

    fprintf(stderr, "starting done!\n");
    
    return 0;
}


/**
 * This function is called periodically (as specified by the control frequency).
 * The useful functions that you can call used in doloop() are listed below.
 *
 * frequency()          returns frequency of simulation.
 * period()             returns period of simulation.
 * duration()           returns duration of simulation.
 * simTicks()           returns elapsed simulation ticks.
 * simTimeInNano()      returns elapsed simulation time in nano seconds.
 * simTimeInMiliSec()   returns elapsed simulation time in miliseconds.
 * simTimeInSec()       returns elapsed simulation time in seconds.
 * overruns()           returns the count of overruns.
 *
 * @return If you return 0, the control will continue to execute. If you return
 * nonzero, the control will abort and stop() function will be called.
 */
int ZenomMatlab::doloop()
{

    tnext = rt_SimGetNextSampleHit();
    rtsiSetSolverStopTime(rtmGetRTWSolverInfo(rtM),tnext);

    OUTPUTS(rtM, 0);

    //rtExtModeSingleTaskUpload(rtM);

    UPDATED(rtM, 0);

    rt_SimUpdateDiscreteTaskSampleHits(rtmGetNumSampleTimes(rtM),
                                       rtmGetTimingData(rtM),
                                       rtmGetSampleHitPtr(rtM),
                                       rtmGetTPtr(rtM));

    if (rtmGetSampleTime(rtM, 0) == CONTINUOUS_SAMPLE_TIME) {
        rt_UpdateContinuousStates(rtM);
    }

    mmi = &(rtmGetDataMapInfo(rtM).mmi);

    Xrt_SetParameterInfo(mmi);

    return 0;
}


/**
 * Called when a timed run ends or the STOP button is pushed from zenom.
 *
 * @return Return non-zero to indicate an error.
 */
int ZenomMatlab::stop()
{


    return 0;
}


/**
 * This function is called when the control is unloaded. It happens when
 * the user loads a new control program or exits.
 *
 * @return Return non-zero to indicate an error.
 */
int ZenomMatlab::terminate()
{
    // if (fd >= 0){
        
    //     Q8Close(fd);        
    //     printf("q8 is closing...\n");
    // }

    TERMINATE(rtM);

    return 0;
}


/**
 * The main function starts the control program
 */
int main( int argc, char *argv[] )
{
    ZenomMatlab c;
    c.run( argc, argv );

    return 0;
}

void ZenomMatlab::init_xenomai() {
 
  mlockall(MCL_CURRENT|MCL_FUTURE);
  rt_print_auto_init(1);
}

void ZenomMatlab::strncpyz(char *dest, const char *src, size_t n)
{
  if (src != NULL) {
    strncpy(dest, src, n);
    n = strlen(src);
  } else
    n = 0;
    
  dest[n] = '\0';
}

char* ZenomMatlab::concatParamName(char * blockName, char * paramName){
    int model_len = strlen("deneme");

    char* name = new char[MAX_NAMES_SIZE];

    memset(name, '\0', MAX_NAMES_SIZE);

    strcat(name, &blockName[model_len+1]);
    strcat(name, "-");
    strcat(name, paramName);
    
    return name;
}

#define RT_GET_SIG_INFO_IF(rtcase, rttype) \
    case rtcase: \
             \
        if (xtsi[sigIdx].isComplex==0) { \
            rttype *param = (rttype *) paramAddress; \
            xtsi[sigIdx].dataValue = (double*)param; \
        } \
    break;  

int ZenomMatlab::Xrt_GetSignalInfo(rtwCAPI_ModelMappingInfo* mmi){

    const rtwCAPI_Signals*         signals;
    const rtwCAPI_DataTypeMap*     dataTypeMap;
    const rtwCAPI_DimensionMap*    dimMap;
    const uint_T*                  dimArray;
    void**                         dataAddrMap;
    void* paramAddress;
    uint_T sigIdx;
    uint_T nSignals;

    uint_T        addrIdx;
    uint16_T      dataTypeIdx;
    uint16_T      dimIndex;
    uint16_T      fxpMapIdx;
    uint8_T       slDataID;

    unsigned short       isComplex;
    uint8_T              numDims;
    uint_T               dimArrayIdx;

    rtwCAPI_Orientation  orientation;
    uint_T            actualDimensions[10];
    uint_T numRows;
    uint_T numColumns;  
    uint_T rowIdx;
    uint_T colIdx;

    const char_T* SigName;
    const char_T* blockName;
    const char_T* cDataName;

    nSignals = rtwCAPI_GetNumSignals(mmi);

    signals = rtwCAPI_GetSignals(mmi);

    dataTypeMap = rtwCAPI_GetDataTypeMap(mmi);

    for(sigIdx=0; sigIdx < nSignals; sigIdx++){
        strncpyz(xtsi[sigIdx].blockName, rtwCAPI_GetSignalBlockPath(signals,sigIdx), MAX_NAMES_SIZE);
        strncpyz(xtsi[sigIdx].signalName, rtwCAPI_GetSignalName(signals,sigIdx), MAX_NAMES_SIZE);
        
        dataTypeIdx = rtwCAPI_GetSignalDataTypeIdx(signals, sigIdx);
        slDataID    = rtwCAPI_GetDataTypeSLId(dataTypeMap, dataTypeIdx);
        xtsi[sigIdx].dataType = slDataID;
    
        cDataName   = rtwCAPI_GetDataTypeCName(dataTypeMap, dataTypeIdx);
        xtsi[sigIdx].isComplex = rtwCAPI_GetDataIsComplex(dataTypeMap,dataTypeIdx);
    }

    
    //Get Dimension

    dimMap   = rtwCAPI_GetDimensionMap(mmi);
    dimArray = rtwCAPI_GetDimensionArray(mmi);

    if ((dimMap == NULL) || (dimArray == NULL)) 
        exit(EXIT_FAILURE);


    for(sigIdx=0; sigIdx < nSignals; sigIdx++){
        dimIndex    = rtwCAPI_GetBlockParameterDimensionIdx(signals, sigIdx);
        numDims     = rtwCAPI_GetNumDims(dimMap, dimIndex); 

        dimArrayIdx = rtwCAPI_GetDimArrayIndex(dimMap, dimIndex);
        orientation = rtwCAPI_GetOrientation(dimMap, dimIndex);

        xtsi[sigIdx].dataOrientation = orientation;

        for(int idx=0; idx < numDims; idx++) {
            actualDimensions[idx] = dimArray[dimArrayIdx + idx];    
        }

        xtsi[sigIdx].numRows = actualDimensions[0];
        xtsi[sigIdx].numColumns = actualDimensions[1];
    }


    for(sigIdx=0; sigIdx < nSignals; sigIdx++){
        dataAddrMap = rtwCAPI_GetDataAddressMap(mmi);
        addrIdx     = rtwCAPI_GetBlockParameterAddrIdx(signals, sigIdx);
        paramAddress= (void *) rtwCAPI_GetDataAddress(dataAddrMap, addrIdx); 
    
        uint_T numPages = 0;
        if (numDims == 3) 
            numPages = actualDimensions[2];

        switch ( xtsi[sigIdx].dataType) {
            RT_GET_SIG_INFO_IF(SS_DOUBLE, real_T)
            RT_GET_SIG_INFO_IF(SS_SINGLE, real32_T)
            RT_GET_SIG_INFO_IF(SS_INT8, int8_T)
            RT_GET_SIG_INFO_IF(SS_UINT8, uint8_T)
            RT_GET_SIG_INFO_IF(SS_INT16, int16_T)
            RT_GET_SIG_INFO_IF(SS_UINT16, uint16_T)
            RT_GET_SIG_INFO_IF(SS_INT32, int32_T)
            RT_GET_SIG_INFO_IF(SS_UINT32, uint32_T)
            RT_GET_SIG_INFO_IF(SS_BOOLEAN, boolean_T)
      
            default:
            {return -1;}

        }//switch
    }

    return 0;
}

#define RT_GET_PARAM_INFO_IF(rtcase, rttype) \
    case rtcase: \
             \
        if (xrtpi[paramIdx].isComplex==0) { \
            rttype *param = (rttype *) paramAddress;\
            switch ( xrtpi[paramIdx].dataOrientation) { \
                case rtwCAPI_SCALAR: {\
                    xrtpi[paramIdx].dataValue[0] = (double)*param;}\
                    break; \
                case rtwCAPI_VECTOR: \
                    for (colIdx = 0; colIdx < xrtpi[paramIdx].numRows*xrtpi[paramIdx].numColumns; colIdx++) { \
                        xrtpi[paramIdx].dataValue[colIdx] = (double)param[colIdx]; \
                        \
                    } \
                    break; \
                case rtwCAPI_MATRIX_ROW_MAJOR: \
                    for(rowIdx = 0; rowIdx < xrtpi[paramIdx].numRows; rowIdx++){ \
                        for(colIdx = 0; colIdx < xrtpi[paramIdx].numColumns; colIdx++) { \
                            xrtpi[paramIdx].dataValue[rowIdx*xrtpi[paramIdx].numColumns + colIdx] = param[rowIdx*xrtpi[paramIdx].numColumns+colIdx]; \
                        } \
                    } \
                    break; \
                case rtwCAPI_MATRIX_COL_MAJOR: \
                    for(rowIdx = 0; rowIdx <  xrtpi[paramIdx].numRows; rowIdx++)  { \
                         for(colIdx = 0; colIdx < xrtpi[paramIdx].numColumns; colIdx++){ \
                            xrtpi[paramIdx].dataValue[colIdx*xrtpi[paramIdx].numRows + rowIdx] = param[colIdx*xrtpi[paramIdx].numRows+ rowIdx]; \
                        } \
                    } \
                    break; \
                                case rtwCAPI_MATRIX_COL_MAJOR_ND: \
                    break; \
                default: \
                    return(1); \
            } \
        } \
    break;  

int_T ZenomMatlab::Xrt_GetParameterInfo(void *rtw_mpi){

  
    rtwCAPI_ModelMappingInfo*      capiMap; 
    const rtwCAPI_BlockParameters* blockParams;
    const rtwCAPI_DataTypeMap*     dataTypeMap;
    const rtwCAPI_DimensionMap*    dimMap;
    const rtwCAPI_FixPtMap*        fxpMap;
    const uint_T*                  dimArray;
    void**                         dataAddrMap;
    void* paramAddress;
    uint_T paramIdx;
    
   
 capiMap= (rtwCAPI_ModelMappingInfo *)rtw_mpi;

    const char_T* paramName;
    const char_T* blockName;
    uint_T        addrIdx;
    uint16_T      dataTypeIdx;
    uint16_T      dimIndex;
    uint16_T      fxpMapIdx;
    uint8_T       slDataID;

   
    const char_T*        cDataName;
    unsigned short       isComplex;
    uint8_T              numDims;
    uint_T               dimArrayIdx;
    rtwCAPI_Orientation  orientation;
    uint_T            actualDimensions[10];
    uint_T numRows;
    uint_T numColumns;  

    int    idx;
    real_T slope = 1.0;
    real_T bias  = 0.0;
    unsigned short modParamFlag = 0;

                    
    uint_T rowIdx;
    uint_T colIdx;
    uint_T pageIdx;
  uint_T nBlockParams;
  
 
      
   nBlockParams=rtwCAPI_GetNumBlockParameters(capiMap);
   
/******************** GET Model Name **************************/ 
  
    for(paramIdx=0;paramIdx<nBlockParams;paramIdx++)
    strncpyz(xrtpi[paramIdx].modelName, STR(MODEL), MAX_NAMES_SIZE);
/**************************************************************/

  /* Assert the parameter index is less than total number of parameters */
 
    for(paramIdx=0;paramIdx<nBlockParams;paramIdx++)
    assert(paramIdx < rtwCAPI_GetNumBlockParameters(capiMap));

    /* Get blockParams, an array of rtwCAPI_ModelParameters structure  */
    blockParams = rtwCAPI_GetBlockParameters(capiMap);
    if (blockParams == NULL) 
        exit(EXIT_FAILURE);
  
/********************GET Block Name**********************************************************/
  for(paramIdx=0;paramIdx<nBlockParams;paramIdx++)      
 strncpyz(xrtpi[paramIdx].blockName,rtwCAPI_GetBlockParameterBlockPath(blockParams,paramIdx), MAX_NAMES_SIZE);
/********************************************************************************************/
 
   

/***********************Get Parameters Names*****************************************************/
for(paramIdx=0;paramIdx<nBlockParams;paramIdx++) {
strncpyz(xrtpi[paramIdx].paramName,rtwCAPI_GetBlockParameterName(blockParams, paramIdx), MAX_NAMES_SIZE);
strncpyz(xrtpi[paramIdx].blockName,rtwCAPI_GetBlockParameterBlockPath(blockParams, paramIdx), MAX_NAMES_SIZE);
}
/**********************************************************************************************/


    /* Get Data Type attributes of the Model Parameter                       */
    /* Accessing the data type information from capiMap is a 3 step process  *
     * 1) Get the dataTypeMap Array from capiMap.                            *
     * 2) Get the index into the above array, dataTypeIdx, from ModelParameter*
     *    structure. The index will point to a rtwCAPI_DataTypeMap structure.*
     * 3) From the structure, get the member values of the structure, namely *
     *       o Simulink Data ID, can be one of the enumerated value          *
     *           SS_DOUBLE, SS_SINGLE, SS_INT8, SS_UINT8, SS_INT16, SS_UINT16*
     *            , SS_INT32, SS_UINT32, SS_BOOLEAN                          *
     *       o Complexity                                                    *
     *       o Data Size                                                     *
     *    For complete structure see matlabroot/rtw/c/src/rtw_capi.h         */

    dataTypeMap = rtwCAPI_GetDataTypeMap(capiMap);
    
    if (dataTypeMap == NULL)
        exit(EXIT_FAILURE);
    
      for(paramIdx=0;paramIdx<nBlockParams;paramIdx++){ 
      
    dataTypeIdx = rtwCAPI_GetBlockParameterDataTypeIdx(blockParams, paramIdx);
    slDataID    = rtwCAPI_GetDataTypeSLId(dataTypeMap, dataTypeIdx);

 /*************GET DATA TYPEs*************************************************/

 xrtpi[paramIdx].dataType=slDataID;        
/***************************************************************************/
    cDataName   = rtwCAPI_GetDataTypeCName(dataTypeMap,dataTypeIdx);
   xrtpi[paramIdx].isComplex   = rtwCAPI_GetDataIsComplex(dataTypeMap,dataTypeIdx);}

    /* Get Dimensions of the Model Parameter                                 */
    /* Accessing the dimension from capiMap is a 4 step process              *
     * 1) Get the dimMap array, an array of CAPI_DimensionMap struct         *
     * 2) Get the dimArray array, an array of all the dimensions in the Map  *
     * 3) Get the index into the dimMap array (dimIdx) from ModelParameter   *
     *    structure. the index will point to rtwCAPI_Dimension structure     *
     * 4) From the rtwCAPI_Dimension structure, get the following information*
     *       o orientation (scalar | vector | matrix)                        *
     *       o Number of dimensions (numDims)                                *
     *       o index into the dimArray (dimArrayIdx)                         *
     * 5) Using numDims and the dimArrayIdx, get the actual dimensions from  *
     *    the dimArray                                                       *
     *       uint_T ActualDims[numDims] = {dimArray[dimArrayIdx],            *
     *                                     dimArray[dimArrayIdx+1],          *
     *                     ...                                   *
     *                     dimArray[dimArrayIdx+(numDims-1)]}    *
     *    For e.g, scalar and 2-D parameters will have numDims = 2, So       *
     *       uint_T ActualDims[2] = {dimArray[dimArrayIdx],                  *
     *                               dimArray[dimArrayIdx +1]}               */
    
    
     //uint8_T             nDims  = rtwCAPI_GetNumDims(dimMap, dimIdx);
    //    uint_T              dIndex = rtwCAPI_GetDimArrayIndex(dimMap, dimIdx);
    
      //uint_T   addrIdx  =  rtwCAPI_GetSignalAddrIdx(bioSig,sigIdx);xrt_HostInterfaceTask
       // uint16_T dTypeIdx =  rtwCAPI_GetSignalDataTypeIdx(bioSig,sigIdx);
       // uint16_T dimIdx   =  rtwCAPI_GetSignalDimensionIdx(bioSig,sigIdx);
    
    
    
    
    dimMap   = rtwCAPI_GetDimensionMap(capiMap);
    dimArray = rtwCAPI_GetDimensionArray(capiMap);

    if ((dimMap == NULL) || (dimArray == NULL))
        exit(EXIT_FAILURE);



for(paramIdx=0;paramIdx<nBlockParams;paramIdx++) {

    dimIndex    = rtwCAPI_GetBlockParameterDimensionIdx(blockParams, paramIdx);
    numDims     = rtwCAPI_GetNumDims(dimMap, dimIndex); 
    
    //actualDimensions=(uint_T *) calloc(numDims, sizeof(uint_T));
    
    dimArrayIdx = rtwCAPI_GetDimArrayIndex(dimMap, dimIndex);
    orientation = rtwCAPI_GetOrientation(dimMap, dimIndex);
  /******************************* GET ORIENTATIONs******************************/
   xrtpi[paramIdx].dataOrientation=orientation;
   for(idx=0; idx < numDims; idx++) {
        actualDimensions[idx] = dimArray[dimArrayIdx + idx];
         //printf("actdim::%d\n",actualDimensions[idx]);
    }
   xrtpi[paramIdx].numRows=actualDimensions[0];
   xrtpi[paramIdx].numColumns=actualDimensions[1];
   
    //free(actualDimensions);
    //printf("orient::%d,%d\n",xrtpi[paramIdx].dataOrientation,paramIdx);
    // printf(" t param numRows,cols::%d %d\n",xrtpi[paramIdx].numRows,xrtpi[paramIdx].numColumns);
   }
  /*****************************************************************************/



          

    /* Get fixed-point information of the parameter */
    fxpMap = rtwCAPI_GetFixPtMap(capiMap);
    if (fxpMap == NULL)
        exit(EXIT_FAILURE);
    
    fxpMapIdx = rtwCAPI_GetBlockParameterFixPtIdx(blockParams, paramIdx);
    if(fxpMapIdx > 0) {
        /* Only Fixed-point parameters have fxpMapIdx > 0 */
        real_T fracslope = rtwCAPI_GetFxpFracSlope(fxpMap,fxpMapIdx);
        int8_T expt      = rtwCAPI_GetFxpExponent(fxpMap,fxpMapIdx);
        
        /* slope = fractional slope * 2^exponent 
         * fractional slope is also called Slope Adjustment Factor. 
         * Type "help fixdt" in MATLAB command window for more information
         * on fixed point data types  
         */
        
        slope = fracslope*pow(2.0,expt);
        bias  = rtwCAPI_GetFxpBias(fxpMap,fxpMapIdx);
    }

 
 for(paramIdx=0;paramIdx<nBlockParams;paramIdx++) {
  /* Get the address to this paxeno_Starrameter                     */
   dataAddrMap = rtwCAPI_GetDataAddressMap(capiMap);
    addrIdx     = rtwCAPI_GetBlockParameterAddrIdx(blockParams,paramIdx);
    paramAddress= (void *) rtwCAPI_GetDataAddress(dataAddrMap,addrIdx); 



uint_T numPages = 0;
    if (numDims == 3) numPages = actualDimensions[2];
    


  
   
  switch ( xrtpi[paramIdx].dataType) {


     RT_GET_PARAM_INFO_IF(SS_DOUBLE, real_T)
     RT_GET_PARAM_INFO_IF(SS_SINGLE, real32_T)
      RT_GET_PARAM_INFO_IF(SS_INT8, int8_T)
      RT_GET_PARAM_INFO_IF(SS_UINT8, uint8_T)
      RT_GET_PARAM_INFO_IF(SS_INT16, int16_T)
      RT_GET_PARAM_INFO_IF(SS_UINT16, uint16_T)
      RT_GET_PARAM_INFO_IF(SS_INT32, int32_T)
      RT_GET_PARAM_INFO_IF(SS_UINT32, uint32_T)
      RT_GET_PARAM_INFO_IF(SS_BOOLEAN, boolean_T)
      
    default:
        return -1;
    }//switch
}//for



 //free(actualDimensions);
 return 0;
}//END 

#define RT_SET_PARAM_INFO_IF(rtcase, rttype) \
    case rtcase: \
             \
        if (xrtpi[paramIdx].isComplex==0) { \
            rttype *param = (rttype *) paramAddress;\
             /*real_T *newParam  = (real_T *) (_newVal);*/\
                    \
            switch (xrtpi[paramIdx].dataOrientation) { \
                case rtwCAPI_SCALAR: \
                     /**param = *newParam;*/\
                      *param= xrtpi[paramIdx].dataValue[0]; \
                     \
                  break;\
                case rtwCAPI_VECTOR: \
                    for (colIdx = 0; colIdx < xrtpi[paramIdx].numRows*xrtpi[paramIdx].numColumns; colIdx++) { \
                        /*param[rowIdx] = newParam[rowIdx]; */\
                          param[colIdx] = xrtpi[paramIdx].dataValue[colIdx] ;\
                    } \
                    break; \
                case rtwCAPI_MATRIX_ROW_MAJOR: \
                    for(rowIdx = 0; rowIdx < xrtpi[paramIdx].numRows; rowIdx++){ \
                        for(colIdx = 0; colIdx <xrtpi[paramIdx].numColumns; colIdx++) { \
                            /*param[rowIdx*xrtpi[paramIdx].numColumns + colIdx] = newParam[rowIdx*xrtpi[paramIdx].numColumns + colIdx];*/ \
                             param[rowIdx*xrtpi[paramIdx].numColumns + colIdx]= xrtpi[paramIdx].dataValue[rowIdx*xrtpi[paramIdx].numColumns + colIdx];\
                        } \
                    } \
                    break; \
                case rtwCAPI_MATRIX_COL_MAJOR: \
                    for(rowIdx = 0; rowIdx < xrtpi[paramIdx].numRows; rowIdx++)  { \
                         for(colIdx = 0; colIdx < xrtpi[paramIdx].numColumns; colIdx++){ \
                            /*param[colIdx*xrtpi[paramIdx].numRows + rowIdx] = newParam[colIdx*xrtpi[paramIdx].numRows + rowIdx];*/ \
                            param[colIdx*xrtpi[paramIdx].numRows + rowIdx]= xrtpi[paramIdx].dataValue[colIdx*xrtpi[paramIdx].numRows+ rowIdx]; \
                        } \
                    } \
                    break; \
                                case rtwCAPI_MATRIX_COL_MAJOR_ND: \
                    break; \
                default: \
                    return(1); \
            } \
        } \
        break;

int_T ZenomMatlab::Xrt_SetParameterInfo(rtwCAPI_ModelMappingInfo* mmi){
    
    const rtwCAPI_BlockParameters* blockParams;
    void**                         dataAddrMap;
    void*                          paramAddress;

    uint_T nRows;
    uint_T nCols;
    uint_T nParams;
    uint_T rowIdx;
    uint_T colIdx;
    uint_T addrIdx;
    uint_T paramIdx;

    int nBlockParams = rtwCAPI_GetNumBlockParameters(mmi);
    blockParams = rtwCAPI_GetBlockParameters(mmi);

    // Parametreleri update eder
    for (paramIdx=0; paramIdx < nBlockParams; paramIdx++){

        dataAddrMap  = rtwCAPI_GetDataAddressMap(mmi);
        addrIdx      = rtwCAPI_GetBlockParameterAddrIdx(blockParams, paramIdx);
        paramAddress = (void *) rtwCAPI_GetDataAddress(dataAddrMap, addrIdx);     
   
        
        switch ( xrtpi[paramIdx].dataType){
            RT_SET_PARAM_INFO_IF(SS_DOUBLE, real_T)
            RT_SET_PARAM_INFO_IF(SS_SINGLE, real32_T)
            RT_SET_PARAM_INFO_IF(SS_INT8, int8_T)
            RT_SET_PARAM_INFO_IF(SS_UINT8, uint8_T)
            RT_SET_PARAM_INFO_IF(SS_INT16, int16_T)
            RT_SET_PARAM_INFO_IF(SS_UINT16, uint16_T)
            RT_SET_PARAM_INFO_IF(SS_INT32, int32_T)
            RT_SET_PARAM_INFO_IF(SS_UINT32, uint32_T)
            RT_SET_PARAM_INFO_IF(SS_BOOLEAN, boolean_T)

            default:
                return -1;
        }
    }
}   
}