//
// Created by Daniil Pastukhov on 2019-04-19.
//

#include "Game.h"

Game::Game() {
    running = true;
    game_mode = VSPLAYER;
}

void Game::init() {
    setlocale(LC_CTYPE, "");
    srand(time(nullptr));
    initscr();
    raw();
    cbreak();
    noecho();
    clear();
    refresh();
    curs_set(0);
    start_color();
    timeout(0);
    keypad(stdscr, true);

    // Colors initialization
    init_pair(COLOR_WALL, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(COLOR_PLAYER, COLOR_WHITE, COLOR_YELLOW);
    init_pair(COLOR_SCORE, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(COLOR_BOX, COLOR_CYAN, COLOR_CYAN);
    init_pair(COLOR_BOMB, COLOR_RED, COLOR_RED);
    init_pair(COLOR_BOMB_BONUS, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_RANGE_BONUS, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_GHOST, COLOR_WHITE, COLOR_WHITE);
    init_pair(COLOR_TEXT, COLOR_WHITE, COLOR_BLACK);

    if (!has_colors()) {
        endwin();
        std::cout << "ERROR: Terminal does not support color." << std::endl;
        exit(1);
    }

    clear();
    attron(COLOR_SCORE);
    if (game_mode == VSPLAYER) attron(A_BOLD);
    mvprintw(SCREEN_HEIGHT / 2 - 5, SCREEN_WIDTH / 2 - 8, "Player vs Player");
    attroff(A_BOLD);
    if (game_mode == VSBOT) attron(A_BOLD);
    mvprintw(SCREEN_HEIGHT / 2 + 5, SCREEN_WIDTH / 2 - 6, "Player vs Bot");
    attroff(A_BOLD);
    refresh();

    bool chosen = false;
    while (!chosen) { // Menu.
        int key;
        if ((key = getch()) > 0) {
            switch (key) {
                case KEY_UP:
                case 'w':
                case 'W': {
                    game_mode = VSPLAYER;
                    break;
                }
                case KEY_DOWN:
                case 's':
                case 'S': {
                    game_mode = VSBOT;
                    break;
                }
                case '\n': // Enter key
                case ' ': { // Space key
                    chosen = true;
                    break;
                }
                case 'q':
                case 'Q': {
                    close();
                    exit(1);
                }
            }
            clear();
            attron(COLOR_SCORE);
            if (game_mode == VSPLAYER) attron(A_BOLD);
            mvprintw(SCREEN_HEIGHT / 2 - 5, SCREEN_WIDTH / 2 - 8, "Player vs Player");
            attroff(A_BOLD);
            if (game_mode == VSBOT) attron(A_BOLD);
            mvprintw(SCREEN_HEIGHT / 2 + 5, SCREEN_WIDTH / 2 - 6, "Player vs Bot");
            attroff(A_BOLD);
            refresh();
        }
    }

    if (game_mode == VSPLAYER) {
        askName(1);
        askName(2);
    } else {
        askName(1);
    }
}

void Game::run() {
    loadMap();
    makeBorders();
    drawMap();

    player1 = std::make_shared<Player>(Player(1, 1, '1'));
    objects.push_back(player1);

    if (game_mode == VSPLAYER) {
        player2 = std::make_shared<Player>(Player(SCREEN_WIDTH - 2, SCREEN_HEIGHT - 2, '2'));
        objects.push_back(player2);
    } else {
        bot = std::make_shared<Bot>(Bot(SCREEN_WIDTH - 2, SCREEN_HEIGHT - 2));
        objects.push_back(bot);
    }

    for (int i = 0; i < GHOST_AMOUNT; i++) { // Add ghosts.
        spawnEnemy();
    }

    refreshMap();

    int ticks = TICKS_MAX * 15;

    while (running) {
        int key;

        if (enemies.size() < GHOST_AMOUNT) {
            spawnEnemy();
        }

        if (ticks % (TICKS_MAX / 2) == 0) {
            if (game_mode == VSBOT) {
                bot->chooseDirection(*this, player1);
                refreshMap();
            }
        }

        if (ticks % TICKS_MAX == 0) {
            moveEnemies();
            refreshMap();
        }

        if (--ticks == 0) {
            int option = rand();
            ticks = TICKS_MAX * 15;
            if (option % 2 == 0) spawnBombBonus();
            else spawnRangeBonus();
            refreshMap();
        }

        if (player1->getBonusStatus()) {
            player1->decreaseBonusTimers();
        }

        if (game_mode == VSPLAYER && player2->getBonusStatus()) {
            player2->decreaseBonusTimers();
        }

        if (game_mode == VSBOT && bot->getBonusStatus()) {
            bot->decreaseBonusTimers();
        }

        if (player1->decreaseBombTimer(*this)) {
            refreshMap();
        }

        if (game_mode == VSPLAYER && player2->decreaseBombTimer(*this)) {
            refreshMap();
        }

        if (game_mode == VSBOT && bot->decreaseBombTimer(*this)) {
            refreshMap();
        }

        if (find(objects.begin(), objects.end(), player1) == objects.end()
            || (game_mode == VSPLAYER && find(objects.begin(), objects.end(), player2) == objects.end())
            || (game_mode == VSBOT && find(objects.begin(), objects.end(), bot) == objects.end())) {
            gameOver();
            running = false;
            break;
        }

        if ((key = getch()) > 0) {
            if (handlePressedKey(key)) {
                refreshMap();
            }
        }
    }
}

void Game::close() {
    endwin();
}

void Game::loadMap() {
    std::ifstream file(path);
    if (!file.is_open()) {
        endwin();
        std::cout << "ERROR: Couldn't open the map.\n" << std::endl;
        exit(1);
    }

    for (int i = 0; i <= SCREEN_HEIGHT; i++) {
        for (int j = 0; j <= SCREEN_WIDTH; j++) {
            file.get(map[i][j]);
        }
    }
}

void Game::makeBorders() {
    attron(COLOR_PAIR(COLOR_WALL));
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        objects.push_back(std::make_shared<Wall>(Wall(i, 0)));
        objects.push_back(std::make_shared<Wall>(Wall(i, SCREEN_HEIGHT - 1)));
    }
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        objects.push_back(std::make_shared<Wall>(Wall(0, i)));
        objects.push_back(std::make_shared<Wall>(Wall(SCREEN_WIDTH - 1, i)));
    }

    refresh();
}

