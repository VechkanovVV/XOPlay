#include "config.h"

#include <cstdlib>
#include <string>

Config::Config()
{
    cfg_[ConfigVariable::BotToken] = getenvOrEmpty("BotToken");
    cfg_[ConfigVariable::DbName] = getenvOrEmpty("POSTGRES_DB");
    cfg_[ConfigVariable::DbPassword] = getenvOrEmpty("POSTGRES_PASSWORD");
    cfg_[ConfigVariable::DbUser] = getenvOrEmpty("POSTGRES_USER");
}

std::string Config::get(ConfigVariable v) const
{
    auto it = cfg_.find(v);
    return (it != cfg_.end()) ? it->second : std::string{};
}

std::string Config::getenvOrEmpty(const char* name)
{
    const char* val = std::getenv(name);
    return val ? std::string(val) : std::string{};
}