#define S_FUNCTION_NAME  sfun_xeno_dac
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include <string.h>
#include "tmwtypes.h"


#define NUM_OF_PARAMS 4


#define  CHANNELARRAY_ARG(S)     (ssGetSFcnParam(S,0))
#define DAC_MODE(S)            (mxGetScalar(ssGetSFcnParam(S,1)))
#define INIT_VALUES(S)               (ssGetSFcnParam(S,2))
#define FINAL_VALUES(S)               (ssGetSFcnParam(S,3))

 
static void mdlInitializeSizes(SimStruct *S)
{


 int_T nChannels;


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
     ssSetSFcnParamTunable(S, 3, SS_PRM_NOT_TUNABLE);

nChannels  = (int_T)mxGetNumberOfElements( CHANNELARRAY_ARG(S) );
	
/* Single input port of width equal to nChannels */
    if ( !ssSetNumInputPorts(  S, 1 ) ) return;
    ssSetInputPortWidth(       S, 0, nChannels );

   ssSetInputPortDirectFeedThrough( S, 0, 1);
  
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
    
   InputPtrsType u0Ptrs = ssGetInputPortSignalPtrs(S,0);
   
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
  uint8_T   dac_mode      = DAC_MODE(S);
 
    if (!ssWriteRTWParamSettings(S, 4,
                                       SSWRITE_VALUE_VECT,"Channel_Vec",
                                 (real_T *)mxGetPr(CHANNELARRAY_ARG(S)), 
                                 mxGetNumberOfElements(CHANNELARRAY_ARG(S)),


                                 SSWRITE_VALUE_DTYPE_NUM,"mode",
                                 &dac_mode,DTINFO(SS_UINT8, COMPLEX_NO),


 

                                 SSWRITE_VALUE_VECT,"init_vec",
                                 (real_T *)mxGetPr(INIT_VALUES(S)),
                                 mxGetNumberOfElements(INIT_VALUES(S)),

                                 SSWRITE_VALUE_VECT,"final_vec",
                                 (real_T *)mxGetPr(FINAL_VALUES(S)),
                                 mxGetNumberOfElements(FINAL_VALUES(S))

                                 )) {
        return; 
    }
   
}


#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
