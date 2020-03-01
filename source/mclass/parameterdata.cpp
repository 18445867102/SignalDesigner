#include "parameterdata.h"

ParameterData::ParameterData()
{
    f_form = false;
    parameterName = "parameter name";
    parameterUnit = "";
    l<<"一"<<"二";
    maximum = 100;
    minimum = 0;
    step = 1.0;
}

ParameterData::ParameterData(const ParameterData& m)
{
    f_form = m.f_form;
    parameterName = m.parameterName;
    parameterUnit = m.parameterUnit;
    l = m.l;
    maximum = m.maximum;
    minimum = m.minimum;
    step = m.step;
}

ParameterData& ParameterData::operator=(const ParameterData& m)
{
    if(this != &m)//防止对象给自己赋值

    {
        f_form = m.f_form;
        parameterName = m.parameterName;
        parameterUnit = m.parameterUnit;
        l = m.l;
        maximum = m.maximum;
        minimum = m.minimum;
        step = m.step;

    }

    return *this;
}
