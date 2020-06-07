#ifndef __TLB_ConfigRegisters
#define __TLB_ConfigRegisters

namespace FASER {

/** *******************************************************
 * \brief Change the configuration.
 ******************************************************** */
class ConfigReg {
public:

  //Definition of the default values for the configuration:
  uint8_t SamplingPhase=0x0;
  uint16_t DirectParam=0x0;
  uint16_t DataReadoutParam=0x1000;
  uint16_t ConfigWord0=0x0000;
  uint16_t ConfigWord1=0x1000;
  uint16_t ConfigWord2=0x2000;
  uint16_t ConfigWord3=0x3000;
  uint16_t ConfigWord4=0x4000;
  uint16_t ConfigWord5=0x5000;
  uint16_t ConfigWord6=0x6000;
  uint16_t ConfigWord7=0x7000;
  uint16_t ConfigWord8=0x8000;
  uint16_t ConfigWord9=0x9000;
  uint16_t ConfigWord10=0xA000;
  uint16_t ConfigWord11=0xB000;
  uint16_t ConfigWord12=0xC000;
  uint16_t ConfigWord13=0xD000;
  uint16_t ConfigWord14=0xE000;
  
  void SetSamplingPhase(int SamplingPhaseNumber,bool Toggle);
  void SetInputDelay(int InputDelay1,int InputDelay2,int InputDelay3,int InputDelay4,int InputDelay5,int InputDelay6,int InputDelay7,int InputDelay8);
  void SetRandomTriggerRate(int RandomTriggerRateValue);
  void SetPrescale(int PrescaleNumber, int value);
  void SetTrackerDelay(int value);
  void SetDigitizerDelay(int value);
  void SetLHC_CLK(int value);
  void SetOrbitDelay(int value);
  void SetDeadtime(int value);
  void SetMonitoringRate(int value);
  void SetOutputDestination(unsigned long int value);
  void SetInput(int InputNumber, int value);
  
  void SetReset(int value);
  void SetECR(int value);
  void SetTriggerEnable(int value);
  void SetSoftwareTrigger(int value);
  void SetBusyDisable(int value);
  
  void SetEnableTriggerData(int value);
  void SetEnableMonitoringData(int value);
  void SetReadoutFIFOReset(int value);

  void PrintConfig();  
  void PrintDirectParam();
  void PrintDataReadoutParam();

};
}
#endif /* __TLB_ConfigRegisters */
