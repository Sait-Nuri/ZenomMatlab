#include "Q8Lib.h"
#include "rtdk.h"
uint8_t dacCon[8]={BIPOLAR_10V,BIPOLAR_10V,BIPOLAR_10V,BIPOLAR_10V,BIPOLAR_10V,BIPOLAR_10V,BIPOLAR_10V,BIPOLAR_10V};
//file descriptor variaBle.it is used as return value of rt_dev_open(...)
 int fd=-1;

///============================================ FUNCTION HEADER=========================================================
/// Name        : Q8Close
/// Input       : device    - device file descriptor 
/// Output      : None
/// Return      : None
/// Description : Close the device
///                                
///=====================================================================================================================


void     Q8Close(device)
{
int ret;
ret = rt_dev_close(device);
	if (ret < 0) {
		rt_printf("ERROR : can't close device Q8\n");
		exit(1);
	}
else 
  fd=-1;

}

// Function declarations

///============================================ FUNCTION HEADER=========================================================
/// Name        : Q8AnalogOutput
/// Input       : iCardNum    - PCI memory region that is mapped to Q8 card memory 
///               iChannelNum - D/A Output channel number
///               fValue      - User defined voltage which is wanted to see at output of the specified channel
/// Output      : None
/// Return      : None
/// Description : Writes user defined voltage value to the specified channel.
///                                
///=====================================================================================================================
void Q8AnalogOutput(int iCardNum,int iChannelNum,real_T dacValue){

 if(fd==-1){
 	fd = rt_dev_open(DEV_NAME, O_RDWR);
 	}
   
   struct_Q8_Config q8Str;
   q8Str.iCardNum=iCardNum;
   q8Str.uiFuncIndex=iChannelNum;
  
 if (dacValue <=0){
   if(dacCon[iChannelNum]==BIPOLAR_10V) {
          q8Str.uwValueUnsigned=(DAC_BIPOLAR_ZERO-(uint16_t)((-dacValue)*4096/20));
         rt_dev_write(fd,&q8Str,sizeof(struct_Q8_Config));
   return;
   }              
  else if(dacCon[iChannelNum]==BIPOLAR_5V){
     q8Str.uwValueUnsigned=(DAC_BIPOLAR_ZERO-(uint16_t)((-dacValue)*4096/10));
     rt_dev_write(fd,&q8Str,sizeof(struct_Q8_Config));
     return;
     }
     else
     return;
  }//if

  if(dacCon[iChannelNum]==BIPOLAR_10V) {
         q8Str.uwValueUnsigned=(DAC_BIPOLAR_ZERO+(uint16_t)(dacValue*4096/20));
         rt_dev_write(fd,&q8Str,sizeof(struct_Q8_Config));
     return;
     }           
 else if(dacCon[iChannelNum]==BIPOLAR_5V){
         q8Str.uwValueUnsigned=(DAC_BIPOLAR_ZERO+(uint16_t)(dacValue*4096/10));
         rt_dev_write(fd,&q8Str,sizeof(struct_Q8_Config));
    return;
     }
 else if(dacCon[iChannelNum]==UNIPOLAR_10V){
        q8Str.uwValueUnsigned=(uint16_t)(dacValue*4096/10);
        rt_dev_write(fd,&q8Str,sizeof(struct_Q8_Config));
        return;
      }
 } // end of function
 
