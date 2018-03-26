#include <iostream>
#include <chrono>
#include <memory>

#include "argparsercpp.hpp"
#include "config_manager.hpp"
#include "settings.hpp"
#include "processor.hpp"
#include "spdlog/spdlog.h"
#include "utils.hpp"

void readconfigFile(std::string configFile, Settings *setts)
{
    if (ConfigManager::RecordExists("input","Main",configFile))
        setts->input_folder = ConfigManager::GetValue("input","Main",configFile);
        
    if (ConfigManager::RecordExists("output", "Main", configFile))
        setts->output_folder = ConfigManager::GetValue("output","Main",configFile);
    
    if (ConfigManager::RecordExists("database_type","Main",configFile))
        setts->database_type = ConfigManager::GetValue("database_type","Main",configFile);
    
    if ((ConfigManager::RecordExists("database_file","Main",configFile)))
        setts->database_file = ConfigManager::GetValue("database_file","Main",configFile);
    
    if (ConfigManager::RecordExists("database_user","Main",configFile))
        setts->user = ConfigManager::GetValue("database_user","Main",configFile);
        
    if (ConfigManager::RecordExists("database_password","Main",configFile))
        setts->password = ConfigManager::GetValue("database_password","Main",configFile);
        
    if (ConfigManager::RecordExists("multiThread","Main",configFile))
        setts->multiThread = ConfigManager::GetValue("multiThread","Main",configFile) == "True" ? true : false;
    
    if (ConfigManager::RecordExists("archive","Main",configFile))
        setts->archive = ConfigManager::GetValue("archive","Main",configFile) == "True" ? true : false;
    
    if (ConfigManager::RecordExists("folder", "Logs",configFile))
        setts->logs_folder = ConfigManager::GetValue("folder", "Logs", configFile);
}

void createLoggerSink(std::string logFolder, std::string appName)
{
    int flag = 1;
    std::string msg;
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
    
    if (!isDir(logFolder))
    {
        if (!mkdir(logFolder))
        {
            msg = "Can't create folder " +  logFolder;
            flag = 0;
        }
    }
    if (flag)
    {
        std::string logFile = logFolder + pathSeparator() + appName + ".log";
        sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>(logFile,0,0));
    }
    
    auto console_multisink = std::make_shared<spdlog::async_logger>("logger",begin(sinks),end(sinks),65536,
        spdlog::async_overflow_policy::block_retry, nullptr, std::chrono::seconds(2));
    spdlog::register_logger(console_multisink);
    
    if (!flag)
        console_multisink->error(msg);
}

int main(int argc, const char *argv[]) {
    auto start = std::chrono::system_clock::now();
    //auto console = spdlog::stdout_color_mt("console");
    // Parse arguments
    Settings settings;
    ArgParser parser("This stores information on Euronext end of day report files into database(s).", 
    "@author: ruidacosta <ruiduartecosta@gmail.com>\n@github: https://github.com/ruidacosta");
    
    parser.addArgument("-i", "--input_folder", "input folder where files are", 1);
    parser.addArgument("-o", "--output_folder", "output folder where files will be", 1);
    parser.addArgument("-d", "--database", "database type [sqlite||mysql||file])", 1);
    parser.addArgument("-a", "--database_file", "database file (if database type is sqlite)");
    parser.addArgument("-u", "--user", "database username", 1);
    parser.addArgument("-p", "--password", "database password", 1);
    parser.addArgument("-l", "--log_folder", "folder where logs are stored");
    parser.addFinalArgument("config", "Configuration file", 1, false);
    
    parser.parse(argc, argv);
    
    //spdlog::get("logger")->info("Reading configuration file...");
    std::string configFile;
    if (parser.retrieve("config", configFile))
        readconfigFile(configFile, &settings);
    //spdlog::get("logger")->info("Configuration file readed");
    
    std::string buffer;
    if (parser.retrieve("input_folder", buffer))
        settings.input_folder = buffer;
    
    if (parser.retrieve("output_folder", buffer))
        settings.output_folder = buffer;
        
    if (parser.retrieve("database",buffer))
    {
        settings.database_type = buffer;
        if (buffer == "sqlite")
        {
            if (parser.retrieve("database_file",buffer))
                settings.database_file = buffer;
        }
    }
    
    if (parser.retrieve("user", buffer))
        settings.user = buffer;
    
    if (parser.retrieve("password", buffer))
        settings.password = buffer;
        
    if (parser.retrieve("log_folder", buffer))
        settings.logs_folder = buffer;
    // Start batch
    
    createLoggerSink(settings.logs_folder, std::string(argv[0]));
    
    spdlog::get("logger")->info("Process start");
    
    spdlog::get("logger")->info("Starting processor...");
    Processor p = Processor(&settings);
    p.start();
    spdlog::get("logger")->info("Processor finish");
    
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedSeconds = end - start;
    spdlog::get("logger")->info("Elapsed time: {} seconds",elapsedSeconds.count());
    spdlog::get("logger")->info("Terminate process");
    spdlog::get("logger")->info("Bye!!!");
    
    spdlog::drop_all();
    
    return 0;
}
