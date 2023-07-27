// Test Send


#include<iostream>
#include<csignal>
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

    signal(SIGINT,[](int){canh.Close();exit(0);});

    CAN_Handler::CANDeviceConfig config;
    config.ChannelID = ChannelID;
    config.Baudrate = CAN_BAUDRATE_1M;
    canh.OpenDevice(config);

    while(1){
        int num = canh.Recieve(RXBUFFER,3);
        int cnt = 0;
        while(cnt<num){
            cout<<"ID : "<<RXBUFFER[cnt].ID<<' ';
            cout<<"DLC : "<<(int)RXBUFFER[cnt].Datalen<<' ';
            cout<<"DATA : ";
            for(int i = 0;i<RXBUFFER[cnt].Datalen;i++){
                cout<<hex<<(int)RXBUFFER[cnt].Data[i]<<' ';
            }
            cout<<endl;
            cnt++;
        }
    }
    
    canh.Close();
    return 0;
}