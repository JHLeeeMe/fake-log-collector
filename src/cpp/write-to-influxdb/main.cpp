#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "mqueue_receiver.hpp"

int main()
{
    struct hostent* host_entry{gethostbyname("influxdb")};
    if (!host_entry)
    {
        std::cerr << "gethostbyname(...) failed..." << std::endl;
        return 1;
    }

    const char* server_ip{inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]))};

    const int port{8089};

    struct sockaddr_in s_sockaddr_info;
    std::memset(&s_sockaddr_info, 0x00, sizeof(s_sockaddr_info));
    s_sockaddr_info.sin_family = AF_INET;
    s_sockaddr_info.sin_addr.s_addr = inet_addr(server_ip);
    s_sockaddr_info.sin_port = htons(port);

    int c_socket{socket(AF_INET, SOCK_DGRAM, 0)};
    if (c_socket < 0)
    {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    std::unique_ptr<char> cwd{get_current_dir_name()};
    std::string cwd_str{cwd.get()};
    //MQReceiver receiver{cwd.get()};
    MQReceiver receiver{cwd_str.substr(0, cwd_str.find_last_of('/'))};

    std::unique_ptr<struct MsgBuf> msg_buf;
    std::string msg;
    std::string key;
    std::string data;
    while (true)
    {
        receiver.recv_msg();
        msg_buf = receiver.get_msg();
        msg = (msg_buf->payload).data;

        key = msg.substr(0, msg.find(','));
        msg = msg.substr(key.length() + 1);

        std::cout << "Message: " << msg << std::endl;

        data.append("log,tag1=" + key);
        data.append(" from=\"write-to-influxdb\"");
        data += '\n';

        std::cout << "Data: " << data << std::endl;

        if (sendto(c_socket, data.c_str(), data.length(), 0,
                   (struct sockaddr*)&s_sockaddr_info, sizeof(s_sockaddr_info)) < 0)
        {
            std::cerr << "sendto(...) failed..." << std::endl;
            close(c_socket);
            return 1;
        }

        data.clear();

        std::cout << "UDP request sent successfully." << std::endl;
    }

    close(c_socket);

    return 0;
}

