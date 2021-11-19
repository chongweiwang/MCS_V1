#include "multicurvesplot.h"
#include <QDebug>
#include <QColorDialog>


#pragma execution_character_set("utf-8")

MultiCurvesPlot::MultiCurvesPlot(int _curvesCnt, QWidget *parent):
    QCustomPlot(parent)
    ,allCurvesData(_curvesCnt)
    ,curvesCnt(allCurvesData.size())
    ,autoScroll(true)
    ,tracer(new QCPItemTracer(this))
    ,tracerEnable(false)
    ,lastPoint(new QPointF(0, 0))
{
    this->setInteractions(QCP::iRangeDrag //可平移
                          | QCP::iRangeZoom //可滚轮缩放
//                          | QCP::iSelectPlottables //可选中曲线
                          | QCP::iSelectLegend );//可选中图例
    this->setNoAntialiasingOnDrag(true);//禁用抗锯齿，以提高性能
    connect(this, SIGNAL(selectionChangedByUser()), this, SLOT(when_selectionChangedByUser()));
    connect(this, SIGNAL(legendDoubleClick(QCPLegend*, QCPAbstractLegendItem*, QMouseEvent*)), this, SLOT(when_legendDoubleClick(QCPLegend*, QCPAbstractLegendItem*, QMouseEvent*)));

    this->legend->setVisible(true);//使能图例
    this->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignTop|Qt::AlignLeft);//设置图例的位置
    //this->setOpenGl(true);
    qDebug() << "QCustomplot opengl status = " << this->openGl();
    this->yAxis2->setVisible(true);
    connect(yAxis, SIGNAL(rangeChanged(QCPRange)), yAxis2, SLOT(setRange(QCPRange)));//左右y轴同步放缩

    QSharedPointer<QCPAxisTickerFixed> intTicker( new  QCPAxisTickerFixed );
    //设置步长精度
    //intTicker->setTickStep(1);
    //QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);//日期做X轴
    //dateTicker->setDateTimeFormat("hh:mm:ss.zzz\nyyyy-MM-dd");//日期格式(可参考QDateTime::fromString()函数)
    //QDateTime::fromString("");
    //this->xAxis->setTicker(intTicker);//设置X轴为时间轴

    this->xAxis->setLabelColor(Qt::black); //设置坐标颜色
    this->xAxis->setLabelPadding(1);       //设置坐标轴名称文本距离坐标轴刻度线距离
    this->xAxis->setTickLabels(true);//显示刻度标签
    this->xAxis->ticker()->setTickCount(5);




    /*显示数值的提示框*/
    textLabel = new QCPItemText(this);
    textLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);//文本框的原点位置
    textLabel->position->setType(QCPItemPosition::ptAxisRectRatio);//位置类型（当前轴范围的比例）
    textLabel->position->setCoords(0.5, 0); // place position at center/top of axis rect
    textLabel->setText("Text Item Demo");
    textLabel->setFont(QFont(font().family(), 16)); // make font a bit larger
    textLabel->setPen(QPen(Qt::black)); // show black border around text
    textLabel->setPadding(QMargins(2,2,2,2));//边界宽度
    textLabel->setBackgroundColor(Qt::green);

    //单击时指向数值的箭头:
    arrow = new QCPItemLine(this);
    arrow->start->setParentAnchor(textLabel->bottom);    
    //arrow->end->setCoords(4, 1.6); // point to (4, 1.6) in x-y-plot coordinates
    arrow->setHead(QCPLineEnding::esSpikeArrow);
    textLabel->setVisible(false);
    arrow->setVisible(false);

    //游标以及游标文本框设置
    this->setMouseTracking(true);
    tracer->setInterpolating(false);//禁用插值
    tracer->setPen(QPen(Qt::DashLine));//虚线游标
    tracer->setStyle(QCPItemTracer::tsCrosshair);//游标样式：十字星、圆圈、方框等
    //游标的X值文本框
    tracerXText = new QCPItemText(this);
    tracerXText->setPositionAlignment(Qt::AlignBottom|Qt::AlignRight);
    tracerXText->position->setType(QCPItemPosition::ptAxisRectRatio);//位置类型（当前轴范围的比例）
    tracerXText->position->setCoords(0, 0.99); // 在plot中的位置
    tracerXText->position->setParentAnchorX(tracer->position);//X位置锚定到游标的X位置
    tracerXText->setText("no curves..");
    tracerXText->setFont(QFont(font().family(), 12));
    tracerXText->setPen(QPen(Qt::black));
    tracerXText->setBackgroundColor(Qt::green);
    tracerXText->setPadding(QMargins(2,2,2,2));//边界宽度
    tracerXText->setVisible(false);
    //游标的Y值文本框
    tracerYText = new QCPItemText(this);
    tracerYText->setPositionAlignment(Qt::AlignTop|Qt::AlignLeft);
    tracerYText->position->setType(QCPItemPosition::ptAxisRectRatio);//位置类型（当前轴范围的比例）
    tracerYText->position->setCoords(0.01, 0); // 在plot中的位置
    tracerYText->setText("no curves..");
    tracerYText->position->setParentAnchorY(tracer->position);//Y位置锚定到游标的Y位置
    tracerYText->setFont(QFont(font().family(), 12));
    tracerYText->setPen(QPen(Qt::black));
    tracerYText->setBackgroundColor(Qt::green);
    tracerYText->setPadding(QMargins(2,2,2,2));//边界宽度
    tracerYText->setVisible(false);

    /*两点差值的显示框*/
    diffText = new QCPItemText(this);
    diffText->setPositionAlignment(Qt::AlignTop|Qt::AlignRight);
    diffText->setTextAlignment(Qt::AlignTop|Qt::AlignLeft);//文本框的原点位置
    diffText->position->setType(QCPItemPosition::ptAxisRectRatio);//位置类型（当前轴范围的比例）
    diffText->position->setCoords(0.98, 0); // place position at center/top of axis rect
    diffText->setText("dt=0\r\ndy=0");
    diffText->setFont(QFont(font().family(), 16)); // make font a bit larger
    diffText->setPen(QPen(Qt::black)); // show black border around text
    diffText->setPadding(QMargins(2,2,2,2));//边界宽度
    diffText->setBackgroundColor(Qt::green);
    diffText->setVisible(false);

    //右键菜单
    this->setContextMenuPolicy(Qt::ActionsContextMenu);
    //菜单-按钮1
    actshowAllGraph = new QAction("全显");
    connect(actshowAllGraph, SIGNAL(triggered(bool)), this, SLOT(showAllGraph()));
    this->addAction(actshowAllGraph);

    //菜单-按钮2
    actClearDatas = new QAction("清空历史数据");
    connect(actClearDatas, SIGNAL(triggered(bool)), this, SLOT(clearAllData()));
    this->addAction(actClearDatas);




    getColor.resize(curvesCnt);
    for(int i = 0; i < getColor.size();i++)
    {
        getColor[i] = QColor(qrand()%255, qrand()%255, qrand()%255);//生成随机颜色
    }
    getName.resize(curvesCnt);
    for(int i = 0; i < getName.size();i++)//设置默认曲线名称
    {
        getName[i] = QString("curve %1").arg(i);
    }

    for(int i = 0; i < allCurvesData.size();i++)//清空备份的数据源
    {
        allCurvesData[i].keyVec.clear();
        allCurvesData[i].valVec.clear();
    }

    startTimer(30, Qt::CoarseTimer);//定频刷新曲线图
}


