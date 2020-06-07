#include<iostream>
#include <bitset>
#include <unistd.h> //for usleep
#define _ms 1000 // used for usleep
#include "TLBAccess/TLBAccess.h"
#include "TLBAccess/TLB_ConfigRegisters.h"


using namespace FASER;

/**
 *  @brief Sets the Sampling Phase.
 *
 *  @details Sets the first 8 bits of the 1st word of the configuration.
 *
 *  @param SamplingPhaseNumber is an int.
 *  @param Toggle is a bool that flips the bit on or off.
 *
 *  @return void
 */
void ConfigReg::SetSamplingPhase(int SamplingPhaseNumber,bool Toggle) {
  if (Toggle==1) {SamplingPhase |= 1UL << (SamplingPhaseNumber);}
  else{SamplingPhase &= ~(1UL << (SamplingPhaseNumber));}
  ConfigWord0&=0xFF00; //We clean all the sampling phase bit so we rewrite them
  ConfigWord0=(ConfigWord0|SamplingPhase); //We write the sampling phase bits
}

/**
 *  @brief Set the Input Delay.
 *
 *  @details 4 MSB of 1st word and 12 bits of 2nd word of the configuration.
 *
 *  @param InputDelay1 is an int that you can set from 0 to 3 included.
 *  @param InputDelay2 is an int that you can set from 0 to 3 included.
 *  @param InputDelay3 is an int that you can set from 0 to 3 included.
 *  @param InputDelay4 is an int that you can set from 0 to 3 included.
 *  @param InputDelay5 is an int that you can set from 0 to 3 included.
 *  @param InputDelay6 is an int that you can set from 0 to 3 included.
 *  @param InputDelay7 is an int that you can set from 0 to 3 included.
 *  @param InputDelay8 is an int that you can set from 0 to 3 included. 
 *
 *  @note If the users sets a value greater than 3 then it automatically sets the values to 3.
 *
 *  @return void
 */
void ConfigReg::SetInputDelay(int InputDelay1,int InputDelay2,int InputDelay3,int InputDelay4,int InputDelay5,int InputDelay6,int InputDelay7,int InputDelay8) {
  //Configuring 1st word for InputDelay1 & InputDelay2
  ConfigWord0&=0xF0FF; //We clean the Inputdelay bits on the 1st word
  ConfigWord1&=0xF000;
  if (InputDelay1>0x3){InputDelay1=0x3;std::cout<<"InputDelay1 value is too big, setting it to 3."<<std::endl;} //To make sure the user doesn't give to big values
  if (InputDelay2>0x3){InputDelay2=0x3;std::cout<<"InputDelay2 value is too big, setting it to 3."<<std::endl;}
  InputDelay1=InputDelay1<<8; //Shift the bits 
  InputDelay2=InputDelay2<<10;
  ConfigWord0|=InputDelay1; //Write the bits
  ConfigWord0|=InputDelay2;
  
  //Configuring 2nd word for InputDelay3 to InputDelay8
  ConfigWord1&=0xF000; //Cleaning, all the word is attributed to the InputDelay
  if (InputDelay3>0x3){InputDelay3=0x3;std::cout<<"InputDelay3 value is too big, setting it to 3."<<std::endl;}
  if (InputDelay4>0x3){InputDelay4=0x3;std::cout<<"InputDelay4 value is too big, setting it to 3."<<std::endl;}
  if (InputDelay5>0x3){InputDelay5=0x3;std::cout<<"InputDelay5 value is too big, setting it to 3."<<std::endl;}
  if (InputDelay6>0x3){InputDelay6=0x3;std::cout<<"InputDelay6 value is too big, setting it to 3."<<std::endl;}
  if (InputDelay7>0x3){InputDelay7=0x3;std::cout<<"InputDelay7 value is too big, setting it to 3."<<std::endl;}
  if (InputDelay8>0x3){InputDelay8=0x3;std::cout<<"InputDelay8 value is too big, setting it to 3."<<std::endl;}
  InputDelay4=InputDelay4<<2; //Shift the bits 
  InputDelay5=InputDelay5<<4;
  InputDelay6=InputDelay6<<6;
  InputDelay7=InputDelay7<<8;
  InputDelay8=InputDelay8<<10;
  ConfigWord1|=InputDelay3; //Write the bits
  ConfigWord1|=InputDelay4;
  ConfigWord1|=InputDelay5;
  ConfigWord1|=InputDelay6;
  ConfigWord1|=InputDelay7;
  ConfigWord1|=InputDelay8;
}

