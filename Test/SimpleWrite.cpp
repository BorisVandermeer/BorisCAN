// Test Send


#include<iostream>
#include<BorisCAN/BorisCAN.h>



using namespace std;
using namespace Drivers;

Drivers::CAN_Handler canh;

CANMsgFrame RXBUFFER[100];
CANMsgFrame TXBUFFER[100];


int main(int argc,char** argv){
    int ChannelID = 1;
    if(argc > 2 ){
        cout<<"Invalid Args"<<endl;
    } else if(argc == 2){
        ChannelID = argv[1][0]-'0';
    }
    CAN_Handler::CANDeviceConfig config;
    config.ChannelID = ChannelID;
    config.Baudrate = CAN_BAUDRATE_1M;
    canh.OpenDevice(config);
    TXBUFFER[1].ID = 10000;
    TXBUFFER[1].Datalen = 4;
    TXBUFFER[1].Data[0] = 0x01;
    TXBUFFER[1].Data[1] = 0x02;
    TXBUFFER[1].Data[2] = 0x03;
    TXBUFFER[1].Data[3] = 0x04;
    TXBUFFER[1].TimeFlag = 0;
    TXBUFFER[0].ID = 10002;
    TXBUFFER[0].Datalen = 3;
    TXBUFFER[0].Data[0] = 0x02;
    TXBUFFER[0].Data[1] = 0xF3;
    TXBUFFER[0].Data[2] = 0x01;
    TXBUFFER[0].Data[3] = 0x04;
    TXBUFFER[0].TimeFlag = 0;
    canh.Transmit(TXBUFFER,1,1);
    canh.TransmitSync(TXBUFFER,2,1);
    canh.Close();
    return 0;
}