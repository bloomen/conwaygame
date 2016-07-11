#include <QtWidgets/QMainWindow>
#include <QtGui/QImage>
#include <QtCore/QTimer>
#include <QtCore/QTime>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGraphicsProxyWidget>
#include <conwaygame/memory.h>
#include <conwaygame/rules.h>
#include "image.h"


namespace cg_gui {


class game : public QWidget {
  Q_OBJECT

 public:
  game();

 private slots:
  void start();
  void stop();
  void update_image();

 private:
  bool running_;
  std::int64_t n_;
  float scale_;
  std::int64_t size_;
  std::mt19937 gen_;
  std::vector<bool> data_;
  std::vector<bool> result_;
  std::unique_ptr<QGraphicsPixmapItem> last_item_;
  std::unique_ptr<QVBoxLayout> vbox_;
  std::unique_ptr<QHBoxLayout> hbox_;
  std::unique_ptr<image> image_;
  std::unique_ptr<QTimer> timer_;
  std::unique_ptr<QPushButton> trigger_button_;
};


}  // namespace cg_gui