/**
 *  @brief Sets the Random Trigger Rate.
 *
 *  @details The first 3 bits of the 3rd word. of the configuration.
 *
 *  @param RandomTriggerRateValue is an int that you can set from 0 to 7 included.
 *
 *  @note If the users sets a value greater than 7 then it automatically sets the RandomTriggerRateValue to 7.
 *
 *  @return void
 */
void ConfigReg::SetRandomTriggerRate(int RandomTriggerRateValue) {
  if (RandomTriggerRateValue>7){RandomTriggerRateValue=7;std::cout<<"RandomTriggerRateValue is too big, setting it to max possible value"<<std::endl;}
  ConfigWord2&=0xFFF8; //We clean all the random trigger rate bits so we can rewrite them.
  ConfigWord2=(ConfigWord2|RandomTriggerRateValue); //We write the Random Trigger Rate  bits.
  }
  
/**
 *  @brief Sets the Prescales.
 *
 *  @details 
 *
 *  @param PrescaleNumber is an int that you can set from 0 to 5 included.
 *  @param value is an int that you can set from 0 to 255
 *
 *  @note If the users sets a value greater than 255 then it automatically sets the value to 255.
 *
 *  @return void
 */
void ConfigReg::SetPrescale(int PrescaleNumber, int value){

  switch(PrescaleNumber){

    //Set Prescale 0
    case 0:{
      if (value>255){value=255;std::cout<<"Prescale 0 value is too big, setting it to 255."<<std::endl;}
      ConfigWord2&=0xF807; //We clean all the prescale 1 bits so we can rewrite them.
      value=value<<3;
      ConfigWord2=(ConfigWord2|value);
      break;
    }

    //Set Prescale 1 (is on word2 and 3).
    case 1:{
      if (value>255){value=255;std::cout<<"Prescale 1 value is too big, setting it to 255."<<std::endl;}
      ConfigWord2&=0xF7FF; //We clean all the prescale 1 bits so we can rewrite them.
      ConfigWord3&=0xFF80; 
      //now we need to split the value into two pieces
      int value1=value;
      int value2=value;
      value1&=0x1; //meaning we only keep the first bit
      value1=value1<<11;
      value2&=0xFE; //meaning we erase the last bit 11111110
      value2=value2>>1;
      ConfigWord2=(ConfigWord2|value1);
      ConfigWord3=(ConfigWord3|value2);
      break;
    }

    //Set Prescale 2 (is on word3 and 4).
    case 2:{
      if (value>255){value=255;std::cout<<"Prescale 2 value is too big, setting it to 255."<<std::endl;}
      ConfigWord3&=0xF07F; //We clean all the prescale 2 bits so we can rewrite them.
      ConfigWord4&=0xFFF8; 
      //now we need to split the value into two pieces
      int value1=value;
      int value2=value;
      value1&=0x1F; //
      value1=value1<<7;
      value2&=0xE0; //
      value2=value2>>5;
      ConfigWord3=(ConfigWord3|value1);
      ConfigWord4=(ConfigWord4|value2);
      break;
    }
    
    //Set Prescale 3 (is on word4).
    case 3:{
      if (value>255){value=255;std::cout<<"Prescale 3 value is too big, setting it to 255."<<std::endl;}
      ConfigWord4&=0xF807; //We clean all the prescale 3 bits so we can rewrite them.
      value=value<<3;
      ConfigWord4=(ConfigWord4|value);
      break;
    }
    
    //Set Prescale 4 (is on word4 and 5).
    case 4:{
      if (value>255){value=255;std::cout<<"Prescale 4 value is too big, setting it to 255."<<std::endl;}
      ConfigWord4&=0xF7FF; //We clean all the prescale 4 bits so we can rewrite them.
      ConfigWord5&=0xFF80; 
      //now we need to split the value into two pieces
      int value1=value;
      int value2=value;
      value1&=0x1; //
      value1=value1<<11;
      value2&=0xFE; //
      value2=value2>>1;
      ConfigWord4=(ConfigWord4|value1);
      ConfigWord5=(ConfigWord5|value2);
      break;
    }

    //Set Prescale 5 (is on word5 and 6).
    case 5:{
      if (value>255){value=255;std::cout<<"Prescale 5 value is too big, setting it to 255."<<std::endl;}
      ConfigWord5&=0xF07F; //We clean all the prescale 2 bits so we can rewrite them.
      ConfigWord6&=0xFFF8; 
      //now we need to split the value into two pieces
      int value1=value;
      int value2=value;
      value1&=0x1F; //
      value1=value1<<7;
      value2&=0xE0; //
      value2=value2>>5;
      ConfigWord5=(ConfigWord5|value1);
      ConfigWord6=(ConfigWord6|value2);
      break;
    }
  }
}

