using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Ports;

namespace Fingerprint
{
    public class FingerSensor
    {
        ushort HEADER = 0xEF01;
        public uint address = 0xFFFFFFFF;
        SerialPort serial = new SerialPort();

        //constructors for fingerSensors, also makes a SerialPort useful
        public FingerSensor(string port, int baud, uint addr)
        {
            address = addr;
            serial.BaudRate = baud;
            serial.PortName = port;
            serial.Open(); ///to open port
        }
        public FingerSensor(string port, int baud)
        {
            serial.BaudRate = baud;
            serial.PortName = port;

            serial.Open(); ///to open port
            serial.DiscardInBuffer();
            serial.DiscardOutBuffer();
        }
        public FingerSensor(string port)
        {
            serial.BaudRate = 57600; //default baud rate for scanner
            serial.PortName = port;
            serial.Open(); ///to open port
        }
        public FingerSensor()
        {
            serial.BaudRate = 1; //for testing
        }

        //handshake command
        public bool Handshake()
        {
            byte[] contents = {0x17,0x00};
            Write(0x01, contents);

            List<byte> packet = Read();
            //test code
            Console.WriteLine("Read: ");
            foreach(byte thing in packet)
            {
                Console.Write(thing.ToString("X")+",");
            }
            Console.Write("\n");
            if (packet[9] == (byte)0)//end test code
            //if (read()[9] == (byte)0)
                return true;
            else
                return false;
        }

        //Used to set the address of the scanner
        public bool SetAddr(uint newAddr)
        {
            byte[] contents = { 0x15, (byte)(newAddr >> 24), (byte)(newAddr >> 16), (byte)(newAddr >> 8), (byte)newAddr };
            Write(0x01, contents);
            List<byte> packet = Read();
            uint readAddr = (uint)packet[5] + ((uint)packet[4] << 8) + ((uint)packet[3] << 16) + ((uint)packet[2] << 24);
            Console.WriteLine("Input Address: {0}", newAddr.ToString("X"));
            Console.WriteLine("Read Address: {0}", readAddr.ToString("X"));

            if (newAddr == readAddr)
            {
                address = newAddr;
                return true;
            }
            else
                return false;
        }

        //writes the command to the fingerprint scanner, requires the package identifier and the instruction packets
        private void Write(byte pid, byte[] packets)
        {
            // turn total packet into a byte array, then send with Serial.Write(Byte[],Int32,Int32)
            uint length = (uint)packets.GetLength(0) + 2; //length is the packet length plus the checksum(2 bytes)
            uint totalLength = length + 9; //length+header(2)+address(4)+length of length(2)+pid(1)
            byte[] command = new byte[totalLength]; //make packet to send to scanner
            uint checksum = (uint)pid + length; //calculate the checksum
            for (int i = 0; i < packets.GetLength(0); i++)
            {
                checksum += (uint)packets[i]; //value depends on value & number of packets
            }

            //add bytes to the command packet
            command[0] = (byte)(HEADER >> 8);
            command[1] = (byte)HEADER;
            for (int i = 2; i < 6; i++)
            {
                command[i] = (byte)(address >> (i - 2) * 8);
            }
            command[6] = (byte)pid;
            command[7] = (byte)(length >> 8);
            command[8] = (byte)length;
            for (int i = 9; i < totalLength-2; i++)
            {
                command[i] = packets[i - 9];
            }
            command[totalLength - 2] = (byte)(checksum >> 8);
            command[totalLength - 1] = (byte)checksum;
            //write to serial
            if(serial.BaudRate > 1) //also test code
                serial.Write(command, 0, (int)totalLength);

            //test code
            for (int i = 0; i < totalLength; i++)
            {
                if (command[i] < 16) //nicer to read
                    Console.Write(0);
                Console.Write(command[i].ToString("X"));
            }
            Console.WriteLine(" ...end of write");
            //
        }//end write

        //returns list containing the packet contents
        private List<byte> Read()
        {
            byte[] headLength = new byte[9];
            List<byte> packet = new List<byte>(10);

            while (serial.BytesToRead < 9)
            {
            }

            //read up to Package Length
            serial.Read(headLength, 0, 9);
            if ((int)headLength[8] < 1)
            {
                packet[9] = 1;//non-positive package length
                return packet;
            }
            for (int i = 0; i < headLength.Length; i++)
            {
                packet.Add(headLength[i]);
            }
            byte[] contents = new byte[(int)headLength[8]];
            //Console.WriteLine("length: " + (int)headLength[8]);
            while (serial.BytesToRead < headLength[8])
            {
            }
            //read rest of package
            serial.Read(contents, 0, (int)headLength[8]);
            ushort checksum = (ushort)((ushort)headLength[6] + (ushort)((ushort)headLength[7] << 8) + (ushort)headLength[8]);
            for (int i = 0; i < (int)headLength[8]-2; i++)
            {
                checksum += (ushort)contents[i];
            }

            //test code//////////////////////////////////
            for (int i = 0; i < 9; i++)
            {
                if (headLength[i] < 16) //nicer to read
                    Console.Write(0);
                Console.Write(headLength[i].ToString("X"));
            }
            for (int i = 0; i < headLength[8]; i++)
            {
                if (contents[i] < 16) //nicer to read
                    Console.Write(0);
                Console.Write(contents[i].ToString("X"));
            }
            Console.WriteLine(" ...end of read");
            //end test///////////////////////////////////

            if (checksum != (ushort)((ushort)((ushort)contents[headLength[8] - 2] << 8) + (ushort)contents[headLength[8] - 1]))
            {
                Console.WriteLine("Either the checksums don't agree or your math is wrong");
                packet[9] = 1;
                return packet;
            }
            for (int i = 0; i < contents.Length; i++)
            {
                packet.Add(contents[i]);
            }
            return packet;
        }//end read
    }
}
