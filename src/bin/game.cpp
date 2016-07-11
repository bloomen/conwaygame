#include "conwaygame_gui/game.h"
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QApplication>
#include <iostream>


namespace cg_gui {


game::game()
: QWidget{},
  running_{false},
  n_{100}, scale_{5.f}, size_{n_*scale_ + 10}, gen_{1},
  data_(cg::random_vector(gen_, n_*n_, 0.33)),
  result_(data_.size()),
  last_item_{},
  vbox_{cg::make_unique<QVBoxLayout>()},
  hbox_{cg::make_unique<QHBoxLayout>()},
  image_{cg::make_unique<image>()},
  timer_{cg::make_unique<QTimer>(this)},
  trigger_button_{cg::make_unique<QPushButton>()}
{
  vbox_->addWidget(trigger_button_.get());
  vbox_->addWidget(image_.get());
  vbox_->addLayout(hbox_.get());
  setLayout(vbox_.get());

  resize(size_, size_ + 50);
  connect(timer_.get(), SIGNAL(timeout()), this, SLOT(update_image()));
  connect(trigger_button_.get(), SIGNAL(clicked()), this, SLOT(start()));
  connect(trigger_button_.get(), SIGNAL(clicked()), this, SLOT(stop()));
  trigger_button_->setText("Start");
}

void game::start() {
  if (!running_) {
    std::cout<<__func__<<std::endl;
    timer_->start(10);
    trigger_button_->setText("Stop");
  }
}

void game::stop() {
  if (running_) {
    std::cout<<__func__<<std::endl;
    timer_->stop();
    trigger_button_->setText("Start");
    running_ = false;
  }
}

void game::update_image() {
  std::cout<<__func__<<std::endl;
  QTime time;
  time.start();

  cg::next_generation(result_, data_, n_);
  data_ = result_;
  std::cout<<time.elapsed()<<std::endl;

  image_->update_image(result_, n_, scale_);

  std::cout<<time.elapsed()<<std::endl;
  timer_->setInterval(time.elapsed());
  running_ = true;
}


}  // namespace cg_gui