/**
 *  @brief Set TrackerDelay.
 *
 *  @details 7 bits on word 6 of the configuration.
 *
 *  @param value is an int that you can set from 0 to 127
 *
 *  @note If the users sets a value greater than 127 then it automatically sets the value to 127.
 *
 *  @return void
 */
void ConfigReg::SetTrackerDelay(int value){
    if (value>127){value=127;std::cout<<"Tracker Delay value is too big, setting it to 127."<<std::endl;}
  ConfigWord6&=0xFC07;
  value=value<<3;
  ConfigWord6=(ConfigWord6|value);
}

/**
 *  @brief Set Digitizer Delay.
 *
 *  @details 7 bits on word6 and 7 of the configuration.
 *
 *  @param value is an int that you can set from 0 to 127
 *
 *  @note If the users sets a value greater than 127 then it automatically sets the value to 127.
 *
 *  @return void
 */
void ConfigReg::SetDigitizerDelay(int value){
  if (value>127){value=127;std::cout<<"Digitizer Delay value is too big, setting it to 127."<<std::endl;}
  ConfigWord6&=0xF3FF;
  ConfigWord7&=0xFFE0; 
  //now we need to split the value into two pieces
  int value1=value;
  int value2=value;
  value1&=0x3; //
  value1=value1<<10;
  value2&=0x7C; //
  value2=value2>>2;
  ConfigWord6=(ConfigWord6|value1);
  ConfigWord7=(ConfigWord7|value2);
}

/**
 *  @brief Set LHC CLK.
 *
 *  @details 1 bit on word 7 of the configuration.
 *
 *  @param value is an int that you can set from 0 to 1
 *
 *  @note If the users sets a value greater than 1 then it automatically sets the value to 1.
 *
 *  @return void
 */
void ConfigReg::SetLHC_CLK(int value){
  if (value>1){value=1;std::cout<<"LHC CLK value is too big, setting it to 1."<<std::endl;}
  ConfigWord7&=0xFFDF;
  value=value<<5;
  ConfigWord7=(ConfigWord7|value);
}

/**
 *  @brief Set Orbit Delay.
 *
 *  @details 12 bits on word7 and 8 of the configuration.
 *
 *  @param value is an int that you can set from 0 to 4095
 *
 *  @note If the users sets a value greater than 4095 then it automatically sets the value to 4095.
 *
 *  @return void
 */
void ConfigReg::SetOrbitDelay(int value){
  if (value>4095){value=4095;std::cout<<"Orbit Delay value is too big, setting it to 4095."<<std::endl;}
  ConfigWord7&=0xF03F;
  ConfigWord8&=0xFFC0; 
  //now we need to split the value into two pieces
  int value1=value;
  int value2=value;
  value1&=0x3F; //
  value1=value1<<6;
  value2&=0xFC0; //
  value2=value2>>6;
  ConfigWord7=(ConfigWord7|value1);
  ConfigWord8=(ConfigWord8|value2);
}

/**
 *  @brief Set Deadtime.
 *
 *  @details 12 bits on word8 and 9 of the configuration.
 *
 *  @param value is an int that you can set from 0 to 4095
 *
 *  @note If the users sets a value greater than 4095 then it automatically sets the value to 4095.
 *
 *  @return void
 */
void ConfigReg::SetDeadtime(int value){
  if (value>4095){value=4095;std::cout<<"Deadtime value is too big, setting it to 4095."<<std::endl;}
  ConfigWord8&=0xF03F;
  ConfigWord9&=0xFFC0; 
  //now we need to split the value into two pieces
  int value1=value;
  int value2=value;
  value1&=0x3F; //
  value1=value1<<6;
  value2&=0xFC0; //
  value2=value2>>6;
  ConfigWord8=(ConfigWord8|value1);
  ConfigWord9=(ConfigWord9|value2);
}

