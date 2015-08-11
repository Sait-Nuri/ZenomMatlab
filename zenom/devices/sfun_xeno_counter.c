#define S_FUNCTION_NAME  sfun_xeno_counter
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#define NUM_OF_PARAMS 3


#define CLOCK_SOURCE(S)        (mxGetScalar(ssGetSFcnParam(S,0)))
#define PERIOD(S)            (mxGetScalar(ssGetSFcnParam(S,1)))
#define CNTRLEN(S)              (mxGetScalar(ssGetSFcnParam(S,2)))


#define VAL_IDX 3 

 
 
static void mdlInitializeSizes(SimStruct *S)
{
/* Set and Check parameter count  */
      
    ssSetNumSFcnParams(S, NUM_OF_PARAMS);
    if (ssGetNumSFcnParams(S) == ssGetSFcnParamsCount(S)) {
        if (ssGetErrorStatus(S) != NULL) {
            return;
        }
    } else {
        return; /* Parameter mismatch will be reported by Simulink */
    }
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) return;
    ssSetSFcnParamTunable(S, 0, SS_PRM_NOT_TUNABLE);
    ssSetSFcnParamTunable(S, 1, SS_PRM_NOT_TUNABLE);
     ssSetSFcnParamTunable(S, 2, SS_PRM_NOT_TUNABLE);
    
	
  
    /* sample times */
    ssSetNumSampleTimes( S, 1 );
    
    /* options */
    ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE); 

}



static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, CONTINUOUS_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);

}


  static void mdlStart(SimStruct *S)
  {
  }




/* Function: mdlOutputs =======================================================
 * Abstract:
 *    In this function, you compute the outputs of your S-function
 *    block.
 */
static void mdlOutputs(SimStruct *S, int_T tid)
{

}



/* Function: mdlTerminate =====================================================
 * Abstract:
 *    In this function, you should perform any actions that are necessary
 *    at the termination of a simulation.  For example, if memory was
 *    allocated in mdlStart, this is the place to free it.
 */
static void mdlTerminate(SimStruct *S)
{
}




 
 
#define MDL_RTW
static void mdlRTW(SimStruct *S)
{
    uint8_T   clock     = CLOCK_SOURCE(S);
    real_T   period    = PERIOD(S);
    uint8_T    cntrlen   = CNTRLEN(S);
  
    
    

 
    if (!ssWriteRTWParamSettings(S, 3,
                                 SSWRITE_VALUE_DTYPE_NUM,"clock",
                                 &clock,DTINFO(SS_UINT8, COMPLEX_NO),
                                 
                                 SSWRITE_VALUE_DTYPE_NUM,"period",
                                 &period,DTINFO(SS_DOUBLE, COMPLEX_NO),
                                 
                                 SSWRITE_VALUE_DTYPE_NUM,"cntrlen",
                                 &cntrlen,DTINFO(SS_UINT8, COMPLEX_NO)
                                 )) {
        return; 
    }
   
}


#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
