#include "game_service.h"

#include <iostream>

#include "game_exception.h"

GameService::GameService(std::shared_ptr<DbService> db) : db_(db)
{
    if (!db_)
    {
        throw std::invalid_argument("DbService cannot be null");
    }
}

int GameService::createGame(int64_t player1_id, int64_t player2_id)
{
    auto player1 = db_->getUserById(player1_id);
    auto player2 = db_->getUserById(player2_id);

    if (!player1 || !player2)
    {
        throw std::runtime_error("One or both players do not exist");
    }

    int game_id = db_->createGame(player1_id, player2_id);
    auto gameData = db_->getGameById(game_id);
    auto game = std::make_shared<GameLogic>(game_id, gameData->first_player_tg, gameData->second_player_tg);
    cacheGame(game, player1_id, player2_id);
    return game_id;
}

std::shared_ptr<GameLogic> GameService::getActiveGameById(int game_id)
{
    auto it = gamesById_.find(game_id);
    if (it != gamesById_.end())
    {
        return it->second;
    }

    auto gameData = db_->getGameById(game_id);
    if (!gameData)
    {
        return nullptr;
    }

    auto game = std::make_shared<GameLogic>(game_id, gameData->first_player_tg, gameData->second_player_tg);

    if (!gameData->board.empty())
    {
        game->setBoardFromString(gameData->board);
    }

    gamesById_[game_id] = game;
    gamesByUser_[gameData->first_player_tg] = game;
    gamesByUser_[gameData->second_player_tg] = game;
    return game;
}

std::optional<std::shared_ptr<GameLogic>> GameService::getGame(int game_id)
{
    auto game = getActiveGameById(game_id);
    if (!game)
    {
        return std::nullopt;
    }
    return game;
}

bool GameService::makeMove(int game_id, int64_t player_id, int column)
{
    auto game = getActiveGameById(game_id);
    if (!game)
    {
        return false;
    }

    auto gameData = db_->getGameById(game_id);
    if (!gameData || (gameData->status != "active" && gameData->status != "pending"))
    {
        return false;
    }

    if (gameData->status == "pending")
    {
        db_->updateGameStatus(game_id, "active");
    }

    if (game->getCurrentPlayer() != player_id)
    {
        return false;
    }

    if (!game->makeMove(player_id, column))
    {
        return false;
    }

    db_->addMoveRecord(game_id, player_id, column);

    db_->updateGameBoard(game_id, game->getBoardString());

    if (game->isWinner(player_id))
    {
        db_->updateGameStatus(game_id, "finished");
        db_->addHistoryRecord(game_id, player_id, "win");

        removeGameFromCache(game_id, gameData->first_player_tg, gameData->second_player_tg);
        return true;
    }

    if (game->isDraw())
    {
        db_->updateGameStatus(game_id, "draw");
        db_->addHistoryRecord(game_id, 0, "draw");

        removeGameFromCache(game_id, gameData->first_player_tg, gameData->second_player_tg);
        return true;
    }

    return true;
}

bool GameService::isGameOver(int game_id, int64_t& winner_id)
{
    auto gameData = db_->getGameById(game_id);
    if (!gameData)
    {
        return false;
    }

    if (gameData->status == "finished")
    {
        auto history = db_->getHistoryByGame(game_id);
        for (const auto& record : history)
        {
            if (record.result == "win")
            {
                winner_id = record.player_tg;
                return true;
            }
        }
        return true;
    }
    else if (gameData->status == "draw")
    {
        winner_id = 0;
        return true;
    }

    return false;
}

std::string GameService::renderGameBoard(int game_id)
{
    auto game = getActiveGameById(game_id);
    if (!game)
    {
        throw std::runtime_error("Game not found");
    }

    return game->renderBoard();
}

bool GameService::isPlayerTurn(int game_id, int64_t player_id)
{
    auto game = getActiveGameById(game_id);
    if (!game)
    {
        return false;
    }

    return game->getCurrentPlayer() == player_id;
}

std::optional<int64_t> GameService::getOpponent(int game_id, int64_t player_id)
{
    auto gameData = db_->getGameById(game_id);
    if (!gameData)
    {
        return std::nullopt;
    }

    if (gameData->first_player_tg == player_id)
    {
        return gameData->second_player_tg;
    }
    else if (gameData->second_player_tg == player_id)
    {
        return gameData->first_player_tg;
    }

    return std::nullopt;
}

bool GameService::abandonGame(int game_id, int64_t player_id)
{
    auto gameData = db_->getGameById(game_id);
    if (!gameData)
    {
        return false;
    }

    if (gameData->first_player_tg != player_id && gameData->second_player_tg != player_id)
    {
        return false;
    }

    int64_t winner_id =
        (gameData->first_player_tg == player_id) ? gameData->second_player_tg : gameData->first_player_tg;

    db_->updateGameStatus(game_id, "abandoned");

    db_->addHistoryRecord(game_id, player_id, "abandon");
    db_->addHistoryRecord(game_id, winner_id, "win");

    removeGameFromCache(game_id, gameData->first_player_tg, gameData->second_player_tg);

    return true;
}

std::optional<std::shared_ptr<GameLogic>> GameService::getUserActiveGame(int64_t tgId)
{
    auto game = getActiveGameByUser(tgId);
    if (!game)
    {
        return std::nullopt;
    }
    return game;
}

std::shared_ptr<GameLogic> GameService::getActiveGameByUser(int64_t user_id)
{
    auto it = gamesByUser_.find(user_id);
    if (it != gamesByUser_.end())
    {
        return it->second;
    }

    auto games = db_->getGamesByPlayer(user_id);
    for (const auto& gameData : games)
    {
        if (gameData.status == "active" || gameData.status == "pending")
        {
            return getActiveGameById(gameData.id);
        }
    }

    return nullptr;
}

void GameService::cacheGame(const std::shared_ptr<GameLogic>& game, int64_t player1_id, int64_t player2_id)
{
    gamesById_[game->getId()] = game;
    gamesByUser_[player1_id] = game;
    gamesByUser_[player2_id] = game;
}

void GameService::removeGameFromCache(int game_id, int64_t player1_id, int64_t player2_id)
{
    gamesById_.erase(game_id);
    gamesByUser_.erase(player1_id);
    gamesByUser_.erase(player2_id);
}