#include "conwaygame_gui/game.h"
#include <QtGui/QFont>
#include <QtGui/QFontDatabase>
#include <iostream>
#include <ctime>
#include <sstream>


namespace cg_gui {


game::game()
: QWidget{},
  running_{false},
  image_size_{650},
  n_{100},
  ratio_alive_{0.1},
  gen_{static_cast<unsigned long>(std::time(0))},
  min_rate_{1000},
  current_rate_{},
  data_(),
  result_(),
  last_item_{},
  vbox_{cg::make_unique<QVBoxLayout>()},
  hbox_{cg::make_unique<QHBoxLayout>()},
  image_{cg::make_unique<image>()},
  timer_{cg::make_unique<QTimer>(this)},
  trigger_button_{cg::make_unique<QPushButton>()},
  restart_button_{cg::make_unique<QPushButton>()},
  speed_slider_{cg::make_unique<QSlider>(Qt::Horizontal)},
  size_spinbox_{cg::make_unique<QSpinBox>()},
  alive_spinbox_{cg::make_unique<QDoubleSpinBox>()},
  speed_label_{cg::make_unique<QLabel>()},
  cells_label_{cg::make_unique<QLabel>()},
  alive_label_{cg::make_unique<QLabel>()}
{
  const auto fixed_font = QFontDatabase::systemFont(QFontDatabase::FixedFont);

  hbox_->addWidget(trigger_button_.get());
  hbox_->addWidget(restart_button_.get());
  hbox_->addWidget(cells_label_.get());
  hbox_->addWidget(size_spinbox_.get());
  hbox_->addWidget(alive_label_.get());
  hbox_->addWidget(alive_spinbox_.get());
  hbox_->addWidget(speed_label_.get());
  hbox_->addWidget(speed_slider_.get());
  vbox_->addWidget(image_.get());
  vbox_->addLayout(hbox_.get());
  setLayout(vbox_.get());

  resize(image_size_, image_size_ + 50);

  connect(timer_.get(), SIGNAL(timeout()), this, SLOT(update_image()));
  connect(timer_.get(), SIGNAL(timeout()), this, SLOT(update_speed()));
  connect(trigger_button_.get(), SIGNAL(clicked()), this, SLOT(start()));
  connect(trigger_button_.get(), SIGNAL(clicked()), this, SLOT(stop()));
  connect(restart_button_.get(), SIGNAL(clicked()), this, SLOT(restart()));
  connect(speed_slider_.get(), SIGNAL(valueChanged(int)), this, SLOT(update_rate(int)));
  connect(size_spinbox_.get(), SIGNAL(valueChanged(int)), this, SLOT(update_size(int)));
  connect(alive_spinbox_.get(), SIGNAL(valueChanged(double)), this, SLOT(update_alive(double)));

  speed_slider_->setValue(49);

  alive_spinbox_->setFont(fixed_font);
  alive_spinbox_->setMinimum(0);
  alive_spinbox_->setMaximum(1);
  alive_spinbox_->setDecimals(2);
  alive_spinbox_->setSingleStep(0.01);
  alive_spinbox_->setValue(ratio_alive_);

  size_spinbox_->setFont(fixed_font);
  size_spinbox_->setMinimum(10);
  size_spinbox_->setMaximum(1000);
  size_spinbox_->setValue(n_);

  timer_->setInterval(current_rate_);
  trigger_button_->setFont(fixed_font);
  trigger_button_->setText("Start");
  restart_button_->setFont(fixed_font);
  restart_button_->setText("Restart");

  speed_label_->setFont(fixed_font);
  cells_label_->setFont(fixed_font);
  cells_label_->setText("Cells");
  alive_label_->setFont(fixed_font);
  alive_label_->setText("Alive");

  update_speed();
  restart();
}

void game::start() {
  if (!running_) {
    std::cout<<__func__<<std::endl;
    timer_->start(current_rate_);
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

void game::restart() {
  std::cout<<__func__<<std::endl;
  data_ = cg::random_vector(gen_, n_*n_, ratio_alive_);
  result_ = data_;
  if (!running_) {
    update_image();
    running_ = false;
  }
}

void game::update_image() {
  QTime time;
  time.start();
  cg::next_generation(result_, data_, n_);
  data_ = result_;
  image_->update_image(result_, n_, image_size_ / static_cast<double>(n_));
  const auto current_max_rate = time.elapsed();
  if (current_rate_ < current_max_rate) {
    current_rate_ = current_max_rate;
    speed_slider_->setValue(100. - current_rate_ / min_rate_ * 100.);
  }
  timer_->setInterval(current_rate_);
  running_ = true;
}

void game::update_speed() {
  const auto speed = 1000. / current_rate_;
  std::ostringstream ss;
  ss.precision(1);
  ss << std::fixed << speed;
  std::string text = ss.str() + " gen/s";
  while (text.size() < 11)
    text = " " + text;
  speed_label_->setText(QString::fromStdString(text));
}

void game::update_rate(int value) {
  std::cout<<__func__<<std::endl;
  current_rate_ = min_rate_ * (100 - value) / 100.;
  update_speed();
  std::cout<<current_rate_<<std::endl;
}

void game::update_size(int value) {
  std::cout<<__func__<<std::endl;
  n_ = value;
  restart();
  std::cout<<n_<<std::endl;
}

void game::update_alive(double value) {
  std::cout<<__func__<<std::endl;
  ratio_alive_ = value;
  restart();
  std::cout<<ratio_alive_<<std::endl;
}

}  // namespace cg_gui
