#include "delegate.h"

#define cout  qDebug() << __LINE__ << ":"
Delegate::Delegate(QObject *parent)
    : QItemDelegate(parent)
{
    flag = true;
}

//void Delegate::setFlag(bool f)
//{
//    flag = f;
//}

void Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
    const QModelIndex &index) const
{
    QItemDelegate::paint(painter, option, index);
}

QSize Delegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QItemDelegate::sizeHint(option, index);
}

QWidget *Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    MCombobox *combobox;
    QLineEdit *lineedit;

    if(index.isValid())
    {
        int row = index.row();
        switch (row) {
        case 0:
            combobox = new MCombobox(parent);
            combobox->setEditable(false);
            combobox->installEventFilter(const_cast<Delegate *>(this));
            return combobox;
        case 2:
            combobox = new MCombobox(parent);
            combobox->setEditable(true);
            combobox->setInsertPolicy(QComboBox::InsertAtTop);
            combobox->installEventFilter(const_cast<Delegate *>(this));
            combobox->setObjectName("combobox");
            return combobox;
        default:
            lineedit = new QLineEdit(parent);
            lineedit->setFrame(false);
            lineedit->installEventFilter(const_cast<Delegate *>(this));
            return lineedit;
        }
    }

    return QItemDelegate::createEditor(parent, option, index);
}

void Delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    MCombobox *combobox;
    QLineEdit *lineedit;
    if (index.isValid())
    {
        int row = index.row();
        QString value = index.model()->data(index, Qt::DisplayRole).toString();

        switch (row) {
        case 0:
            combobox = static_cast<MCombobox *>(editor);
            combobox->addItem("档位");
            combobox->addItem("连续可调");
            combobox->setCurrentText(value);
            break;
        case 2:
            combobox = static_cast<MCombobox *>(editor);
            foreach (QString str, strlist)
            {
                combobox->addItem(str);
            }
            combobox->setCurrentText(value);
            break;
        default:
            lineedit = static_cast<QLineEdit *>(editor);
            lineedit->setText(value);
            break;
        }
    }else
    {
        QItemDelegate::setEditorData(editor, index);
    }
}

void Delegate::setModelData(QWidget *editor,
                            QAbstractItemModel *model,
                      const QModelIndex &index) const
{
    MCombobox *combobox;
    QLineEdit *lineedit;
    if (index.isValid())
    {
        int row = index.row();

        switch (row) {
        case 0:
            combobox = static_cast<MCombobox *>(editor);
            model->setData(index, combobox->currentText());
            break;
        case 2:
            combobox = static_cast<MCombobox *>(editor);
            model->setData(index, combobox->currentText());
            break;
        default:
            lineedit = static_cast<QLineEdit *>(editor);
            model->setData(index,lineedit->text());
            break;
        }
    }
    else
    {
        QItemDelegate::setModelData(editor, model, index);
    }
}
bool Delegate::editorEvent(QEvent *event,
                           QAbstractItemModel *model,
                     const QStyleOptionViewItem &option,
                     const QModelIndex &index)
{
    //QString value = index.model()->data(index, Qt::DisplayRole).toString();
    //cout<<"setModeldata";
    if (index.isValid())
    {
        int row = index.row();

        switch (row) {
        case 0:
            break;
        default:
            break;
        }
    }

    return QItemDelegate::editorEvent(event,model,option,index);
}
bool Delegate::eventFilter(QObject *editor, QEvent *event)
{
    //cout<<event->type();

    if(event->type() == QEvent::KeyPress)
    {
        if(editor->objectName() == "combobox")
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            QComboBox *x = static_cast<MCombobox *>(editor);
            QString s = x->currentText();

            if (keyEvent->key() == Qt::Key_Return) {
                if(s.isEmpty()){
                    if(!strlist.isEmpty())
                    {
                        x->setCurrentText(strlist.first());
                        return true;
                    }else
                        return true;
                }
                if(!strlist.contains(s))
                {
                    strlist.append(s);
                }
            } else if(keyEvent->key() == Qt::Key_Delete){
                if(strlist.contains(s))
                {
                    strlist.removeOne(s);
                    x->clearEditText();
                    if(!strlist.isEmpty())
                    {
                        x->removeItem(x->currentIndex());
                        x->setCurrentText(strlist.first());
                    }
                }
            }
        }
    }

    return QItemDelegate::eventFilter(editor,event);
}

void Delegate::strListAssignment(QList<QString> l)
{
    strlist.clear();
    strlist = l;
}

void Delegate::GetStrList(QList<QString> &l)
{
    l = strlist;
}

Delegate::~Delegate()
{

}
