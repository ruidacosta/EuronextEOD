#include "instrument.hpp"
#include "spdlog/spdlog.h"
#include "sqlQueries.hpp"

/*
 * Class Equity
 */
Equity::Equity(const CSVRow *row, sqlite3 *db)
{
    this->_db = db;
    this->loadData(row);
    std::string query = "INSERT INTO EQUITIES (NAME,ISIN,SYMBOL,MARKET,TRADING_CURRENCY,OPEN,HIGH,LOW,LAST,LAST_DATETIME,TIMEZONE,VOLUME,TURNOVER) " \
    "VALUES (?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, ?9, ?10, ?11, ?12, ?13);";
    sqlite3_prepare_v2(this->_db, query.c_str(), -1, &this->stmt, NULL);
}

void Equity::loadData(const CSVRow *row)
{
    this->Name = (std::string) (*row)[EquitiesFields::Eq_Name];
    this->ISIN = (std::string) (*row)[EquitiesFields::Eq_ISIN];
    this->Symbol = (std::string) (*row)[EquitiesFields::Eq_Symbol];
    this->Market = (std::string) (*row)[EquitiesFields::Eq_Market];
    this->TradingCurrency = (std::string) (*row)[EquitiesFields::Eq_TradingCurrency];
    this->Open = (std::string) (*row)[EquitiesFields::Eq_Open];
    this->High = (std::string) (*row)[EquitiesFields::Eq_High];
    this->Low = (std::string) (*row)[EquitiesFields::Eq_Low];
    this->Last = (std::string) (*row)[EquitiesFields::Eq_Last];
    this->LastDateTime = (std::string) (*row)[EquitiesFields::Eq_LastDateTime];
    this->TimeZone = (std::string) (*row)[EquitiesFields::Eq_TimeZone];
    this->Volume = (std::string) (*row)[EquitiesFields::Eq_Volume];
    this->Turnover = (std::string) (*row)[EquitiesFields::Eq_Turnover];
}

void Equity::saveDb()
{
    this->Name.empty() ?
    sqlite3_bind_null(this->stmt, 1) :
    sqlite3_bind_text(this->stmt, 1, this->Name.c_str(), -1, SQLITE_STATIC);
    
    this->ISIN.empty() ?
    sqlite3_bind_null(this->stmt, 2) :
    sqlite3_bind_text(this->stmt, 2, this->ISIN.c_str(), -1, SQLITE_STATIC);
    
    this->Symbol.empty() ?
    sqlite3_bind_null(this->stmt, 3) :
    sqlite3_bind_text(this->stmt, 3, this->Symbol.c_str(), -1, SQLITE_STATIC);
    
    this->Market.empty() ?
    sqlite3_bind_null(this->stmt, 4) :
    sqlite3_bind_text(this->stmt, 4, this->Market.c_str(), -1, SQLITE_STATIC);
    
    this->TradingCurrency.empty() ?
    sqlite3_bind_null(this->stmt, 5) :
    sqlite3_bind_text(this->stmt, 5, this->TradingCurrency.c_str(), -1, SQLITE_STATIC);
    
    this->Open.empty() ?
    sqlite3_bind_null(this->stmt, 6) :
    sqlite3_bind_text(this->stmt, 6, this->Open.c_str(), -1, SQLITE_STATIC);
    
    this->High.empty() ?
    sqlite3_bind_null(this->stmt, 7) :
    sqlite3_bind_text(this->stmt, 7, this->High.c_str(), -1, SQLITE_STATIC);
    
    this->Low.empty() ?
    sqlite3_bind_null(this->stmt, 8) :
    sqlite3_bind_text(this->stmt, 8, this->Low.c_str(), -1, SQLITE_STATIC);
    
    this->Last.empty() ?
    sqlite3_bind_null(this->stmt, 9) :
    sqlite3_bind_text(this->stmt, 9, this->Last.c_str(), -1, SQLITE_STATIC);
    
    this->LastDateTime.empty() ?
    sqlite3_bind_null(this->stmt, 10) :
    sqlite3_bind_text(this->stmt, 10, this->LastDateTime.c_str(), -1, SQLITE_STATIC);
    
    this->TimeZone.empty() ?
    sqlite3_bind_null(this->stmt, 11) :
    sqlite3_bind_text(this->stmt, 11, this->TimeZone.c_str(), -1, SQLITE_STATIC);
    
    this->Volume.empty() ?
    sqlite3_bind_null(this->stmt, 12) :
    sqlite3_bind_text(this->stmt, 12, this->Volume.c_str(), -1, SQLITE_STATIC);
    
    this->Turnover.empty() ?
    sqlite3_bind_null(this->stmt, 13) :
    sqlite3_bind_text(this->stmt, 13, this->Turnover.c_str(), -1, SQLITE_STATIC);
    
    if (sqlite3_step(this->stmt) != SQLITE_DONE)
    {
        spdlog::get("logger")->error("Query: {}", sqlite3_sql(this->stmt));
        spdlog::get("logger")->error("ERROR inserting data: {}", sqlite3_errmsg(this->_db));
    }
        
    sqlite3_reset(this->stmt);
}

