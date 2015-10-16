#include <Arduino.h>
#include <PID_v1.h>
#include <pot_box.h>
#include <aJSON.h>
#include <Wire.h>
#include <command_processor.h>

#define AUTONOMOUS_OPERATION 1

#include "motor_state.h"
#include "DeviceState.h"
#include "eng_iface.h"
#include "motors.h"
#include "pins.h"

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

void TransitionToState(DeviceStates toState);
void DeviceDisabledSlice();
void DeviceActiveSlice();
void DeviceFaultSlice();

aJsonStream mJSONSerialStream(&Serial);
const char* kArduinoID = "mcon";

unsigned long mLastTelem;
unsigned long mLastHeartbeat;
unsigned long mLastMillis;
unsigned long mActiveMillis;
int mTelemetryPeriodMS;
int mHeartbeatPeriodMS;
bool mTelemetryEnabled = false;
bool mHeartbeatEnabled = false;

MotorState::tMotorControllerState mDeviceState;

void setup()
{ 
  CommandProcessor::SetTypeIDRequestCallback(IDRequestCallback);
  CommandProcessor::SetTelemetryRequestCallback(TelemetryEnableCb);
  CommandProcessor::SetEnableToggleRequestCallback(InitializeDeviceCb);
  CommandProcessor::SetHeartbeatRequestCallback(HeartbeatEnableCb);
  
  Serial.begin(57600);
  
  //Initialize our device state
  mDeviceState.State = Uninitialized;
  for (int i = 0; i < NUM_MOTOR_CONTROLLERS; ++i)
  {
     mDeviceState.Currents[i] = 0.0f;
     mDeviceState.Millivolts[i] = 0;     
     mDeviceState.OutputPressure[i] = 0.0f;
     mDeviceState.LoadPressure[i] = 0.0f;
     mDeviceState.MotorDutyCycle[i] = 0.0f;     
  }
  mDeviceState.IsLoadSense = false;  
  
  ProsthesisMotors::Initialize();
  ProsthesisMotors::SetKneeMotorPinout(MOTOR_KNEE_THROTTLE_PIN, MOTOR_KNEE_PRESSURE_IN, MOTOR_KNEE_LOAD_IN);
  ProsthesisMotors::SetHipMotorPinout(MOTOR_HIP_THROTTLE_PIN, MOTOR_KNEE_PRESSURE_IN, MOTOR_HIP_LOAD_IN);
  
  ProsthesisMotors::ToggleKneeMotorControl(false);
  ProsthesisMotors::ToggleHipMotorControl(false);
 
  ProsthesisEngineeringInterface::InitializeInterface(MIN_PRESSURE_ADDRESS, MIN_UP_PIN, MIN_DOWN_PIN, MAX_PRESSURE_ADDRESS, MAX_UP_PIN, MIN_DOWN_PIN);
  ProsthesisEngineeringInterface::SetRockerTickAmount(50);
  
  ProsthesisEngineeringInterface::SetLeftDisplayDataCallback(GetHipPressureSetPoint);
  ProsthesisEngineeringInterface::SetLeftDisplayRockerUpCallback(HipRockerDown);
  ProsthesisEngineeringInterface::SetLeftDisplayRockerDownCallback(HipRockerDown);
  
  ProsthesisEngineeringInterface::SetRightDisplayDataCallback(GetKneePressureSetPoint);
  ProsthesisEngineeringInterface::SetRightDisplayRockerUpCallback(KneeRockerDown);
  ProsthesisEngineeringInterface::SetRightDisplayRockerDownCallback(KneeRockerDown);
  
#if AUTONOMOUS_OPERATION
  TransitionToState(Active);
#endif
}

