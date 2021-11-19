#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "multicurvesplot.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
      void timerEvent(QTimerEvent *event);


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
