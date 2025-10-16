#include "db_service.h"

#include <iostream>
#include <vector>

DbService::DbService(std::shared_ptr<Database> db) : db_(db)
{
    if (!db || !db->isConnected())
    {
        throw std::runtime_error("Invalid database connection");
    }
}

bool DbService::addUser(int64_t tg_id, const std::string& username, const std::string& first_name,
                        const std::string& last_name)
{
    try
    {
        pqxx::connection& conn = db_->getConnection();
        pqxx::work tx{conn};

        tx.exec_params(
            "INSERT INTO users (tg_id, username, first_name, last_name) "
            "VALUES ($1, $2, $3, $4) "
            "ON CONFLICT (tg_id) DO UPDATE SET "
            "username = $2, first_name = $3, last_name = $4",
            tg_id, username, first_name, last_name);

        tx.commit();

        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error adding user: " << e.what() << std::endl;
        return false;
    }
}

std::optional<User> DbService::getUserById(int64_t id)
{
    try
    {
        pqxx::connection& conn = db_->getConnection();
        pqxx::work tx{conn};
        pqxx::result result = tx.exec_params(
            "SELECT tg_id, username, first_name, last_name "
            "FROM users WHERE tg_id = $1",
            id);
        if (result.empty())
        {
            return std::nullopt;
        }

        User user;
        user.tg_id = result[0]["tg_id"].as<int64_t>();
        user.username = result[0]["username"].as<std::string>();
        user.first_name = result[0]["first_name"].as<std::string>();
        user.last_name = result[0]["last_name"].as<std::string>();

        return user;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error getting user: " << e.what() << std::endl;
        return std::nullopt;
    }
}

