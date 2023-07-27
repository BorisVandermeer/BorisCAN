
#include<canlib.h>

void main(void)
{
    canHandle h;
    canInitializeLibrary(); 
    h = canOpenChannel(0, canWANT_EXCLUSIVE);
    if (h != canOK) {
        char msg[64];
        canGetErrorText((canStatus)h, msg, sizeof(msg));
        // fprintf(stderr, "canOpenChannel failed (%s)\n", msg);
        exit(1);
    }
    canSetBusParams(h, BAUD_250K, 0);
    canSetBusOutputControl(h, canDRIVER_NORMAL);
    canBusOn(h);
    canWrite(h, 123, "HELLO!", 6, 0);
    canWriteSync(h, 500);
    canBusOff(h);
    canClose(h);
}