void Game::drawMap() {
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            switch (map[i][j]) {
                case '#': {
                    objects.push_back(std::make_shared<Wall>(Wall(j, i)));
                    break;
                }
                case 'b': {
                    objects.push_back(std::make_shared<Box>(Box(j, i)));
                    break;
                }
            }
        }
    }

    refresh();
}

void Game::refreshMap() {
    clear();
    for (const auto &item: objects) { // Draw every object.
        item->draw();
//        attron(COLOR_PAIR(item->color()));
//        mvaddch(item->getY(), item->getX(), item->symbol());
    }
    player1->draw();
    if (game_mode == VSPLAYER) player2->draw();
    else bot->draw();
    attron(COLOR_PAIR(COLOR_SCORE));
    mvprintw(SCREEN_HEIGHT - 1, 0, "%s: %d, %s: %d", player1_name.c_str(), player1->getScore(),
             (game_mode == VSPLAYER) ? player2_name.c_str() : "Bot",
             (game_mode == VSPLAYER) ? player2->getScore() : bot->getScore()); // Print score.
    refresh();
}

bool Game::handlePressedKey(int key_code) {
    bool update = false;
    switch (key_code) {
        case 'A':
        case 'a': {
            update = makeMove(-1, 0, player1);
            break;
        }
        case 'D':
        case 'd': {
            update = makeMove(1, 0, player1);
            break;
        }
        case 'S':
        case 's': {
            update = makeMove(0, 1, player1);
            break;
        }
        case 'W':
        case 'w': {
            update = makeMove(0, -1, player1);
            break;
        }
        case ' ': {
            player1->placeBomb(*this);
            update = true;
            break;
        }
        case 'q':
        case 'Q': {
            running = false;
        }
    }

    if (game_mode == VSPLAYER) {
        switch (key_code) {
            case KEY_LEFT: {
                update = makeMove(-1, 0, player2);
                break;
            }
            case KEY_RIGHT: {
                update = makeMove(1, 0, player2);
                break;
            }
            case KEY_DOWN: {
                update = makeMove(0, 1, player2);
                break;
            }
            case KEY_UP: {
                update = makeMove(0, -1, player2);
                break;
            }
            case 'o': {
                player2->placeBomb(*this);
                update = true;
                break;
            }
            case 'q':
            case 'Q': {
                running = false;
            }
        }
    }

    return update;
}

