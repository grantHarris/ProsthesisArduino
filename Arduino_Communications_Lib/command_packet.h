#ifndef __COMMAND_PACKET_H_
#define __COMMAND_PACKET_H_

typedef struct {
  char ID[2];
  char *Data;
} PacketKV;

class CommandPacket
{
  public:
    CommandPacket();
    ~CommandPacket();
    
    void SetData(char *fromBuffer, int dataLength);
    
  private:
    
   
};

#endif //__COMMAND_PACKET_H_
