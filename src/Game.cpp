//
// Created by Daniil Pastukhov on 2019-04-19.
//

#include "Game.h"

Game::Game() {
    running = true;
    gameMode = VSPLAYER;
}

void Game::init() {
    srand(time(nullptr));
    initscr();
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

    if (!has_colors()) {
        endwin();
        std::cout << "ERROR: Terminal does not support color." << std::endl;
        exit(1);
    }

    bool chosen = false;
    while (!chosen) { // Menu.
        int key;
        clear();
        attron(COLOR_SCORE);
        if (gameMode == VSPLAYER) attron(A_BOLD);
        mvprintw(SCREEN_HEIGHT / 2 - 5, SCREEN_WIDTH / 2 - 8, "Player vs Player");
        attroff(A_BOLD);
        if (gameMode == VSBOT) attron(A_BOLD);
        mvprintw(SCREEN_HEIGHT / 2 + 5, SCREEN_WIDTH / 2 - 6, "Player vs Bot");
        attroff(A_BOLD);

        if ((key = getch()) > 0) {
            switch (key) {
                case KEY_UP:
                case 'w':
                case 'W': {
                    gameMode = VSPLAYER;
                    break;
                }
                case KEY_DOWN:
                case 's':
                case 'S': {
                    gameMode = VSBOT;
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
        }
        refresh();
    }
}

void Game::run() {
    loadMap();
    makeBorders();
    drawMap();

    player1 = std::make_shared<Player>(Player(1, 1, '1'));
    objects.push_back(player1);

    if (gameMode == VSPLAYER) {
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
            if (gameMode == VSBOT) {
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

        if (gameMode == VSPLAYER && player2->getBonusStatus()) {
            player2->decreaseBonusTimers();
        }

        if (gameMode == VSBOT && bot->getBonusStatus()) {
            bot->decreaseBonusTimers();
        }

        if (player1->decreaseBombTimer(*this)) {
            refreshMap();
        }

        if (gameMode == VSPLAYER && player2->decreaseBombTimer(*this)) {
            refreshMap();
        }

        if (gameMode == VSBOT && bot->decreaseBombTimer(*this)) {
            refreshMap();
        }

        if (find(objects.begin(), objects.end(), player1) == objects.end()
            || (gameMode == VSPLAYER && find(objects.begin(), objects.end(), player2) == objects.end())
            || (gameMode == VSBOT && find(objects.begin(), objects.end(), bot) == objects.end())) {
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
    for (const auto &item: objects) {
        attron(COLOR_PAIR(item->color()));
        mvaddch(item->getY(), item->getX(), item->symbol());
    }
    player1->draw();
    if (gameMode == VSPLAYER) player2->draw();
    else bot->draw();
    attron(COLOR_PAIR(COLOR_SCORE));
    mvprintw(SCREEN_HEIGHT - 1, 0, "Player1: %d, player2: %d", player1->getScore(),
             (gameMode == VSPLAYER) ? player2->getScore() : bot->getScore());
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

    if (gameMode == VSPLAYER) {
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
    attron(COLOR_PAIR(COLOR_SCORE));
    mvprintw(SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2 - 5, "Game over!");
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

    while (checkCollision(pos_x, pos_y) || counter++ == objects.size()) {
        pos_x = rand() % 78 + 1;
        pos_y = rand() % 22 + 1;
    }

    if (counter < objects.size())
        objects.push_back(std::make_shared<BombBonus>(pos_x, pos_y));
}

void Game::spawnRangeBonus() {
    int counter = 0;
    int pos_x = rand() % 78 + 1;
    int pos_y = rand() % 22 + 1;

    while (checkCollision(pos_x, pos_y) || counter++ == objects.size()) {
        pos_x = rand() % 78 + 1;
        pos_y = rand() % 22 + 1;
    }
    if (counter < objects.size())
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

// TODO
//void Game::askName(int i) {
//    bool written = false;
//    while (!written) {
//        attron(A_BOLD);
//        mvprintw(0, 0, "Fill the name: ");
//        attroff(A_BOLD);
//
//    }
//}
