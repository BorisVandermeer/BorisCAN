/*********************************************************************
 * 
 * Author : BorisVandermeer
 * 
 * Discription ：
 *      A implement of CAN Abstraction
 * 
*********************************************************************/

#include<vector>
#include<string>


#define BORIS_CAN_RECIEVE_WAIT_INFINITE  (-1)
#define BORIS_CAN_TRANSMIT_WAIT_INFINITE (-1)

namespace Drivers{

    enum CAN_BAUDRATE{
        CAN_BAUDRATE_1M   = 1000000,
        CAN_BAUDRATE_500K =  500000,
        CAN_BAUDRATE_250K =  250000,
        CAN_BAUDRATE_125K =  125000,
        CAN_BAUDRATE_100K =  100000,
        CAN_BAUDRATE_50K  =   50000,
        CAN_BAUDRATE_10K  =   10000,
    };

    enum CAN_MSG_TYPE{
        Unknown,
        Data,
        Remote,
        Extern,
    };

    struct  CANMsgFrame{
        CAN_MSG_TYPE    Type;
        unsigned int	ID;
        unsigned int	TimeStamp;
        unsigned char	TimeFlag;
        unsigned char	Datalen;
        unsigned char	Data[8];
        unsigned char	Reserved[3];
    };

    #define BORIS_CAN_EVENT_RX (0x01<<0x00)
    #define BORIS_CAN_EVENT_TX (0x01<<0x01)
    // #define BORIS_CAN_EVENT_BUS (0x01<<0x01);

    struct CANCallbackData{
        int eventType;
        struct { long id; unsigned long time;} rx;
        struct { long id; unsigned long time;} tx;
    };
    
    class CAN_Handler{
    public:
        struct CANDeviceConfig{
            int DeviceID;
            int ChannelID;
            CAN_BAUDRATE Baudrate;
            unsigned int AccCode;
            unsigned int AccMask;
        };

    public:        
        CAN_Handler() = default;
        bool OpenDevice(const CANDeviceConfig &config);
        bool isOpen();
        bool Close();
        bool SetCallBack(void (*callback)(const CANCallbackData *),unsigned int flags);
        int  Recieve(CANMsgFrame* Buffer);
        int  Recieve(CANMsgFrame* Buffer, int size);
        int  RecieveSync(CANMsgFrame* Buffer, int timeout); // timeout == -1 for INFINITE,return -1 for err
        int  RecieveSync(CANMsgFrame* Buffer, int size, int timeout);
        int  Transmit(CANMsgFrame* Buffer, int size,int timeout);
        int  TransmitSync(CANMsgFrame* Buffer, int size, int timeout);
        
    private:
        // bool DLL_OK = false;
        // bool Config_OK = false;
        int  Device_ID = -1;
        int  Channel_ID = -1;
        int  CallBackId = -1;


        // CAN_BAUDRATE baud_;

        // CAN_Msg ReciBuffer[20];
        // CAN_Msg SendBuffer[20];
        
    };

}//namespace Drivers

