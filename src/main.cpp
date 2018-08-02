#include <QtWidgets/QApplication>
#include "game.h"


int main(int argc, char** argv) {
  QApplication app(argc, argv);
  cg_gui::game game;
  game.show();
  return app.exec();
}
