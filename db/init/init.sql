CREATE TABLE IF NOT EXISTS users (
  tg_id      BIGINT PRIMARY KEY,
  username   VARCHAR(64),
  first_name VARCHAR(100),
  last_name  VARCHAR(100)
);

CREATE TABLE IF NOT EXISTS games (
  id               SERIAL PRIMARY KEY,
  first_player_tg  BIGINT NOT NULL REFERENCES users(tg_id) ON DELETE CASCADE,
  second_player_tg BIGINT NOT NULL REFERENCES users(tg_id) ON DELETE CASCADE,
  status           VARCHAR(32) DEFAULT 'pending',
  board VARCHAR(100) DEFAULT '...........................................'
);

CREATE INDEX IF NOT EXISTS idx_games_first_player_tg ON games(first_player_tg);
CREATE INDEX IF NOT EXISTS idx_games_second_player_tg ON games(second_player_tg);

CREATE TABLE IF NOT EXISTS history (
  id         SERIAL PRIMARY KEY,
  game_id    INTEGER NOT NULL REFERENCES games(id) ON DELETE CASCADE,
  player_tg  BIGINT REFERENCES users(tg_id),
  result     TEXT
);

CREATE INDEX IF NOT EXISTS idx_history_game_id ON history(game_id);
CREATE INDEX IF NOT EXISTS idx_history_player_tg ON history(player_tg);
