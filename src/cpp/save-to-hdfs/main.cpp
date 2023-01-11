#include "mqueue_receiver.h"

int main()
{
    MQReceiver receiver{MQReceiver()};

    while (true)
    {
        receiver.recv_msg();
    }

    return 0;
}