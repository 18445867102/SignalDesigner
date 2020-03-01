#ifndef MODULEDATA_H
#define MODULEDATA_H

#include <QtWidgets>
#include "mclass/parameterdata.h"

class ModuleData
{
public:
    ModuleData();
    ~ModuleData();
    ModuleData(const ModuleData &m);
    ModuleData& operator=(const ModuleData& m);
    ParameterData* addParameter();
    void decParameter(int);

    QString moduleName;        //模块名称
    qint32 inputNum;           //输入通道数
    qint32 outputNum;          //输出通道数
    qint32 moduleNum;          //模块数量
    qreal inputVoltageMin;     //输入电压最小值
    qreal inputVoltageMax;     //输入电压最大值
    qreal outputVoltageMin;    //输出电压最小值
    qreal outputVoltageMax;    //输出电压最大值
    quint32 inputResistance;   //输入电阻
    quint32 outputResistance;  //输出电阻

    QList<ParameterData*> parameterList;//参数列表
};

#endif // MODULEDATA_H
