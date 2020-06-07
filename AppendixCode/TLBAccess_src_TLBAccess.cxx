//Author: Eliott Johnson
#include<iostream>
#include<bitset>
#include<vector>
#include<unistd.h> //for usleep
#define _ms 1000 // used for usleep
#define MAX_VERIFY 2
#include <fstream>
#include <string>
#include "TLBAccess/TLBAccess.h"
#include "TLBAccess/TLBDecode.h"
#include <string> 

using namespace FASER;
using json = nlohmann::json;

TLBAccess::~TLBAccess()
{
}


/**
 *  @brief Cleans the TLB.
 *
 *  @details
 *   Sends a serie of SendAndRetrieve to set
 *   all 15 Configuration Word (0-14) to zero
 *   that are public attributes of ConfigReg.
 *
 *  @see   GPIOBaseClass::SendAndRetrieve
 *  @see   TLBCmdID
 *  @see   ConfigReg
 * 
 *  @note You need to clean the board so that you toggle only what you need and you don't get stuck with old configuration.
 *  @warning The inital ConfigWord get overwritten.
 *  
 *  Example: 
 *  @code SendAndRetrieve(TLBCmdID::USER_SET_CONFIG,m_tlb_config.ConfigWord0);
    @endcode
 *  @return void
 */ 
void TLBAccess::CleanConfig(){
  //std::cout<<"Cleaning the board"<<std::endl;
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG, 0x0000);
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG, 0x1000);
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG, 0x2000);
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG, 0x3000);
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG, 0x4000);
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG, 0x5000);
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG, 0x6000);
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG, 0x7000);
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG, 0x8000);
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG, 0x9000);
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG, 0xA000);
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG, 0xB000);
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG, 0xC000);
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG, 0xD000);
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG, 0xE000);
  //std::cout << "   Done."<<std::endl;
}


 /**
 *  @brief Send the configuration.
 *
 *  @details
 *   Sends all the Configuration Words at once
 *   using 15 SendAndRetrieve.
 *
 *  @see   GPIOBaseClass::SendAndRetrieve
 *  @see   TLBCmdID
 *  @see   ConfigReg
 *  @see   ConfigureTLB
 * 
 *  @pre  The ConfigWords need to be set with the ConfigureTLB().
 *
 *  Example: 
 *  @code SendAndRetrieve(TLBCmdID::USER_SET_CONFIG,m_tlb_config.ConfigWord0);
    @endcode
 *  @return void
 */
void TLBAccess::SendConfig()
{
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG,m_tlb_config.ConfigWord0);
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG,m_tlb_config.ConfigWord1);
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG,m_tlb_config.ConfigWord2);
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG,m_tlb_config.ConfigWord3);
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG,m_tlb_config.ConfigWord4);
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG,m_tlb_config.ConfigWord5);
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG,m_tlb_config.ConfigWord6);
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG,m_tlb_config.ConfigWord7);
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG,m_tlb_config.ConfigWord8);
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG,m_tlb_config.ConfigWord9);
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG,m_tlb_config.ConfigWord10);
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG,m_tlb_config.ConfigWord11);
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG,m_tlb_config.ConfigWord12);
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG,m_tlb_config.ConfigWord13);
  SendAndRetrieve(TLBCmdID::USER_SET_CONFIG,m_tlb_config.ConfigWord14);
}

 /**
 *  @brief Sends the direct parameters.
 *
 *  @details
 *   Sends the direct parameters Reset, ECR, TriggerEnable, Software Trigger and BusyDisable.
 *
 *  @see   GPIOBaseClass::SendAndRetrieve
 *  @see   TLBCmdID
 *  @see   ConfigReg
 *
 *  Example: 
 *  @code SendAndRetrieve(TLBCmdID::SET_DIRECT_PARAM, m_tlb_config.DirectParam);
    @endcode
 *  @return void
 */
