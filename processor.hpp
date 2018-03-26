#ifndef processor_h
#define processor_h

#include <string>
#include <vector>
#include <sqlite3.h>

#include "settings.hpp"

enum FileType {
    EquitiesFile,
    BondsFile,
    ETPsFile,
    IndicesFile,
    StructProdFile
};

class CSVRow;

class Processor
{
    public:
        Processor(Settings *setts);
        ~Processor();
        bool start(void);
    private:
        Settings *m_setts;
        std::vector<std::string> getFilesFromDir(std::string path);
        void processFile(std::string filename);
        void getFilenameInfo(std::string *filename, std::string *date, FileType *type);
        void prepareDatabaseFile(sqlite3 **db);
        void saveRow(const CSVRow *row, FileType type, sqlite3 *db);
        void archiveInputFiles(std::string filename);
};

#endif