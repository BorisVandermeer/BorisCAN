/*********************************************************************
 * 
 * Author : BorisVandermeer
 * 
 * Discription ：
 *      A implement of CANalyst CAN Abstraction
 * 
*********************************************************************/

/*********************************************************************
 * 
 * Author : BorisVandermeer
 * 
 * Discription ：
 *      A implement of Base CAN ABS
 * 
*********************************************************************/

#include<vector>
#include<iostream>

#include<controlcan.h>

#include<BorisCAN/BorisCAN.h>

using namespace std;

#define CAN_OK (1)
#define CAN_ERR (0)
#define VCI_DEVICE_TYPE (4)

namespace Drivers{
    static bool BaudToTimming(CAN_BAUDRATE baud, unsigned char& T0, unsigned char& T1 ){
             if(baud == CAN_BAUDRATE_1M  ) T0=0x00,T1=0x14;
        else if(baud == CAN_BAUDRATE_500K) T0=0x00,T1=0x1c;
        else if(baud == CAN_BAUDRATE_250K) T0=0x01,T1=0x1c;
        else if(baud == CAN_BAUDRATE_125K) T0=0x03,T1=0x1c;
        else if(baud == CAN_BAUDRATE_100K) T0=0x04,T1=0x1c;
        else if(baud == CAN_BAUDRATE_50K ) T0=0x09,T1=0x1c;
        else if(baud == CAN_BAUDRATE_10K ) T0=0x31,T1=0x1c;
        else return false;
        return true;

    }

    bool CAN_Handler::OpenDevice(const CANDeviceConfig & config){
        Device_ID = config.DeviceID;
        Channel_ID = config.ChannelID;
        VCI_INIT_CONFIG InitConfig{
            .AccCode = config.AccCode,
            .AccMask = config.AccMask,
            .Filter  = 1,
            .Mode    = 0,
        };
        auto stat = BaudToTimming(config.Baudrate,InitConfig.Timing0,InitConfig.Timing1);
        if(!stat) return false;
        if(VCI_OpenDevice(VCI_DEVICE_TYPE,config.DeviceID,0)!=CAN_OK ) return false;
        if(VCI_InitCAN(VCI_DEVICE_TYPE,Device_ID,Channel_ID,&InitConfig)!=CAN_OK)return false;
        if(VCI_StartCAN(VCI_DEVICE_TYPE,Device_ID,Channel_ID)!=1) return false;
        return true;
    }


    int CAN_Handler::Recieve(CANMsgFrame* Buffer){
        // int size = VCI_GetReceiveNum(VCI_DEVICE_TYPE,Device_ID,Channel_ID);
        // if(size = 0) return;
        VCI_CAN_OBJ rec[3000];
        int reclen=0;
        if((reclen=VCI_Receive(VCI_DEVICE_TYPE,Device_ID,Channel_ID,rec,3000,100))>0)//调用接收函数，如果有数据，进行数据处理显示。
        {
            for(int j=0;j<reclen;j++)
            {   Buffer[j].Datalen = rec[j].DataLen;
                Buffer[j].ID      = rec[j].ID;
                Buffer[j].TimeStamp      = rec[j].TimeStamp;
                Buffer[j].TimeFlag      = rec[j].TimeFlag;
                Buffer[j].Type  = CAN_MSG_TYPE_UNKNOWN;
                Buffer[j].Type |= CAN_MSG_TYPE_STANDARD;
                if(rec[j].ExternFlag) Buffer[j].Type = Buffer[j].Type|CAN_MSG_TYPE_EXTERN;
                if(rec[j].RemoteFlag) Buffer[j].Type = Buffer[j].Type|CAN_MSG_TYPE_REMOTE;

                for(int i = 0; i < rec[j].DataLen; i++)
                {
                    Buffer[j].Data[i] = rec[j].Data[i];
                }
            }
        }
        return reclen;
    }

