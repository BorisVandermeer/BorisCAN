

#include <Kvaser/include/canlib.h>
#include <canstat.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

#define READ_WAIT_INFINITE (unsigned long)(-1)

static unsigned int msgCounter = 0;

static void check(const char *id, canStatus stat)
{
    if (stat != canOK) {
        char buf[50];
        buf[0] = '\0';
        canGetErrorText(stat, buf, sizeof(buf));
        printf("%s: failed, stat=%d (%s)\n", id, (int)stat, buf);
    }
}

static void printUsageAndExit(char *prgName)
{
    printf("Usage: '%s <channel>'\n", prgName);
    exit(1);
}

static void sighand(int sig, siginfo_t *info, void *ucontext)
{
    (void)sig;
    (void)info;
    (void)ucontext;
}

static const char *busStatToStr(const unsigned long flag)
{
    switch (flag) {
    case canSTAT_ERROR_PASSIVE:
        return "canSTAT_ERROR_PASSIVE";

    case canSTAT_BUS_OFF:
        return "canSTAT_BUS_OFF";

    case canSTAT_ERROR_WARNING:
        return "canSTAT_ERROR_WARNING";

    case canSTAT_ERROR_ACTIVE:
        return "canSTAT_ERROR_ACTIVE";

    default:
        return "";
    }
}

void notifyCallback(canNotifyData *data)
{
    switch (data->eventType) {
    case canEVENT_STATUS:
        printf("CAN Status Event: %s\n", busStatToStr(data->info.status.busStatus));
        break;

    case canEVENT_ERROR:
        printf("CAN Error Event\n");
        break;

    case canEVENT_TX:
        printf("CAN Tx Event\n");
        break;

    case canEVENT_RX:
        printf("CAN Rx Event\n");
        break;
    }
    return;
}

int main(int argc, char *argv[])
{
    canHandle hnd;
    canStatus stat;
    int channel;
    struct sigaction sigact;

    if (argc != 2) {
        printUsageAndExit(argv[0]);
    }

    {
        char *endPtr = NULL;
        errno = 0;
        channel = strtol(argv[1], &endPtr, 10);
        if ((errno != 0) || ((channel == 0) && (endPtr == argv[1]))) {
            printUsageAndExit(argv[0]);
        }
    }

    /* Use sighand and allow SIGINT to interrupt syscalls */
    sigact.sa_flags = SA_SIGINFO;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_sigaction = sighand;
    if (sigaction(SIGINT, &sigact, NULL) != 0) {
        perror("sigaction SIGINT failed");
        return -1;
    }

    printf("Reading CAN messages on channel %d\n", channel);

    canInitializeLibrary();

    /* Open channel, set parameters and go on bus */
    hnd = canOpenChannel(channel,
                         canOPEN_EXCLUSIVE | canOPEN_REQUIRE_EXTENDED | canOPEN_ACCEPT_VIRTUAL);
    if (hnd < 0) {
        printf("canOpenChannel %d", channel);
        check("", static_cast<canStatus>(hnd));
        return -1;
    }

    stat = canSetNotify(hnd, notifyCallback,
                        canNOTIFY_RX | canNOTIFY_TX | canNOTIFY_ERROR | canNOTIFY_STATUS |
                            canNOTIFY_ENVVAR,
                        (char *)0);
    check("canSetNotify", stat);

    stat = canSetBusParams(hnd, canBITRATE_1M, 0, 0, 0, 0, 0);
    check("canSetBusParams", stat);
    if (stat != canOK) {
        goto ErrorExit;
    }

    stat = canBusOn(hnd);
    check("canBusOn", stat);
    if (stat != canOK) {
        goto ErrorExit;
    }

    do {
        long id;
        unsigned char msg[8];
        unsigned int dlc;
        unsigned int flag;
        unsigned long time;

        stat = canReadWait(hnd, &id, &msg, &dlc, &flag, &time, READ_WAIT_INFINITE);

        if (stat == canOK) {
            msgCounter++;
            if (flag & canMSG_ERROR_FRAME) {
                printf("(%u) ERROR FRAME", msgCounter);
            } else {
                unsigned j;

                printf("(%u) id:%ld dlc:%u data: ", msgCounter, id, dlc);
                if (dlc > 8) {
                    dlc = 8;
                }
                for (j = 0; j < dlc; j++) {
                    printf("%2.2x ", msg[j]);
                }
            }
            printf(" flags:0x%x time:%lu\n", flag, time);
        } else {
            if (errno == 0) {
                check("\ncanReadWait", stat);
            } else {
                perror("\ncanReadWait error");
            }
        }

    } while (stat == canOK);

ErrorExit:

    stat = canBusOff(hnd);
    check("canBusOff", stat);
    usleep(50 * 1000); // Sleep just to get the last notification.
    stat = canClose(hnd);
    check("canClose", stat);
    stat = canUnloadLibrary();
    check("canUnloadLibrary", stat);

    return 0;
}
