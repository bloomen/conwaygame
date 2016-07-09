#include "conwaygame_gui/game.h"
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QApplication>


namespace cg_gui {


game::game()
: QMainWindow{},
  n_{250}, gen_{1},
  data_(cg::random_vector(gen_, n_*n_, 0.1)),
  scene_{cg::make_unique<QGraphicsScene>(this)},
  view_{cg::make_unique<QGraphicsView>(scene_.get(), this)},
  timer_{cg::make_unique<QTimer>(this)}
{
  resize(n_*3 + 50, n_*3 + 50);
  view_->resize(n_*3 + 50, n_*3 + 50);

  connect(timer_.get(), SIGNAL(timeout()), this, SLOT(update_image()));
  timer_->start(100);
}

void game::update_image() {
  data_ = cg::next_generation(data_, n_, 8);

  QImage image(n_, n_, QImage::Format_Mono);

  QRgb white = qRgb(255, 255, 255);
  image.setColor(1, white);

  QRgb black = qRgb(0, 0, 0);
  image.setColor(0, black);

  for (std::int64_t i=0; i < n_; ++i) {
    for (std::int64_t j=0; j < n_; ++j) {
      image.setPixel(i, j, data_[i*n_ + j]);
    }
  }

  scene_->addPixmap(QPixmap::fromImage(image.scaled(n_*3, n_*3)));
}


}  // namespace cg_gui