bool Game::makeMove(int x, int y, std::shared_ptr<Player> &player) {
    auto collision = checkCollision(player->getX() + x, player->getY() + y);
    if (std::dynamic_pointer_cast<BombBonus>(collision)) {
        player->increaseBombBonus();
        player->move(x, y);
        objects.erase(find(objects.begin(), objects.end(), collision));
    } else if (std::dynamic_pointer_cast<RangeBonus>(collision)) {
        player->increaseRangeBonus();
        player->move(x, y);
        objects.erase(find(objects.begin(), objects.end(), collision));
    } else if (std::dynamic_pointer_cast<Ghost>(collision)) {
        gameOver();
    } else if (!collision) player->move(x, y);
    else return false;
    return true;
}

std::shared_ptr<Object> Game::checkCollision(int x, int y) const {
    for (const auto &i: objects) {
        if (i->getX() == x && i->getY() == y) {
            return i;
        }
    }

    return nullptr;
}

void Game::gameOver() {
    clear();
    attron(COLOR_PAIR(COLOR_TEXT));
    mvprintw(SCREEN_HEIGHT / 2 - 4, SCREEN_WIDTH / 2 - 5, "Game over!");
    if (find(objects.begin(), objects.end(), player1) == objects.end()) {
        if (game_mode == VSPLAYER) mvprintw(SCREEN_HEIGHT / 2 - 3, SCREEN_WIDTH / 2 - 5,"%s won!", player2_name.c_str());
        else mvprintw(SCREEN_HEIGHT / 2 - 3, SCREEN_WIDTH / 2 - 4,"Bot won!");
    } else {
        mvprintw(SCREEN_HEIGHT / 2 - 3, SCREEN_WIDTH / 2 - 6, "%s won!", player1_name.c_str());
    }
    handleScore();
    int key = 0;
    while (true) {
        if ((key = getch()) > 0 && (key == 'q' || key == 'Q')) {
            break;
        }
    }
}

void Game::spawnBombBonus() {
    int counter = 0;
    int pos_x = rand() % 78 + 1;
    int pos_y = rand() % 22 + 1;

    while (checkCollision(pos_x, pos_y) || counter++ == (int) objects.size()) {
        pos_x = rand() % 78 + 1;
        pos_y = rand() % 22 + 1;
    }

    if (counter < (int) objects.size())
        objects.push_back(std::make_shared<BombBonus>(pos_x, pos_y));
}

void Game::spawnRangeBonus() {
    int counter = 0;
    int pos_x = rand() % 78 + 1;
    int pos_y = rand() % 22 + 1;

    while (checkCollision(pos_x, pos_y) || counter++ == (int) objects.size()) {
        pos_x = rand() % 78 + 1;
        pos_y = rand() % 22 + 1;
    }
    if (counter < (int) objects.size())
        objects.push_back(std::make_shared<RangeBonus>(pos_x, pos_y));
}

