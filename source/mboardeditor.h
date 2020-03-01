#ifndef MCREATEHARDWARE_H
#define MCREATEHARDWARE_H

#include <QtWidgets>
#include <mclass/mcombobox.h>
#include "mclass/delegate.h"
#include "mclass/parameterdata.h"
#include "mclass/moduledata.h"
#include "mclass/mtablewidget.h"
#include "mclass/spinboxdelegate.h"
#include "mclass/mlistwidget.h"
#include "mclass/qflowlayout.h"

namespace Ui {
class MBoardEditor;
}

class MBoardEditor : public QWidget
{
    Q_OBJECT

public:
    explicit MBoardEditor(QWidget *parent = 0);
    ~MBoardEditor();
    void interfaceInit();
    void saveAsJsonFile();

public slots:
    void slot_moudleAdd();
    void slot_moudleDec();
    void slot_propertyAdd();
    void slot_propertyDec();
    void slot_moduleCellChanged(int, int);
    void slot_propertyData(int,int);
    void slot_moduleSelectionChanged();
    void slot_propertySelectionChanged();
    void slot_propertyNameChanged(QListWidgetItem*);


private slots:

    void on_btn_save_clicked();

    void on_btn_open_clicked();

    void on_btn_new_clicked();

private:
    Ui::MBoardEditor *ui;
    MTableWidget *tbw_modules;
    QTableWidget *tbw_propertys;
    QListWidget *lst_parameterName;

    QList<ModuleData*> module;
    Delegate *d;

    bool F_ModuleCellChangeLock;
    bool F_ModuleCurrentRowLock;
    bool F_PropertyCellChangeLock;
    bool F_PropertyCurrentRowLock;
};

#endif // MCREATEHARDWARE_H
