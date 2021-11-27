#ifndef WAVEPLOT_H
#define WAVEPLOT_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include "multicurvesplot.h"

struct WaveInfo
{
    uint16_t id;     // 序号
    uint16_t ch;     // 通道
    char unit[128];  // 单位
    char name[128];  // 名字
};


struct WaveProp
{
    struct WaveInfo info;

    float *x;
    float *y;

    uint32_t size;
};


class WavePlot:public QWidget
{
    Q_OBJECT
public:
    explicit WavePlot(QWidget *parent= nullptr);
    ~WavePlot();

    static void init(WavePlot* self);
    static WavePlot *instance();

    // 接口
    void addSimWave(WaveInfo info,float *x, float *y, uint32_t size);

    void setSimWaveCnt(uint32_t cnt);
    void SimWavePlot(void);


private:


    int16_t mWaveCnt;

    QList<WaveProp> mPropList;
    QList<MultiCurvesPlot *> mMcPlotList;

    QVBoxLayout *mBoxLayout;



};

#endif // WAVEPLOT_H
