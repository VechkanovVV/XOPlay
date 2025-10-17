#include "db.h"

#include <iostream>

Database::Database(const std::string& connection_string) : connection_string_(connection_string), connected_(false)
{
    try
    {
        pqxx::connection test_conn(connection_string_);
        connected_ = test_conn.is_open();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Database connection error: " << e.what() << std::endl;
        connected_ = false;
    }
}

bool Database::isConnected() const
{
    return connected_.load();
}

void Database::executeQuery(const std::string& query)
{
    if (!isConnected())
    {
        throw std::runtime_error("Database not connected");
    }

    try
    {
        pqxx::connection& conn = getConnection();
        pqxx::work txn{conn};
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
    thread_local std::unique_ptr<pqxx::connection> thread_conn;
    if (!thread_conn || !thread_conn->is_open())
    {
        thread_conn = std::make_unique<pqxx::connection>(connection_string_);
    }
    return *thread_conn;
}