#include "../TLBAccess/TLBDecode.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <bitset>
#include <string>

using namespace FASER;

 
// constructor
TLBDecode::~TLBDecode()
{
}
 
bool TLBDecode::IsTriggerHeader(uint32_t data)
{
  bool isTriggerHeader = false;
  if (data == TRIGGER_HEADER) {
    isTriggerHeader = true;
    //std::cout << " Found trigger header with EVENT CNT = "<<data<<std::endl;
  }
  return isTriggerHeader;
}

bool TLBDecode::IsMonitoringHeader(uint32_t data)
{
  bool isMonitoringHeader = false;
  if (data == MONITORING_HEADER) {
    isMonitoringHeader = true;
    //std::cout << " Found monitoring header with EVENT CNT = "<<data<<std::endl;
  }
  return isMonitoringHeader;
}

unsigned int TLBDecode::GetL1IDandBCID(std::vector<uint32_t> event, uint64_t &L1ID, uint16_t &BCID){
  unsigned int status=0;
  if (event.size()>3){ //checks if trigger or monitoring event is larger then 3 to extract data
    L1ID=event[1];
    BCID=event[3];
  }
  if ((IsTriggerHeader(event[0]) && event.size()!=5)|| (IsMonitoringHeader(event[0]) && event.size()!=26)){
    status=1<<5; //EventStatus from EventFormat.hpp for CorruptedFragment
  }
  return(status);
}

void TLBDecode::SetupStorageDecodedStream()
{
  if (m_dataOutStreamTrigger.is_open()){
  m_dataOutStreamTrigger.close();
  }
  if (m_dataOutStreamMonitoring.is_open()){
  m_dataOutStreamMonitoring.close();
  }
  
  m_dataOutStreamTrigger.open ("TriggerData.JSON", std::ios::trunc);
  m_dataOutStreamMonitoring.open ("MonitoringData.JSON", std::ios::trunc);
  
    if (!m_dataOutStreamTrigger.is_open()){
    std::cout << "ERROR: could not open file "<<"TriggerData.JSON"<<" no data will be written to disk!"<<std::endl;
  }
    if (!m_dataOutStreamMonitoring.is_open()){
    std::cout << "ERROR: could not open file "<<"MonitoringData.JSON"<<" no data will be written to disk!"<<std::endl;
  }
} 

void TLBDecode::CloseStorageDecodedStream()
{
  m_dataOutStreamTrigger.close();
  m_dataOutStreamMonitoring.close(); 
}

//print out the MonitoringDataVector
void TLBDecode::PrintDataVector(std::vector<uint32_t> Vector)
{
  for (int i=0; i<Vector.size(); i++){
    std::cout<<"Error: Vector["<<i<<"]: "<<std::bitset<32>(Vector[i])<<std::endl;
  }
}

// Decode the Monitoring Data Vector
void TLBDecode::DecodeMonitoringDataVector(std::vector<uint32_t> MonitoringVector)
{  
  m_dataOutStreamMonitoring<<"{\"EventCounter\": "<<MonitoringVector[0]<<",";
  m_dataOutStreamMonitoring<<"\"OrbitCounter\": "<<MonitoringVector[1]<<",";
  m_dataOutStreamMonitoring<<"\"BunchCounter\": "<<MonitoringVector[2]<<",";

  for (int i=0; i<6; i++){
    m_dataOutStreamMonitoring<<"\"TBP"<<i<<"\": "<<MonitoringVector[3+i]<<",";
  }
  for (int i=0; i<6; i++){
    m_dataOutStreamMonitoring<<"\"TAP"<<i<<"\": "<<MonitoringVector[9+i]<<",";
  }  
  for (int i=0; i<6; i++){
    m_dataOutStreamMonitoring<<"\"TAV"<<i<<"\": "<<MonitoringVector[16+i]<<",";
  }    
  
  m_dataOutStreamMonitoring<<"\"DeadtimeVetoCounter\": "<<MonitoringVector[21]<<",";
  m_dataOutStreamMonitoring<<"\"BusyVetoCounter\": "<<MonitoringVector[22]<<",";
  m_dataOutStreamMonitoring<<"\"RateLimiterVetoCounter\": "<<MonitoringVector[23]<<",";
  m_dataOutStreamMonitoring<<"\"BCRVetoCounter\": "<<MonitoringVector[24]<<"}\n";
}


