///---------------------------- FILE HEADER ------------------------------------------------------
/// component   : QUANSER Q8 CARD
/// file        : Q8Lib.h
/// description : Implementation for user space functions of the Q8 card driver                  
///               
/// author      : Egemen Kaleli, EK
///               Gebze Institute of Technology,Robotics and Control Laboratory
///
/// date        : july-2008
///
///------------------------------------------------------------------------------------------------
#ifndef _Q8_LIB_H_
#define _Q8_LIB_H_

#include <stdlib.h>
#include <rtdm/rtdm.h>
#include "tmwtypes.h"

#define ADC_FACTOR          (10.0 / 8192)
#define DAC_BIPOLAR_ZERO   (0X0800)
#define UNIPOLAR_10V  0
#define BIPOLAR_5V    1
#define BIPOLAR_10V   2

#define DEV_NAME       "Q80"

#define NORMAL_COUNT      0 /*ENCODER COUNT MODE CONSTANTS */
#define RANGE_LIMIT_COUNT 1
#define NON_RECYCLE_COUNT 2
#define MODULO_NCount     3 


#define NONQ              0/*ENCODER QUADRATURE MOD CONSTANTS*/
#define _1_X_             1 
#define  _2_X_            2
#define _4_X_             3

#define POS               0/*ENCODER INDEX POLARITY CONSTANTS*/
#define NEG               1

#define BINARY            0/*BINARY-BCD MODE CONSTANTS*/
#define BCD               1 

#define ENABLE_INDEX      0/*INDEX CONSTANTS*/
#define DISABLE_INDEX     1


#define COUNTER           0
#define WATCHDOG          1


struct Q8_Config{
	int iChannelNum;
	int iCardNum;
	int16_t wValueSigned;
	uint16_t uwValueUnsigned;//Analog output value
	unsigned int uiFuncIndex;
	uint8_t ubDacMode;
	uint8_t  ubEcountMode;/*encoder*/
	uint8_t ubEbcdMode;
	uint8_t ubEquadratureMode;
	uint8_t ubIndexEnable;
	uint8_t ubIndexPolarity;
	int32_t dwNvalue;//encoder counter value
	uint32_t udwDataDirection;/*digital I/0*/
	uint32_t udwDataRegister;
	int iClockSource;/*COUNTER*/
	uint32_t udwPeriod;
	uint32_t udwLow;
	uint32_t udwHigh;
	int iCntrlEn;
        uint8_t irq;
        int onlyDigitalData;
 };


typedef struct Q8_Config struct_Q8_Config;

// function predefinitioıns
 void    Q8Close(int device);
 void    Q8AnalogOutput( int cardNum, int channelNum,real_T value);
 void    Q8AnalogConfig(int cardNum,int channelNum,int mode);
 int16_t Q8AnalogInput(int cardNum,int channelNum);//no need for configuration
 int32_t Q8EncoderInput(int cardNum,int channelNum);
 int Q8EncoderConfig(int cardNum,
                      int channelNum,
                      int count_mode,
                      int bcd,int quadrature,
                      int ubIndexEnable,int ubIndexPolarity);                     
 void Q8EncoderReset(int cardNum,int channelNum);
 void  Q8DigitalOutput(int cardNum,uint32_t channels,uint32_t _digdata,int writeOnly);
 uint32_t Q8DigitalInput(int cardNum,uint32_t channels,int readonly);
 void Q8CounterOutput(int cardNum,int iClockSource,  float udwPeriod,int iCntrlEn );
 void Q8PwmOutput(int cardNum,int iClockSource, float udwPeriod, float pulse_width,int iCntrlEn);
 void Q8WatchdogTimer(int cardNum, float timeout_interval);
 void Q8TimerStart(int cardNum);
 void Q8TimerSet(int cardNum,float period);
 void Q8TimerStart(int cardNum);
 void Q8TimerStop(int cardNum); 
 void Q8TimerIntEnable(int cardNum);
 uint8_t Q8GetIrq(int cardNum);
#endif
