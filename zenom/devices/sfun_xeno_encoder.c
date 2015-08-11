#define S_FUNCTION_NAME  sfun_xeno_encoder
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"

#define NUM_OF_PARAMS 8


#define CHANNELARRAY_ARG(S)         (ssGetSFcnParam(S,0))
#define COUNT_MODE(S)            (mxGetScalar(ssGetSFcnParam(S,1)))
#define BCD_MODE(S)               (mxGetScalar(ssGetSFcnParam(S,2)))
#define QUAD_MODE(S)              (mxGetScalar(ssGetSFcnParam(S,3)))
#define INDEX_ENABLE(S)           (mxGetScalar(ssGetSFcnParam(S,4)))
#define INDEX_POLARITY(S)        (mxGetScalar(ssGetSFcnParam(S,5)))
#define RESET(S)                 (mxGetScalar(ssGetSFcnParam(S,6)))
#define SET_VALUE(S)                (ssGetSFcnParam(S,7))
/*#define INIT_VAL(S)                 (ssGetSFcnParam(S,8))
#define FINAL_VAL(S)                (ssGetSFcnParam(S,9))*/
 
 
static void mdlInitializeSizes(SimStruct *S)
{

  int_T nChannels;

/* Set and Check parameter count  */
      
    ssSetNumSFcnParams(S, NUM_OF_PARAMS);
    if (ssGetNumSFcnParams(S) == ssGetSFcnParamsCount(S)) {
        if (ssGetErrorStatus(S) != NULL) {
            return;
        }
    } else 
      return; /* Parameter mismatch will be reported by Simulink */
  /*  
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) return; 
    ssSetSFcnParamTunable(S, 0, SS_PRM_NOT_TUNABLE);
     ssSetSFcnParamTunable(S, 1, SS_PRM_NOT_TUNABLE);
     ssSetSFcnParamTunable(S, 2, SS_PRM_NOT_TUNABLE);
     ssSetSFcnParamTunable(S, 3, SS_PRM_NOT_TUNABLE);
     ssSetSFcnParamTunable(S, 4, SS_PRM_NOT_TUNABLE);   
	 ssSetSFcnParamTunable(S, 5, SS_PRM_NOT_TUNABLE); 
	 ssSetSFcnParamTunable(S, 6, SS_PRM_NOT_TUNABLE); 
     ssSetSFcnParamTunable(S, 7, SS_PRM_NOT_TUNABLE);
     ssSetSFcnParamTunable(S, 8, SS_PRM_NOT_TUNABLE); 
    */
      int_T i;
    for (i=0; i<NUM_OF_PARAMS; i++){
        ssSetSFcnParamNotTunable (S, i);
    }
nChannels  = (int_T)mxGetNumberOfElements( CHANNELARRAY_ARG(S) );


	
 /* Single output port of width equal to nChannels */
    if ( !ssSetNumOutputPorts( S, 1 ) ) return;
    ssSetOutputPortWidth( S, 0,nChannels );
	
   if ( !ssSetNumInputPorts(  S, 1 ) ) return;
    ssSetInputPortWidth( S, 0, 1);
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
    
    float       *y = ssGetOutputPortSignal(S,0);
    y[0] = 0;
     
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

    uint8_T   count_mode    = COUNT_MODE(S);
    uint8_T   bcd_mode      = BCD_MODE(S);
    uint8_T   quad_mode     = QUAD_MODE(S);
    uint8_T   indexEnable   = INDEX_ENABLE(S);
    uint8_T   indexPolarity = INDEX_POLARITY(S);
    uint8_T   reset         = RESET(S);
 
 
    if (!ssWriteRTWParamSettings(S, 8,
                              
				 SSWRITE_VALUE_VECT,"Channel_Vec",
                                 (real_T *)mxGetPr(CHANNELARRAY_ARG(S)), 
                                 mxGetNumberOfElements(CHANNELARRAY_ARG(S)),
								
			         SSWRITE_VALUE_DTYPE_NUM,"count_mode",
                                 &count_mode,DTINFO(SS_UINT8, COMPLEX_NO),

                                 SSWRITE_VALUE_DTYPE_NUM,"bcd_mode",
                                 &bcd_mode,DTINFO(SS_UINT8, COMPLEX_NO),

                                 SSWRITE_VALUE_DTYPE_NUM,"quad_mode",
                                 &quad_mode,DTINFO(SS_UINT8, COMPLEX_NO),
                                 
                                 SSWRITE_VALUE_DTYPE_NUM,"index",
                                 &indexEnable,DTINFO(SS_UINT8, COMPLEX_NO),
                                 
                                 SSWRITE_VALUE_DTYPE_NUM,"index_Polarity",
                                 &indexPolarity,DTINFO(SS_UINT8, COMPLEX_NO),
                                 
                                 SSWRITE_VALUE_DTYPE_NUM,"Reset",
                                 &reset,DTINFO(SS_UINT8, COMPLEX_NO),
                                 
                                 SSWRITE_VALUE_VECT,"offset_vec",
                                 (real_T *)mxGetPr(SET_VALUE(S)), 
                                 mxGetNumberOfElements(SET_VALUE(S))

                              
                                 )) {
        return; 
    }
   
}


#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
