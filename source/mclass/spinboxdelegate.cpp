#include "SpinBoxDelegate.h"
#define cout  qDebug() << __LINE__ << ":"
SpinBoxDelegate::SpinBoxDelegate(QObject *parent) : QItemDelegate(parent)
{

}
QWidget *SpinBoxDelegate::createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
    QSpinBox *spinbox;

    if(index.isValid())
    {
        spinbox = new QSpinBox(parent);
        spinbox->setFrame(false);
        spinbox->installEventFilter(const_cast<SpinBoxDelegate *>(this));
        return spinbox;
    }
    else
    {
        return QItemDelegate::createEditor(parent, option, index);
    }
}

void SpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QSpinBox *spinbox;
    if (index.isValid())
    {
        int value = index.model()->data(index, Qt::DisplayRole).toInt();

        spinbox = static_cast<QSpinBox *>(editor);
        spinbox->setValue(value);
    }
    else
    {
        QItemDelegate::setEditorData(editor, index);
    }
}

void SpinBoxDelegate::setModelData(QWidget *editor,
                            QAbstractItemModel *model,
                      const QModelIndex &index) const
{
    if (index.isValid())
    {
        QSpinBox *spinbox = static_cast<QSpinBox *>(editor);
        model->setData(index, spinbox->value());
    }
    else
    {
        QItemDelegate::setModelData(editor, model, index);
    }
}
