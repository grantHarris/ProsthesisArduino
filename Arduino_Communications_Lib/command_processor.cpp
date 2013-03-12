#include <Arduino.h>
#include "command_processor.h"
#include "command_packet.h"

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
  byte hasStart = 0;
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
  int commandEnd = mBufferStart;
  for (; commandEnd < mBufferEnd; ++commandEnd)
  {
    if (mBuffer[commandEnd] == '}')
    {
      break;
    } 
  }
  
  CommandPacket *packet = (CommandPacket*)malloc(sizeof(CommandPacket));
  packet->SetData((mBuffer + mBufferStart), commandEnd - mBufferStart);
  memcpy(mBuffer, (mBuffer + mBufferStart), mBufferSize - commandEnd);
  mBufferStart = 0;
  mBufferEnd = mBufferEnd - commandEnd;
  
  return packet; 
}