void TLBAccess::SendDirectParameters()
{
  SendAndRetrieve(TLBCmdID::SET_DIRECT_PARAM, m_tlb_config.DirectParam);
}

 /**
 *  @brief Disables the Trigger.
 *
 *  @details
 *   Sends the direct parameters with Reset = 0, ECR = 0, TriggerEnable = 0, Software Trigger as in the config and BusyDisable as in the config.
 *
 *  @see   GPIOBaseClass::SendAndRetrieve
 *  @see   TLBCmdID
 *  @see   ConfigReg
 *
 *  Example: 
 *  @code SendAndRetrieve(TLBCmdID::SET_DIRECT_PARAM, m_tlb_config.DirectParam);
    @endcode
 *  @return void
 */
void TLBAccess::DisableTrigger()
{
  uint16_t copy = m_tlb_config.DirectParam;
  copy&=0x18; // sets //000
  //std::cout<<"m_tlb_config.DirectParam: "<<std::bitset<16>(m_tlb_config.DirectParam)<<" copy: "<<std::bitset<16>(copy)<<std::endl;
  SendAndRetrieve(TLBCmdID::SET_DIRECT_PARAM, copy);
}

 /**
 *  @brief Enables the Trigger.
 *
 *  @details
 *   Sends the direct parameters with ECR and Reset as a boolean arguments. First argument is ECR, second argument is Reset.
 *   TriggerEnable is set to 1, Software Trigger and BusyDisable as in the config.
 *
 *  @see   GPIOBaseClass::SendAndRetrieve
 *  @see   TLBCmdID
 *  @see   ConfigReg
 *
 *  Example: 
 *  @code SendAndRetrieve(TLBCmdID::SET_DIRECT_PARAM, m_tlb_config.DirectParam);
    @endcode
 *  @return void
 */
void TLBAccess::EnableTrigger(bool ECR, bool Reset)
{
  uint16_t copy = m_tlb_config.DirectParam;
  copy&=0x18; //set Trigger, Reset and ECR to OFF to clean the bits if they were on, //000
  if (ECR==true){
   copy|=0x2; //sets the ECR to ON ///1/
  }
  if (Reset==true){
   copy|=0x1; //sets the Reset to ON ////1
  }
  copy|=0x4; //sets Trigger Enable to ON //1//
  //std::cout<<"m_tlb_config.DirectParam: "<<std::bitset<16>(m_tlb_config.DirectParam)<<" copy: "<<std::bitset<16>(copy)<<std::endl;
  SendAndRetrieve(TLBCmdID::SET_DIRECT_PARAM, copy);
}

 /**
 *  @brief Send ECR.
 *
 *  @details 
 *  Send the direct parameters with TriggerEnable=0, ECR=1, Reset=0. SoftwareTrigger and BusyDisable as in the config.
 *
 *  @see   GPIOBaseClass::SendAndRetrieve
 *  @see   TLBCmdID
 *  @see   ConfigReg
 *
 *  Example: 
 *  @code SendAndRetrieve(TLBCmdID::SET_DIRECT_PARAM, m_tlb_config.DirectParam);
    @endcode
 *  @return void
 */
void TLBAccess::SendECR()
{
  uint16_t copy = m_tlb_config.DirectParam;
  copy&=0x18; //set Trigger, Reset and ECR to OFF to clean the bits if they were on, //000
  copy|=0x2; //sets ECR to ON ///1/
  SendAndRetrieve(TLBCmdID::SET_DIRECT_PARAM, copy);
}

//THIS I THINK WE DON'T NEED
/**
 *  @brief Function that will send the Readout Parameters.
 *
 *
 *  @see   GPIOBaseClass::SendAndRetrieve
 *  @see   TLBCmdID
 *  @see   ConfigReg
 *
 *  @note  I think we can delete this function because we actually use the values when we start readout.
 *
 *  Example: 
 *  @code SendAndRetrieve(TLBCmdID::SET_DIRECT_PARAM, m_tlb_config.DataReadoutParam);
    @endcode
 *  @return void
 */
void TLBAccess::SendDataReadoutParameters()
{
  std::cout << "Sending Data Readout parameters."<<std::endl;
  SendAndRetrieve(TLBCmdID::SET_DIRECT_PARAM, m_tlb_config.DataReadoutParam);
  std::cout << "   Done."<<std::endl; 
}