    int  CAN_Handler::Recieve(CANMsgFrame* Buffer, int _size){
        int size = VCI_GetReceiveNum(VCI_DEVICE_TYPE,Device_ID,Channel_ID);
        if(_size>size) _size = size;
        VCI_CAN_OBJ rec[_size];
        int reclen=0;
        if((reclen=VCI_Receive(VCI_DEVICE_TYPE,Device_ID,Channel_ID,rec,_size,0))>0)//调用接收函数，如果有数据，进行数据处理显示。
        {
            for(int j=0;j<reclen;j++)
            {
                Buffer[j].Datalen = rec[j].DataLen;
                Buffer[j].ID      = rec[j].ID;
                Buffer[j].TimeStamp  = rec[j].TimeStamp;
                Buffer[j].TimeFlag   = rec[j].TimeFlag;
                Buffer[j].Type  = CAN_MSG_TYPE_UNKNOWN;
                Buffer[j].Type |= CAN_MSG_TYPE_STANDARD;
                if(rec[j].ExternFlag) Buffer[j].Type = Buffer[j].Type|CAN_MSG_TYPE_EXTERN;
                if(rec[j].RemoteFlag) Buffer[j].Type = Buffer[j].Type|CAN_MSG_TYPE_REMOTE;

                for(int i = 0; i < rec[j].DataLen; i++)
                {
                    Buffer[j].Data[i] = rec[j].Data[i];
                }
            }
        }
        return reclen;
    }

    // int __attribute__((weak)) CAN_Handler::RecieveSync(CANMsgFrame* Buffer,int timeout){
    //     cout<< " This Device Don't Support RecieveSync ";
    //     throw " This Device Don't Support RecieveSync ";
    // }

    // int __attribute__((weak)) CAN_Handler::RecieveSync(CANMsgFrame* Buffer,int size,int timeout){
    //     cout<< " This Device Don't Support RecieveSync with size";
    //     throw " This Device Don't Support RecieveSync with size";
    // }


    int CAN_Handler::Transmit(CANMsgFrame* Buffer, int size,int timeout){
        VCI_CAN_OBJ buf[size];
        for(int j=0;j<size;j++){
            buf[j].DataLen = Buffer[j].Datalen;
            buf[j].ID      = Buffer[j].ID;
            buf[j].TimeStamp  = Buffer[j].TimeStamp;
            buf[j].TimeFlag   = Buffer[j].TimeFlag;
            for(int i = 0; i < Buffer[j].Datalen; i++)
            {
                buf[j].Data[i] = Buffer[j].Data[i];
            }
            buf[j].RemoteFlag = 0;
            if(Buffer[j].Type&=CAN_MSG_TYPE_REMOTE)  buf[j].RemoteFlag = 1;
            buf[j].ExternFlag = 0;
            if(Buffer[j].Type&=CAN_MSG_TYPE_EXTERN)  buf[j].ExternFlag = 1;
        }
        return VCI_Transmit(VCI_DEVICE_TYPE,Device_ID,Channel_ID,buf,size);
    }

    int CAN_Handler::TransmitSync(CANMsgFrame* Buffer, int size,int timeout){
        VCI_CAN_OBJ buf[size];
        for(int j=0;j<size;j++){
            buf[j].DataLen = Buffer[j].Datalen;
            buf[j].ID      = Buffer[j].ID;
            buf[j].TimeStamp  = Buffer[j].TimeStamp;
            buf[j].TimeFlag   = Buffer[j].TimeFlag;
            for(int i = 0; i < Buffer[j].Datalen; i++)
            {
                buf[j].Data[i] = Buffer[j].Data[i];
            }
            buf[j].RemoteFlag = 0;
            if(Buffer[j].Type&=CAN_MSG_TYPE_REMOTE)  buf[j].RemoteFlag = 1;
            buf[j].ExternFlag = 0;
            if(Buffer[j].Type&=CAN_MSG_TYPE_EXTERN)  buf[j].ExternFlag = 1;
        }
        return VCI_Transmit(VCI_DEVICE_TYPE,Device_ID,Channel_ID,buf,size);
    }

    bool CAN_Handler::Close(){
        return VCI_CloseDevice(4,Device_ID);
    }

}//namespace Drivers
