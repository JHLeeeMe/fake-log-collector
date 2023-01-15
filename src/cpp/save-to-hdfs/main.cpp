#include <curl/curl.h>

#include "mqueue_receiver.h"

void set_url(std::string* url,
             const std::string op,
             const std::string& path = "",
             const std::string& addr = "master",
             const std::string& port = "50070")
{
    url->clear();
    url->append("http://" + addr + ":" + port);
    url->append("/webhdfs/v1/" + path);
    url->append("?op=" + op);
    url->append("&noredirect=true");
}

size_t write_callback(char* response_data, size_t size, size_t nmemb, void* user_data)
{
    ((std::string*)user_data)->append(response_data, size * nmemb);

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

    std::string url;
    std::string response;

    set_url(&url, "CREATE", "tmp");
    curl_easy_setopt(curl.get(), CURLOPT_URL, &url);
    curl_easy_setopt(curl.get(), CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &response);

    res = curl_easy_perform(curl.get());
    if(CURLE_OK != res)
    {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        exit(1);
    }

    std::cout << response << std::endl;

    set_url(&url, "APPEND");
    curl_easy_setopt(curl.get(), CURLOPT_URL, &url);
    curl_easy_setopt(curl.get(), CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl.get(), CURLOPT_POSTFIELDS, "hello, webHDFS!");
    curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &response);


    while (true)
    {
        receiver.recv_msg();
        //std::unique_ptr<struct MsgBuf> msg = receiver.get_msg();
        //std::cout << "Type: " << msg->type << std::endl;
        //std::cout << "Message: " << (msg->payload).data << std::endl;
    }

    return 0;
}