/**
 *  @brief Sends the TLB configuration.
 *
 *  @details
 *   Cleans the board, reads the configuration JSON then sets and sends configuration + direct parameters. 
 *   After that, verifies if the configuration has been applied correctly. If the verification has failed
 *   it will try again to configure the board.
 *
 *  @see   ConfigReg
 * 
 *  @note  This uses multiple smaller functions.
 *
 *  Example: 
 *  @code SendAndRetrieve(TLBCmdID::USER_SET_CONFIG,m_tlb_config.ConfigWord0);
    @endcode
 *  @return void.
 */
void TLBAccess::ConfigureTLB(json myjson)
{
  CleanConfig();
  
  for (int i=0; i<8; i++){
  m_tlb_config.SetSamplingPhase(i,myjson["SamplingPhase"][i].get<bool>());}
  m_tlb_config.SetInputDelay(myjson["InputDelay"][0].get<int>(),myjson["InputDelay"][1].get<int>(),myjson["InputDelay"][2].get<int>(),myjson["InputDelay"][3].get<int>(),myjson["InputDelay"][4].get<int>(),myjson["InputDelay"][5].get<int>(),myjson["InputDelay"][6].get<int>(),myjson["InputDelay"][7].get<int>());
  m_tlb_config.SetRandomTriggerRate(myjson["RandomTriggerRate"].get<int>());
  for (int i=0; i<6; i++){
  m_tlb_config.SetPrescale(i,myjson["Prescale"][i].get<int>());}//4 is random generator, 5 is software trigger
  m_tlb_config.SetTrackerDelay(myjson["TrackerDelay"].get<int>());
  m_tlb_config.SetDigitizerDelay(myjson["DigitizerDelay"].get<int>());
  m_tlb_config.SetLHC_CLK(myjson["LHC_CLK"].get<bool>());
  m_tlb_config.SetOrbitDelay(myjson["OrbitDelay"].get<int>());
  m_tlb_config.SetDeadtime(myjson["Deadtime"].get<int>());
  m_tlb_config.SetMonitoringRate(myjson["MonitoringRate"].get<int>());
  m_tlb_config.SetOutputDestination(myjson["OutputDestination"].get<int>());
  for (int i=0; i<8; i++){
  m_tlb_config.SetInput(i,myjson["Input"][i].get<bool>());}
  SendConfig();
  //m_tlb_config.PrintConfig();
  m_tlb_config.SetReset(myjson["Reset"].get<bool>());
  m_tlb_config.SetECR(myjson["ECR"].get<bool>());
  m_tlb_config.SetTriggerEnable(myjson["TriggerEnable"].get<bool>());
  m_tlb_config.SetSoftwareTrigger(myjson["SoftwareTrigger"].get<bool>());
  m_tlb_config.SetBusyDisable(myjson["BusyDisable"].get<bool>());
  SendDirectParameters();
  //m_tlb_config.PrintDirectParam();
}

/**
 *  @brief Sends the TLB configuration and Verifies it.
 *
 *  @details
 *   Cleans the board, reads the configuration JSON then sets and sends configuration + direct parameters. 
 *   After that, verifies if the configuration has been applied correctly. If the verification has failed
 *   it will try again to configure the board.
 *
 *  @see   ConfigReg
 * 
 *  @note  This uses multiple smaller functions.
 *
 *  Example: 
 *  @code SendAndRetrieve(TLBCmdID::USER_SET_CONFIG,m_tlb_config.ConfigWord0);
    @endcode
 *  @return a boolean called ConfigSuccess that is true if no error.
 */
bool TLBAccess::ConfigureAndVerifyTLB(json myjson)
{
  bool ConfigSuccess = false;
  uint8_t NumberOfVerify = 0;
  
  while (!ConfigSuccess && NumberOfVerify<=MAX_VERIFY){//if there is an error then you can retry a number of time to reconfigure the TLB
    NumberOfVerify+=1;
    ConfigureTLB(myjson);
    if (VerifyConfiguration()){ConfigSuccess=true;}
    else {
      std::cout<<"   Error: Verification failed - there was an error configuring the board "<<std::endl;
      std::cout<<"      Re-configuring TLB. Attempt: "<<NumberOfVerify<<std::endl;
    }
  }
  
  return (ConfigSuccess);
}