/*
 * Class Bond
 */
Bond::Bond(const CSVRow *row, sqlite3 *db)
{
    this->_db = db;
    this->loadData(row);
    std::string query = "INSERT INTO BONDS (NAME,ISIN,SYMBOL,LOCATION,MATURITY,TRADING_CURRENCY,OPEN,HIGH,LOW,LAST,LAST_DATETIME,TIMEZONE,VOLUME,TURNOVER) " \
    "VALUES (?1,?2,?3,?4,?5,?6,?7,?8,?9,?10,?11,?12,?13,?14);";
    sqlite3_prepare_v2(this->_db, query.c_str(), -1, &this->stmt, NULL);
}

void Bond::loadData(const CSVRow *row)
{
    this->Name = (std::string) (*row)[BondsFields::Bd_Name];
    this->ISIN = (std::string) (*row)[BondsFields::Bd_ISIN];
    this->Symbol = (std::string) (*row)[BondsFields::Bd_Symbol];
    this->Location = (std::string) (*row)[BondsFields::Bd_Location];
    this->Maturity = (std::string) (*row)[BondsFields::Bd_Maturity];
    this->TradingCurrency = (std::string) (*row)[BondsFields::Bd_TradingCurrency];
    this->Open = (std::string) (*row)[BondsFields::Bd_Open];
    this->High = (std::string) (*row)[BondsFields::Bd_High];
    this->Low = (std::string) (*row)[BondsFields::Bd_Low];
    this->Last = (std::string) (*row)[BondsFields::Bd_Last];
    this->LastDateTime = (std::string) (*row)[BondsFields::Bd_LastDateTime];
    this->TimeZone = (std::string) (*row)[BondsFields::Bd_TimeZone];
    this->Volume = (std::string) (*row)[BondsFields::Bd_Volume];
    this->Turnover = (std::string) (*row)[BondsFields::Bd_Turnover];
}