/*
QList<uint16_t> idxList;
list << 0 << 3;
plot->showCurves(idxList);//显示出第0和第3条曲线
*/
void MultiCurvesPlot::showCurves(QList<uint16_t> _idxList)
{
//    qDebug() << "MultiCurvesPlot::showCurves";
//    qDebug() << "MultiCurvesPlot::showCurves:" << idxList;

    this->clearGraphs();//先移除所有的graph
    idxList = _idxList;//备份，可能别的函数需要它
    curveIdx2graphPtr.clear();//清除曲线编号->graph指针的映射
    graphPtr2curveIdx.clear();//清除graph指针->曲线编号的映射


    int graphIdx = 0;

    for(QList<uint16_t>::const_iterator it = idxList.begin(); it != idxList.end(); it++)
    {   //*it为每一个要显示的曲线编号
        uint16_t curveIdx = *it;
        if(curveIdx > allCurvesData.size())
        {
            qDebug() << QString("warning: MultiCurvesPlot::showCurves->超出数据源max index").arg(*it);
            continue;
        }

        this->addGraph(this->xAxis, this->yAxis);
        QCPGraph* pGraph = graph(graphIdx);
        curveIdx2graphPtr[curveIdx] = pGraph;//记录：曲线索引->graph指针的映射
        graphPtr2curveIdx[pGraph] = curveIdx;//记录：graph指针->曲线索引的映射
        pGraph->setData(allCurvesData[curveIdx].keyVec, allCurvesData[curveIdx].valVec, true);//数据源 （todo:*it的合法性）
        pGraph->setPen(QPen(getColor[curveIdx]));//线的颜色随机
        pGraph->setLineStyle(QCPGraph::lsStepLeft);//阶梯线样式        
        //this->graph(graphIdx)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));//显示散点
        //this->graph(graphIdx)->setName(getName[curveIdx]);
        graphIdx++;        
    }
    if(graphCount() > 0)
        traceGraph = graph(0);
    else
        traceGraph = NULL;
//    showAllGraph();
    this->replot();
}

