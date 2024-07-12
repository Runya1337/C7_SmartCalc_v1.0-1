#include "calculator.h"

#include "ui_calculator.h"

CALCULATOR::CALCULATOR(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CALCULATOR) {
  ui->setupUi(this);
  connect(ui->AC, SIGNAL(clicked()), this, SLOT(all_delete()));
  connect(ui->asin, SIGNAL(clicked()), this, SLOT(digitsSlot_function()));
  connect(ui->acos, SIGNAL(clicked()), this, SLOT(digitsSlot_function()));
  connect(ui->atan, SIGNAL(clicked()), this, SLOT(digitsSlot_function()));
  connect(ui->bracket_open, SIGNAL(clicked()), this, SLOT(digitsSlot()));
  connect(ui->Bracket_close, SIGNAL(clicked()), this, SLOT(digitsSlot()));
  connect(ui->deg, SIGNAL(clicked()), this, SLOT(digitsSlot_function()));
  connect(ui->cos, SIGNAL(clicked()), this, SLOT(digitsSlot_function()));
  connect(ui->div, SIGNAL(clicked()), this, SLOT(digitsSlot()));
  connect(ui->dot, SIGNAL(clicked()), this, SLOT(digitsSlot()));
  connect(ui->eight, SIGNAL(clicked()), this, SLOT(digitsSlot()));
  connect(ui->five, SIGNAL(clicked()), this, SLOT(digitsSlot()));
  connect(ui->four, SIGNAL(clicked()), this, SLOT(digitsSlot()));
  connect(ui->ln, SIGNAL(clicked()), this, SLOT(digitsSlot_function()));
  connect(ui->log, SIGNAL(clicked()), this, SLOT(digitsSlot_function()));
  connect(ui->minus, SIGNAL(clicked()), this, SLOT(digitsSlot()));
  connect(ui->mod, SIGNAL(clicked()), this, SLOT(digitsSlot()));
  connect(ui->mul, SIGNAL(clicked()), this, SLOT(digitsSlot()));
  connect(ui->nine, SIGNAL(clicked()), this, SLOT(digitsSlot()));
  connect(ui->one, SIGNAL(clicked()), this, SLOT(digitsSlot()));
  connect(ui->plus, SIGNAL(clicked()), this, SLOT(digitsSlot()));
  connect(ui->seven, SIGNAL(clicked()), this, SLOT(digitsSlot()));
  connect(ui->sin, SIGNAL(clicked()), this, SLOT(digitsSlot_function()));
  connect(ui->six, SIGNAL(clicked()), this, SLOT(digitsSlot()));
  connect(ui->sqrt, SIGNAL(clicked()), this, SLOT(digitsSlot_function()));
  connect(ui->tan, SIGNAL(clicked()), this, SLOT(digitsSlot_function()));
  connect(ui->three, SIGNAL(clicked()), this, SLOT(digitsSlot()));
  connect(ui->two, SIGNAL(clicked()), this, SLOT(digitsSlot()));
  connect(ui->zero, SIGNAL(clicked()), this, SLOT(digitsSlot()));
  connect(ui->equale, SIGNAL(clicked()), this, SLOT(equaleSlot()));
  connect(ui->x_place, SIGNAL(clicked()), this, SLOT(digitsSlot()));
  connect(ui->grafic, SIGNAL(clicked()), this, SLOT(drawgrafic()));
}

CALCULATOR::~CALCULATOR() { delete ui; }

void CALCULATOR::drawgrafic() {
  QByteArray cppstring = ui->Main_Field->text().toUtf8();
  char *string_final = cppstring.data();
  double xBegin, xEnd, h, X, Y, yBegin, yEnd;
  int error = 0;
  QVector<double> x, y;

  h = 0.01;
  xBegin = ui->x_min_value->value();
  xEnd = ui->x_max_value->value();
  yBegin = ui->y_min_value->value();
  yEnd = ui->y_max_value->value();

  ui->widget->xAxis->setRange(xBegin, xEnd);
  ui->widget->yAxis->setRange(yBegin, yEnd);
  for (X = xBegin; X <= xEnd; X += h) {
    Y = s21_smart_calc_func(string_final, X, &error);
    if (error != 1) {
      x.push_back(X);
      y.push_back(Y);
    }
    error = 0;
  }
  ui->widget->clearGraphs();
  ui->widget->addGraph();
  ui->widget->graph(0)->addData(x, y);
  ui->widget->replot();
}

void CALCULATOR::equaleSlot() {
  QByteArray cppstring = ui->Main_Field->text().toUtf8();
  char *string_final = cppstring.data();
  int error = 0;
  double answer = 0;
  qDebug() << ui->x_value->value();
  answer = s21_smart_calc_func(string_final, ui->x_value->value(), &error);
  printf("error - %d", error);
  if (error == 0) {
    if (fabs(answer) == INFINITY || answer != answer) {
      error = 1;
    }
  }

  if (error == 1) {
    ui->Main_Field->setText("BRUH");
  } else {
    ui->Main_Field->setText(QString::number(answer, 'g', 7));
  }
}

void CALCULATOR::digitsSlot() {
  QPushButton *button = (QPushButton *)sender();
  ui->Main_Field->setText(ui->Main_Field->text() + button->text());
}

void CALCULATOR::digitsSlot_function() {
  QPushButton *button = (QPushButton *)sender();
  ui->Main_Field->setText(ui->Main_Field->text() + button->text() + "(");
}

void CALCULATOR::all_delete() { ui->Main_Field->setText(""); }
