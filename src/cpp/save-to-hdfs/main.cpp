#include <curl/curl.h>

#include "mqueue_receiver.h"

size_t write_callback(char* response_data, size_t size, size_t nmemb, void* user_data)
{
    ((std::string*)user_data)->append(response_data, size * nmemb);
    //response.append(data, size * nmemb);
    return size * nmemb;
}

int main()
{
    MQReceiver receiver{MQReceiver()};

    ///////////////////////////////////////////////////////////////////////////
    std::unique_ptr<CURL, decltype(&curl_easy_cleanup)> curl{curl_easy_init(), &curl_easy_cleanup};
    if(!curl)
    {
        std::cerr << "curl_easy_init() failed..." << std::endl;
        exit(1);
    }

    CURLcode res;

    std::string data;
    std::string url{"http://master:50070/webhdfs/v1/?op=CREATE&noredirect=true"};
    std::string response;

    curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl.get(), CURLOPT_CUSTOMREQUEST, "PUT");
    //curl_easy_setopt(curl.get(), CURLOPT_POSTFIELDS, "hello, webHDFS!");
    curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &response);

    res = curl_easy_perform(curl.get());
    if(CURLE_OK != res)
    {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        exit(1);
    }

    std::cout << "hi" << std::endl;
    std::cout << response << std::endl;

    //char* ct;
    /* ask for the content-type */
    //res = curl_easy_getinfo(curl.get(), CURLINFO_CONTENT_TYPE, &ct);

    //if((CURLE_OK == res) && ct)
    //{
    //    std::cout << "\n---------------- Content-Type ----------------\n"
    //              << ct << std::endl;;
    //}
    ///////////////////////////////////////////////////////////////////////////


    std::cout << "yahoo!" << std::endl;

    while (true)
    {
        receiver.recv_msg();
        //std::unique_ptr<struct MsgBuf> msg = receiver.get_msg();
        //std::cout << "Type: " << msg->type << std::endl;
        //std::cout << "Message: " << (msg->payload).data << std::endl;
    }

    return 0;
}

