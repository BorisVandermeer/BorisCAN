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

#include<canlib.h>

#include<BorisCAN/BorisCAN.h>

using namespace std;

namespace Drivers{

    bool __attribute__((weak))  CAN_Handler::OpenDevice(const CANDeviceConfig & config){
       cout<< " This Device Don't Support OpenDevice ";
       throw " This Device Don't Support OpenDevice ";
    }


    bool __attribute__((weak)) CAN_Handler::isOpen(){
        cout<< " This Device Don't Support isOpen ";
        throw " This Device Don't Support isOpen ";
    }

    bool __attribute__((weak)) CAN_Handler::SetCallBack(void (*callback)(const CANCallbackData *),unsigned int flags){
        
        cout<< " This Device Don't Support SetCallBack ";
        throw " This Device Don't Support SetCallBack ";
    }

    int __attribute__((weak)) CAN_Handler::Recieve(CANMsgFrame* Buffer){

        cout<< " This Device Don't Support Recieve ";
        throw " This Device Don't Support Recieve ";
    }

    int  __attribute__((weak)) CAN_Handler::Recieve(CANMsgFrame* Buffer, int size){
        cout<< " This Device Don't Support Recieve with size";
        throw " This Device Don't Support Recieve with size";
    }

    int __attribute__((weak)) CAN_Handler::RecieveSync(CANMsgFrame* Buffer,int timeout){
        cout<< " This Device Don't Support RecieveSync ";
        throw " This Device Don't Support RecieveSync ";
    }

    int __attribute__((weak)) CAN_Handler::RecieveSync(CANMsgFrame* Buffer,int size,int timeout){
        cout<< " This Device Don't Support RecieveSync with size";
        throw " This Device Don't Support RecieveSync with size";
    }


    int __attribute__((weak)) CAN_Handler::Transmit(CANMsgFrame* Buffer, int size,int timeout){
        cout<< " This Device Don't Support Transmit ";

        throw " This Device Don't Support Transmit ";
    }

    int __attribute__((weak)) CAN_Handler::TransmitSync(CANMsgFrame* Buffer, int size,int timeout){
        cout<< " This Device Don't Support TransmitSync ";

        throw " This Device Don't Support TransmitSync ";
    }

    bool __attribute__((weak)) CAN_Handler::Close(){
        cout<< " This Device Don't Support Close ";
        throw " This Device Don't Support Close ";
    }

}//namespace Drivers
