#include <QtCore/QTimer>
#include <QtCore/QTime>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QLabel>
#include "memory.h"
#include "rules.h"
#include "image.h"


namespace cg_gui {


class game : public QWidget {
  Q_OBJECT

 public:
  game();

 private slots:
  void start();
  void stop();
  void restart();
  void update_image();
  void update_speed();
  void update_rate(int value);
  void update_size(int value);
  void update_alive(double value);

 private:
  bool app_started_;
  bool running_;
  int image_size_;
  int n_;
  double ratio_alive_;
  std::mt19937 gen_;
  double min_rate_;
  double current_rate_;
  std::vector<bool> data_;
  std::vector<bool> result_;
  std::unique_ptr<QGraphicsPixmapItem> last_item_;
  std::unique_ptr<QVBoxLayout> vbox_;
  std::unique_ptr<QHBoxLayout> hbox_;
  std::unique_ptr<image> image_;
  std::unique_ptr<QTimer> timer_;
  std::unique_ptr<QPushButton> trigger_button_;
  std::unique_ptr<QPushButton> restart_button_;
  std::unique_ptr<QSlider> speed_slider_;
  std::unique_ptr<QSpinBox> size_spinbox_;
  std::unique_ptr<QDoubleSpinBox> alive_spinbox_;
  std::unique_ptr<QLabel> speed_label_;
  std::unique_ptr<QLabel> cells_label_;
  std::unique_ptr<QLabel> alive_label_;
};


}  // namespace cg_gui
