#include "defaults.h"
#include "game.h"
#include "gamebuilder.h"

#include <QApplication>

Game *createGame(GameBuilder &builder)
{
    builder.setSize(W_WIDTH, W_HEIGHT);
    builder.setConfig(CONFIG_FILE);
    builder.makeShip();
    builder.makeAlien();
    builder.startTimer(TICK);

    return builder.getGame();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameBuilder builder;
    Game *w = createGame(builder);
    w->show();

    int ret = a.exec();
    delete w;
    return ret;
}
