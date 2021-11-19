#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "app/sim_main.h"
#include "waveplot.h"

#define CURVE_CNT 5 //最多显示几条曲线

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    WavePlot *wave = new WavePlot(ui->scrollAreaWidgetContents);
    wave->init(wave);

    // 仿真
    sim_main();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

}



//void MainWindow::on_checkBox_tracer_clicked(bool checked)
//{
//    plot->setTracerEnable(checked);
//}

//void MainWindow::on_checkBox_scroll_clicked(bool checked)
//{
//    plot->setAutoScroll(checked);
//}
