#include <QtGui/QApplication>
#include <QtGui/QTextEdit>


int main(int argv, char **args) {
  QApplication app(argv, args);

  QTextEdit textEdit;
  textEdit.show();

  return app.exec();
}
