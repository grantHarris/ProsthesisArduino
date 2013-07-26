#ifndef __PROSTHESIS_LED__
#define __PROSTHESIS_LED__

namespace Prosthesis
{
  class ProsthesisLEDBoard
  {
    public: 
      ProsthesisLEDBoard(uint8_t address);
      void Initialize(void);
      void SetColour(uint8_t channel, uint16_t r, uint16_t g, uint16_t b);
      void Reset(void);   
      void SetPWMFreq(float freq);
      
    private:
      uint8_t mAddress;
      
      void SetPWM(uint8_t num, uint16_t on, uint16_t off);         
      uint8_t Read8(uint8_t addr);
      void Write8(uint8_t addr, uint8_t d);      
  };
}

#endif //__PROSTHESIS_LED__