void loop()
{  
   //Send updates to engineering interface as often as possible.
   ProsthesisEngineeringInterface::Update();
   
   if (mJSONSerialStream.available()) 
   {
     // First, skip any accidental whitespace like newlines.
     mJSONSerialStream.skip();
   }

  if (mJSONSerialStream.available()) 
  {
     // Something real on input, let's take a look. 
     aJsonObject *msg = aJson.parse(&mJSONSerialStream);
     
#if DEBUG_CBS     
      char *outMsg = aJson.print(msg);
      if (outMsg != NULL)
      {
        Serial << outMsg << "\n"; 
        free(outMsg);
      }     
#endif
     
     CommandProcessor::ProcessMessage(msg);
     aJson.deleteItem(msg);    
  }

  //Soft realtime telemetry. Who cares about missed deadlines for these? The mission critical stuff goes into the interrupt CBs
  if (mTelemetryEnabled && millis() - mLastTelem > mTelemetryPeriodMS)
  {
#if DEBUG_CBS        
    const char *deviceState = mDeviceState.State == Active ? "Enabled" : "Disabled";
    Serial << "Telem. Diff is: " << millis() - mLastTelem << "ms. Device is " << deviceState << "\n";    
#endif    

    //Update our state with the latest info
    const ProsthesisMotors::tMotorConfig *hipConfig = ProsthesisMotors::GetHipMotorConfig();
    const ProsthesisMotors::tMotorConfig *kneeConfig = ProsthesisMotors::GetKneeMotorConfig();
    
    mDeviceState.MotorDutyCycle[HIP_MOTOR_INDEX] = hipConfig->mThrottle / 255.0f;
    mDeviceState.OutputPressure[HIP_MOTOR_INDEX] = hipConfig->mSampleAvg;
    mDeviceState.PressureSetPoints[HIP_MOTOR_INDEX] = hipConfig->mPressureSetpoint;
    
#if OUTPUT_PID_TUNINGS
    mDeviceState.PTuning[HIP_MOTOR_INDEX] = hipConfig->mP;
    mDeviceState.ITuning[HIP_MOTOR_INDEX] = hipConfig->mI;
    mDeviceState.DTuning[HIP_MOTOR_INDEX] = hipConfig->mD;    
#endif
    
    mDeviceState.PressureSetPoints[HIP_MOTOR_INDEX] = hipConfig->mPressureSetpoint;
    
    mDeviceState.MotorDutyCycle[KNEE_MOTOR_INDEX] = kneeConfig->mThrottle / 255.0f;
    mDeviceState.OutputPressure[KNEE_MOTOR_INDEX] = kneeConfig->mSampleAvg;
    mDeviceState.PressureSetPoints[KNEE_MOTOR_INDEX] = kneeConfig->mPressureSetpoint;
    
#if OUTPUT_PID_TUNINGS
    mDeviceState.PTuning[KNEE_MOTOR_INDEX] = kneeConfig->mP;
    mDeviceState.ITuning[KNEE_MOTOR_INDEX] = kneeConfig->mI;
    mDeviceState.DTuning[KNEE_MOTOR_INDEX] = kneeConfig->mD;    
#endif    
    
    aJsonObject *msg = MotorState::MotorStateToJSON(mDeviceState);
    if (msg != NULL)
    {
      CommandProcessor::SendMessage(msg);
      aJson.deleteItem(msg);
    }
    mLastTelem = millis(); 
  }
  
  //Soft realtime heartbeat. It is up to the client to decide how many deadlines have passed before assuming the Arduino has gone rogue
  if (mHeartbeatEnabled && millis() - mLastHeartbeat > mHeartbeatPeriodMS)
  {
#if DEBUG_CBS    
    Serial << "Heartbeat. Diff is: " << millis() - mLastHeartbeat << "ms\n";    
#endif  
    
    aJsonObject *msg = aJson.createObject();
    if (msg != NULL)
    {
      aJson.addItemToObject(msg, CommandProcessor::PacketKeys::kCommandID, aJson.createItem(CommandProcessor::CommandIDs::kHeartBeat));
      CommandProcessor::SendMessage(msg);
      aJson.deleteItem(msg);
    }
    mLastHeartbeat = millis(); 
  }
  
  switch (mDeviceState.State)
  {
   case Disabled:
    DeviceDisabledSlice();
    break;
   
   case Active:
    DeviceActiveSlice();
    break;
   
   case Fault:
    DeviceFaultSlice();
    break; 
    
   default:
    break;
  }
}

aJsonObject * IDRequestCallback(aJsonObject *msg)
{
  aJsonObject *ackMsg = CommandProcessor::CreateCommandAckMessage();
  aJson.addItemToObject(ackMsg, CommandProcessor::PacketKeys::kArduinoID, aJson.createItem(kArduinoID));
  
  aJson.addItemToObject(ackMsg, CommandProcessor::PacketKeys::kTelemetryState, aJson.createItem(mTelemetryEnabled));
  aJson.addItemToObject(ackMsg, CommandProcessor::PacketKeys::kDeviceState, aJson.createItem(mDeviceState.State));
  return ackMsg;
}