void MultiCurvesPlot::storeAxisScope(bool x, bool y)
{
    if(x)
        Xscope = this->xAxis->range();
    if(y)
        Yscope = this->yAxis->range();
}

void MultiCurvesPlot::resumeAxisScope(bool x, bool y)
{

    if(x)
        this->xAxis->setRange(Xscope);
    if(y)
        this->yAxis->setRange(Yscope);

}


void MultiCurvesPlot::setCurveName(int idx, const QString newName)
{
    if(idx < getName.size())
        getName[idx] = newName;
    else
        qDebug() << "warning: MultiCurvesPlot::setCurveName -> invalid idx!";
}
/*
如果参数_nameVec成员过多，则截断；过少则补长
*/
void MultiCurvesPlot::setCurvesName(QVector<QString> _nameVec)
{
    _nameVec.resize(getName.size());//截断或补长
    getName = _nameVec;
}
/*
典型用法示例：
int idx = 0;//曲线0
double x = (double)(QDateTime::currentMSecsSinceEpoch()) / 1000.0;//当前时间
double y = qSin(x);
plot->addData(idx, x, y);
*/
void MultiCurvesPlot::addData(int idx, double x, double y)
{
    if(idx > allCurvesData.size())
    {
        qDebug() << QString("warning:下位机企图绘制编号为%1的曲线！").arg(idx);
        return;
    }
    if(y > 1e10 || y < -1e10)//接收到的异常值直接不绘制
        return;

    allCurvesData[idx].keyVec.append(x);//备份数据源
    allCurvesData[idx].valVec.append(y);

    if(curveIdx2graphPtr.contains(idx))//第idx个曲线正在显示中
    {
        QCPGraph* pGraph = curveIdx2graphPtr[idx];
        pGraph->addData(x, y);
        pGraph->data().data()->removeBefore(0);
    }

    if (x>xMax)
    {
        xMax = x;
    }

}

void MultiCurvesPlot::setDiffSolveEnable(bool enable)
{
    diffText->setVisible(enable);
}

void MultiCurvesPlot::clearAllData()
{
    if(QMessageBox::No == QMessageBox::information(this,
                                "请再次确认",
                                "确认清空吗？",
                                QMessageBox::Yes,
                                QMessageBox::No | QMessageBox::Default))
    {
        return;
    }


    for(int i = 0; i < graphCount(); i++)
    {
        graph(i)->data().data()->clear();
    }

    for(int idx = 0; idx < allCurvesData.size(); idx++)
    {
        allCurvesData[idx].keyVec.clear();
        allCurvesData[idx].valVec.clear();
    }
}

void MultiCurvesPlot::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    if(autoScroll)
    {
        double curSeclf = xMax;//(double)(QDateTime::currentMSecsSinceEpoch()) / 1000.0;
        this->xAxis->setRange(curSeclf - xAxis->range().size(), curSeclf);
    }

    this->replot();
}

/*
1、 按下时，显示按下点的数值，松手时隐藏
2、 计算游标值，并emit
*/
void MultiCurvesPlot::mousePressEvent(QMouseEvent *event)
{
    QCustomPlot::mousePressEvent(event);

    if(event->buttons() & Qt::LeftButton)
    {
        textLabel->setVisible(true);
        arrow->setVisible(true);
        double x = xAxis->pixelToCoord(event->pos().x());//像素坐标转plot坐标
        double y = yAxis->pixelToCoord(event->pos().y());
        arrow->end->setCoords(x, y); // 箭头的终点
        //QString xTime = QDateTime::fromMSecsSinceEpoch(x * 1000.0).toString("hh:mm:ss.zzz");
        textLabel->setText(QString("x = %1\ny=%2").arg(x).arg(y));//箭头框显示点击值

        solveDifference(QPointF(x, y));
    }
}

