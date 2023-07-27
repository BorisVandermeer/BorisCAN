/*********************************************************************
 * 
 * Author : BorisVandermeer
 * 
 * Discription ：
 *      A implement of Kvaser CAN Abstraction
 * 
*********************************************************************/

#include<vector>

#include<canlib.h>

#include<BorisCAN/BorisCAN.h>

namespace Drivers{

    std::vector<void (*)(const CANCallbackData *)> foos;

    static unsigned char BaduEnumToConfig(CAN_BAUDRATE baud){
        if(baud == CAN_BAUDRATE_1M) return canBITRATE_1M;
        else if(baud == CAN_BAUDRATE_500K) return canBITRATE_500K;
        else if(baud == CAN_BAUDRATE_250K) return canBITRATE_250K;
        else if(baud == CAN_BAUDRATE_125K) return canBITRATE_125K;
        else if(baud == CAN_BAUDRATE_100K) return canBITRATE_100K;
        else if(baud == CAN_BAUDRATE_50K ) return canBITRATE_50K ;
        else if(baud == CAN_BAUDRATE_10K ) return canBITRATE_10K ;
        else{
            throw "Invalid BAUDRATE For This Device";
        }
    }
    CANCallbackData d;
    static void notifyCallback(canNotifyData *data){
        
        if(data->eventType==canEVENT_RX){
            d.eventType = BORIS_CAN_EVENT_RX;
            d.rx.id = data->info.rx.id;
            d.rx.time = data->info.rx.time;
            foos[0](&d);
        } else if (data->eventType==canEVENT_TX){
            d.eventType = BORIS_CAN_EVENT_TX;
            d.tx.id = data->info.tx.id;
            d.tx.time = data->info.tx.time;
            foos[0](&d);
        }
    }

    bool CAN_Handler::OpenDevice(const CANDeviceConfig & config){
        canInitializeLibrary();
        Channel_ID = config.ChannelID;
        Device_ID = canOpenChannel(config.ChannelID,canOPEN_EXCLUSIVE | canOPEN_REQUIRE_EXTENDED | canOPEN_ACCEPT_VIRTUAL);
        auto baudset = BaduEnumToConfig(config.Baudrate);
        auto stat = canSetBusParams(Device_ID, baudset, 0, 0, 0, 0, 0);
        if(stat != canOK) return false;
        stat = canBusOn(Device_ID);
        if(stat != canOK) return false;
            
        return true;
    }


    bool CAN_Handler::isOpen(){
        throw "Kaser No This Func";

    }

    bool CAN_Handler::SetCallBack(void (*callback)(const CANCallbackData *),unsigned int flags){
        
        if(CallBackId == -1){
            foos.push_back(callback);
            CallBackId = foos.size()-1;
        } else {
            foos[CallBackId] = callback;
        }

        unsigned int flag = 0x00U;
        if(flags&BORIS_CAN_EVENT_RX) flag |= canNOTIFY_RX;
        if(flags&BORIS_CAN_EVENT_TX) flag |= canNOTIFY_TX;

        auto stat =  canSetNotify(Device_ID, notifyCallback,flag,nullptr);
        if(stat != canOK){
            return false;
        }
        return true;
    }

    int CAN_Handler::Recieve(CANMsgFrame* Buffer){
        int a = 0;
        long id;
        // unsigned char msg[8];
        unsigned int dlc;
        unsigned int flag;
        unsigned long time;
        while(1){
            auto stat = canRead(Device_ID, &id, &Buffer[a].Data, &dlc, &flag, &time);
            if(stat!=canOK) break;
            auto & frame = Buffer[a];
            frame.Type = Unknown;
            frame.ID = id;frame.Datalen = dlc;frame.TimeFlag=flag;frame.TimeStamp=time;
            a++;
        }
        return a;
    }

    int CAN_Handler::Recieve(CANMsgFrame* Buffer,int size){
        int a = 0;
        long id;
        // unsigned char msg[8];
        unsigned int dlc;
        unsigned int flag;
        unsigned long time;
        while(a<size){
            auto stat = canRead(Device_ID, &id, &Buffer[a].Data, &dlc, &flag, &time);
            if(stat!=canOK) break;
            auto & frame = Buffer[a];
            frame.Type = Unknown;
            frame.ID = id;frame.Datalen = dlc;frame.TimeFlag=flag;frame.TimeStamp=time;
            a++;
        }
        return a;
    }