/**
 *  @brief Sends the LUT.
 *
 *  @details Sends the LUT to the board and verifies (by CRC check) if 
 *  the configuration has been done properly (i.e. there has not been any transfer error).
 *
 *  @return void
 */
void TLBAccess::ConfigureLUT(std::string PathToLUT1) //TODO Change parameter from string to json object (or anything else but string), which does not have to be hardcoded in the source code
{
  uint8_t DevNum = 0;
  
  SetLUT(DevNum, PathToLUT1);

  if (VerifyLUT_byCRC(DevNum)){ std::cout << "   LUT Verification OK"<< std::endl;}
  else {std::cout << "LUT Verification failed."<< std::endl;}
}

/**
 *  @brief Verifies the configuration on the board.
 *
 *  @details
 *   Read the configuration on the board and compares it to the ConfigWords.
 *
 * 
 *  @warning I run twice 15 SendAndRetrieve to "refresh the buffer" because otherwise you get the old configuration
 *  but I'm not sure why you need to do this.
 *
 *  @return a boolean called VerifySuccess that is true if no error.
 */
bool TLBAccess::VerifyConfiguration()
{
  bool VerifySuccess = true;

  uint16_t param = 0x0,answer = 0x0;
  for (int i = 0; i < 15; i++){
    SendAndRetrieve(TLBCmdID::USER_GET, param, &answer);
    if (i==0 && (answer>>12)!=0) {i=-1;} //avoids not starting at zero
   }
  for (int i = 0; i < 15; i++){ //I don't know why but you need to read twice to get a correct readout
    SendAndRetrieve(TLBCmdID::USER_GET, param, &answer);
    if (i==0 && (answer>>12)!=0) {i=-1;} //avoids not starting at zero
    else{
      if (i==0 && (m_tlb_config.ConfigWord0 != answer)){std::cout<<"Configuration verification failed for word "<<i<<std::endl;VerifySuccess=false;}
      if (i==1 && (m_tlb_config.ConfigWord1 != answer)){std::cout<<"Configuration verification failed for word "<<i<<std::endl;VerifySuccess=false;}
      if (i==2 && (m_tlb_config.ConfigWord2 != answer)){std::cout<<"Configuration verification failed for word "<<i<<std::endl;VerifySuccess=false;}
      if (i==3 && (m_tlb_config.ConfigWord3 != answer)){std::cout<<"Configuration verification failed for word "<<i<<std::endl;VerifySuccess=false;}
      if (i==4 && (m_tlb_config.ConfigWord4 != answer)){std::cout<<"Configuration verification failed for word "<<i<<std::endl;VerifySuccess=false;}
      if (i==5 && (m_tlb_config.ConfigWord5 != answer)){std::cout<<"Configuration verification failed for word "<<i<<std::endl;VerifySuccess=false;}
      if (i==6 && (m_tlb_config.ConfigWord6 != answer)){std::cout<<"Configuration verification failed for word "<<i<<std::endl;VerifySuccess=false;}
      if (i==7 && (m_tlb_config.ConfigWord7 != answer)){std::cout<<"Configuration verification failed for word "<<i<<std::endl;VerifySuccess=false;}
      if (i==8 && (m_tlb_config.ConfigWord8 != answer)){std::cout<<"Configuration verification failed for word "<<i<<std::endl;VerifySuccess=false;}
      if (i==9 && (m_tlb_config.ConfigWord9 != answer)){std::cout<<"Configuration verification failed for word "<<i<<std::endl;VerifySuccess=false;}
      if (i==10 && (m_tlb_config.ConfigWord10 != answer)){std::cout<<"Configuration verification failed for word "<<i<<std::endl;VerifySuccess=false;}
      if (i==11 && (m_tlb_config.ConfigWord11 != answer)){std::cout<<"Configuration verification failed for word "<<i<<std::endl;VerifySuccess=false;}
      if (i==12 && (m_tlb_config.ConfigWord12 != answer)){std::cout<<"Configuration verification failed for word "<<i<<std::endl;VerifySuccess=false;}
      if (i==13 && (m_tlb_config.ConfigWord13 != answer)){std::cout<<"Configuration verification failed for word "<<i<<std::endl;VerifySuccess=false;}
      if (i==14 && (m_tlb_config.ConfigWord14 != answer)){std::cout<<"Configuration verification failed for word "<<i<<std::endl;VerifySuccess=false;}
    }
  }
  return (VerifySuccess);
}


