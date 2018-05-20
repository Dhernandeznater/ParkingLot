#include "Arduino.h"
#include "ParkingSpace.h"
#include "Time.h"

ParkingSpace::ParkingSpace()
{
  isUsable=true;
  isReserved=false;
  isTaken=false;
}

void ParkingSpace::setReserved(bool reserved)
{
  isReserved=reserved;
}

void ParkingSpace::setTaken(bool taken)
{
    isTaken=taken;
}

void ParkingSpace::setUsability(bool usability)
{
    isUsable=usability;
}

bool ParkingSpace::getReserved()
{
    return isReserved;
}

bool ParkingSpace::getTaken()
{
    return isTaken;
}

bool ParkingSpace::getUsability()
{
    return isUsable;
}






