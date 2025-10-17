#pragma once

#include <functional>
#include <string>

#include "db_service.h"
#include "game/game_logic.h"

struct StartGameParams
{
    int64_t player_id;
    std::function<void(std::string)> callback;
};

struct StopGameParams
{
    int64_t player_id;
    std::function<void(std::string, int64_t)> callback;
};

struct MakeMoveParams
{
    int64_t player_id;
    int position;
    std::function<void(bool, std::string, std::shared_ptr<GameLogic> gl)> callback;
};

struct SendMessageParams
{
    int64_t from_id;
    std::string message;
    std::function<void(bool, std::string, int64_t)> callback;
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