/**
 *  @brief Set Monitoring Rate.
 *
 *  @details 20 bits on word9, 10 and 11 of the configuration.
 *
 *  @param value is an int that you can set from 0 to 1048575
 *
 *  @note If the users sets a value greater than 1048575 then it automatically sets the value to 1048575.
 *
 *  @return void
 */
void ConfigReg::SetMonitoringRate(int value){
  if (value>1048575){value=1048575;std::cout<<"Monitoring Rate value is too big, setting it to 1048575."<<std::endl;}
  ConfigWord9&=0xF03F;
  ConfigWord10&=0xF000;
  ConfigWord11&=0xFFFC;  
  //now we need to split the value into three pieces
  int value1=value;
  int value2=value;
  int value3=value;
  value1&=0x3F; //
  value1=value1<<6;
  value2&=0x3FFC0;
  value2=value2>>6;
  value3&=0xC0000;
  value3=value3>>18;
  ConfigWord9=(ConfigWord9|value1);
  ConfigWord10=(ConfigWord10|value2);
  ConfigWord11=(ConfigWord11|value3);
}

/**
 *  @brief Set Output Destination.
 *
 *  @details 32 bits on word11, 12 and 13 of the configuration.
 *
 *  @param value is an int that you can set from 0 to 4294967295
 *
 *  @note If the users sets a value greater than 4294967295 then it automatically sets the value to 4294967295.
 *
 *  @return void
 */
void ConfigReg::SetOutputDestination(unsigned long int value){
  if (value>4294967295){value=4294967295;std::cout<<"Output Destination value is too big, setting it to 4294967295."<<std::endl;}
  ConfigWord11&=0xF003;
  ConfigWord12&=0xF000;
  ConfigWord13&=0xFC00;  
  //now we need to split the value into three pieces
  unsigned long int value1=value;
  unsigned long int value2=value;
  unsigned long int value3=value;
  value1&=0x3FF; //
  value1=value1<<2;
  value2&=0x3FFC00;
  value2=value2>>10;
  value3&=0xFFC00000;
  value3=value3>>22;
  ConfigWord11=(ConfigWord11|value1);
  ConfigWord12=(ConfigWord12|value2);
  ConfigWord13=(ConfigWord13|value3);
}


/**
 *  @brief Set the Inputs.
 *
 *  @details 
 *
 *  @param InputNumber is an int that select which input to toggle.
 *  @param value is an int that you can set from 0 to 1
 *
 *  @note If the users sets a value greater than 1 then it automatically sets the value to 1.
 *
 *  @return void
 */
void ConfigReg::SetInput(int InputNumber, int value){
  
  switch (InputNumber)
  {
    //Set Input 0, 1 bit on word 13.
    case 0:{
      if (value>1){value=1;std::cout<<"Input 0 value is too big, setting it to 1."<<std::endl;}
      ConfigWord13&=0xFBFF;
      value=value<<10;
      ConfigWord13=(ConfigWord13|value);
      break;
    }

    //Set Input 1, 1 bit on word 13.
    case 1:{
      if (value>1){value=1;std::cout<<"Input 1 value is too big, setting it to 1."<<std::endl;}
      ConfigWord13&=0xF7FF;
      value=value<<11;
      ConfigWord13=(ConfigWord13|value);
      break;
    }

    //Set Input 2, 1 bit on word 14.
    case 2:{
      if (value>1){value=1;std::cout<<"Input 2 value is too big, setting it to 1."<<std::endl;}
      ConfigWord14&=0xFFFE;
      ConfigWord14=(ConfigWord14|value);
      break;
    }

    //Set Input 3, 1 bit on word 14.
    case 3:{
      if (value>1){value=1;std::cout<<"Input 3 value is too big, setting it to 1."<<std::endl;}
      ConfigWord14&=0xFFFD;
      value=value<<1;
      ConfigWord14=(ConfigWord14|value);
      break;
    }

    //Set Input 4, 1 bit on word 14.
    case 4:{
      if (value>1){value=1;std::cout<<"Input 4 value is too big, setting it to 1."<<std::endl;}
      ConfigWord14&=0xFFFB;
      value=value<<2;
      ConfigWord14=(ConfigWord14|value);
      break;
    }

    //Set Input 5, 1 bit on word 14.
    case 5:{
      if (value>1){value=1;std::cout<<"Input 5 value is too big, setting it to 1."<<std::endl;}
      ConfigWord14&=0xFFF7;
      value=value<<3;
      ConfigWord14=(ConfigWord14|value);
      break;
    }

    //Set Input 6, 1 bit on word 14.
    case 6:{
      if (value>1){value=1;std::cout<<"Input 6 value is too big, setting it to 1."<<std::endl;}
      ConfigWord14&=0xFFEF;
      value=value<<4;
      ConfigWord14=(ConfigWord14|value);
      break;
    }

    //Set Input 7, 1 bit on word 14.
    case 7:{
      if (value>1){value=1;std::cout<<"Input 7 value is too big, setting it to 1."<<std::endl;}
      ConfigWord14&=0xFFDF;
      value=value<<5;
      ConfigWord14=(ConfigWord14|value);
      break;
    }
  }
}

