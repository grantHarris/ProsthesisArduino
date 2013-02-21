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
        uint address = 0xFFFFFFFF;
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
        public bool handshake()
        {
            byte[] contents = {0x17,0x00};
            write((uint)0x01, contents);

            /////////////////wait
            int j = 0;
            for (int i = 0; i < 500; i++)
            {
                j++;
            }
            /////////////

            return read();
        }

        //writes the command to the fingerprint scanner, requires the package identifier and the instruction packets
        void write(uint pid, byte[] packets)
        {
            // turn total packet into a byte array, then send with Serial.Write(Byte[],Int32,Int32)
            uint length = (uint)packets.GetLength(0) + 2; //length is the packet length plus the checksum(2 bytes)
            uint totalLength = length + 9; //length+header(2)+address(4)+length of length(2)+pid(1)
            byte[] command = new byte[totalLength]; //make packet to send to scanner
            uint checksum = pid + length; //calculate the checksum
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

        }//end write


        bool read()
        {
            byte[] headLength = new byte[9];

            while (serial.BytesToRead < 9)
            {
            }

            serial.Read(headLength, 0, 9);
            if ((int)headLength[8] < 1)
                return false;//non-positive package length
            byte[] contents = new byte[(int)headLength[8]];
            //Console.WriteLine("length: " + (int)headLength[8]);
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
                return false;
            }
            return true;
        }//end read
    }
}
