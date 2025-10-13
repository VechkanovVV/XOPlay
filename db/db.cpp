#include "db.h"

#include <iostream>

Database::Database(const std::string& connection)
{
    try
    {
        conn_ = std::make_unique<pqxx::connection>(connection);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Database connection error: " << e.what() << std::endl;
        conn_.reset();
    }
}

bool Database::isConnected() const
{
    return conn_ && conn_->is_open();
}

void Database::executeQuery(const std::string& query)
{
    if (!isConnected())
    {
        throw std::runtime_error("Database not connected");
    }

    try
    {
        pqxx::work txn{*conn_};
        txn.exec(query);
        txn.commit();
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error(std::string("Query execution failed: ") + e.what());
    }
}

pqxx::connection& Database::getConnection()
{
    if (!conn_ || !conn_->is_open())
    {
        throw std::runtime_error("Database connection not available");
    }
    return *conn_;
}
