#include <QtWidgets/QMainWindow>
#include <QtGui/QImage>
#include <QtCore/QTimer>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <conwaygame/memory.h>
#include <conwaygame/rules.h>


namespace cg_gui {


class game : public QMainWindow {
  Q_OBJECT

 public:
  game();

 private slots:
  void update_image();

 private:
  std::int64_t n_;
  std::mt19937 gen_;
  std::vector<bool> data_;
  std::unique_ptr<QGraphicsScene> scene_;
  std::unique_ptr<QGraphicsView> view_;
  std::unique_ptr<QTimer> timer_;
};


}  // namespace cg_gui