void Game::moveEnemies() {
    for (auto &i: enemies) {
        int direction = rand() % 4;
        switch (direction) {
            case 0: {
                makeEnemyStep(i, 1, 0);
                break;
            }
            case 1: {
                makeEnemyStep(i, -1, 0);
                break;
            }
            case 2: {
                makeEnemyStep(i, 0, 1);
                break;
            }
            case 3: {
                makeEnemyStep(i, 0, -1);
            }
        }
    }
}

void Game::makeEnemyStep(std::shared_ptr<Ghost> &g, int x, int y) {
    auto collision = checkCollision(g->getX() + x, g->getY() + y);
    if (!collision) {
        g->move(x, y);
    } else if (std::dynamic_pointer_cast<Player>(collision)) {
        gameOver();
    }
}


void Game::spawnEnemy() {
    int x = rand() % 78 + 1;
    int y = rand() % 22 + 1;

    while (checkCollision(x, y)) {
        x = rand() % 78 + 1;
        y = rand() % 22 + 1;
    }
    auto ghost = std::make_shared<Ghost>(Ghost(x, y));
    objects.push_back(ghost);
    enemies.push_back(ghost);
}

void Game::askName(int i) {
    clear();
    std::string name;
    bool written = false;
    while (!written) {
        int key;
        attron(A_BOLD);
        mvprintw(SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2 - 20,
                 (i == 1) ? "Enter first player's name: %s" : "Enter second player's name: %s", name.c_str());
        attroff(A_BOLD);
        if ((key = getch()) > 0) {
            switch (key) {
                case '\n': { // Enter key
                    written = true;
                    break;
                }
                case KEY_BACKSPACE:
                case '\b':
                case 127: {
                    name.pop_back();
                    clear();
                    break;
                }
                case KEY_EXIT: {
                    close();
                    exit(1);
                    break;
                }
                default: {
                    name += key;
                }
            }
        }
        refresh();
    }

    if (i == 1) player1_name = name;
    else if (i == 2) player2_name = name;
}

void Game::handleScore() {
    std::fstream lb("src/leaderboards.txt");
    if (!lb.is_open()) {
        attron(COLOR_PAIR(COLOR_TEXT));
        mvprintw(SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2 - 14, "Failed to load leaderboards.");
    } else {
        attron(A_BOLD);
        mvprintw(SCREEN_HEIGHT / 2 - 1, SCREEN_WIDTH / 2 - 6, "Leaderboards");
        attroff(A_BOLD);
        std::multimap<std::string, int> scores;
        std::string buffer;
        int score = 0;
        int padding = 0;
        attron(COLOR_PAIR(COLOR_TEXT));
        while (getline(lb, buffer)) { // Load previous leaders
            std::stringstream ss(buffer);
            std::string name;
            ss >> name >> score;
            name.pop_back(); // Delete ':' from the name
            scores.insert({name, score});
        }

        lb.close();

        std::ofstream lb("src/leaderboards.txt", std::ofstream::out | std::ofstream::trunc);

        if (game_mode == VSPLAYER) {
            scores.insert({player1_name, player1->getScore()});
            scores.insert({player2_name, player2->getScore()});
        } else {
            scores.insert({player1_name, player1->getScore()});
        }

        std::vector<std::pair<std::string, int>> pairs; // Sorted map by score.
        for (auto &score: scores) pairs.push_back(score);
        sort(pairs.begin(), pairs.end(),
             [=](std::pair<std::string, int> &a, std::pair<std::string, int> &b) { return a.second > b.second; });

        int counter = 0;
        for (const auto &i: pairs) { // Write new leaderboard.
            if (counter++ <= 7)  break;
            mvprintw(SCREEN_HEIGHT / 2 - padding--, SCREEN_WIDTH / 2 - 6, "%s: %d", i.first.c_str(), i.second);
            lb << i.first << ": " << i.second << std::endl;
        }

        lb.close();
    }
}
