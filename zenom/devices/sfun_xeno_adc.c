#define S_FUNCTION_NAME  sfun_xeno_adc
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"

#define NUM_OF_PARAMS 2


#define CHANNELARRAY_ARG(S)        (ssGetSFcnParam(S,0)) 
#define MODE(S)            (mxGetScalar(ssGetSFcnParam(S,1))) 


 
 
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
  
nChannels  = (int_T)mxGetNumberOfElements( CHANNELARRAY_ARG(S) );
  	
 /* Single output port of width equal to nChannels */
    if ( !ssSetNumOutputPorts( S, 1 ) ) return;
    ssSetOutputPortWidth( S, 0, nChannels);
	

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
    
    float     *y = ssGetOutputPortSignal(S,0);
    y[0] = 0;
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

    uint8_T   mode      = MODE(S);
    
    if (!ssWriteRTWParamSettings(S, 2,
                                 SSWRITE_VALUE_VECT,"Channel_Vec",
                                 (real_T *)mxGetPr(CHANNELARRAY_ARG(S)), 
                                 mxGetNumberOfElements(CHANNELARRAY_ARG(S)),
                                 
                                 SSWRITE_VALUE_DTYPE_NUM,"mode",
                                 &mode,DTINFO(SS_UINT8, COMPLEX_NO)

                                 )) 
    {
        return; 
    }
   
}


#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
