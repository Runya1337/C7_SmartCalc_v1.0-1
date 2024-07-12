#include <QApplication>

#include "calculator.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  CALCULATOR w;
  w.show();
  return a.exec();
}