///==========================================FUNCTION HEADER============================================================
/// Name        : Q8AnalogInput
/// Input       : iCardNum    - PCI memory region that is mapped to Q8 card memory 
///               iCahnnelNum - D/A input channel number
/// Output      : None
/// Return      : None
/// Description : Reads the  voltage value from the specified channel.
///                                
///=====================================================================================================================
int16_t Q8AnalogInput(int iCardNum,int iChannelNum){

	 if(fd==-1){
		fd = rt_dev_open(DEV_NAME, O_RDWR);}
	struct_Q8_Config q8Str;
	q8Str.iCardNum=iCardNum;
	q8Str.uiFuncIndex=iChannelNum+8;
	rt_dev_read(fd,&q8Str,sizeof(struct_Q8_Config));
	return q8Str.wValueSigned;
}
///============================================== FUNCTION HEADER=======================================================
/// Name        : Q8EncoderInput
/// Input       : iCardNum    - PCI memory region that is mapped to Q8 card memory 
///               iChannelNum - Encoder channel number
/// Output      : None
/// Return      : an int32_t value that is read
/// Description : Reads the specified channel of encoder.Actually the value read is the counter value of the specified 
///               channel
///                                
///=====================================================================================================================
int32_t Q8EncoderInput(int iCardNum,int iChannelNum){
 	if(fd==-1){
		fd = rt_dev_open(DEV_NAME, O_RDWR);}
	struct_Q8_Config q8Str;
	q8Str.uiFuncIndex=iChannelNum+16;
	rt_dev_read(fd,&q8Str,sizeof(struct_Q8_Config));
	return q8Str.dwNvalue;
}
void Q8EncoderReset(int   iCardNum,int iChannelNum){
 	if(fd==-1){
 	fd = rt_dev_open(DEV_NAME, O_RDWR);
	}
	struct_Q8_Config q8Str;
	q8Str.iCardNum=iCardNum;
 	q8Str.uiFuncIndex=iChannelNum+31;
	rt_dev_write(fd,&q8Str,sizeof(struct_Q8_Config));
}
///============================================== FUNCTION HEADER=======================================================
/// Name        : Q8DigitalOutput
/// Input       : iCardNum        - PCI memory region that is mapped to Q8 card memory. 
///               iPtrChannelNums - Array address.In this array numbers of channels written are defined and stored.
///               iArraySİze      - Array size.          
/// Output      : None
/// Return      : None
/// Description : Configures  the specified channel(s) as output(s) and writes the specified channel(s)
///=====================================================================================================================
void  Q8DigitalOutput(int iCardNum,uint32_t channel,uint32_t data,int writeOnly){
 	if(fd==-1){
	fd = rt_dev_open(DEV_NAME, O_RDWR);
	}
 
       
        struct_Q8_Config q8Str;
  	q8Str.uiFuncIndex=24;
        q8Str.onlyDigitalData=writeOnly;
  	q8Str.udwDataDirection=0;
  	q8Str.udwDataDirection|=channel;
        q8Str.udwDataRegister=data;
  	q8Str.iCardNum=iCardNum;

	rt_dev_write(fd,&q8Str,sizeof(struct_Q8_Config));
}
///================================================ FUNCTION HEADER======================================================
/// Name        : Q8DigitalInput
/// Input       : iCardNum        - PCI memory region that is mapped to Q8 card memory. 
///               iPtrChannelNums - Array address.In this array numbers of channels read are defined and stored.
///               iArraySİze      - Array size.          
/// Output      : None
/// Return      : an unint32_t value
/// Description : Configures  the specified channel(s) as input(s) and reads the specified channel(s)
///                                
///=====================================================================================================================
uint32_t  Q8DigitalInput(int iCardNum,uint32_t channel,int readonly){
	
	
 	if(fd==-1){
		fd = rt_dev_open(DEV_NAME, O_RDWR);
	}
       
	uint32_t udwDigitalRead=0;
	struct_Q8_Config q8Str;
	q8Str.uiFuncIndex=25;
        q8Str.onlyDigitalData=readonly;
        q8Str.udwDataDirection=0xFFFFFFFF; 
	q8Str.udwDataDirection&=channel;
	q8Str.iCardNum=iCardNum;
	int i;
	uint32_t udwDataBuffer;
	uint32_t udwDataReg;
	rt_dev_read(fd,&q8Str,sizeof(struct_Q8_Config));
	return q8Str.udwDataRegister;
}
///=============================================== FUNCTION HEADER =====================================================
/// Name        : Q8CounterOutput
/// Input       : iCardNum     - PCI memory region that is mapped to Q8 card memory 
///               iClockSource - ClockSource selection,WATCHDOG or COUNTER
///               fPeriod      - Square wave period
///               iCntrlEn     - Not used
/// Output      : None
/// Return      : None
/// Description : Configures counter output and generates periodic square wave
///                                
///=====================================================================================================================
void Q8CounterOutput(int iCardNum,int iClockSource, float fPeriod,int iCntrlEn )
{
 	if(fd==-1){
   		fd = rt_dev_open(DEV_NAME, O_RDWR);
   	}
 	struct_Q8_Config q8Str;
 	q8Str.uiFuncIndex=26;
 	q8Str.iClockSource=iClockSource;
 	q8Str.udwPeriod=(uint32_t)(fPeriod / 60e-9)-1;
 	q8Str.iCntrlEn=iCntrlEn;
 	rt_dev_write(fd,&q8Str,sizeof(struct_Q8_Config));
}
///=============================================== FUNCTION HEADER =====================================================
/// Name        : Q8PwmOutput
/// Input       : iCardNum     - PCI memory region that is mapped to Q8 card memory 
///               iClockSource - ClockSource selection,WATCHDOG or COUNTER
///               fPeriod      - PWM period
///               iCntrlEn     - Not used
///               fPulseWidth  - PWM pulse width,0...100
/// Output      : None
/// Return      : None
/// Description : Configures counter output and generates PWM based periodic wave.
///                                
///=====================================================================================================================
void Q8PwmOutput(int iCardNum,int iClockSource, float fPeriod,float fPulseWidth,int iCntrlEn){
 	if(fd==-1){
    	 fd = rt_dev_open(DEV_NAME, O_RDWR);
 	} 
	struct_Q8_Config q8Str;
	q8Str.uiFuncIndex=27;
	q8Str.iClockSource=iClockSource;
	q8Str.udwLow=(uint32_t)( ((100-fPulseWidth)*fPeriod/100) / 30e-9) -1;
	q8Str.udwHigh=(uint32_t)( ((fPulseWidth)*fPeriod/100) / 30e-9)-1;
	q8Str.iCntrlEn=iCntrlEn;
	rt_dev_write(fd,&q8Str,sizeof(struct_Q8_Config));
}
///============================================FUNCTION HEADER=========================================================
/// Name        : Q8WatchdogTimer
/// Input       : iCardNum  - PCI memory region that is mapped to Q8 card memory 
///               fInterval - Timeout interval needed  analog output channnels to be set
/// Output      : None
/// Return      : None
/// Description : Configures  and enables Watchdog timer
///                                
///=====================================================================================================================
void Q8WatchdogTimer(int iCardNum,float fInterval){
	struct_Q8_Config q8Str;
	q8Str.uiFuncIndex=28;
	if(fd==-1){
		fd = rt_dev_open(DEV_NAME, O_RDWR);
	}
	q8Str.udwPeriod=(uint32_t)(fInterval/ 60e-9)-1;
	rt_dev_write(fd,&q8Str,sizeof(struct_Q8_Config));
}
///================================================FUNCTION HEADER=====================================================
/// Name        : Q8EncoderConfig
/// Input       : iCardNum        - PCI memory region that is mapped to Q8 card memory 
///               iChannelNum     - Encoder channel number
///               iCountMode      - Count mode to use
///               iBcd            - Bcd or binary counting
///               iQuadrature     - Quadrature  type selection
///               iIndexEn        - Enable or disable index feature
///               iIndexPol       - Select index polarity
/// Output      : None
/// Return      : None
/// Description : Configures  the encoder channel 
///                                
///=====================================================================================================================
int Q8EncoderConfig(int iCardNum, int iChannelNum,int iCountMode,
                     int iBcd, int iQuadrature, int iIndexEn, int iIndexPol)
{
  struct_Q8_Config q8Str;
  q8Str.uiFuncIndex=29;
  q8Str.iCardNum=iCardNum;
  q8Str.iChannelNum=iChannelNum;
  q8Str.ubEcountMode=iCountMode;
  q8Str.ubEbcdMode=iBcd;
  q8Str.ubEquadratureMode=iQuadrature;
  q8Str.ubIndexEnable=iIndexEn;
  q8Str.ubIndexPolarity=iIndexPol;
  if(fd < 0){ 
		fd = rt_dev_open(DEV_NAME, O_RDWR);
  	}
  rt_dev_write(fd,&q8Str,sizeof(struct_Q8_Config));

  return fd;
}
///================================================= FUNCTION HEADER====================================================
/// Name        : Q8AnalogConfig
/// Input       : iCardNum     - PCI memory region that is mapped to Q8 card memory 
///               iChannelNum  - D/A Output channel number
///               iMode        - Output voltage range selection
/// Output      : None        
/// Return      : None
/// Description : Configures  the specified analog output channel
///                                
///=====================================================================================================================
void    Q8AnalogConfig(int iCardNum,int iChannelNum,int iMode){
//update process of configuration of analogoutput mode
	dacCon[iChannelNum]=iMode;
	struct_Q8_Config q8Str;
	q8Str.iCardNum=iCardNum;
	q8Str.uiFuncIndex=iChannelNum;
	q8Str.ubDacMode=iMode;
	q8Str.iChannelNum=iChannelNum;
	q8Str.uiFuncIndex=30;
	if(fd==-1){
		fd = rt_dev_open(DEV_NAME, O_RDWR);
	}
	rt_dev_write(fd,&q8Str,sizeof(struct_Q8_Config));
}
///================================================= FUNCTION HEADER====================================================
/// Name        : Q8TimerSet
/// Input       : iCardNum     - PCI memory region that is mapped to Q8 card memory 
///               fPeriod      - Timer set period in seconds
/// Output      : None        
/// Return      : None
/// Description : Configures  32 bit counter as timer interrupt source
///                                
///=====================================================================================================================
void Q8TimerSet(int cardNum,float fPeriod){
	struct_Q8_Config q8Str;
        q8Str.udwPeriod=(uint32_t)(fPeriod / 60e-9)-1; 
	q8Str.uiFuncIndex=39;
        if(fd==-1){
		fd = rt_dev_open(DEV_NAME, O_RDWR);
	}
	rt_dev_write(fd,&q8Str,sizeof(struct_Q8_Config));
}
///================================================= FUNCTION HEADER====================================================
/// Name        : Q8TimerStart
/// Input       : iCardNum     - PCI memory region that is mapped to Q8 card memory 
/// Output      : None        
/// Return      : None
/// Description : Activates the timer to count
///=====================================================================================================================
void Q8TimerStart(int cardNum){
	struct_Q8_Config q8Str; 
	q8Str.uiFuncIndex=40;
         if(fd==-1){
		fd = rt_dev_open(DEV_NAME, O_RDWR);
	}
	rt_dev_write(fd,&q8Str,sizeof(struct_Q8_Config));
}
///================================================= FUNCTION HEADER==================================================
/// Name        : Q8TimerStart
/// Input       : iCardNum     - PCI memory region that is mapped to Q8 card memory 
/// Output      : None        
/// Return      : None
/// Description : Deacctivates the timer to stop counting
///===================================================================================================================
void Q8TimerStop(int cardNum){
	struct_Q8_Config q8Str; 
	q8Str.uiFuncIndex=41;
         if(fd==-1){
		fd = rt_dev_open(DEV_NAME, O_RDWR);
	}
	rt_dev_write(fd,&q8Str,sizeof(struct_Q8_Config));
}
///================================================= FUNCTION HEADER==================================================
/// Name        : Q8GetIrq
/// Input       : iCardNum     - PCI memory region that is mapped to Q8 card memory 
/// Output      : None        
/// Return      : Q8 IRQ Line number
/// Description : This function fetches irq line number assigned to Q8 card.
///===================================================================================================================
uint8_t Q8GetIrq(int cardNum){

	struct_Q8_Config q8Str; 
	q8Str.uiFuncIndex=42;
         if(fd==-1){
		fd = rt_dev_open(DEV_NAME, O_RDWR);
	}
        rt_dev_read(fd,&q8Str,sizeof(struct_Q8_Config));
        return q8Str.irq;
}
void Q8TimerIntEnable(int cardNum){
	
	struct_Q8_Config q8Str; 
	q8Str.uiFuncIndex=43;
         if(fd==-1){
		fd = rt_dev_open(DEV_NAME, O_RDWR);
	}
	rt_dev_write(fd,&q8Str,sizeof(struct_Q8_Config));
}
