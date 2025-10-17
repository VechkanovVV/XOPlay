# XOPlay Bot

**XOPlay** is a simple Telegram bot for playing **Connect Four (4-in-a-row)** online with other players. The bot supports creating games, making moves, sending messages to opponents and viewing game history.

---

## Features

* Play Connect Four with other users via Telegram
* Create and join online games
* Make moves by column number
* Send messages to your opponent during a game
* View your game history
* Simple command-based interface

---

## Available commands

```
/start         - Start a new game
/move <column> - Make a move (column from 1 to 7)
/stop          - Stop the current game
/history       - Show your game history
/message <text>- Send a message to opponent
/help          - Show this help message
```

---

## Example interaction (short)

Below is a short, **anonymized** example that explicitly shows which messages are **user commands** and which messages are **bot responses**. This demonstrates how commands trigger bot replies (e.g. `/start` triggers the bot to post `[Waiting for an opponent...]`).

```
User: /start
Bot: [Waiting for an opponent...]                # bot informs that it is searching for an opponent

# (match found)
Bot: [Game created, waiting for opponent’s move]

Player2: [Opponent's move: 1]
   1  2  3  4  5  6  7
  +---------------------+
  | .  .  .  .  .  .  . |
  | .  .  .  .  .  .  . |
  | .  .  .  .  .  .  . |
  | .  .  .  .  .  .  . |
  | .  .  .  .  .  .  . |
  | .  X  .  .  .  .  . |
  +---------------------+
Bot: [Your turn]

User: /move 2
Bot: [✅ Successful move]
   1  2  3  4  5  6  7
  +---------------------+
  | .  .  .  .  .  .  . |
  | .  .  .  .  .  .  . |
  | .  .  .  .  .  .  . |
  | .  .  .  .  .  .  . |
  | .  .  .  .  .  .  . |
  | .  X  O  .  .  .  . |
  +---------------------+
Bot: [Wait for opponent’s move...]

User: /message haha hi
Bot: [✅ Message delivered]

# Game continues...

Player2: [Opponent's move: 3]
Bot: [Your turn]
User: /move 4
Bot: [✅ Successful move]
   1  2  3  4  5  6  7
  +---------------------+
  | .  .  .  .  .  .  . |
  | .  .  .  .  .  .  . |
  | .  .  .  .  X  .  . |
  | .  .  .  .  X  .  . |
  | .  .  .  .  X  .  . |
  | O  O  .  O  X  .  . |
  +---------------------+
Bot: [🏆 Congrats! You win!]

User: /history
Bot: [Opponent TG ID: 5240622580, Result: move:3]
Bot: [Opponent TG ID: 5240622580, Result: abandon]
Bot: [Opponent TG ID: 5240622580, Result: win]
Bot: [Opponent TG ID: 5240622580, Result: abandon]
Bot: [Opponent TG ID: 5240622580, Result: win]
Bot: [Opponent TG ID: 5240622580, Result: win]

User: /help
Bot: [Available commands:
/start - Start a new game
/move <column> - Make a move
/stop - Stop current game
/history - Show your game history
/message <text> - Send a message to opponent
/help - Show this help message]
```

---------------------+
| .  .  .  .  .  .  . |
| .  .  .  .  .  .  . |
| .  .  .  .  .  .  . |
| .  .  .  .  .  .  . |
| .  .  .  .  .  .  . |
| .  X  .  .  .  .  . |
+---------------------+
[Your turn]

[18.10.2025 00:11] Vlad: /move 2
[18.10.2025 00:11] Bot: [✅ Successful move]
[18.10.2025 00:11] Bot: [Wait for opponent’s move…]

[18.10.2025 00:12] Vlad: /message haha hi
[18.10.2025 00:12] Bot: [✅ Message delivered]

... (game continues)

[18.10.2025 00:14] Bot: [🏆 Congrats! You win!]

````

---

## Quick start

1. Clone the repository:

```bash
git clone https://github.com/VechkanovVV/XOPlay.git
cd XOPlay
````

2. Create a `.env` file in the project root and fill required variables:

```
POSTGRES_USER=myuser
POSTGRES_PASSWORD=mypassword
POSTGRES_DB=gamedb
BOT_TOKEN=your_telegram_bot_token_here
```

> **Important:** Do **not** commit your real bot token or passwords to a public repository. Use environment variables or a secret manager in production.

3. Run with Docker Compose (recommended):

```bash
docker-compose up --build
```

4. Or build manually with CMake:

```bash
# Install build dependencies (Debian/Ubuntu)
sudo apt-get update && sudo apt-get install -y build-essential cmake clang-format libpqxx-dev libboost-system-dev libcurl4-openssl-dev libssl-dev zlib1g-dev

# Then build
mkdir -p build && cd build
cmake ..
make -j$(nproc)
```

---

## Dependencies

* PostgreSQL (database)
* libpqxx (C++ PostgreSQL client)
* Boost.System
* libcurl
* OpenSSL
* zlib
* CMake & a C++ compiler (e.g. clang or gcc)

---

## Running & Deployment

* Use Docker Compose to run the bot and a local Postgres instance for development.
* For production, set environment variables and run the compiled binary or container behind a process manager.

---

## Contributing

Contributions, bug reports and feature suggestions are welcome. Please open an issue or submit a pull request.

---

## License

This project is open-source — add your preferred license here (e.g. MIT).

---

## Contact

Vlad Vechkanov — GitHub: [VechkanovVV](https://github.com/VechkanovVV)
