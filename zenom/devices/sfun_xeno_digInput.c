#define S_FUNCTION_NAME  sfun_xeno_digInput
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"

#define NUM_OF_PARAMS 1


#define CHANNELARRAY_ARG(S)        (ssGetSFcnParam(S,0)) 

 
 
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

nChannels  = (int_T)mxGetNumberOfElements( CHANNELARRAY_ARG(S) );/* number of elements burada alınıyor habrin o

  	
 /* Single output port of width equal to nChannels */
    if ( !ssSetNumOutputPorts( S, 1 ) ) return;
    ssSetOutputPortWidth( S, 0, nChannels);
   /* ssSetOutputPortDataType(  S, 0, SS_BOOLEAN );*/
	

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
  
    
    
    if (!ssWriteRTWParamSettings(S, 1,
                                  SSWRITE_VALUE_VECT,"Channel_Vec",
                                 (real_T *)mxGetPr(CHANNELARRAY_ARG(S)), 
                                 mxGetNumberOfElements(CHANNELARRAY_ARG(S))

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
