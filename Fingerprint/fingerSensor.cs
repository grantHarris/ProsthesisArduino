using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Ports;

namespace Fingerprint
{
    public class fingerSensor
    {
        uint HEADER = 0xEF01;
        uint address = 0xFFFFFFFF;
        SerialPort serial = new SerialPort();

        //constructors for fingerSensors, also makes a SerialPort useful
        public fingerSensor(string port, int baud, uint addr)
        {
            address = addr;
            serial.BaudRate = baud;
            serial.PortName = port;
        }
        public fingerSensor(string port, int baud)
        {
            serial.BaudRate = baud;
            serial.PortName = port;
        }
        public fingerSensor()
        {
            serial.BaudRate = 1; //for testing
        }

        //handshake command
        public void handshake()
        {
            byte[] contents = {0x17,0x00};
            write((uint)0x01, contents);
        }

        //writes the command to the fingerprint scanner, requires the package identifier and the instruction packets
        void write(uint pid, byte[] packets)
        {
            // turn total packet into a byte array, then send with Serial.Write(Byte[],Int32,Int32)
            uint length = (uint)packets.GetLength(0) + 2; //length is the packet length plus the checksum(2 bytes)
            uint total_length = length + 9; //length+header(2)+address(4)+length of length(2)+pid(1)
            byte[] command = new byte[total_length]; //make packet to send to scanner
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
            for (int i = 9; i < total_length-2; i++)
            {
                command[i] = packets[i - 9];
            }
            command[total_length - 2] = (byte)(checksum >> 8);
            command[total_length - 1] = (byte)checksum;
            //write to serial
            if(serial.BaudRate > 1) //also test code
                serial.Write(command, 0, (int)total_length);

            //test code
            for (int i = 0; i < total_length; i++)
            {
                if (command[i] < 16) //nicer to read
                    Console.Write(0);
                Console.Write(command[i].ToString("X"));
            }

        }
    }
}
