#include <QtWidgets/QMainWindow>


namespace cg_gui {


class game : public QMainWindow {
  Q_OBJECT

 public:
  explicit game(QWidget* parent = 0);
  ~game();

};


}  // namespace cg_gui
