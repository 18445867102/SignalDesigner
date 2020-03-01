#include "moduledata.h"

#define cout  qDebug() << __LINE__ << ":"
ModuleData::ModuleData()
{
    moduleName = "module name";
    inputNum = 1;
    outputNum = 1;
    moduleNum = 5;
    inputVoltageMin = 0;
    inputVoltageMax = 3.3;
    outputVoltageMin = 0;
    outputVoltageMax = 3.3;
    inputResistance = 1000000;
    outputResistance = 20;

//    property<<nullptr;
}
ModuleData::ModuleData(const ModuleData &m)
{
    moduleName = m.moduleName;
    moduleNum = m.moduleNum;
    inputNum = m.inputNum;
    outputNum = m.outputNum;

    inputVoltageMin = m.inputVoltageMin;
    inputVoltageMax = m.inputVoltageMax;
    outputVoltageMin = m.outputVoltageMin;
    outputVoltageMax = m.outputVoltageMax;
    inputResistance = m.inputResistance;
    outputResistance = m.outputResistance;
    parameterList = m.parameterList;
}

ModuleData& ModuleData::operator=(const ModuleData& m)
{
    if(this != &m){
        moduleName = m.moduleName;
        moduleNum = m.moduleNum;
        inputNum = m.inputNum;
        outputNum = m.outputNum;
        inputVoltageMin = m.inputVoltageMin;
        inputVoltageMax = m.inputVoltageMax;
        outputVoltageMin = m.outputVoltageMin;
        outputVoltageMax = m.outputVoltageMax;
        inputResistance = m.inputResistance;
        outputResistance = m.outputResistance;
        parameterList = m.parameterList;
    }

    return *this;
}
ParameterData* ModuleData::addParameter()
{
    ParameterData *p = new ParameterData;

    parameterList.append(p);

    return p;
}

void ModuleData::decParameter(int row)
{
    if(row >= 0 && row < parameterList.size())
    {
        delete parameterList.takeAt(row);
    }
}
ModuleData::~ModuleData()
{

}
