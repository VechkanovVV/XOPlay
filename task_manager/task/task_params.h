#pragma once

#include <functional>
#include <string>

#include "db_service.h"

struct StartGameParams
{
    int64_t initiator_id;
    int64_t opponent_id;
    std::function<void(int)> callback;
};

struct StopGameParams
{
    int game_id;
    int64_t player_id;
    std::function<void(bool)> callback;
};

struct MakeMoveParams
{
    int game_id;
    int64_t player_id;
    int position;
    std::function<void(bool, std::string)> callback;
};

struct SendMessageParams
{
    int64_t from_id;
    int64_t to_id;
    std::string message;
    std::function<void(bool, std::string)> callback;
};

struct GetHistoryParams
{
    int64_t player_id;
    std::function<void(std::string)> callback;
};

struct HelpParams
{
    int64_t player_id;
    std::function<void(std::string)> callback;
};