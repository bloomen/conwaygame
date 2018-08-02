#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtOpenGL/QGLWidget>
#include "memory.h"


namespace cg_gui {


class image : public QGLWidget {
  Q_OBJECT

 public:
  explicit image(QWidget* parent = nullptr);

  void update_image(const std::vector<bool>& data, int n, double scale);

 private:
  std::unique_ptr<QGraphicsPixmapItem> last_item_;
  std::unique_ptr<QGraphicsScene> scene_;
  std::unique_ptr<QGraphicsView> view_;
};


}  // namespace cg_gui