/*
按下ctrl时，实时显示游标
关联：this->setmouseTracking(true)
*/
void MultiCurvesPlot::mouseMoveEvent(QMouseEvent *event)
{
    QCustomPlot::mouseMoveEvent(event);
//    qDebug() << "mouseMoveEvent";

    if(tracerEnable)//下面这段计算量略大
    {
        if(getGraphs().contains(traceGraph) && traceGraph->data().data()->size() != 0)
        {
            //qDebug() <<  "trace graph point cnt = " << traceGraph->data().data()->size();
            double x = xAxis->pixelToCoord(event->pos().x());//像素坐标转plot坐标
            tracer->setGraph(traceGraph);
            tracer->setGraphKey(x);
            double traceX = tracer->position->key();
            double traceY = tracer->position->value();

            tracerXText->setText(QDateTime::fromMSecsSinceEpoch(traceX * 1000.0).toString("hh:mm:ss.zzz"));//游标文本框
            tracerYText->setText(QString::number(traceY));

            for(int i = 0; i < graphCount(); i++)//计算所有曲线的游标值
            {
                QCPDataContainer<QCPGraphData>::const_iterator coorPoint = graph(i)->data().data()->findBegin(traceX);//搜索左边离traceX最近的key对应的点
                if(graphPtr2curveIdx.contains(graph(i)))
                {
                    int curveIdx = graphPtr2curveIdx[graph(i)];
                    emit noniusValueChanged(curveIdx, QString::number(coorPoint->value));
                }
            }
        }
    }
}
void MultiCurvesPlot::mouseReleaseEvent(QMouseEvent *event)
{
    QCustomPlot::mouseReleaseEvent(event);

    if(event->button() == Qt::LeftButton)
    {
        textLabel->setVisible(false);
        arrow->setVisible(false);
    }
}
//void MultiCurvesPlot::keyPressEvent(QKeyEvent *event)
//{
//    QCustomPlot::keyPressEvent(event);
//}
//void MultiCurvesPlot::keyReleaseEvent(QKeyEvent *event)
//{
//    QCustomPlot::keyReleaseEvent(event);
//}
void MultiCurvesPlot::setColors(QVector<QColor> _colorsIn)
{
    if(_colorsIn.size() != getColor.size())
    {
        qDebug() << "warning: MultiCurvesPlot::setColors failed!";
        return;
    }

    getColor = _colorsIn;
}

/*用户点选了图中的元素时：同步选中曲线和曲线图例*/
void MultiCurvesPlot::when_selectionChangedByUser()
{
    for (int i = 0; i < this->graphCount(); ++i)
     {
       QCPGraph *graph = this->graph(i);

       QCPPlottableLegendItem *item = this->legend->itemWithPlottable(graph);
       if (item->selected() || graph->selected())//选中了哪条曲线或者曲线的图例
       {
//            item->setSelected(true);//同时选中曲线和图例
//            graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
            traceGraph = graph;
       }
     }
}
/*
双击图例修改曲线颜色
*/
void MultiCurvesPlot::when_legendDoubleClick (QCPLegend *legend, QCPAbstractLegendItem *legendItem, QMouseEvent *event)
{
    Q_UNUSED(legend);
    Q_UNUSED(event);

    for (int i = 0; i < this->graphCount(); ++i)
    {
        QCPGraph *pGraph = this->graph(i);
        if(legendItem == this->legend->itemWithPlottable(pGraph))//查询被点击的图例项是哪个graph的图例项
        {
            if(graphPtr2curveIdx.contains(pGraph))
            {
                int curveIdx = graphPtr2curveIdx[pGraph];//被选中的曲线编号
//                qDebug() << QString("被双击的curve %1").arg(curveIdx);
                QColor newColor = QColorDialog::getColor(getColor[curveIdx], //默认颜色
                                                         this,//父窗体
                                                         "选择新颜色");
                if(newColor.isValid())//用户点击了确定
                {
                    getColor[curveIdx] = newColor;
//                    pGraph->setPen(QPen(newColor));//修改颜色，这行代码不管用，原因未知
                    showCurves(idxList);//只能用这个来修改颜色了

                }
            }
            this->deselectAll();//取消所有的点选
            break;
        }
    }
}

/*求差，并更新到显示框*/
void MultiCurvesPlot::solveDifference(QPointF newPoint)
{
    diffText->setText(QString("dt=%1秒\r\ndy=%2")
                      .arg(newPoint.x()-lastPoint->x())
                      .arg(newPoint.y()-lastPoint->y()));
    *lastPoint = newPoint;
}
/*
如果设置了主图，那么本图将被作为副图，本图的X轴范围会跟着主图变
*/
void MultiCurvesPlot::setMainPlot(QCustomPlot *_mainPlot)
{
    //connect
}


void MultiCurvesPlot::setTracerEnable(bool enable)
{
    tracerEnable = enable;
    if(tracerEnable)
    {
        tracer->setVisible(true);
        tracerXText->setVisible(true);
        tracerYText->setVisible(true);
    }
    else
    {
        tracer->setVisible(false);
        tracerXText->setVisible(false);
        tracerYText->setVisible(false);
    }
}

void MultiCurvesPlot::setScatterPointEnable(bool enable)
{
    for (int i = 0; i < this->graphCount(); ++i)
    {
        if(enable)
            this->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));//显示散点
        else
            this->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));//不显示散点
    }
}

const QVector<CurveData> *MultiCurvesPlot::getAllData() const
{
    return &allCurvesData;
}

QString MultiCurvesPlot::getCurveName(int idx)
{
    if(idx < getName.size())
        return getName[idx];
    else
        return "notExist";
}