/**
 *  @brief Starts the data readout.
 *
 *  @details
 *   Sends a SendAndRetrieve command to the TLB to start the data readout using a param
 *   that specifies if you want trigger or monitoring. It also check wether the param==answer.
 *   It also starts a new thread for data polling.
 *
 *  @param param is a unint16_t that combines EnableTriggerData EnableMonitoringData and ReadoutFIFOReset.
 *
 *  @return void
 */
void TLBAccess::StartReadout(uint16_t param)
{
  std::cout << "Reading Data"<<std::endl;
  m_DataWordOutputBuffer.clear();
  mMutex_TLBEventData.lock(); //here we have to lock because its a mutex object and we are accessing it in a thread with shared ressources
  m_TLBEventData.clear(); //this is the vector of vector being cleared
  mMutex_TLBEventData.unlock();
  uint16_t answer = 0x0;
  SendAndRetrieve(TLBCmdID::DATA_READOUT, param, &answer);
  if (answer!=param) {std::cout<<"Error: param sent does not match answer"<<std::endl;}
  // now start the data polling in a separate thread
  m_daqRunning = true;
  m_daqThread = new std::thread(&TLBAccess::PollData, this);
}

 /**
 *  @brief Send 'stop readout'command to TLB
 *
 *  @details
 *   Sends a SendAndRetrieve with a special parameter that stops the readout.
 *   It also closes the data polling thread.
 *
 *  @param param is a unint16_t = 0x1.
 *
 *  @return void
 */
void TLBAccess::StopReadout(){
  uint16_t param = 0x1;
  SendAndRetrieve(TLBCmdID::DATA_READOUT, param);
  // and now stop the data polling thread
  if (m_daqThread != nullptr){
    m_daqRunning = false;
    m_daqThread->join();
    delete m_daqThread;
    m_daqThread = nullptr;
  }
  std::cout << "   Stopped readout thread."<<std::endl; 
}
 
 /**
 *  @brief Return a vector of TLB RAW data per event.
 *
 *  @details
 *   
 *
 *
 *  @note The internally stored raw data will be deleted when called!
 *
 *  @return Returns a vector<std::vector<uint32_t>> containing events in the first vector and the event's word in a vector inside the first vector.
 */
std::vector<std::vector<uint32_t>> TLBAccess::GetTLBEventData() 
{
   mMutex_TLBEventData.lock(); 
   auto copy = m_TLBEventData; 
   m_TLBEventData.clear();
   mMutex_TLBEventData.unlock(); 

   return copy;
}

 /**
 *  @brief Read any data pending in the data-out buffer of the TLB
 *
 *  @details
 *   
 *
 *  @return void
 */
