#ifndef _TLBDecode
#define _TLBDecode

#include <string>
#include <cstdint>
#include <vector>
#include <fstream>
#include <map>
#include <iostream>

namespace FASER {

  /** *******************************************************
   \brief API to decode the Trigger Logic Board (TLB).
   ******************************************************** */


  class TLBDecode
  {
  private:
  
  std::ofstream m_dataOutStreamMonitoring;
  std::ofstream m_dataOutStreamTrigger;
  
  const uint32_t TRIGGER_HEADER    = 0xFEAD000A;
  const uint32_t MONITORING_HEADER = 0xFEAD0050;

  public:
    ~TLBDecode();
    
    bool IsTriggerHeader(uint32_t data);
    bool IsMonitoringHeader(uint32_t data);
    unsigned int GetL1IDandBCID(std::vector<uint32_t> vector_of_raw_events, uint64_t &L1ID, uint16_t &BCID);
    void SetupStorageDecodedStream();
    void CloseStorageDecodedStream();
    void PrintDataVector(std::vector<uint32_t> Vector);
    void DecodeMonitoringDataVector(std::vector<uint32_t> MonitoringVector);
    void DecodeTriggerDataVector(std::vector<uint32_t> TriggerVector);
    void RawDataCheckIncompleteEvents();
  };
  
}
#endif // _TLBDecode
