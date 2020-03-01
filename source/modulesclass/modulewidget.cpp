#include "modulewidget.h"

#define cout  qDebug() << __LINE__ << ":"
ModuleWidget::ModuleWidget(QVariantMap obj, QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle("参数设置");
    setMinimumWidth(340);
    setWindowModality(Qt::ApplicationModal);

    QJsonArray parameters = obj.value("parameters").toJsonArray();
    int parameterCount = parameters.size();

    QVBoxLayout *hlayout = new QVBoxLayout;
    for(int i = 0; i<parameterCount; i++)
    {
        QJsonObject parameter = parameters.at(i).toObject();
        QLabel *l = new QLabel;
        l->setText(parameter.value("parameterName").toString()+':');
        if(parameter.value("form").toBool()){//combobox
            //cout<<"combobox";

            QComboBox *c = new QComboBox;
            QJsonArray array = parameter.value("combobox").toArray();
            int arrayCount = array.size();
            for(int j=0; j<arrayCount; j++){
                QString str = array.at(j).toString();
                c->addItem(str);
            }
            QHBoxLayout *h = new QHBoxLayout;
            h->addWidget(l);
            h->addWidget(c);
            hlayout->addLayout(h);
        } else {//spinbox
            //cout<<"spinbox";

            QDoubleSpinBox *s = new QDoubleSpinBox;
            //cout<<parameter.value("max").toDouble()<<parameter.value("min").toDouble()<<
            //      parameter.value("step").toDouble();
            s->setMaximum(parameter.value("max").toDouble());
            s->setMinimum(parameter.value("min").toDouble());
            s->setSingleStep(parameter.value("step").toDouble());
            QHBoxLayout *h = new QHBoxLayout;
            h->addWidget(l);
            h->addWidget(s);
            hlayout->addLayout(h);
        }
    }

    confirm = new QPushButton("确认");
    cancel = new QPushButton("取消");
    QHBoxLayout *buttonlayout = new QHBoxLayout;
    buttonlayout->addStretch();
    buttonlayout->addWidget(confirm);
    buttonlayout->addWidget(cancel);

    hlayout->addLayout(buttonlayout);
    setLayout(hlayout);

    connect(confirm,SIGNAL(clicked()),this,SLOT(slot_buttonConfirm()));
    connect(cancel,SIGNAL(clicked()),this,SLOT(slot_buttonCancel()));
}

void ModuleWidget::slot_buttonConfirm()
{
    close();
}

void ModuleWidget::slot_buttonCancel()
{
    close();
}

ModuleWidget::~ModuleWidget()
{
}
