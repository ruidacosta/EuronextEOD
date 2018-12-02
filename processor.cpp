#include <dirent.h>
#include <thread>
#include <regex>
#include <exception>
#include <fstream>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <experimental/filesystem>

#include "spdlog/spdlog.h"
#include "csvUtils.hpp"
#include "processor.hpp"
#include "instrument.hpp"
#include "config_manager.hpp"
#include "sqlQueries.hpp"
#include "utils.hpp"

Processor::Processor(Settings *setts)
{
    this->m_setts = setts;
}

Processor::~Processor()
{
    
}

bool Processor::start(void)
{
    // Get all files on input_folder
    std::vector<std::string> files = getFilesFromDir(this->m_setts->input_folder);
    // Create a Thread for each file
    
    if (this->m_setts->multiThread)
        spdlog::get("logger")->info("Multi-thread enable");
    else
        spdlog::get("logger")->info("Multi-thread disable");
        
    std::vector<std::thread*> threadsList;
    for (auto file : files)
    {
        spdlog::get("logger")->info("Processing {}",file);
        if (this->m_setts->multiThread)
        {
            std::thread *worker = new std::thread(&Processor::processFile, this, file); 
            threadsList.push_back(worker);
        }
        else
        {
            processFile(file);
        }
    }
    
    if (this->m_setts->multiThread)
    {
        for (auto thread : threadsList)
        {
            thread->join();
        }
        threadsList.erase(threadsList.begin(),threadsList.end());
    }
    // wait for all threads
    return true;
}

std::vector<std::string> Processor::getFilesFromDir(std::string path)
{
    std::vector<std::string> result;

    if (path.back() != '/')
        path += "/";

    std::regex regExCsv("[a-zA-Z_0-9]+\\.csv");
    std::string ext(".csv");
    for (auto &p : std::experimental::filesystem::recursive_directory_iterator(path))
    {
        if (p.path().extension() == ext)
            result.push_back(p.path());        
    }
    
    // std::smatch match;
    
    // try catch on regex using regex alternative
    // if (auto dir = opendir(path.c_str()))
    // {
    //     while (auto f = readdir(dir))
    //     {
    //         if ((!f->d_name) ||
    //             (f->d_name[0] == '.') ||
    //             (!std::regex_search(std::string(f->d_name),match,regExCsv)))
    //             continue;
    //         result.push_back(path + f->d_name);
    //     }
    // }
    // else
    // {
    //     // cannot open directory
    //     spdlog::get("logger")->error("Cannot open directory {}", path);
    // }
    return result;
}

void Processor::processFile(std::string filename)
{
    sqlite3 *db;
    std::string date;
    FileType type;
    getFilenameInfo(&filename, &date, &type);
    
    std::ifstream file(filename);
    
    // Check file is open
    if (!file.is_open())
    {
        spdlog::get("logger")->error("Cannot open file {}", filename);
        return;
    }
    
    // Check database connection
    prepareDatabaseFile(&db);
    
    int count = 0;
    CSVIterator loop(file);
    loop++; loop++; loop++; loop++;//remove 4 first lines
    for (; loop != CSVIterator(); ++loop)
    {
        saveRow(&(*loop), type, db);
        count++;
    }
    
    sqlite3_close(db);
    spdlog::get("logger")->info("{} lines readed", count);
    
    if (this->m_setts->archive)
        archiveInputFiles(filename);
}

void Processor::getFilenameInfo(std::string *filename, std::string *date, FileType *type)
{
    // Get date and file content type from filename
    try
    {
        std::regex regExType("_([A-Z][a-zA-Z]+_?[A-Z]?[a-z]+)_");
        std::smatch matchType;
        if (std::regex_search(*filename, matchType, regExType))
        {
            std::string typeStr = matchType.str();
            if (typeStr == "_Indices_") *type = FileType::IndicesFile;
            if (typeStr == "_Bonds_") *type = FileType::BondsFile;
            if (typeStr == "_Equities_") *type = FileType::EquitiesFile;
            if (typeStr == "_ETPs_") *type = FileType::ETPsFile;
            if (typeStr == "_Structured_Products_") *type = FileType::StructProdFile;
        }
    
        std::regex regExDate("\\d{4}-\\d{2}-\\d{2}");
        std::smatch matchDate;
        if (std::regex_search(*filename, matchDate, regExDate))
        {
            *date = matchDate.str();
        }
    }
    catch (const std::regex_error& ex)
    {
        spdlog::get("logger")->error("->{} - {}",ex.what(),ex.code());
    }
}

void Processor::prepareDatabaseFile(sqlite3 **db)
{
    char *zErrMsg = 0;
    std::string sql = createEquitiesSql + createBondsSql + createIndicesSql 
        + createETFSSql + createStructProdsSql;
    
    if (sqlite3_open(this->m_setts->database_file.c_str(), db))
    {
        spdlog::get("logger")->error("Can't open database {}: {}", this->m_setts->database_file, sqlite3_errmsg(*db));
        return;
    }
    
    if (sqlite3_exec(*db, sql.c_str(), nullptr, 0, &zErrMsg) != SQLITE_OK)
    {
        spdlog::get("logger")->error("Can't execute sql query: {}", zErrMsg);
        spdlog::get("logger")->error("Sql Query: {}", sql);
        return;
    }
}

void Processor::saveRow(const CSVRow *row, FileType type, sqlite3 *db)
{
    Instrument *inst;
    switch(type)
    {
        case FileType::EquitiesFile:
            inst = new Equity(row,db);
            break;
        case FileType::IndicesFile:
            inst = new Indice(row,db);
            break;
        case FileType::BondsFile:
            inst = new Bond(row,db);
            break;
        case FileType::ETPsFile:
            inst = new ETF(row,db);
            break;
        case FileType::StructProdFile:
            inst = new StrcPrd(row,db);
            break;
    }
    
    inst->saveDb();
    
    delete inst;
}

void Processor::archiveInputFiles(std::string file)
{
    if (!isDir(this->m_setts->output_folder))
    {
        if (!mkdir(this->m_setts->output_folder))
        {
            spdlog::get("logger")->error("Can't create directory {}", this->m_setts->output_folder);
            return;
        }
        spdlog::get("logger")->info("Created directory {}", this->m_setts->output_folder);
    }
    
    std::string baseFilename = file.substr(file.find_last_of(pathSeparator()) + 1);
    std::string archiveFile = this->m_setts->output_folder + pathSeparator() + baseFilename;
    
    if (std::rename(file.c_str(), archiveFile.c_str()))
        spdlog::get("logger")->error("Can't move file from {} to {}", file, archiveFile);
}