int DbService::createGame(int64_t first_player_tg, int64_t second_player_tg)
{
    try
    {
        pqxx::connection& conn = db_->getConnection();
        pqxx::work tx{conn};

        pqxx::result result = tx.exec_params(
            "INSERT INTO games (first_player_tg, second_player_tg) "
            "VALUES ($1, $2) "
            "RETURNING id",
            first_player_tg, second_player_tg);

        tx.commit();

        if (!result.empty())
        {
            return result[0]["id"].as<int>();
        }
        else
        {
            return -1;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error creating game: " << e.what() << std::endl;
        return -1;
    }
}

bool DbService::updateGameStatus(int game_id, const std::string& status)
{
    try
    {
        pqxx::connection& conn = db_->getConnection();
        pqxx::work tx{conn};

        tx.exec_params("UPDATE games SET status = $1 WHERE id = $2", status, game_id);

        tx.commit();
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error updating game status: " << e.what() << std::endl;
        return false;
    }
}

std::vector<Game> DbService::getGamesByPlayer(int64_t tg_id)
{
    std::vector<Game> games;
    try
    {
        pqxx::connection& conn = db_->getConnection();
        pqxx::work tx{conn};

        pqxx::result res = tx.exec_params(
            "SELECT id, first_player_tg, second_player_tg, status, board "
            "FROM games "
            "WHERE (first_player_tg = $1 OR second_player_tg = $1) "
            "AND status IN ('pending', 'active') "
            "ORDER BY id DESC",
            tg_id);

        for (const auto& row : res)
        {
            Game game;
            game.id = row["id"].as<int>();
            game.first_player_tg = row["first_player_tg"].as<int64_t>();
            game.second_player_tg = row["second_player_tg"].as<int64_t>();
            game.status = row["status"].as<std::string>();
            game.board = row["board"].as<std::string>();
            games.push_back(game);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error getting games by player: " << e.what() << std::endl;
    }
    return games;
}

std::optional<Game> DbService::getGameById(int64_t game_id)
{
    try
    {
        pqxx::connection& conn = db_->getConnection();
        pqxx::work tx{conn};

        pqxx::result res = tx.exec_params(
            "SELECT id, first_player_tg, second_player_tg, status "
            "FROM games "
            "WHERE id = $1",
            game_id);

        if (res.empty())
        {
            return std::nullopt;
        }

        Game game;
        game.id = res[0]["id"].as<int>();
        game.first_player_tg = res[0]["first_player_tg"].as<int64_t>();
        game.second_player_tg = res[0]["second_player_tg"].as<int64_t>();
        game.status = res[0]["status"].as<std::string>();

        return game;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error getting game by id: " << e.what() << std::endl;
        return std::nullopt;
    }
}

void DbService::addHistoryRecord(int game_id, int64_t player_tg, const std::string& result)
{
    try
    {
        pqxx::connection& conn = db_->getConnection();
        pqxx::work tx{conn};

        tx.exec_params(
            "INSERT INTO history (game_id, player_tg, result) "
            "VALUES ($1, $2, $3)",
            game_id, player_tg, result);

        tx.commit();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error adding history record: " << e.what() << std::endl;
    }
}

std::vector<HistoryRecord> DbService::getHistoryByGame(int game_id)
{
    std::vector<HistoryRecord> historyRecords;
    try
    {
        pqxx::connection& conn = db_->getConnection();
        pqxx::work tx{conn};

        pqxx::result res = tx.exec_params(
            "SELECT id, game_id, player_tg, result "
            "FROM history WHERE game_id = $1",
            game_id);

        for (const auto& row : res)
        {
            HistoryRecord record;
            record.game_id = row["game_id"].as<int>();
            record.id = row["id"].as<int>();
            record.player_tg = row["player_tg"].as<int64_t>();
            record.result = row["result"].as<std::string>();
            historyRecords.push_back(record);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error getting history by game: " << e.what() << std::endl;
    }

    return historyRecords;
}

std::vector<HistoryRecord> DbService::getHistoryByPlayer(int64_t player_tg)
{
    std::vector<HistoryRecord> historyRecords;
    try
    {
        pqxx::connection& conn = db_->getConnection();
        pqxx::work tx{conn};

        pqxx::result res = tx.exec_params(
            "SELECT id, game_id, player_tg, result "
            "FROM history WHERE player_tg = $1",
            player_tg);

        for (const auto& row : res)
        {
            HistoryRecord record;
            record.game_id = row["game_id"].as<int>();
            record.id = row["id"].as<int>();
            record.player_tg = row["player_tg"].as<int64_t>();
            record.result = row["result"].as<std::string>();
            historyRecords.push_back(record);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error getting history by player: " << e.what() << std::endl;
    }

    return historyRecords;
}

bool DbService::updateGameBoard(int game_id, const std::string& board)
{
    try
    {
        pqxx::connection& conn = db_->getConnection();
        pqxx::work tx{conn};

        tx.exec_params(
            "UPDATE games SET board = $1 "
            "WHERE id = $2",
            board, game_id);

        tx.commit();
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error updating game board: " << e.what() << std::endl;
        return false;
    }
}

void DbService::addMoveRecord(int game_id, int64_t player_tg, int column)
{
    try
    {
        pqxx::connection& conn = db_->getConnection();
        pqxx::work tx{conn};

        std::string result = "move:" + std::to_string(column);

        tx.exec_params(
            "INSERT INTO history (game_id, player_tg, result) "
            "VALUES ($1, $2, $3)",
            game_id, player_tg, result);

        tx.commit();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error adding move record: " << e.what() << std::endl;
    }
}

std::optional<Game> DbService::getActiveGameByPlayers(int64_t first_player_tg, int64_t second_player_tg)
{
    try
    {
        pqxx::connection& conn = db_->getConnection();
        pqxx::work tx{conn};

        pqxx::result res = tx.exec_params(
            "SELECT id, first_player_tg, second_player_tg, stauts, board "
            "FROM games "
            "WHERE ((first_player_tg = $1 AND second_player_tg = $2) "
            "       OR (first_player_tg = $2 AND second_player_tg = $1)) "
            "AND status IN ('pending', 'active') "
            "ORDER BY id DESC LIMIT 1",
            first_player_tg, second_player_tg);

        if (res.empty())
        {
            return std::nullopt;
        }

        Game game;
        game.id = res[0]["id"].as<int>();
        game.first_player_tg = res[0]["first_player_tg"].as<int64_t>();
        game.second_player_tg = res[0]["second_player_tg"].as<int64_t>();
        game.status = res[0]["status"].as<std::string>();
        game.board = res[0]["board"].is_null() ? std::string() : res[0]["board"].as<std::string>();

        return game;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error getting active game by players: " << e.what() << std::endl;
        return std::nullopt;
    }
}