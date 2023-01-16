#include <curl/curl.h>

#include "rapidjson/document.h"
#include "mqueue_receiver.h"

struct timespec start, end;

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

std::unique_ptr<rapidjson::Document> get_json(const char* str)
{
    auto doc = std::make_unique<rapidjson::Document>();
    doc->Parse(str);

    return doc;
}

size_t write_callback(char* response_data, size_t size, size_t nmemb, void* user_data)
{
    ((std::string*)user_data)->append(response_data, size * nmemb);

    return size * nmemb;
}

int main()
{
    MQReceiver receiver;
    //HDFSWriter hdfs_writer;

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
    std::string location;
    std::unique_ptr<rapidjson::Document> doc;

    ////////// Create /tmp
    set_url(&url, "CREATE", "tmp");
    curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl.get(), CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &response);

    res = curl_easy_perform(curl.get());
    if(CURLE_OK != res)
    {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        exit(1);
    }

    doc = get_json(response.c_str());
    for (const auto& m : doc->GetObject())
    {
        if (m.name.GetString() == std::string("RemoteException"))
        {
            std::cerr << m.value["exception"].GetString() << std::endl;
            exit(1);
        }

        location = m.value.GetString();
    }

    response.clear();
    curl_easy_reset(curl.get());
    curl_easy_setopt(curl.get(), CURLOPT_URL, location.c_str());
    curl_easy_setopt(curl.get(), CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &response);

    res = curl_easy_perform(curl.get());
    if(CURLE_OK != res)
    {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        exit(1);
    }
    ////////////////////////

    //////////////// Append to tmp
    //for (int i = 0; i < 1000; i++)
    //{
    //    set_url(&url, "APPEND", "tmp");
    //    response.clear();
    //    curl_easy_reset(curl.get());
    //    curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());
    //    curl_easy_setopt(curl.get(), CURLOPT_CUSTOMREQUEST, "POST");
    //    curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, write_callback);
    //    curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &response);

    //    res = curl_easy_perform(curl.get());
    //    if(CURLE_OK != res)
    //    {
    //        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    //        exit(1);
    //    }

    //    doc = get_json(response.c_str());
    //    for (const auto& m : doc.GetObject())
    //    {
    //        if (m.name.GetString() == std::string("RemoteException"))
    //        {
    //            std::cerr << m.value["exception"].GetString() << std::endl;
    //            exit(1);
    //        }

    //        location = m.value.GetString();
    //    }

    //    response.clear();
    //    curl_easy_reset(curl.get());
    //    curl_easy_setopt(curl.get(), CURLOPT_URL, location.c_str());
    //    curl_easy_setopt(curl.get(), CURLOPT_CUSTOMREQUEST, "POST");
    //    curl_easy_setopt(curl.get(), CURLOPT_POSTFIELDS, "hello, webHDFS!\n");
    //    curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, write_callback);
    //    curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &response);

    //    res = curl_easy_perform(curl.get());
    //    if(CURLE_OK != res)
    //    {
    //        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    //        exit(1);
    //    }
    //    std::cout << i << std::endl;
    //}
    //////////////////////

    while (true)
    {
        receiver.recv_msg();
        std::unique_ptr<struct MsgBuf> msg = receiver.get_msg();
        //std::cout << "Type: " << msg->type << std::endl;
        //std::cout << "Message: " << (msg->payload).data << std::endl;
        std::string tmp = std::string((msg->payload).data) + "\n";
        set_url(&url, "APPEND", "tmp");
        response.clear();
        curl_easy_reset(curl.get());
        curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl.get(), CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl.get());
        if(CURLE_OK != res)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            exit(1);
        }

        doc = get_json(response.c_str());
        for (const auto& m : doc->GetObject())
        {
            if (m.name.GetString() == std::string("RemoteException"))
            {
                std::cerr << m.value["exception"].GetString() << std::endl;
                exit(1);
            }

            location = m.value.GetString();
        }

        response.clear();
        curl_easy_reset(curl.get());
        curl_easy_setopt(curl.get(), CURLOPT_URL, location.c_str());
        curl_easy_setopt(curl.get(), CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl.get(), CURLOPT_POSTFIELDS, tmp.c_str());
        curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl.get());
        if(CURLE_OK != res)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            exit(1);
        }
        std::cout << "ok." << std::endl;
    }

    return 0;
}

