#ifndef ParkingSpace_h
#define ParkingSpace_h


#include "Arduino.h"
#include "Time.h"

class ParkingSpace { //Parking Spot class
    public:
		ParkingSpace();
		void setReserved(bool reserved);
		void setTaken(bool taken);
		void setUsability(bool usability);
		bool getReserved();
		bool getTaken();
		bool getUsability();
      
    
	private:
		bool isTaken;
		bool isReserved;
		bool isUsable;
        time_t timeReserved;
    
};

#endif



