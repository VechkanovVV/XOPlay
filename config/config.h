#pragma once

#include <cstdlib>
#include <string>
#include <type_traits>
#include <unordered_map>

enum class ConfigVariable
{
    BotToken,
    DbUser,
    DbPassword,
    DbName
};

struct HashEnum
{
    std::size_t operator()(ConfigVariable v) const noexcept
    {
        using UT = std::underlying_type_t<ConfigVariable>;
        return std::hash<UT>{}(static_cast<UT>(v));
    }
};

class Config
{
   public:
    Config();
    std::string get(ConfigVariable v) const;

   private:
    static std::string getenvOrEmpty(const char* name);

    std::unordered_map<ConfigVariable, std::string, HashEnum> cfg_;
};
