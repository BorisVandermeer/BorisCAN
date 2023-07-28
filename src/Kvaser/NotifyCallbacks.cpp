
#include<vector>

#include<canlib.h>

#include<BorisCAN/BorisCAN.h>

namespace Drivers{
    std::vector<void (*)(const CANCallbackData *)> cancallbacks;
    
    void notifyCallback0(canNotifyData *data){

        CANCallbackData d;
        if(data->eventType==canEVENT_RX){
            d.eventType = BORIS_CAN_EVENT_RX;
            d.rx.id = data->info.rx.id;
            d.rx.time = data->info.rx.time;
            cancallbacks[0](&d);
        } else if (data->eventType==canEVENT_TX){
            d.eventType = BORIS_CAN_EVENT_TX;
            d.tx.id = data->info.tx.id;
            d.tx.time = data->info.tx.time;
            cancallbacks[0](&d);
        }
    }

    void notifyCallback1(canNotifyData *data){
        CANCallbackData d;
        if(data->eventType==canEVENT_RX){
            d.eventType = BORIS_CAN_EVENT_RX;
            d.rx.id = data->info.rx.id;
            d.rx.time = data->info.rx.time;
            cancallbacks[1](&d);
        } else if (data->eventType==canEVENT_TX){
            d.eventType = BORIS_CAN_EVENT_TX;
            d.tx.id = data->info.tx.id;
            d.tx.time = data->info.tx.time;
            cancallbacks[1](&d);
        }
    }

    void notifyCallback2(canNotifyData *data){

        CANCallbackData d;
        if(data->eventType==canEVENT_RX){
            d.eventType = BORIS_CAN_EVENT_RX;
            d.rx.id = data->info.rx.id;
            d.rx.time = data->info.rx.time;
            cancallbacks[2](&d);
        } else if (data->eventType==canEVENT_TX){
            d.eventType = BORIS_CAN_EVENT_TX;
            d.tx.id = data->info.tx.id;
            d.tx.time = data->info.tx.time;
            cancallbacks[2](&d);
        }
    }

    void notifyCallback3(canNotifyData *data){
        CANCallbackData d;
        if(data->eventType==canEVENT_RX){
            d.eventType = BORIS_CAN_EVENT_RX;
            d.rx.id = data->info.rx.id;
            d.rx.time = data->info.rx.time;
            cancallbacks[3](&d);
        } else if (data->eventType==canEVENT_TX){
            d.eventType = BORIS_CAN_EVENT_TX;
            d.tx.id = data->info.tx.id;
            d.tx.time = data->info.tx.time;
            cancallbacks[3](&d);
        }
    }

    void notifyCallback4(canNotifyData *data){

        CANCallbackData d;
        if(data->eventType==canEVENT_RX){
            d.eventType = BORIS_CAN_EVENT_RX;
            d.rx.id = data->info.rx.id;
            d.rx.time = data->info.rx.time;
            cancallbacks[4](&d);
        } else if (data->eventType==canEVENT_TX){
            d.eventType = BORIS_CAN_EVENT_TX;
            d.tx.id = data->info.tx.id;
            d.tx.time = data->info.tx.time;
            cancallbacks[4](&d);
        }
    }

    void notifyCallback5(canNotifyData *data){
        CANCallbackData d;
        if(data->eventType==canEVENT_RX){
            d.eventType = BORIS_CAN_EVENT_RX;
            d.rx.id = data->info.rx.id;
            d.rx.time = data->info.rx.time;
            cancallbacks[5](&d);
        } else if (data->eventType==canEVENT_TX){
            d.eventType = BORIS_CAN_EVENT_TX;
            d.tx.id = data->info.tx.id;
            d.tx.time = data->info.tx.time;
            cancallbacks[5](&d);
        }
    }
    auto kvnotifys = 
    {notifyCallback0,notifyCallback1,notifyCallback2,notifyCallback3,notifyCallback4,notifyCallback5};
}//namespace Drivers