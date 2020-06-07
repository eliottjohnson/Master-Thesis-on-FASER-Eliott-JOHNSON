#ifndef __TLBAccess
#define __TLBAccess

#include "GPIOBase/GPIOBaseClass.h"
#include "GPIOBase/CommunicationInterface.h"
#include "TLB_ConfigRegisters.h"
#include <fstream>
#include <thread>
#include <mutex>
#include "GPIOBase/CRC.h"
#include <bitset>
#include <unistd.h>
#include "GPIOBase/nlohmann/json.hpp"
#include "TLBDecode.h"
#include <iostream>

using json = nlohmann::json;

namespace FASER {
  

  /** *******************************************************
   \brief API to communicate with the Trigger Logic Board (TLB).
   ******************************************************** */


  class TLBAccess : public GPIOBaseClass
  {
 
  const uint32_t SUBCMDID_LSHIFT = 12;

  public:
    // CONSTRUCTOR / DESTRUCTOR
    TLBAccess(bool emulateInterface = false) : GPIOBaseClass(emulateInterface){}
    ~TLBAccess();
    
    void CleanConfig();
    void SendConfig();
    void SendDirectParameters();
    void DisableTrigger();
    void EnableTrigger(bool ECR, bool Reset);
    void SendECR();
    void SendDataReadoutParameters();
    void ConfigureTLB(json myjson);
    bool ConfigureAndVerifyTLB(json myjson);
    void ConfigureLUT(std::string PathToLUT1);
    bool VerifyConfiguration();
    void StartReadout(uint16_t param);
    void StopReadout();
    void PollData();
    std::vector< std::vector<uint32_t> > GetTLBEventData();
  
    ConfigReg m_tlb_config = ConfigReg();
  
    bool LoadLUT(std::string x_PathToLUT, std::vector<uint16_t> *x_LUT); 
    bool SetLUT(uint8_t x_device, std::string x_PathToLUT);
    std::vector<uint16_t> GetLUT(uint16_t x_device, uint16_t *CRC = nullptr);
    std::vector<uint16_t> DecodeLUT(std::vector<uint16_t> x_LUT);
    bool VerifyLUT_byCRC(uint8_t x_device);
    bool VerifyLUT_byBits(uint8_t x_device, std::vector<uint16_t> x_LUT);
    //bool ApplyConfig(uint8_t x_device);
  
  private:
    std::thread *m_daqThread;
    bool m_daqRunning;
    
    std::mutex mMutex_TLBEventData; //mutex object vector of vectors
    std::vector <uint32_t> m_DataWordOutputBuffer; //vector of words
    std::vector<std::vector<uint32_t>> m_TLBEventData; // complete TRB raw data for every event
    
    bool IsBinary(std::string l_value);
    };
  }


/** *******************************************************
 * \brief Definitions of CommandRequest IDs.
 ******************************************************** */
class TLBCmdID:public GPIOCmdID {
public:
  static const uint8_t USER_SET_CONFIG     = 0x08;
  static const uint8_t USER_GET     = 0x10;
};

/** *******************************************************
 \brief Definitions of ERROR codes retrieved from TLB.
 ******************************************************** */

class TLBErrors:public GPIOErrors {
};

/** *******************************************************
 \brief Definitions of WORD ID (bit 31-28 of command request header)
 ******************************************************** */
class TLBWordID:public GPIOWordID{

};

#endif /* __TLBAccess */