/////////////////////
//Direct Parameters//
/////////////////////

/**
 *  @brief Set Reset.
 *
 *  @details 1bit.
 *
 *  @param value is an int that you can set from 0 to 1
 *
 *  @note If the users sets a value greater than 1 then it automatically sets the value to 1.
 *
 *  @return void
 */
void ConfigReg::SetReset(int value){
  if (value>1){value=1;std::cout<<"Reset value is too big, setting it to 1."<<std::endl;}
  DirectParam&=0xFFFE;
  DirectParam=(DirectParam|value);  
}

/**
 *  @brief Set ECR.
 *
 *  @details 1bit.
 *
 *  @param value is an int that you can set from 0 to 1
 *
 *  @note If the users sets a value greater than 1 then it automatically sets the value to 1.
 *
 *  @return void
 */
void ConfigReg::SetECR(int value){
  if (value>1){value=1;std::cout<<"ECR value is too big, setting it to 1."<<std::endl;}
  DirectParam&=0xFFFD;
  value=value<<1;
  DirectParam=(DirectParam|value);  
}

/**
 *  @brief Set Trigger Enable.
 *
 *  @details 1bit.
 *
 *  @param value is an int that you can set from 0 to 1
 *
 *  @note If the users sets a value greater than 1 then it automatically sets the value to 1.
 *
 *  @return void
 */
void ConfigReg::SetTriggerEnable(int value){
  if (value>1){value=1;std::cout<<"TriggerEnable value is too big, setting it to 1."<<std::endl;}
  DirectParam&=0xFFFB;
  value=value<<2;
  DirectParam=(DirectParam|value);  
}

/**
 *  @brief Set Software Trigger.
 *
 *  @details 1bit.
 *
 *  @param value is an int that you can set from 0 to 1
 *
 *  @note If the users sets a value greater than 1 then it automatically sets the value to 1.
 *
 *  @return void
 */
void ConfigReg::SetSoftwareTrigger(int value){
  if (value>1){value=1;std::cout<<"SoftwareTrigger value is too big, setting it to 1."<<std::endl;}
  DirectParam&=0xFFF7;
  value=value<<3;
  DirectParam=(DirectParam|value);  
}

/**
 *  @brief Set Busy Disable.
 *
 *  @details 1bit.
 *
 *  @param value is an int that you can set from 0 to 1
 *
 *  @note If the users sets a value greater than 1 then it automatically sets the value to 1.
 *
 *  @return void
 */
void ConfigReg::SetBusyDisable(int value){
  if (value>1){value=1;std::cout<<"BusyDisable value is too big, setting it to 1."<<std::endl;}
  DirectParam&=0xFFEF;
  value=value<<4;
  DirectParam=(DirectParam|value);  
}

/////////////////////////////
//Data Readout Parameters////
/////////////////////////////

/**
 *  @brief Set Enable Trigger Data.
 *
 *  @details 1bit.
 *
 *  @param value is an int that you can set from 0 to 1
 *
 *  @note If the users sets a value greater than 1 then it automatically sets the value to 1.
 *
 *  @return void
 */
void ConfigReg::SetEnableTriggerData(int value){
  if (value>1){value=1;std::cout<<"EnableTriggerData value is too big, setting it to 1."<<std::endl;}
  DataReadoutParam&=0xFFFE;
  DataReadoutParam=(DataReadoutParam|value);  
}

/**
 *  @brief Set Enable Monitoring Data.
 *
 *  @details 1bit.
 *
 *  @param value is an int that you can set from 0 to 1
 *
 *  @note If the users sets a value greater than 1 then it automatically sets the value to 1.
 *
 *  @return void
 */
