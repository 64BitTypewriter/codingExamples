#include <stdio>
#include <stdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <TTree.h>

double getNumber(const std::string &s);

//MAIN FUNCTION
void makeTree(){

  //Declare initial variables
  const int n_variables(11);
  std::string line;
  std::string date, clock;
  std::string dv[n_variables],dne[n_variables],dpe[n_variables];

  //Data structure for HV variables
  struct HVdata{
    double value, negError, posError;
    char severity[20], status[20];    
  };

  //Data structure for time variables
  struct HVtime{
    int year, month, day, hour, minute, second, millisecond, date, clock;
  };

  //Initiate variables 
  HVdata data[n_variables];
  HVtime time;

  //Create TFile, TTree and TBranches
  TFile *f = new TFile("OutTree.root","RECREATE");
  TTree *tree = new TTree("Tree","Tree for data from HV");
  tree->Branch("Time",&time.year,"year/I:month:day:hour:minute:second:millisecond:date:clock");
  tree->Branch("uB_TPCDrift_HV01_keithleyPickOff_getVoltage",&data[0].value,"value/D:negError:posError:severity[20]/C:status[20]/C");
  tree->Branch("uB_TPCDrift_HV01_keithleyCurrMon_calcCurrent",&data[1].value,"value/D:negError:posError:severity[20]/C:status[20]/C");
  tree->Branch("uB_OnDetPower_TPCPS_1_1_0_CURR_READ",&data[2].value,"value/D:negError:posError:severity[20]/C:status[20]/C");
  tree->Branch("uB_OnDetPower_TPCPS_1_1_2_CURR_READ",&data[3].value,"value/D:negError:posError:severity[20]/C:status[20]/C");
  tree->Branch("uB_OnDetPower_TPCPS_1_1_4_CURR_READ",&data[4].value,"value/D:negError:posError:severity[20]/C:status[20]/C");
  tree->Branch("uB_OnDetPower_TPCPS_1_1_6_CURR_READ",&data[5].value,"value/D:negError:posError:severity[20]/C:status[20]/C");
  tree->Branch("uB_PMTHV_TRPM_M2C0_CURR",&data[6].value,"value/D:negError:posError:severity[20]/C:status[20]/C");
  tree->Branch("uB_TPCDrift_HV01_keithleyPickOff_voltDiff5s60s",&data[7].value,"value/D:negError:posError:severity[20]/C:status[20]/C");
  tree->Branch("uB_TPCDrift_HV01_1_0_voltage",&data[8].value,"value/D:negError:posError:severity[20]/C:status[20]/C");
  tree->Branch("uB_Cryo_IFIX_1_0_LAR_PUMP_1_FORWARD",&data[9].value,"value/D:negError:posError:severity[20]/C:status[20]/C");
  tree->Branch("uB_TPCDrift_HV01_keithleyCurrMon_getVoltage",&data[10].value,"value/D:negError:posError:severity[20]/C:status[20]/C");


  //Open file and read out the values
  std::fstream Input;
  std::string filenameDATA = "HVdata.dat";
  //Opening file
  Input.open(filenameDATA.c_str(), std::ios::in);
  if(Input.is_open()){
    while(std::getline(Input,line,'\n')){
      //Read all values as strings provided line is not empty or commented
      if(line[0] != '#' && line.length()>0){
	stringstream(line) >> date >> clock
			   >> dv[0] >> dne[0] >> dpe[0] >> data[0].severity >> data[0].status
			   >> dv[1] >> dne[1] >> dpe[1] >> data[1].severity >> data[1].status
			   >> dv[2] >> dne[2] >> dpe[2] >> data[2].severity >> data[2].status
			   >> dv[3] >> dne[3] >> dpe[3] >> data[3].severity >> data[3].status
			   >> dv[4] >> dne[4] >> dpe[4] >> data[4].severity >> data[4].status
			   >> dv[5] >> dne[5] >> dpe[5] >> data[5].severity >> data[5].status
			   >> dv[6] >> dne[6] >> dpe[6] >> data[6].severity >> data[6].status
			   >> dv[7] >> dne[7] >> dpe[7] >> data[7].severity >> data[7].status
			   >> dv[8] >> dne[8] >> dpe[8] >> data[8].severity >> data[8].status
			   >> dv[9] >> dne[9] >> dpe[9] >> data[9].severity >> data[9].status
			   >> dv[10] >> dne[10] >> dpe[10] >> data[10].severity >> data[10].status;
	
	// "value", "negError" and "posError" need to be double, so convert them
	for(int i=0;i<n_variables;i++){
	  data[i].value = getNumber(dv[i]);
	  data[i].negError = getNumber(dne[i]);
	  data[i].posError = getNumber(dpe[i]);
	}

	//Break down "date" and "clock" strings into time struct values
	time.year = atoi(date.substr(0,4).c_str());
	time.month = atoi(date.substr(5,2).c_str());
	time.day = atoi(date.substr(8,2).c_str());
	time.hour = atoi(clock.substr(0,2).c_str());
	time.minute = atoi(clock.substr(3,2).c_str());
	time.second = atoi(clock.substr(6,2).c_str());
	time.millisecond = atoi(clock.substr(9,3).c_str());

	//Just to have a feeling that the script is running, comment it for faster execution
	std::cout << date << "   " << clock << std::endl;

	//Save "date" and "clock" as integers (and get rid of '/' and ':' separators)
	for(int i=0; i<=date.size(); i++){
	  if(date[i]=='/') {
	    date.erase(i,1);
	  }
	}
	for(int i=0; i<=clock.size(); i++){
	  if(clock[i]==':') {
	    clock.erase(i,1);
	  }
	}
	time.date = atoi(date.substr(0,8).c_str());
	time.clock = atoi(clock.substr(0,7).c_str());

	//Fill TTree
	tree->Fill();
      }
    }
  }
  Input.close();
  
  f->Write();
  f->Close();
}

//Take a string and convert it to a double,
//paying attention to thousand separators and #N/A values
double getNumber(const std::string &s){
  double number;
  if(s=="#N/A") number = -999999;
  else{
    for(int i=0; i<=s.size(); i++){
      if(s[i]==',') {
	s.erase(i,1);
      }
    }
    number = atof(s.c_str());
  }
  return number;
}
