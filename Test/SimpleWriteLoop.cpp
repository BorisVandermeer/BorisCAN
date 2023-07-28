// Test Send


#include<iostream>

#include <functional>
#include <atomic>
#include <mutex>
#include <thread>
#include <condition_variable>
#include<csignal>

#include<BorisCAN/BorisCAN.h>

using namespace std;
using namespace Drivers;

Drivers::CAN_Handler canh;

class SimpleTimer{
public:
    SimpleTimer(): _expired(true), _try_to_expire(false){};
    SimpleTimer(const SimpleTimer & timer){
        _expired = timer._expired.load();
        _try_to_expire = timer._try_to_expire.load();
    }

    void startSync(int interval, std::function<void()> task); // rel = interval+task
    void startASync(int interval, std::function<void()> task); // rel = interval
    void startOnce(int delay, std::function<void()> task);
    void stop();

private:
    std::atomic<bool> _expired; // timer stopped status
    std::atomic<bool> _try_to_expire; // timer is in stop process
    std::mutex _mutex;
    std::condition_variable _expired_cond;

};

void SimpleTimer::startSync(int interval, std::function<void()> task){
    // is started, do not start again
    if (_expired == false) return;

    _expired = false;
    std::thread([this, interval, task]()->void{
        while (!_try_to_expire)
        {
            // sleep every interval and do the task again and again until times up
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            task();
        }

        {
            // timer be stopped, update the condition variable expired and wake main thread
            std::lock_guard<std::mutex> locker(_mutex);
            _expired = true;
            _expired_cond.notify_one();
        }
    }).detach();
}

void SimpleTimer::startASync(int interval, std::function<void()> task){
    if (_expired == false) return;

    _expired = false;
    std::thread([this, interval, task]()->void{
        while (!_try_to_expire)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            std::thread(task).detach();
        }

        {
            // timer be stopped, update the condition variable expired and wake main thread
            std::lock_guard<std::mutex> locker(_mutex);
            _expired = true;
            _expired_cond.notify_one();
        }
    }).detach();
}

void SimpleTimer::startOnce(int delay, std::function<void()> task){
    std::thread([delay, task]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        task();
    }).detach();
}

void SimpleTimer::stop(){
    // do not stop again
    if (_expired)
        return;

    if (_try_to_expire)
        return;

    // wait until timer 
    _try_to_expire = true; // change this bool value to make timer while loop stop
    {
        std::unique_lock<std::mutex> locker(_mutex);
        _expired_cond.wait(locker, [this] {return _expired == true; });

        // reset the timer
        if (_expired == true)
            _try_to_expire = false;
    }
}

SimpleTimer timer;
CANMsgFrame RXBUFFER[100];
CANMsgFrame TXBUFFER[100];

unsigned int t = 0;

void SendCan(){
    TXBUFFER[1].ID = 10000;
    TXBUFFER[1].Type = CAN_MSG_TYPE_STANDARD|CAN_MSG_TYPE_DATA;
    TXBUFFER[1].Datalen = 4;
    TXBUFFER[1].Data[0] = 0x01;
    TXBUFFER[1].Data[1] = 0x02;
    TXBUFFER[1].Data[2] = 0x03;
    TXBUFFER[1].Data[3] = 0x04;
    TXBUFFER[1].TimeFlag = 0;
    TXBUFFER[0].ID = 10002;
    TXBUFFER[0].Type = CAN_MSG_TYPE_STANDARD|CAN_MSG_TYPE_DATA;
    TXBUFFER[0].Datalen = 8;
    TXBUFFER[0].TimeFlag = 0;
    unsigned int * ptr = (unsigned int *)TXBUFFER[0].Data;
    *ptr = t;
    t++;
    if(t>0x0FFFFFF) t = 0;
    TXBUFFER[0].TimeFlag = 0;
    canh.Transmit(TXBUFFER,1,1);
    canh.TransmitSync(TXBUFFER,2,1);
}

int main(int argc,char** argv){
    int ChannelID = 1;
    if(argc > 2 ){
        cout<<"Invalid Args"<<endl;
    } else if(argc == 2){
        ChannelID = argv[1][0]-'0';
    }

    signal(SIGINT,[](int){timer.stop();canh.Close(); exit(0);});
    CAN_Handler::CANDeviceConfig config;
    config.DeviceID = 0;
    config.ChannelID = ChannelID;
    config.Baudrate = CAN_BAUDRATE_1M;
    if(!canh.OpenDevice(config)) return 1;

    timer.startSync(25,SendCan);
    
    while(1);

}