void TLBAccess::PollData(){
  std::vector<unsigned char> tmp;
  uint32_t TLBWord = 0;
  int TLBWord_ID = 0;
  int bytesRead = 0;
  int totalBytesTransfered = 0;
  int sumBytesTransfered = 0;
  int transfers = 0;
  int HeaderNumber = 0;

  TLBDecode *decode = new TLBDecode();
  
  auto start = std::chrono::high_resolution_clock::now();
  auto stop = start; 
  
  while (m_daqRunning || bytesRead !=0){
  //usleep(50*_ms);
    try {
      if (m_DEBUG){std::cout << "Issuing USB read, last read returned "<< bytesRead<< " bytes" <<std::endl;}
      tmp = m_interface->ReadData(bytesRead);
      stop = std::chrono::high_resolution_clock::now();
      totalBytesTransfered += bytesRead;
      sumBytesTransfered += bytesRead;
      transfers++;
      
      auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
      if (duration.count() > 1000000){
        if (m_DEBUG){std::cout << "   Data rate = "<<(double)sumBytesTransfered/(double)duration.count() << " MB/sec"<<std::endl;}
        start =  std::chrono::high_resolution_clock::now();
        sumBytesTransfered = 0;
      } 
    }
    catch (...){
    std::cout << "ERROR: Read Serial threw an error. Continuing anyways"<<std::endl;
    }

    //std::cout<<"bytesRead: "<<bytesRead<<std::endl;
    for (unsigned int i = 0; i < bytesRead; i++){
    TLBWord |= ((uint32_t)tmp[i]) << (TLBWord_ID*8);
    TLBWord_ID++;
      if (TLBWord_ID == 4) { // word complete 
        if (decode->IsTriggerHeader(TLBWord) || decode->IsMonitoringHeader(TLBWord) || TLBWord==0xCAFEDECA){ //if it finds any header or trailer then stores an event
          if (m_DataWordOutputBuffer.size()!=0){
            mMutex_TLBEventData.lock();
            m_TLBEventData.push_back(m_DataWordOutputBuffer); // store complete (or incomplete) event
            mMutex_TLBEventData.unlock();
          }
          m_DataWordOutputBuffer.clear();
          HeaderNumber+=1;
          if (m_DEBUG){
            std::cout<<std::dec<<"   Wrote header number: "<<HeaderNumber<<std::endl;
          }
        }
        if (TLBWord!=0xCAFEDECA){
        m_DataWordOutputBuffer.push_back(TLBWord); //Once the word is complete, we push it to this vector.
        }
        TLBWord_ID = 0;
        TLBWord = 0;
      }
    }
  }
  std::cout << "   DAQ: bytes transfered "<<std::dec<<totalBytesTransfered<<" in "<<transfers<<" transfers"<< std::endl;
}

 /**
 *  @brief Load LUT from file to x_LUT
 *
 *  @details Loads LUT from config file to x_LUT vector. One vector component 
 *  contains four 4-bit numbers from LUT. Basic checks of config file 
 *  validiy are performed. Path to file has to be absolute. 
 *   
 *
 *  @return true
 */
bool TLBAccess::LoadLUT(std::string x_PathToLUT, std::vector<uint16_t> *x_LUT)
{   
    std::ifstream LUT_File(x_PathToLUT);
    std::string l_val1, l_val2, l_val3, l_val4;
    int VectorComponent;   

    x_LUT->clear(); //Check that vector is empty 

    while (LUT_File >> l_val1 >> l_val2 >> l_val3 >> l_val4){
        if (l_val1.size() != 4 | l_val2.size() != 4 | l_val3.size() != 4 | l_val4.size() != 4) {
            std::cout << "ERROR: Provided LUT contains value with size different from 4 bits!" << std::endl;
            return false;
        }

        if (!IsBinary(l_val1) | !IsBinary(l_val2) | !IsBinary(l_val3) | !IsBinary(l_val4)){
            std::cout << "ERROR: Provided LUT contains character different from 0 or 1!" << std::endl;
            return false;
        }

        //concatenate 4 4-bit values from the file and convert them to an integer       
        VectorComponent = std::stoi(l_val4 + l_val3 + l_val2 + l_val1, nullptr, 2); 

        x_LUT->push_back(VectorComponent & 0xffff);
    }    
   
    if (x_LUT->size() != 64){
        std::cout << "ERROR: Suspicious LUT dimension. Input file is expected to have 256 lines!" << std::endl;
        return false;
    }    
 
    return true;
}

/**
*  @brief Checks if given string contains only binary characters (i.e. 0 or 1)
*
*   @details Checks if provided string contains only characters 0 or 1.
*   
*
*  @return true
*/
bool TLBAccess::IsBinary(std::string l_value)
{
    if (l_value.find_first_not_of("01", 0) != -1){
        return false;
    }
    return true;
}

/**
*  @brief Send LUT to the board.
*
*  @details Send LUT to the board.
*   
*
*  @return true
*/
bool TLBAccess::SetLUT(uint8_t x_device, std::string x_PathToLUT)
{    
    uint8_t l_CmdID = 0x04;
    std::vector<uint16_t> l_LUT;
    
    if(!LoadLUT(x_PathToLUT, &l_LUT)){
        return false;
    }

    if(!SendMultiargAndRetrieve(l_CmdID, x_device, l_LUT)){
        std::cout << "SetLUT_Send() failed." << std::endl;       
        return false;
    }
    
    if(!VerifyLUT_byBits(x_device, l_LUT)){
        std::cout << "Error during transmission. Try loading LUT another time." << std::endl;
        return false;
    }

    return true;
}