    int CAN_Handler::RecieveSync(CANMsgFrame* Buffer,int timeout){
        int a = 0;
        long id;
        // unsigned char msg[8];
        unsigned int dlc;
        unsigned int flag;
        unsigned long time;
        canStatus stat;
        if(timeout == BORIS_CAN_RECIEVE_WAIT_INFINITE){
            stat = canReadWait(Device_ID, &id, &Buffer[a].Data, &dlc, &flag, &time, BORIS_CAN_RECIEVE_WAIT_INFINITE);
            if(stat != canOK) return 0;
        } else {
            stat = canReadWait(Device_ID, &id, &Buffer[a].Data, &dlc, &flag, &time, timeout);
            if(stat != canOK) return 0;
        }
        while(1){
            auto & frame = Buffer[a];
            frame.Type = Unknown;
            frame.ID = id;frame.Datalen = dlc;frame.TimeFlag=flag;frame.TimeStamp=time;
            a++;
            auto stat = canRead(Device_ID, &id, &Buffer[a].Data, &dlc, &flag, &time);
            if(stat!=canOK) break;
        }
        return a;
    }

    int CAN_Handler::RecieveSync(CANMsgFrame* Buffer,int size, int timeout){
        int a = 0;
        long id;
        // unsigned char msg[8];
        unsigned int dlc;
        unsigned int flag;
        unsigned long time;
        canStatus stat;
        if(timeout == BORIS_CAN_RECIEVE_WAIT_INFINITE){
            stat = canReadWait(Device_ID, &id, &Buffer[a].Data, &dlc, &flag, &time, BORIS_CAN_RECIEVE_WAIT_INFINITE);
            if(stat != canOK) return 0;
        } else {
            stat = canReadWait(Device_ID, &id, &Buffer[a].Data, &dlc, &flag, &time, timeout);
            if(stat != canOK) return 0;
        }
        while(a<size){
            auto & frame = Buffer[a];
            frame.Type = Unknown;
            frame.ID = id;frame.Datalen = dlc;frame.TimeFlag=flag;frame.TimeStamp=time;
            a++;
            auto stat = canRead(Device_ID, &id, &Buffer[a].Data, &dlc, &flag, &time);
            if(stat!=canOK) break;
        }
        return a;
    }


    int CAN_Handler::Transmit(CANMsgFrame* Buffer, int size,int timeout){
        int cnt = 0;
        while(cnt<size){
            auto stat = canWriteSync(Device_ID,timeout);
            if(stat!=canOK) break;
            long id = Buffer[cnt].ID;
            unsigned char* msg  = Buffer[cnt].Data;
            unsigned int   dlc  = Buffer[cnt].Datalen;
            unsigned int   flag = Buffer[cnt].TimeFlag;
            stat = canWrite(Device_ID, id, msg, dlc, flag);
            if(stat!=canOK) break;
            cnt++;
        }
        return size-cnt;
    }

    int CAN_Handler::TransmitSync(CANMsgFrame* Buffer, int size,int timeout){
        int cnt = 0;
        while(cnt<size){
            long id = Buffer[cnt].ID;
            unsigned char* msg  = Buffer[cnt].Data;
            unsigned int   dlc  = Buffer[cnt].Datalen;
            unsigned int   flag = Buffer[cnt].TimeFlag;
            auto stat = canWriteWait(Device_ID, id, msg, dlc, flag,timeout);
            if(stat!=canOK) break;
            cnt++;
        }
        return size-cnt;
    }

    bool CAN_Handler::Close(){
        auto stat = canBusOff(Device_ID);
        if(stat!=canOK) return false;
        stat = canClose(Device_ID);
        if(stat!=canOK) return false;
        stat = canUnloadLibrary();
        if(stat!=canOK) return false;
        return true;
    }

}//namespace Drivers
