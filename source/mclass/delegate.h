#ifndef DELEGATE_H
#define DELEGATE_H

#include "mcombobox.h"

#include <QtWidgets>


class Delegate : public QItemDelegate
{
    Q_OBJECT

public:
    Delegate(QObject *parent = nullptr);
    ~Delegate();

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const;
    bool editorEvent(QEvent *event,
                     QAbstractItemModel *model,
                     const QStyleOptionViewItem &option,
                     const QModelIndex &index);
    bool eventFilter(QObject *editor, QEvent *event);

    void strListAssignment(QList<QString>);
    void GetStrList(QList<QString>&);

private:
    bool flag;
    QList<QString> strlist;
};

#endif // DELEGATE_H
