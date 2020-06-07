#include "../../TLBAccess/TLBAccess.h"
#include "../../TLBAccess/TLBDecode.h"

#define _ms 1000 // used for usleep

using namespace FASER;
using json = nlohmann::json;

int main() 
{
  TLBAccess *tlb = new TLBAccess();
  TLBDecode *decode = new TLBDecode();
  std::cout<<"███████╗██╗     ██╗ ██████╗ ████████╗████████╗ ██╗ ███████╗    ███████╗ █████╗ ███████╗███████╗██████╗     ██████╗  █████╗  ██████╗ "<<std::endl;
  std::cout<<"██╔════╝██║     ██║██╔═══██╗╚══██╔══╝╚══██╔══╝ █╔╝ ██╔════╝    ██╔════╝██╔══██╗██╔════╝██╔════╝██╔══██╗    ██╔══██╗██╔══██╗██╔═══██╗"<<std::endl;
  std::cout<<"█████╗  ██║     ██║██║   ██║   ██║      ██║    ╚╝  ███████╗    █████╗  ███████║███████╗█████╗  ██████╔╝    ██║  ██║███████║██║   ██║"<<std::endl;
  std::cout<<"██╔══╝  ██║     ██║██║   ██║   ██║      ██║        ╚════██║    ██╔══╝  ██╔══██║╚════██║██╔══╝  ██╔══██╗    ██║  ██║██╔══██║██║▄▄ ██║"<<std::endl;
  std::cout<<"███████╗███████╗██║╚██████╔╝   ██║      ██║        ███████║    ██║     ██║  ██║███████║███████╗██║  ██║    ██████╔╝██║  ██║╚██████╔╝"<<std::endl;
  std::cout<<"╚══════╝╚══════╝╚═╝ ╚═════╝    ╚═╝      ╚═╝        ╚══════╝    ╚═╝     ╚═╝  ╚═╝╚══════╝╚══════╝╚═╝  ╚═╝    ╚═════╝ ╚═╝  ╚═╝ ╚══▀▀═╝ \n"<<std::endl;
  //usleep(1000*_ms);

  tlb->SetDebug(0); //Set to 0 for no debug, to 1 for debug. Changes the m_DEBUG variable
  
  //JSON readout
  std::ifstream f("/home/eljohnso/gpiodrivers/TLBAccess/config/TLBConfiguration.JSON");
  json myjson = json::parse(f);
  
  std::cout<<"Configuring TLB"<<std::endl;
  if (tlb->ConfigureAndVerifyTLB(myjson)){std::cout<<"   TLB Configuration OK"<<std::endl;}
  else{std::cout<<"   Error: TLB Configuration failed"<<std::endl;}
  std::cout << "   Done.\n"<<std::endl;

  std::cout<<"Configuring LUT"<<std::endl;
  tlb->ConfigureLUT("/home/eljohnso/gpiodrivers/TLBAccess/config/LUT1.txt"); //Path has to be absolut
  std::cout << "   Done.\n"<<std::endl;

  
  /*
  //Testing the SoftwareTrigger
  for (int i=0; i<10;i++){
    tlb->m_tlb_config.SetSoftwareTrigger(1);
    tlb->SendDirectParameters();
    tlb->m_tlb_config.PrintDirectParam();
    usleep(100*_ms);
    tlb->m_tlb_config.SetSoftwareTrigger(0);
    tlb->SendDirectParameters();
    tlb->m_tlb_config.PrintDirectParam();
    usleep(100*_ms);
  } 
  */


  //THESE I THINK DONT DO ANYTHING
  //tlb->m_tlb_config.SetEnableTriggerData(myjson["EnableTriggerData"].get<bool>());
  //tlb->m_tlb_config.SetEnableMonitoringData(myjson["EnableMonitoringData"].get<bool>());
  //tlb->m_tlb_config.SetReadoutFIFOReset(myjson["ReadoutFIFOReset"].get<bool>());
  //tlb->SendDirectParameters();
  //tlb->m_tlb_config.PrintDataReadoutParam();
  
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////// Read out Data ////////////////////////////////////
  int WhatToRead=0x0;
  WhatToRead=(WhatToRead|(myjson["EnableTriggerData"].get<bool>()<<13));
  WhatToRead=(WhatToRead|(myjson["EnableMonitoringData"].get<bool>()<<14));
  WhatToRead=(WhatToRead|(myjson["ReadoutFIFOReset"].get<bool>()<<15));
  tlb->StartReadout(WhatToRead);
  auto startDAQ = std::chrono::high_resolution_clock::now();
  auto stopDAQ = startDAQ;
  int DAQhours=0;
  int DAQminutes=0;
  int DAQseconds=20; //Time you want to measure data in seconds.
  
  uint64_t L1ID=0;
  uint16_t BCID=0;
  unsigned int status=0;
  
  while(true){
    stopDAQ = std::chrono::high_resolution_clock::now();
    auto durationDAQ = std::chrono::duration_cast<std::chrono::microseconds>(stopDAQ - startDAQ).count();

    std::vector<std::vector<uint32_t>> vector_of_raw_events;
    vector_of_raw_events = tlb->GetTLBEventData();
    
    
    if (vector_of_raw_events.size()!=0){
      for(int i=0; i<vector_of_raw_events.size(); i++){ //event is a single vector containing one event
        status=decode->GetL1IDandBCID(vector_of_raw_events[i], L1ID, BCID);
        std::cout<<std::dec<<"L1ID: "<<L1ID<<" BCID: "<<BCID<<" Status: "<<status<<std::endl;
      }
      std::cout<<std::endl; //backspace at the end of each vector_of_raw_events
    }
    
    if (durationDAQ == (DAQhours*60*60+DAQminutes*60+DAQseconds*1000000)/3){
        std::cout<<"disabling trigger\n";
        tlb->DisableTrigger();
    }
    if (durationDAQ == (DAQhours*60*60+DAQminutes*60+DAQseconds*1000000)/2){
      std::cout<<"enabling trigger\n";
      tlb->EnableTrigger(true,true);
    }
    
    if (durationDAQ > (DAQhours*60*60+DAQminutes*60+DAQseconds*1000000)){
        std::cout<<"   End of DAQ timer, stopping readout."<<std::endl;
        tlb->StopReadout();
        usleep(100);
        vector_of_raw_events = tlb->GetTLBEventData();
        
        if (vector_of_raw_events.size()!=0){
          for(int i=0; i<vector_of_raw_events.size(); i++){ //event is a single vector containing one event
            status=decode->GetL1IDandBCID(vector_of_raw_events[i], L1ID, BCID);
            std::cout<<"L1ID: "<<L1ID<<" BCID: "<<BCID<<std::endl;
          }
        }
        break;
    }
  }

  
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
  //std::cout<<"Performing RawDataCheckIncompleteEvents()"<<std::endl;
  //decode->RawDataCheckIncompleteEvents();
 
  
 
 return 0;
}

