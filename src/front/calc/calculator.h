#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QDebug>
#include <QMainWindow>
#include <QVector>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../backend/s21_smartcalc.h"
#ifdef __cplusplus
}
#endif

QT_BEGIN_NAMESPACE
namespace Ui {
class CALCULATOR;
}
QT_END_NAMESPACE

class CALCULATOR : public QMainWindow {
  Q_OBJECT

 public:
  CALCULATOR(QWidget *parent = nullptr);
  ~CALCULATOR();

 private:
  Ui::CALCULATOR *ui;

 signals:
 private slots:
  void digitsSlot();
  void digitsSlot_function();
  void all_delete();
  void equaleSlot();
  void drawgrafic();
};
#endif  // CALCULATOR_H
