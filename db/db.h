#pragma once

#include <atomic>
#include <memory>
#include <pqxx/pqxx>
#include <string>

class Database final
{
   public:
    explicit Database(const std::string& connection_string);
    ~Database() = default;

    bool isConnected() const;

    void executeQuery(const std::string& query);

    pqxx::connection& getConnection();

   private:
    std::string connection_string_;
    std::atomic<bool> connected_;
};