void Bond::saveDb()
{
    this->Name.empty() ?
    sqlite3_bind_null(this->stmt, 1) :
    sqlite3_bind_text(this->stmt, 1, this->Name.c_str(), -1, SQLITE_STATIC);
    
    this->ISIN.empty() ?
    sqlite3_bind_null(this->stmt, 2) :
    sqlite3_bind_text(this->stmt, 2, this->ISIN.c_str(), -1, SQLITE_STATIC);
    
    this->Symbol.empty() ?
    sqlite3_bind_null(this->stmt, 3) :
    sqlite3_bind_text(this->stmt, 3, this->Symbol.c_str(), -1, SQLITE_STATIC);
    
    this->Location.empty() ?
    sqlite3_bind_null(this->stmt, 4) :
    sqlite3_bind_text(this->stmt, 4, this->Location.c_str(), -1, SQLITE_STATIC);
    
    this->Maturity.empty() ?
    sqlite3_bind_null(this->stmt, 5) :
    sqlite3_bind_text(this->stmt, 5, this->Maturity.c_str(), -1, SQLITE_STATIC);
    
    this->TradingCurrency.empty() ?
    sqlite3_bind_null(this->stmt, 6) :
    sqlite3_bind_text(this->stmt, 6, this->TradingCurrency.c_str(), -1, SQLITE_STATIC);
    
    this->Open.empty() ?
    sqlite3_bind_null(this->stmt, 7) :
    sqlite3_bind_text(this->stmt, 7, this->Open.c_str(), -1, SQLITE_STATIC);
    
    this->High.empty() ?
    sqlite3_bind_null(this->stmt, 8) :
    sqlite3_bind_text(this->stmt, 8, this->High.c_str(), -1, SQLITE_STATIC);
    
    this->Low.empty() ?
    sqlite3_bind_null(this->stmt, 9) :
    sqlite3_bind_text(this->stmt, 9, this->Low.c_str(), -1, SQLITE_STATIC);
    
    this->Last.empty() ?
    sqlite3_bind_null(this->stmt, 10) :
    sqlite3_bind_text(this->stmt, 10, this->Last.c_str(), -1, SQLITE_STATIC);
    
    this->LastDateTime.empty() ?
    sqlite3_bind_null(this->stmt, 11) :
    sqlite3_bind_text(this->stmt, 11, this->LastDateTime.c_str(), -1, SQLITE_STATIC);
    
    this->TimeZone.empty() ?
    sqlite3_bind_null(this->stmt, 12) :
    sqlite3_bind_text(this->stmt, 12, this->TimeZone.c_str(), -1, SQLITE_STATIC);
    
    this->Volume.empty() ?
    sqlite3_bind_null(this->stmt, 13) :
    sqlite3_bind_text(this->stmt, 13, this->Volume.c_str(), -1, SQLITE_STATIC);
    
    this->Turnover.empty() ?
    sqlite3_bind_null(this->stmt, 14) :
    sqlite3_bind_text(this->stmt, 14, this->Turnover.c_str(), -1, SQLITE_STATIC);
    
    if (sqlite3_step(this->stmt) != SQLITE_DONE)
    {
        spdlog::get("logger")->error("Query: {}", sqlite3_sql(this->stmt));
        spdlog::get("logger")->error("ERROR inserting data: {}", sqlite3_errmsg(this->_db));
    }
    
    sqlite3_reset(this->stmt);
}

/*
 * Class Indice
 */
Indice::Indice(const CSVRow *row, sqlite3 *db)
{
    this->_db = db;
    this->loadData(row);
    std::string query = "INSERT INTO INDICES (NAME,ISIN,SYMBOL,TRADING_CURRENCY,OPEN,HIGH,LOW,LAST,LAST_DATETIME,TIMEZONE) " \
    "VALUES (?1,?2,?3,?4,?5,?6,?7,?8,?9,?10);";
    sqlite3_prepare_v2(this->_db, query.c_str(), -1, &this->stmt, NULL);
}

void Indice::loadData(const CSVRow *row)
{
    this->Name = (std::string) (*row)[IndicesFields::Ind_Name];
    this->ISIN = (std::string) (*row)[IndicesFields::Ind_ISIN];
    this->Symbol = (std::string) (*row)[IndicesFields::Ind_Symbol];
    this->TradingCurrency = (std::string) (*row)[IndicesFields::Ind_TradingCurrency];
    this->Open = (std::string) (*row)[IndicesFields::Ind_Open];
    this->High = (std::string) (*row)[IndicesFields::Ind_High];
    this->Low = (std::string) (*row)[IndicesFields::Ind_Low];
    this->Last = (std::string) (*row)[IndicesFields::Ind_Last];
    this->LastDateTime = (std::string) (*row)[IndicesFields::Ind_LastDateTime];
    this->TimeZone = (std::string) (*row)[IndicesFields::Ind_TimeZone];
}

