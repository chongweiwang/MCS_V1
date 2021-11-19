#include "waveplot.h"


WavePlot::WavePlot(QWidget *parent) : QWidget(parent)
{

    mBoxLayout = new QVBoxLayout(parent);


//    double x[20] = {1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,1,2};
//    double y[20] = {1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,1,2};
//    WaveInfo info;
//    strcpy(info.name, "wangchongwei");
//    addSimWave(info, x, y, 20);
//    mWaveCnt = 4;
//    SimWavePlot();
}

static WavePlot * mSelf;
WavePlot *WavePlot::instance()
{
    if(mSelf == nullptr) {
        mSelf = new WavePlot();
    }

    return mSelf;
}

void WavePlot::init(WavePlot* self)
{
    mSelf = self;
}
WavePlot::~WavePlot()
{
    // delete
}


void WavePlot::addSimWave(WaveInfo info,double *x, double *y, uint32_t size)
{
    WaveProp prop;

    prop.info = info;
    prop.x = x;
    prop.y = y;
    prop.size = size;

    mPropList.append(prop);
}

void WavePlot::setSimWaveCnt(uint32_t cnt)
{
    mWaveCnt = cnt;
}

void WavePlot::SimWavePlot(void)
{
    // 波形界面
    for (int i = 0; i<mWaveCnt; i++) {
        MultiCurvesPlot *plot = new MultiCurvesPlot(1,this);
        QList<uint16_t> list ;
        list<<0;

        plot->showCurves(list);

        mMcPlotList.append(plot);
        mBoxLayout->addWidget(plot);
    }

    // 添加波形
    for (int i = 0; i<mWaveCnt; i++)
    {
        if (mPropList.size()<=i) break;

        QString name = QString(QLatin1String(mPropList[i].info.name));

        mMcPlotList[i]->graph(0)->setName(name);

        uint32_t tmp_size = mPropList[i].size;

        for (uint32_t cnt = 0; cnt < tmp_size; cnt++)
        {
            mMcPlotList[i]->addData(0,mPropList[i].x[cnt],mPropList[i].y[cnt]);
        }
        mMcPlotList[i]->setAutoScroll(false);
        mMcPlotList[i]->showAllGraph();
    }
}