aJsonObject * TelemetryEnableCb(aJsonObject *msg, bool enable)
{
  //No ACK necessary for this
  mTelemetryEnabled = enable;
  aJsonObject *telemPeriod = aJson.getObjectItem(msg, CommandProcessor::PacketKeys::kPeriod);
  if (telemPeriod != NULL)
  {    
     mTelemetryPeriodMS = telemPeriod->valueint;
#if DEBUG_CBS
     const char *enableText = mTelemetryEnabled ? "on" : "off";
     const char *deviceState = mDeviceState.State == Active ? "Enabled" : "Disabled";
     Serial << "Setting telemetry period to " << mTelemetryPeriodMS << "ms. Telemetry is " << enableText << ". Device is " << deviceState << "\n";
#endif
  }
  return NULL;
}

aJsonObject * HeartbeatEnableCb(aJsonObject *msg, bool enable)
{
  //No ACK necessary for this
  mHeartbeatEnabled = enable;
  aJsonObject *heartbeatPeriod = aJson.getObjectItem(msg, CommandProcessor::PacketKeys::kPeriod);
  if (heartbeatPeriod != NULL)
  {
     mHeartbeatPeriodMS = heartbeatPeriod->valueint;
#if DEBUG_CBS
     const char *enableText = mHeartbeatEnabled ? "on" : "off";
     const char *deviceState = mDeviceState.State == Active ? "Enabled" : "Disabled";
     Serial << "Setting heartbeat period to " << mTelemetryPeriodMS << "ms. Heartbeat is " << enableText << ". Device is " << deviceState << "\n";
#endif
  }
  return NULL;
}

aJsonObject * InitializeDeviceCb(aJsonObject *msg, bool enable)
{
  aJsonObject *enableValue = aJson.getObjectItem(msg, CommandProcessor::PacketKeys::kEnable);
  
  if (enableValue != NULL && mDeviceState.State != Fault)
  {
    if (enableValue->valuebool)
    {
      TransitionToState(Active);
    }
    else
    {
      TransitionToState(Disabled);
    } 
  }
  
#if DEBUG_CBS
  Serial << "Device initialise " << enable << "\n";
#endif

  return NULL;
}

void TransitionToState(DeviceStates toState)
{
  //TODO: Perform state transitions here
  DeviceStates fromState = mDeviceState.State;
  aJsonObject *msg = NULL;
  bool successful = false;
  switch (fromState)
  {
  case Uninitialized:
    if (toState != Uninitialized)
    {
      successful = true; 
    }
    break;
    
  case Disabled:
    if (toState != Uninitialized)
    {
      successful = true; 
    }    
    break;
  
  case Active:
    if (toState != Uninitialized)
    {
      successful = true; 
    }  
    break;
    
  //No recovery from faults
  case Fault:
    break;
  } 
  
  if (successful && toState == Active)
  {
    digitalWrite(DEVICE_ACTIVE_PIN, HIGH);
    ProsthesisMotors::ToggleKneeMotorControl(true);
    ProsthesisMotors::ToggleHipMotorControl(true);
  }
  else
  {
    digitalWrite(DEVICE_ACTIVE_PIN, LOW);  
    ProsthesisMotors::ToggleKneeMotorControl(false);
    ProsthesisMotors::ToggleHipMotorControl(false);
  }
  
  if (successful)
  {
    msg = aJson.createObject();
    aJson.addItemToObject(msg, CommandProcessor::PacketKeys::kCommandID, aJson.createItem(CommandProcessor::CommandIDs::kStateChange));
    aJson.addItemToObject(msg, CommandProcessor::PacketKeys::kChangeFrom , aJson.createItem(mDeviceState.State));
    aJson.addItemToObject(msg, CommandProcessor::PacketKeys::kChangeTo , aJson.createItem(toState));
    mDeviceState.State = toState;    
  }
  
  if (msg != NULL)
  {
    CommandProcessor::SendMessage(msg);
    aJson.deleteItem(msg);
  }
}

void DeviceDisabledSlice()
{
  mLastMillis = millis();
}

void DeviceActiveSlice()
{
  unsigned long currMillis = millis();
  mActiveMillis += currMillis - mLastMillis;
  
  ProsthesisMotors::UpdateMotors();
  
  mLastMillis = currMillis;
}

void DeviceFaultSlice()
{
  mLastMillis = millis();  
}

int GetHipPressureSetPoint()
{
  return ProsthesisMotors::GetHipMotorConfig()->mPressureSetpoint;
}

int GetKneePressureSetPoint()
{
  return ProsthesisMotors::GetKneeMotorConfig()->mPressureSetpoint;
}

void HipRockerDown(int amount)
{
  ProsthesisMotors::ChangeHipMotorSetPoint(amount);
}

void KneeRockerDown(int amount)
{
  ProsthesisMotors::ChangeKneeMotorSetPoint(amount);
}

