#ifndef instrument_h
#define instrument_h
#include <sqlite3.h>
#include <string>
#include "csvUtils.hpp"

class Instrument
{
    public:
        virtual void loadData(const CSVRow *row) =0;
        virtual void saveDb() =0;
        virtual ~Instrument(){};
};

enum EquitiesFields {
    Eq_Name = 0,
    Eq_ISIN,
    Eq_Symbol,
    Eq_Market,
    Eq_TradingCurrency,
    Eq_Open,
    Eq_High,
    Eq_Low,
    Eq_Last,
    Eq_LastDateTime,
    Eq_TimeZone,
    Eq_Volume,
    Eq_Turnover
};

class Equity : public Instrument
{
    public:
        Equity(const CSVRow *row, sqlite3 *db);
        ~Equity(){ sqlite3_finalize(stmt); };
        void loadData(const CSVRow * row);
        void saveDb();
        std::string Name;
        std::string ISIN;
        std::string Symbol;
        std::string Market;
        std::string TradingCurrency;
        std::string Open;
        std::string High;
        std::string Low;
        std::string Last;
        std::string LastDateTime;
        std::string TimeZone;
        std::string Volume;
        std::string Turnover;
    private:
        sqlite3_stmt *stmt;
        sqlite3 *_db;
};

enum BondsFields {
    Bd_Name = 0,
    Bd_ISIN,
    Bd_Symbol,
    Bd_Location,
    Bd_Maturity,
    Bd_TradingCurrency,
    Bd_Open,
    Bd_High,
    Bd_Low,
    Bd_Last,
    Bd_LastDateTime,
    Bd_TimeZone,
    Bd_Volume,
    Bd_Turnover    
};

class Bond : public Instrument
{
    public:
        Bond(const CSVRow *row, sqlite3 *db);
        ~Bond(){ sqlite3_finalize(stmt); };
        void loadData(const CSVRow * row);
        void saveDb();
        std::string Name;
        std::string ISIN;
        std::string Symbol;
        std::string Location;
        std::string Maturity;
        std::string TradingCurrency;
        std::string Open;
        std::string High;
        std::string Low;
        std::string Last;
        std::string LastDateTime;
        std::string TimeZone;
        std::string Volume;
        std::string Turnover;
    private:
        sqlite3_stmt *stmt;
        sqlite3 *_db;
};

enum IndicesFields {
    Ind_Name = 0,
    Ind_ISIN,
    Ind_Symbol,
    Ind_TradingCurrency,
    Ind_Open,
    Ind_High,
    Ind_Low,
    Ind_Last,
    Ind_LastDateTime,
    Ind_TimeZone
};

class Indice : public Instrument
{
    public:
        Indice(const CSVRow *row, sqlite3 *db);
        ~Indice(){ sqlite3_finalize(stmt); };
        void loadData(const CSVRow * row);
        void saveDb();
        std::string Name;
        std::string ISIN;
        std::string Symbol;
        std::string TradingCurrency;
        std::string Open;
        std::string High;
        std::string Low;
        std::string Last;
        std::string LastDateTime;
        std::string TimeZone;
    private:
        sqlite3_stmt *stmt;
        sqlite3 *_db;
};

enum ETPsFields {
    ETP_Name = 0,
    ETP_ISIN,
    ETP_Symbol,
    ETP_Location,
    ETP_TradingCurrency,
    ETP_Open,
    ETP_High,
    ETP_Low,
    ETP_Last,
    ETP_LastDateTime,
    ETP_TimeZone,
    ETP_Volume,
    ETP_Turnover
};

class ETF : public Instrument
{
    public:
        ETF(const CSVRow *row, sqlite3 *db);
        ~ETF(){ sqlite3_finalize(stmt); }
        void loadData(const CSVRow * row);
        void saveDb();
        std::string Name;
        std::string ISIN;
        std::string Symbol;
        std::string Location;
        std::string TradingCurrency;
        std::string Open;
        std::string High;
        std::string Low;
        std::string Last;
        std::string LastDateTime;
        std::string TimeZone;
        std::string Volume;
        std::string Turnover;
    private:
        sqlite3_stmt *stmt;
        sqlite3 *_db;
};

enum StructProdFields {
    SP_Name = 0,
    SP_ISIN,
    SP_Symbol,
    SP_Location,
    SP_CP,
    SP_Strike,
    SP_Maturity,
    SP_TradingCurrency,
    SP_Open,
    SP_High,
    SP_Low,
    SP_Last,
    SP_LastDateTime,
    SP_TimeZone,
    SP_Volume,
    SP_Turnover
};

class StrcPrd : public Instrument
{
    public:
        StrcPrd(const CSVRow *row, sqlite3 *db);
        ~StrcPrd() { sqlite3_finalize(stmt); };
        void loadData(const CSVRow * row);
        void saveDb();
        std::string Name;
        std::string ISIN;
        std::string Symbol;
        std::string Location;
        std::string C_P;
        std::string Strike;
        std::string Maturity;
        std::string TradingCurrency;
        std::string Open;
        std::string High;
        std::string Low;
        std::string Last;
        std::string LastDateTime;
        std::string TimeZone;
        std::string Volume;
        std::string Turnover;
    private:
        sqlite3_stmt *stmt;
        sqlite3 *_db;
};

#endif