void ConfigReg::SetEnableMonitoringData(int value){
  if (value>1){value=1;std::cout<<"enableMonitoringData value is too big, setting it to 1."<<std::endl;}
  DataReadoutParam&=0xFFFD;
  value=value<<1;
  DataReadoutParam=(DataReadoutParam|value);  
}

/**
 *  @brief Set FIFO Reset.
 *
 *  @details 1bit.
 *
 *  @param value is an int that you can set from 0 to 1
 *
 *  @note If the users sets a value greater than 1 then it automatically sets the value to 1.
 *
 *  @return void
 */
void ConfigReg::SetReadoutFIFOReset(int value){
  if (value>1){value=1;std::cout<<"ReadoutFIFOReset value is too big, setting it to 1."<<std::endl;}
  DataReadoutParam&=0xFFFB;
  value=value<<2;
  DataReadoutParam=(DataReadoutParam|value);  
}

/**
 *  @brief Prints the Configuration.
 *
 *  @details Couts the 15 Configuration words.
 *
 *  Example: 
 *  @code std::cout<<"ConfigWord0:  "<<std::bitset<16>(ConfigWord0)<<std::endl;
    @endcode
 *  @return void
 */
void ConfigReg::PrintConfig(){
  std::cout<<std::endl;
  std::cout<<"Printing the Configuration"<<std::endl;
  std::cout<<"______________________________"<<std::endl;
  std::cout<<"ConfigWord0:  "<<std::bitset<16>(ConfigWord0)<<std::endl;
  std::cout<<"ConfigWord1:  "<<std::bitset<16>(ConfigWord1)<<std::endl;
  std::cout<<"ConfigWord2:  "<<std::bitset<16>(ConfigWord2)<<std::endl;
  std::cout<<"ConfigWord3:  "<<std::bitset<16>(ConfigWord3)<<std::endl;
  std::cout<<"ConfigWord4:  "<<std::bitset<16>(ConfigWord4)<<std::endl;
  std::cout<<"ConfigWord5:  "<<std::bitset<16>(ConfigWord5)<<std::endl;
  std::cout<<"ConfigWord6:  "<<std::bitset<16>(ConfigWord6)<<std::endl;
  std::cout<<"ConfigWord7:  "<<std::bitset<16>(ConfigWord7)<<std::endl;
  std::cout<<"ConfigWord8:  "<<std::bitset<16>(ConfigWord8)<<std::endl;
  std::cout<<"ConfigWord9:  "<<std::bitset<16>(ConfigWord9)<<std::endl;
  std::cout<<"ConfigWord10: "<<std::bitset<16>(ConfigWord10)<<std::endl;
  std::cout<<"ConfigWord11: "<<std::bitset<16>(ConfigWord11)<<std::endl;
  std::cout<<"ConfigWord12: "<<std::bitset<16>(ConfigWord12)<<std::endl;
  std::cout<<"ConfigWord13: "<<std::bitset<16>(ConfigWord13)<<std::endl;
  std::cout<<"ConfigWord14: "<<std::bitset<16>(ConfigWord14)<<std::endl;
  std::cout<<"______________________________"<<std::endl;  
}

/**
 *  @brief Print the Direct Parameters configuration.
 *
 *  @details
 *
 *  Example: 
 *  @code std::cout<<"DirectParam:  "<<std::bitset<16>(DirectParam)<<std::endl;
    @endcode
 *  @return void
 */
void ConfigReg::PrintDirectParam(){
  std::cout<<std::endl;
  std::cout<<"Printing the DirectParameters"<<std::endl;
  std::cout<<"______________________________"<<std::endl;
  std::cout<<"DirectParam:  "<<std::bitset<16>(DirectParam)<<std::endl;

  std::cout<<"______________________________"<<std::endl;  
}

/**
 *  @brief Print the Data Readout parameters configuration.
 *
 *  @details
 *
 *  Example: 
 *  @code std::cout<<"DataReadoutParam:  "<<std::bitset<16>(DataReadoutParam)<<std::endl;
    @endcode
 *  @return void
 */
void ConfigReg::PrintDataReadoutParam(){
  std::cout<<std::endl;
  std::cout<<"Printing the Data Readout Parameters"<<std::endl;
  std::cout<<"___________________________________"<<std::endl;
  std::cout<<"DataReadoutParam:  "<<std::bitset<16>(DataReadoutParam)<<std::endl;

  std::cout<<"___________________________________"<<std::endl;  
}