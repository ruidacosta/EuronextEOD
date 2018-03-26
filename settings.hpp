#pragma once
struct Settings 
{
    std::string input_folder;
    std::string output_folder;
    std::string database_type;
    std::string database_file;
    std::string user;
    std::string password;
    bool multiThread;
    bool archive;
    std::string logs_folder;
};