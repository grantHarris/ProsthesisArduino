#ifndef __ENGINEERING_INTERFACE_H__
#define __ENGINEERING_INTERFACE_H__

typedef void (*tRockerPressedCallback)(int);
typedef int (*tGetDisplayValueCallback)(void);

namespace ProsthesisEngineeringInterface
{
  void InitializeInterface(int leftDisplayAddress, int leftRockerUpPin, int leftRockerDownPin, int rightDisplayAddress, int rightRockerUpPin, int rightRockerDownPin);
  void SetRockerTickAmount(int amount);
  void SetDisplayUpdateRefreshPeriodMS(int timeMS);
  void Update();
  
  void SetLeftDisplayDataCallback(tGetDisplayValueCallback cb);
  void SetLeftDisplayRockerUpCallback(tRockerPressedCallback cb);
  void SetLeftDisplayRockerDownCallback(tRockerPressedCallback cb);  
  
  void SetRightDisplayDataCallback(tGetDisplayValueCallback cb);
  void SetRightDisplayRockerUpCallback(tRockerPressedCallback cb);  
  void SetRightDisplayRockerDownCallback(tRockerPressedCallback cb);
}

#endif //__ENGINEERING_INTERFACE_H__


