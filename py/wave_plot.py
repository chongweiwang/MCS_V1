'''
波形打印参考陈嘉豪大佬的py：https://github.com/horychen/ACMSIMC_TUT/tree/vvvf

设计
1. 输入的是c语言传入的list，和list的数量
// 其实这里我更想是传入的是地址，和长度。暂时不知道怎么实现，就先打算用值传递。

2. 得到list后，需要进行一次处理。主要是波形的设置什么的。

3. 还需要设计一个传入每条波形的名字

就有个问题，多次调用，下次调用使用上次调用的传参数，是不是就没有了。

解决这个问题，py写成一个类。
'''
import pylab 


def get_axis(c, r):
    fig, axes = pylab.subplots(c, r, sharex=True, figsize=(16*0.8, 9*0.8), dpi=80, facecolor='w', edgecolor='k')
    fig.subplots_adjust(right=0.95, bottom=0.1, top=0.95, hspace=0.2, wspace=0.02)
    if (c+r)<=2:
        return axes
    else:
        return axes.ravel()

class MPlot:
    constructor = 0

    __name = [[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],
              [],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],
              [],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],
              [],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],
              [],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[]]

    __xList = [[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],
              [],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],
              [],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],
              [],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],
              [],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[]]

    __yList = [[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],
            [],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],
            [],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],
            [],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],
            [],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[]]

    __count = 0

    def __init__(self,init):
        self.init = init
        MPlot.constructor += 1
        print("__init__")


    def plt(self):
        if self.__count>1:
            for i in range(self.__count):
                self.ax_list[i].plot(self.__xList[i], self.__yList[i])
                self.ax_list[i].set_ylabel(self.__name[i])
        elif self.__count == 1:
            self.ax_list.plot(self.__xList[0], self.__yList[0])
        else:
            print("no wave")

        pylab.show()

    def addWave(self, id, xlist, ylist):
        self.__xList[id] = xlist
        self.__yList[id] = ylist

    def setWaveNum(self, count):
        self.__count = count
        print (count)
        self.ax_list = get_axis(count,1)

    def setWaveName(self, id, name):
        self.__name[id] = name


'''
lt = [1,2,3,4,5,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4]
list1 = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21]

pl = MPlot(0)

pl.setWaveName(0,"ch0")
pl.setWaveName(1,"ch1")
pl.setWaveName(2,"ch2")
pl.addWave(0,lt,list1)
pl.addWave(1,lt,list1)
pl.addWave(2,lt,list1)
pl.setWaveNum(3)

pl.plt() 
'''
