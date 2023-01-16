#include "hdfs_writer.h"

HDFSWriter::HDFSWriter()
    : _curl(curl_easy_init(), &curl_easy_cleanup)
{
    if (!_curl)
    {
        std::cerr << "curl_easy_init() failed..." << std::endl;
        exit(1);
    }
}

HDFSWriter::~HDFSWriter()
{}

void HDFSWriter::set_url()
{
    url->clear();
    url->append(
        "http://" + addr + ":" + port +
        "/webhdfs/v1/" + path +
        "?op=" + op +
        "&noredirect=true"
    );
}
