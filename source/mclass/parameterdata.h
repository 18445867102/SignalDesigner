#ifndef PARAMETERDATA_H
#define PARAMETERDATA_H

#include <QtWidgets>

class ParameterData
{
public:
    ParameterData();
    ParameterData(const ParameterData& m);
    ParameterData&operator=(const ParameterData& c);

    bool f_form;
    QString parameterName;
    QString parameterUnit;
    QStringList l;
    qreal maximum;
    qreal minimum;
    qreal step;
};

#endif // PARAMETERDATA_H