/**
*  @brief Get LUT from the board (+ optionally CRC).
*
*  @details  Read out LUT from the board. LUT is encoded in form of vector of uint16_t.
 Can retrieve CRC red out from the board.
*   
*
*  @return l_LUT
*/
std::vector<uint16_t> TLBAccess::GetLUT(uint16_t x_device, uint16_t *x_CRC)
{
    uint8_t l_CmdID = 0x05;
    std::vector<uint16_t> l_LUT;

    if(!SendAndRetrieveMultiarg(l_CmdID, x_device, &l_LUT, x_CRC)){
        std::cout << "GetLUT() failed." << std::endl;
    }

    return l_LUT;
}

/**
*  @brief Decode LUT encoded to vector of 16-bit integers to readable format.
*
*  @details  Decodes LUT (encoded to vector of uint16_t) to vector of individual values
 stored to the LUT cells.
*
*  @return l_DecodedLUT
*/
std::vector<uint16_t> TLBAccess::DecodeLUT(std::vector<uint16_t> x_LUT)
{
    std::vector<uint16_t> l_DecodedLUT(x_LUT.size()*4);

    for (int i = 0; i < x_LUT.size(); i++)
    {   
        l_DecodedLUT[4*i]     =  x_LUT[i] & 0x000f;
        l_DecodedLUT[4*i + 1] = (x_LUT[i] & 0x00f0) >> 4;
        l_DecodedLUT[4*i + 2] = (x_LUT[i] & 0x0f00) >> 8;
        l_DecodedLUT[4*i + 3] = (x_LUT[i] & 0xf000) >> 12;
    } 

    return l_DecodedLUT;   
}

/**
*  @brief Verification of the configuration to avoid transfer errors. Comparing only CRC.
*
*  @details  Reads out LUT from the board, localy computes its CRC and compares it to the 
 CRC which was retrieved from the board. Returns true for matching CRCs.
*
*  @return true
*/
bool TLBAccess::VerifyLUT_byCRC(uint8_t x_device)
{
    uint16_t l_computedCRC;
    uint16_t l_receivedCRC;
    std::vector<uint16_t> l_receivedLUT;

    CRC_16_1A2EB crc16_1A2EB;
    UFE_CRC CRC_Class = UFE_CRC(crc16_1A2EB.CRC);

    l_receivedLUT = GetLUT(x_device, &l_receivedCRC); 
    l_computedCRC = CRC_Class.crc(CRC_Class.convert16to8bit(l_receivedLUT));

    if (l_computedCRC != l_receivedCRC){
        std::cout << "WARNING: Received CRC does not match the one localy computed from the LUT." << std::endl;
        return false;
    }
    
    return true;      
}

/**
*  @brief Verification of the configuration to avoid transfer errors. Bit by bit comparison of LUT from the device with LUT provided by user.
*
*  @details   Reads out LUT from the board and compares it bit by bit to LUT (encoded 
 to vector of uint16_t) provided by user. Returns true for matching LUTs.
*
*  @return true
*/
bool TLBAccess::VerifyLUT_byBits(uint8_t x_device, std::vector<uint16_t> x_LUT)
{
    std::vector<uint16_t> l_receivedLUT;

    l_receivedLUT = GetLUT(x_device);
 
    if (x_LUT != l_receivedLUT){
        std::cout << "WARNING: LUT provided does not match LUT received from TLB." << std::endl;
        return false;
    }    
   
    return true;
}

//Following comand is not used for TLB
/*bool TLBAccess::ApplyConfig(uint8_t x_device)
{
    uint8_t l_CmdID = 0x06;
    uint8_t l_CmdArg = (0x1 << x_device);
    
    if (x_device > 7){
        std::cout << "WARNING: Device number should be between 0-7." << std::endl;
        return false;
    }

    if (!SendAndRetrieve(l_CmdID, l_CmdArg)){
        std::cout << "ApplyConfig() failed." << std::endl;
    }
    
    return true;
}*/