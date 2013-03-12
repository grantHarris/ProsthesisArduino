#ifndef __COMMAND_PROCESSOR_H_
#define __COMMAND_PROCESSOR_H_

class CommandPacket;

class CommandProcessor
{
  public:
    CommandProcessor(int bufferSize);
    ~CommandProcessor();
    
    bool HasCommand() const;
    bool AddChar(char data);
    
    const char *GetBuffer(int &length) const;
    CommandPacket *GetCommand();

  private:
    int mBufferSize;
    int mBufferStart;
    int mBufferEnd;
    char* mBuffer;
};

#endif //__COMMAND_PROCESSOR_H_