void Indice::saveDb()
{
    this->Name.empty() ?
    sqlite3_bind_null(this->stmt, 1) :
    sqlite3_bind_text(this->stmt, 1, this->Name.c_str(), -1, SQLITE_STATIC);
    
    this->ISIN.empty() ?
    sqlite3_bind_null(this->stmt, 2) :
    sqlite3_bind_text(this->stmt, 2, this->ISIN.c_str(), -1, SQLITE_STATIC);
    
    this->Symbol.empty() ?
    sqlite3_bind_null(this->stmt, 3) :
    sqlite3_bind_text(this->stmt, 3, this->Symbol.c_str(), -1, SQLITE_STATIC);
    
    this->TradingCurrency.empty() ?
    sqlite3_bind_null(this->stmt, 4) :
    sqlite3_bind_text(this->stmt, 4, this->TradingCurrency.c_str(), -1, SQLITE_STATIC);
    
    this->Open.empty() ?
    sqlite3_bind_null(this->stmt, 5) :
    sqlite3_bind_text(this->stmt, 5, this->Open.c_str(), -1, SQLITE_STATIC);
    
    this->High.empty() ?
    sqlite3_bind_null(this->stmt, 6) :
    sqlite3_bind_text(this->stmt, 6, this->High.c_str(), -1, SQLITE_STATIC);
    
    this->Low.empty() ?
    sqlite3_bind_null(this->stmt, 7) :
    sqlite3_bind_text(this->stmt, 7, this->Low.c_str(), -1, SQLITE_STATIC);
    
    this->Last.empty() ?
    sqlite3_bind_null(this->stmt, 8) :
    sqlite3_bind_text(this->stmt, 8, this->Last.c_str(), -1, SQLITE_STATIC);
    
    this->LastDateTime.empty() ?
    sqlite3_bind_null(this->stmt, 9) :
    sqlite3_bind_text(this->stmt, 9, this->LastDateTime.c_str(), -1, SQLITE_STATIC);
    
    this->TimeZone.empty() ?
    sqlite3_bind_null(this->stmt, 10) :
    sqlite3_bind_text(this->stmt, 10, this->TimeZone.c_str(), -1, SQLITE_STATIC);
    
    if (sqlite3_step(this->stmt) != SQLITE_DONE)
    {
        spdlog::get("logger")->error("Query: {}", sqlite3_sql(this->stmt));
        spdlog::get("logger")->error("ERROR inserting data: {}", sqlite3_errmsg(this->_db));
    }
    
    sqlite3_reset(this->stmt);
}

/*
 * Class ETF
 */
ETF::ETF(const CSVRow *row, sqlite3 *db)
{
    this->_db = db;
    this->loadData(row);
    std::string query = "INSERT INTO ETFS (NAME,ISIN,SYMBOL,LOCATION,TRADING_CURRENCY,OPEN,HIGH,LOW,LAST,LAST_DATETIME,TIMEZONE,VOLUME,TURNOVER) " \
    "VALUES (?1,?2,?3,?4,?5,?6,?7,?8,?9,?10,?11,?12,?13);";
    sqlite3_prepare_v2(this->_db, query.c_str(), -1, &this->stmt, NULL);
}

void ETF::loadData(const CSVRow *row)
{
    this->Name = (std::string) (*row)[ETPsFields::ETP_Name];
    this->ISIN = (std::string) (*row)[ETPsFields::ETP_ISIN];
    this->Symbol = (std::string) (*row)[ETPsFields::ETP_Symbol];
    this->Location = (std::string) (*row)[ETPsFields::ETP_Location];
    this->TradingCurrency = (std::string) (*row)[ETPsFields::ETP_TradingCurrency];
    this->Open = (std::string) (*row)[ETPsFields::ETP_Open];
    this->High = (std::string) (*row)[ETPsFields::ETP_High];
    this->Low = (std::string) (*row)[ETPsFields::ETP_Low];
    this->Last = (std::string) (*row)[ETPsFields::ETP_Last];
    this->LastDateTime = (std::string) (*row)[ETPsFields::ETP_LastDateTime];
    this->TimeZone = (std::string) (*row)[ETPsFields::ETP_TimeZone];
    this->Volume = (std::string) (*row)[ETPsFields::ETP_Volume];
    this->Turnover = (std::string) (*row)[ETPsFields::ETP_Turnover];
}

