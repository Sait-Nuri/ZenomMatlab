/*
 * deneme.c
 *
 * Code generation for model "deneme".
 *
 * Model version              : 1.8
 * Simulink Coder version : 8.6 (R2014a) 27-Dec-2013
 * C source code generated on : Mon Aug 10 17:05:58 2015
 *
 * Target selection: zenom.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */
#include "rt_logging_mmi.h"
#include "deneme_capi.h"
#include "deneme.h"
#include "deneme_private.h"

/* Block signals (auto storage) */
B_deneme_T deneme_B;

/* Continuous states */
X_deneme_T deneme_X;

/* Block states (auto storage) */
DW_deneme_T deneme_DW;

/* Real-time model */
RT_MODEL_deneme_T deneme_M_;
RT_MODEL_deneme_T *const deneme_M = &deneme_M_;

/*
 * This function updates continuous states using the ODE1 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE1_IntgData *id = (ODE1_IntgData *)rtsiGetSolverData(si);
  real_T *f0 = id->f[0];
  int_T i;
  int_T nXc = 1;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);
  rtsiSetdX(si, f0);
  deneme_derivatives();
  rtsiSetT(si, tnew);
  for (i = 0; i < nXc; i++) {
    *x += h * f0[i];
    x++;
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model output function */
void deneme_output(void)
{
  real_T *lastU;
  real_T rtb_Derivative;
  if (rtmIsMajorTimeStep(deneme_M)) {
    /* set solver stop time */
    if (!(deneme_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&deneme_M->solverInfo,
                            ((deneme_M->Timing.clockTickH0 + 1) *
        deneme_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&deneme_M->solverInfo, ((deneme_M->Timing.clockTick0
        + 1) * deneme_M->Timing.stepSize0 + deneme_M->Timing.clockTickH0 *
        deneme_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(deneme_M)) {
    deneme_M->Timing.t[0] = rtsiGetT(&deneme_M->solverInfo);
  }

  /* Sin: '<Root>/Sine Wave' */
  deneme_B.Sineinput = sin(deneme_P.SineWave_Freq * deneme_M->Timing.t[0] +
    deneme_P.SineWave_Phase) * deneme_P.SineWave_Amp + deneme_P.SineWave_Bias;

  /* Derivative: '<Root>/Derivative' */
  if ((deneme_DW.TimeStampA >= deneme_M->Timing.t[0]) && (deneme_DW.TimeStampB >=
       deneme_M->Timing.t[0])) {
    rtb_Derivative = 0.0;
  } else {
    rtb_Derivative = deneme_DW.TimeStampA;
    lastU = &deneme_DW.LastUAtTimeA;
    if (deneme_DW.TimeStampA < deneme_DW.TimeStampB) {
      if (deneme_DW.TimeStampB < deneme_M->Timing.t[0]) {
        rtb_Derivative = deneme_DW.TimeStampB;
        lastU = &deneme_DW.LastUAtTimeB;
      }
    } else {
      if (deneme_DW.TimeStampA >= deneme_M->Timing.t[0]) {
        rtb_Derivative = deneme_DW.TimeStampB;
        lastU = &deneme_DW.LastUAtTimeB;
      }
    }

    rtb_Derivative = (deneme_B.Sineinput - *lastU) / (deneme_M->Timing.t[0] -
      rtb_Derivative);
  }

  /* End of Derivative: '<Root>/Derivative' */

  /* Sum: '<Root>/Sum' incorporates:
   *  Gain: '<Root>/Gain'
   *  Integrator: '<Root>/Integrator'
   *  Sum: '<Root>/Sum1'
   */
  deneme_B.SineOutput = (deneme_P.Gain_Gain * deneme_B.Sineinput +
    rtb_Derivative) + deneme_X.Integrator_CSTATE;
  if (rtmIsMajorTimeStep(deneme_M)) {
  }
}

/* Model update function */
void deneme_update(void)
{
  real_T *lastU;

  /* Update for Derivative: '<Root>/Derivative' */
  if (deneme_DW.TimeStampA == (rtInf)) {
    deneme_DW.TimeStampA = deneme_M->Timing.t[0];
    lastU = &deneme_DW.LastUAtTimeA;
  } else if (deneme_DW.TimeStampB == (rtInf)) {
    deneme_DW.TimeStampB = deneme_M->Timing.t[0];
    lastU = &deneme_DW.LastUAtTimeB;
  } else if (deneme_DW.TimeStampA < deneme_DW.TimeStampB) {
    deneme_DW.TimeStampA = deneme_M->Timing.t[0];
    lastU = &deneme_DW.LastUAtTimeA;
  } else {
    deneme_DW.TimeStampB = deneme_M->Timing.t[0];
    lastU = &deneme_DW.LastUAtTimeB;
  }

  *lastU = deneme_B.Sineinput;

  /* End of Update for Derivative: '<Root>/Derivative' */
  if (rtmIsMajorTimeStep(deneme_M)) {
    rt_ertODEUpdateContinuousStates(&deneme_M->solverInfo);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++deneme_M->Timing.clockTick0)) {
    ++deneme_M->Timing.clockTickH0;
  }

  deneme_M->Timing.t[0] = rtsiGetSolverStopTime(&deneme_M->solverInfo);

  {
    /* Update absolute timer for sample time: [0.001s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick1"
     * and "Timing.stepSize1". Size of "clockTick1" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick1 and the high bits
     * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++deneme_M->Timing.clockTick1)) {
      ++deneme_M->Timing.clockTickH1;
    }

    deneme_M->Timing.t[1] = deneme_M->Timing.clockTick1 *
      deneme_M->Timing.stepSize1 + deneme_M->Timing.clockTickH1 *
      deneme_M->Timing.stepSize1 * 4294967296.0;
  }
}

/* Derivatives for root system: '<Root>' */
void deneme_derivatives(void)
{
  XDot_deneme_T *_rtXdot;
  _rtXdot = ((XDot_deneme_T *) deneme_M->ModelData.derivs);

  /* Derivatives for Integrator: '<Root>/Integrator' */
  _rtXdot->Integrator_CSTATE = deneme_B.Sineinput;
}

/* Model initialize function */
void deneme_initialize(void)
{
  /* InitializeConditions for Integrator: '<Root>/Integrator' */
  deneme_X.Integrator_CSTATE = deneme_P.Integrator_IC;

  /* InitializeConditions for Derivative: '<Root>/Derivative' */
  deneme_DW.TimeStampA = (rtInf);
  deneme_DW.TimeStampB = (rtInf);
}

/* Model terminate function */
void deneme_terminate(void)
{
  /* (no terminate code required) */
}

/*========================================================================*
 * Start of Classic call interface                                        *
 *========================================================================*/

/* Solver interface called by GRT_Main */
#ifndef USE_GENERATED_SOLVER

void rt_ODECreateIntegrationData(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

void rt_ODEDestroyIntegrationData(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

void rt_ODEUpdateContinuousStates(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

#endif

void MdlOutputs(int_T tid)
{
  deneme_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  deneme_update();
  UNUSED_PARAMETER(tid);
}

void MdlInitializeSizes(void)
{
}

void MdlInitializeSampleTimes(void)
{
}

void MdlInitialize(void)
{
}

void MdlStart(void)
{
  deneme_initialize();
}

void MdlTerminate(void)
{
  deneme_terminate();
}

/* Registration function */
RT_MODEL_deneme_T *deneme(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)deneme_M, 0,
                sizeof(RT_MODEL_deneme_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&deneme_M->solverInfo, &deneme_M->Timing.simTimeStep);
    rtsiSetTPtr(&deneme_M->solverInfo, &rtmGetTPtr(deneme_M));
    rtsiSetStepSizePtr(&deneme_M->solverInfo, &deneme_M->Timing.stepSize0);
    rtsiSetdXPtr(&deneme_M->solverInfo, &deneme_M->ModelData.derivs);
    rtsiSetContStatesPtr(&deneme_M->solverInfo, (real_T **)
                         &deneme_M->ModelData.contStates);
    rtsiSetNumContStatesPtr(&deneme_M->solverInfo,
      &deneme_M->Sizes.numContStates);
    rtsiSetErrorStatusPtr(&deneme_M->solverInfo, (&rtmGetErrorStatus(deneme_M)));
    rtsiSetRTModelPtr(&deneme_M->solverInfo, deneme_M);
  }

  rtsiSetSimTimeStep(&deneme_M->solverInfo, MAJOR_TIME_STEP);
  deneme_M->ModelData.intgData.f[0] = deneme_M->ModelData.odeF[0];
  deneme_M->ModelData.contStates = ((real_T *) &deneme_X);
  rtsiSetSolverData(&deneme_M->solverInfo, (void *)&deneme_M->ModelData.intgData);
  rtsiSetSolverName(&deneme_M->solverInfo,"ode1");

  /* Initialize timing info */
  {
    int_T *mdlTsMap = deneme_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    mdlTsMap[1] = 1;
    deneme_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    deneme_M->Timing.sampleTimes = (&deneme_M->Timing.sampleTimesArray[0]);
    deneme_M->Timing.offsetTimes = (&deneme_M->Timing.offsetTimesArray[0]);

    /* task periods */
    deneme_M->Timing.sampleTimes[0] = (0.0);
    deneme_M->Timing.sampleTimes[1] = (0.001);

    /* task offsets */
    deneme_M->Timing.offsetTimes[0] = (0.0);
    deneme_M->Timing.offsetTimes[1] = (0.0);
  }

  rtmSetTPtr(deneme_M, &deneme_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = deneme_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    mdlSampleHits[1] = 1;
    deneme_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(deneme_M, 10.0);
  deneme_M->Timing.stepSize0 = 0.001;
  deneme_M->Timing.stepSize1 = 0.001;

  /* Setup for data logging */
  {
    static RTWLogInfo rt_DataLoggingInfo;
    deneme_M->rtwLogInfo = &rt_DataLoggingInfo;
  }

  /* Setup for data logging */
  {
    rtliSetLogXSignalInfo(deneme_M->rtwLogInfo, (NULL));
    rtliSetLogXSignalPtrs(deneme_M->rtwLogInfo, (NULL));
    rtliSetLogT(deneme_M->rtwLogInfo, "tout");
    rtliSetLogX(deneme_M->rtwLogInfo, "");
    rtliSetLogXFinal(deneme_M->rtwLogInfo, "");
    rtliSetLogVarNameModifier(deneme_M->rtwLogInfo, "rt_");
    rtliSetLogFormat(deneme_M->rtwLogInfo, 0);
    rtliSetLogMaxRows(deneme_M->rtwLogInfo, 1000);
    rtliSetLogDecimation(deneme_M->rtwLogInfo, 1);
    rtliSetLogY(deneme_M->rtwLogInfo, "");
    rtliSetLogYSignalInfo(deneme_M->rtwLogInfo, (NULL));
    rtliSetLogYSignalPtrs(deneme_M->rtwLogInfo, (NULL));
  }

  deneme_M->solverInfoPtr = (&deneme_M->solverInfo);
  deneme_M->Timing.stepSize = (0.001);
  rtsiSetFixedStepSize(&deneme_M->solverInfo, 0.001);
  rtsiSetSolverMode(&deneme_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  deneme_M->ModelData.blockIO = ((void *) &deneme_B);
  (void) memset(((void *) &deneme_B), 0,
                sizeof(B_deneme_T));

  /* parameters */
  deneme_M->ModelData.defaultParam = ((real_T *)&deneme_P);

  /* states (continuous) */
  {
    real_T *x = (real_T *) &deneme_X;
    deneme_M->ModelData.contStates = (x);
    (void) memset((void *)&deneme_X, 0,
                  sizeof(X_deneme_T));
  }

  /* states (dwork) */
  deneme_M->ModelData.dwork = ((void *) &deneme_DW);
  (void) memset((void *)&deneme_DW, 0,
                sizeof(DW_deneme_T));

  /* Initialize DataMapInfo substructure containing ModelMap for C API */
  deneme_InitializeDataMapInfo(deneme_M);

  /* Initialize Sizes */
  deneme_M->Sizes.numContStates = (1); /* Number of continuous states */
  deneme_M->Sizes.numY = (0);          /* Number of model outputs */
  deneme_M->Sizes.numU = (0);          /* Number of model inputs */
  deneme_M->Sizes.sysDirFeedThru = (0);/* The model is not direct feedthrough */
  deneme_M->Sizes.numSampTimes = (2);  /* Number of sample times */
  deneme_M->Sizes.numBlocks = (7);     /* Number of blocks */
  deneme_M->Sizes.numBlockIO = (2);    /* Number of block outputs */
  deneme_M->Sizes.numBlockPrms = (6);  /* Sum of parameter "widths" */
  return deneme_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
