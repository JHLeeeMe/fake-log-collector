#include <iostream>
#include <cstring>

#include <hdfs.h>

int main()
{
    hdfsFS fs = hdfsConnect("master", 9000);

    std::string out_path = "/tmp/testfile.txt";
    hdfsFile out_file = hdfsOpenFile(fs, out_path.c_str(), O_WRONLY |O_CREAT, 0, 0, 0);
    if(!out_file)
    {
        std::cerr << "Failed to open " << out_path << " for writing!" << std::endl;
        exit(1);
    }

    std::string buf = "Hello, World!";
    tSize num_written_bytes = hdfsWrite(fs, out_file, buf.c_str(), buf.length() + 1);
    if (hdfsFlush(fs, out_file))
    {
        std::cerr << "Failed to 'flush' " << out_path << std::endl;
        exit(-1);
    }

    hdfsCloseFile(fs, out_file);

    return 0;
}