void TLBDecode::DecodeTriggerDataVector(std::vector<uint32_t> TriggerVector)
{
  m_dataOutStreamTrigger<<"{\"EventCounter\": "<<TriggerVector[0]<<",";
  m_dataOutStreamTrigger<<"\"OrbitCounter\": "<<TriggerVector[1]<<",";
  m_dataOutStreamTrigger<<"\"BunchCounter\": "<<TriggerVector[2]<<",";
  m_dataOutStreamTrigger<<"\"TAP0\": "<<((TriggerVector[3]&0x1000000)>>24)<<",";
  m_dataOutStreamTrigger<<"\"TAP1\": "<<((TriggerVector[3]&0x2000000)>>25)<<",";
  m_dataOutStreamTrigger<<"\"TAP2\": "<<((TriggerVector[3]&0x4000000)>>26)<<",";
  m_dataOutStreamTrigger<<"\"TAP3\": "<<((TriggerVector[3]&0x8000000)>>27)<<",";
  m_dataOutStreamTrigger<<"\"TAP4\": "<<((TriggerVector[3]&0x10000000)>>28)<<",";
  m_dataOutStreamTrigger<<"\"TAP5\": "<<((TriggerVector[3]&0x20000000)>>29)<<",";  
  m_dataOutStreamTrigger<<"\"TBP0\": "<<((TriggerVector[3]&0x10000)>>16)<<",";
  m_dataOutStreamTrigger<<"\"TBP1\": "<<((TriggerVector[3]&0x20000)>>17)<<",";
  m_dataOutStreamTrigger<<"\"TBP2\": "<<((TriggerVector[3]&0x40000)>>18)<<",";
  m_dataOutStreamTrigger<<"\"TBP3\": "<<((TriggerVector[3]&0x80000)>>19)<<",";
  m_dataOutStreamTrigger<<"\"TBP4\": "<<((TriggerVector[3]&0x100000)>>20)<<",";
  m_dataOutStreamTrigger<<"\"TBP5\": "<<((TriggerVector[3]&0x200000)>>21)<<",";  
  m_dataOutStreamTrigger<<"\"InputBits\": "<<((TriggerVector[3]&0xFF00)>>7)<<",";
  m_dataOutStreamTrigger<<"\"InputBitsNextCLK\": "<<(TriggerVector[3]&0xFF)<<"}\n";
}

void TLBDecode::RawDataCheckIncompleteEvents()
{
  int TriggerCounter=0;
  int MonitoringCounter=0;
  bool WrongTriggerSize=false;
  bool WrongMonitoringSize=false;
  std::fstream newfile;
   newfile.open("DataReadout.txt",std::ios::in); //open a file to perform read operation using file object
   if (newfile.is_open()){   //checking whether the file is open
      std::string tp;
      while(getline(newfile, tp)){ //read data from file object and put it into string.
 
         if ((std::bitset<32>(tp)==0xFEAD000A || std::bitset<32>(tp)==0xFEAD0050) && TriggerCounter>=1){
          if (TriggerCounter!=6){
            std::cout<<"Wrong Trigger Size, the header was: "<<std::hex<<std::bitset<32>(tp)<<" and I was filling a trigger event"<<std::endl;
          }
          TriggerCounter=0;
         }
         if ((std::bitset<32>(tp)==0xFEAD000A || std::bitset<32>(tp)==0xFEAD0050) && MonitoringCounter>=1){
          if (MonitoringCounter!=27){
            std::cout<<"Wrong Monitoring Size, the header was: "<<std::hex<<std::bitset<32>(tp)<<" and I was filling a monitoring event"<<std::endl;
          }
          MonitoringCounter=0;
         }
         
        if (std::bitset<32>(tp)==0xFEAD000A){
            TriggerCounter=1;

        }
        if (std::bitset<32>(tp)==0xFEAD0050){
          MonitoringCounter=1;
        } 
         
          if (TriggerCounter!=0){TriggerCounter=TriggerCounter+1;}
          if (MonitoringCounter!=0){MonitoringCounter=MonitoringCounter+1;}
          if (TriggerCounter==6){TriggerCounter=0;}
          if (MonitoringCounter==27){MonitoringCounter=0;}
      }
      newfile.close(); //close the file object.
   }
}