void ETF::saveDb()
{
    this->Name.empty() ?
    sqlite3_bind_null(this->stmt, 1) :
    sqlite3_bind_text(this->stmt, 1, this->Name.c_str(), -1, SQLITE_STATIC);
    
    this->ISIN.empty() ?
    sqlite3_bind_null(this->stmt, 2) :
    sqlite3_bind_text(this->stmt, 2, this->ISIN.c_str(), -1, SQLITE_STATIC);
    
    this->Symbol.empty() ?
    sqlite3_bind_null(this->stmt, 3) :
    sqlite3_bind_text(this->stmt, 3, this->Symbol.c_str(), -1, SQLITE_STATIC);
    
    this->Location.empty() ?
    sqlite3_bind_null(this->stmt, 4) :
    sqlite3_bind_text(this->stmt, 4, this->Location.c_str(), -1, SQLITE_STATIC);
    
    this->TradingCurrency.empty() ?
    sqlite3_bind_null(this->stmt, 5) :
    sqlite3_bind_text(this->stmt, 5, this->TradingCurrency.c_str(), -1, SQLITE_STATIC);
    
    this->Open.empty() ?
    sqlite3_bind_null(this->stmt, 6) :
    sqlite3_bind_text(this->stmt, 6, this->Open.c_str(), -1, SQLITE_STATIC);
    
    this->High.empty() ?
    sqlite3_bind_null(this->stmt, 7) :
    sqlite3_bind_text(this->stmt, 7, this->High.c_str(), -1, SQLITE_STATIC);
    
    this->Low.empty() ?
    sqlite3_bind_null(this->stmt, 8) :
    sqlite3_bind_text(this->stmt, 8, this->Low.c_str(), -1, SQLITE_STATIC);
    
    this->Last.empty() ?
    sqlite3_bind_null(this->stmt, 9) :
    sqlite3_bind_text(this->stmt, 9, this->Last.c_str(), -1, SQLITE_STATIC);
    
    this->LastDateTime.empty() ?
    sqlite3_bind_null(this->stmt, 10) :
    sqlite3_bind_text(this->stmt, 10, this->LastDateTime.c_str(), -1, SQLITE_STATIC);
    
    this->TimeZone.empty() ?
    sqlite3_bind_null(this->stmt, 11) :
    sqlite3_bind_text(this->stmt, 11, this->TimeZone.c_str(), -1, SQLITE_STATIC);
    
    this->Volume.empty() ?
    sqlite3_bind_null(this->stmt, 12) :
    sqlite3_bind_text(this->stmt, 12, this->Volume.c_str(), -1, SQLITE_STATIC);
    
    this->Turnover.empty() ?
    sqlite3_bind_null(this->stmt, 13) :
    sqlite3_bind_text(this->stmt, 13, this->Turnover.c_str(), -1, SQLITE_STATIC);
    
    if (sqlite3_step(this->stmt) != SQLITE_DONE)
    {
        spdlog::get("logger")->error("Query: {}", sqlite3_sql(this->stmt));
        spdlog::get("logger")->error("ERROR inserting data: {}", sqlite3_errmsg(this->_db));
    }
    
    sqlite3_reset(this->stmt);
}

/*
 * Class StrcPrd
 */
StrcPrd::StrcPrd(const CSVRow *row,sqlite3 *db)
{
    this->_db = db;
    this->loadData(row);
    std::string query = "INSERT INTO STRUCT_PRODS (NAME,ISIN,SYMBOL,LOCATION,C_P,STRIKE,MATURITY,TRADING_CURRENCY,OPEN,HIGH,LOW,LAST,LAST_DATETIME,TIMEZONE,VOLUME,TURNOVER) " \
    "VALUES (?1,?2,?3,?4,?5,?6,?7,?8,?9,?10,?11,?12,?13,?14,?15,?16);";
    sqlite3_prepare_v2(this->_db, query.c_str(), -1, &this->stmt, NULL);
}

void StrcPrd::loadData(const CSVRow *row)
{
    this->Name = (std::string) (*row)[StructProdFields::SP_Name];
    this->ISIN = (std::string) (*row)[StructProdFields::SP_ISIN];
    this->Symbol = (std::string) (*row)[StructProdFields::SP_Symbol];
    this->Location = (std::string) (*row)[StructProdFields::SP_Location];
    this->C_P = (std::string) (*row)[StructProdFields::SP_CP];
    this->Strike = (std::string) (*row)[StructProdFields::SP_Strike];
    this->Maturity = (std::string) (*row)[StructProdFields::SP_Maturity];
    this->TradingCurrency = (std::string) (*row)[StructProdFields::SP_TradingCurrency];
    this->Open = (std::string) (*row)[StructProdFields::SP_Open];
    this->High = (std::string) (*row)[StructProdFields::SP_High];
    this->Low = (std::string) (*row)[StructProdFields::SP_Low];
    this->Last = (std::string) (*row)[StructProdFields::SP_Last];
    this->LastDateTime = (std::string) (*row)[StructProdFields::SP_LastDateTime];
    this->TimeZone = (std::string) (*row)[StructProdFields::SP_TimeZone];
    this->Volume = (std::string) (*row)[StructProdFields::SP_Volume];
    this->Turnover = (std::string) (*row)[StructProdFields::SP_Turnover];
}

