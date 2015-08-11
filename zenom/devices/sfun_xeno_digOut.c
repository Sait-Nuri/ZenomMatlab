#define S_FUNCTION_NAME  sfun_xeno_digOut
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include <string.h>
#include "tmwtypes.h"
#include "matrix.h"
/*#include "mex.h"*/


#define NUM_OF_PARAMS 3

#define CHANNELARRAY_ARG(S)         (ssGetSFcnParam(S,0))
#define INIT_VALUES(S)             (ssGetSFcnParam(S,1))
#define FINAL_VALUES(S)            (ssGetSFcnParam(S,2))

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
   
   
nChannels  = (int_T)mxGetNumberOfElements( CHANNELARRAY_ARG(S) );/* number of elements burada alınıyor habrin oLA
    
	
/* Single input port of width equal to nChannels */
    if ( !ssSetNumInputPorts(  S, 1 ) ) return;
    ssSetInputPortWidth( S, 0, nChannels);
   /*ssSetInputPortDataType(  S, 0, SS_UINT8 );*/
    
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
   /* uint8_T   channel_no    = CHANNEL_NUMBER(S);
    uint8_T   dac_mode    = DAC_MODE(S);
    real_T   value      =   INIT_VALUE(S);*/
  
    /*uint8_T vec= (uint8_T)VEC(S);*/
   /*int32_T * channel_vec= (int32_T *)(mxGetData(CHANNELARRAY_ARG(S)));
      uint8_T*        init_vec=(uint8_T*)mxGetData(INIT_VALUES(S)) ;
            uint8_T* final_vec=(uint8_T*)mxGetData(FINAL_VALUES(S)); 
 */

   /*int_T ch_len=mxGetM(CHANNELS_TO_USE(S));*/
   
/*
 if (!ssWriteRTWVectParam(S, const_char_*_name, const_void_*_value,
                                   int_data_type_of_value, int_vect_len)){
             return;
          }
*/
   
  /*  if (!ssWriteRTWParamSettings(S,4,
                    
                            
                                      
                                 SSWRITE_VALUE_DTYPE_VECT,"Channel_Vec",
                                 channel_vec,mxGetNumberOfElements(CHANNELARRAY_ARG(S)),DTINFO(SS_INT32,COMPLEX_NO),
                                 


                                 SSWRITE_VALUE_DTYPE_VECT,"init_vec",
                                (uint16_T *)init_vec,mxGetNumberOfElements(CHANNELARRAY_ARG(S)),DTINFO(SS_UINT8, COMPLEX_NO),
                                  
				 SSWRITE_VALUE_DTYPE_VECT,"final_vec",
                                 final_vec,mxGetNumberOfElements(CHANNELARRAY_ARG(S)),DTINFO(SS_UINT8, COMPLEX_NO),
 
                                 SSWRITE_VALUE_DTYPE_NUM,"Vec_Test",
                                 &vec_test,DTINFO(SS_UINT8, COMPLEX_NO)
				)) {
        return; 
    }
   */


    if (!ssWriteRTWParamSettings(S, 3,                        
                                 SSWRITE_VALUE_VECT,"Channel_Vec",
                                 (real_T *)mxGetPr(CHANNELARRAY_ARG(S)), 
                                 mxGetNumberOfElements(CHANNELARRAY_ARG(S)),

                                 SSWRITE_VALUE_VECT,"init_vec",
                                 (real_T *)mxGetPr(INIT_VALUES(S)),
                                 mxGetNumberOfElements(INIT_VALUES(S)),

                                 SSWRITE_VALUE_VECT,"final_vec",
                                 (real_T *)mxGetPr(FINAL_VALUES(S)),
                                 mxGetNumberOfElements(FINAL_VALUES(S))

				
                                 
                                  
                                 )) {
        return; /* An error occurred which will be reported by SL */
    }




                   
 
    /*if (!ssWriteRTWParamSettings(S, 3,
                                 SSWRITE_VALUE_DTYPE_NUM,"channel_no",
                                 &channel_no,DTINFO(SS_UINT8, COMPLEX_NO),
                                 
                                 SSWRITE_VALUE_DTYPE_NUM,"dacmode",
                                 &dac_mode,DTINFO(SS_UINT8, COMPLEX_NO),
                                 
                                 SSWRITE_VALUE_DTYPE_NUM,"initvalue",
                                 &value,DTINFO(SS_DOUBLE, COMPLEX_NO)
                                 )) {
        return; 
    }
   */
}


#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
