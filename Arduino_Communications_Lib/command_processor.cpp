#include <Arduino.h>
#include "command_processor.h"
#include "command_packet.h"

//#define DEBUG_COMMAND_PROCESSING 1

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

CommandProcessor::CommandProcessor(int bufferSize)
:
mBufferSize(bufferSize),
mBufferStart(0),
mBufferEnd(0)
{
  mBuffer = (char*)malloc(bufferSize * sizeof(char));
}

CommandProcessor::~CommandProcessor()
{
  free(mBuffer);
}

bool CommandProcessor::AddChar(char data)
{
  if (mBufferEnd < mBufferSize - 1)
  {
    mBuffer[mBufferEnd++] = data;
    return true;
  }
  else
  {
    return false; 
  }
}

bool CommandProcessor::HasCommand() const
{
  bool hasStart = false;
  for (int i = mBufferStart; i < mBufferEnd; ++i)
  {
    if (!hasStart && mBuffer[i] == '{')
    {
      hasStart = true;
    }
    else if (hasStart && mBuffer[i] == '}')
    {
      return true; 
    }
  }
  return false; 
}

const char *CommandProcessor::GetBuffer(int &length) const
{
  length = mBufferEnd - mBufferStart;
  return (mBuffer + mBufferStart);
}

CommandPacket *CommandProcessor::GetCommand()
{
  int commandStart = -1;
  int commandEnd = -1;
  bool hasStart = false;
  for (int i = mBufferStart; i < mBufferEnd; ++i)
  {
    if (!hasStart && mBuffer[i] == '{')
    {
      commandStart = i;
      hasStart = true;
    }
    else if (hasStart && mBuffer[i] == '}')
    {
      commandEnd = i;
      break;
    }
  }
  
  CommandPacket *packet = NULL;
  
  if (commandStart != -1 && commandStart != commandEnd)
  {
    packet = (CommandPacket*)malloc(sizeof(CommandPacket));
    packet->SetData((mBuffer + commandStart), commandEnd - commandStart);
    memcpy(mBuffer, (mBuffer + commandStart), mBufferSize - commandEnd);
    mBufferStart = 0;
    mBufferEnd -= commandEnd + 1;
    
    #if DEBUG_COMMAND_PROCESSING
    Serial << "Buffer size is " << mBufferSize << "\n";
    Serial << "Command start was " << commandStart << " Command end was " << commandEnd << ". Remaining text is " << mBufferEnd - mBufferStart << "\n";
    Serial << "buffer start was " << mBufferStart << " buffer end was " << mBufferEnd << "\n";
    #endif
  }
  
  return packet; 
}