void StrcPrd::saveDb()
{
    this->Name.empty() ?
    sqlite3_bind_null(this->stmt, 1) :
    sqlite3_bind_text(this->stmt, 1, this->Name.c_str(), -1, SQLITE_STATIC);
    
    this->ISIN.empty() ?
    sqlite3_bind_null(this->stmt, 2) :
    sqlite3_bind_text(this->stmt, 2, this->ISIN.c_str(), -1, SQLITE_STATIC);
    
    this->Symbol.empty() ?
    sqlite3_bind_null(this->stmt, 3) :
    sqlite3_bind_text(this->stmt, 3, this->Symbol.c_str(), -1, SQLITE_STATIC);
    
    this->Location.empty() ?
    sqlite3_bind_null(this->stmt, 4) :
    sqlite3_bind_text(this->stmt, 4, this->Location.c_str(), -1, SQLITE_STATIC);
    
    this->C_P.empty() ?
    sqlite3_bind_null(this->stmt, 5) :
    sqlite3_bind_text(this->stmt, 5, this->C_P.c_str(), -1, SQLITE_STATIC);
    
    this->Strike.empty() ?
    sqlite3_bind_null(this->stmt, 6) :
    sqlite3_bind_text(this->stmt, 6, this->Strike.c_str(), -1, SQLITE_STATIC);
    
    this->Maturity.empty() ?
    sqlite3_bind_null(this->stmt, 7) :
    sqlite3_bind_text(this->stmt, 7, this->Maturity.c_str(), -1, SQLITE_STATIC);
    
    this->TradingCurrency.empty() ?
    sqlite3_bind_null(this->stmt, 8) :
    sqlite3_bind_text(this->stmt, 8, this->TradingCurrency.c_str(), -1, SQLITE_STATIC);
    
    this->Open.empty() ?
    sqlite3_bind_null(this->stmt, 9) :
    sqlite3_bind_text(this->stmt, 9, this->Open.c_str(), -1, SQLITE_STATIC);
    
    this->High.empty() ?
    sqlite3_bind_null(this->stmt, 10) :
    sqlite3_bind_text(this->stmt, 10, this->High.c_str(), -1, SQLITE_STATIC);
    
    this->Low.empty() ?
    sqlite3_bind_null(this->stmt, 11) :
    sqlite3_bind_text(this->stmt, 11, this->Low.c_str(), -1, SQLITE_STATIC);
    
    this->Last.empty() ?
    sqlite3_bind_null(this->stmt, 12) :
    sqlite3_bind_text(this->stmt, 12, this->Last.c_str(), -1, SQLITE_STATIC);
    
    this->LastDateTime.empty() ?
    sqlite3_bind_null(this->stmt, 13) :
    sqlite3_bind_text(this->stmt, 13, this->LastDateTime.c_str(), -1, SQLITE_STATIC);
    
    this->TimeZone.empty() ?
    sqlite3_bind_null(this->stmt, 14) :
    sqlite3_bind_text(this->stmt, 14, this->TimeZone.c_str(), -1, SQLITE_STATIC);
    
    this->Volume.empty() ?
    sqlite3_bind_null(this->stmt, 15) :
    sqlite3_bind_text(this->stmt, 15, this->Volume.c_str(), -1, SQLITE_STATIC);
    
    this->Turnover.empty() ?
    sqlite3_bind_null(this->stmt, 16) :
    sqlite3_bind_text(this->stmt, 16, this->Turnover.c_str(), -1, SQLITE_STATIC);
    
    if (sqlite3_step(this->stmt) != SQLITE_DONE)
    {
        spdlog::get("logger")->error("Query: {}", sqlite3_sql(this->stmt));
        spdlog::get("logger")->error("ERROR inserting data: {}", sqlite3_errmsg(this->_db));
    }
    
    sqlite3_reset(this->stmt);
}
