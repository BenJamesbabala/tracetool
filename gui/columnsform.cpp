/**********************************************************************
** Copyright (C) 2010 froglogic GmbH.
** All rights reserved.
**********************************************************************/

#include "columnsform.h"

#include "columnsinfo.h"

#include <cassert>

const int RealIndexDataRole = Qt::UserRole;

ColumnsForm::ColumnsForm(Settings *settings,
                         QWidget *parent, Qt::WindowFlags flags)
    : QDialog(parent, flags),
      m_settings(settings)
{
    setupUi(this);

    connect(upButton, SIGNAL(clicked()),
            this, SLOT(moveUp()));
    connect(downButton, SIGNAL(clicked()),
            this, SLOT(moveDown()));
    connect(toInvisible, SIGNAL(clicked()),
            this, SLOT(moveToInvisible()));
    connect(toVisible, SIGNAL(clicked()),
            this, SLOT(moveToVisible()));

    restoreSettings();
}

void ColumnsForm::accept()
{
    saveSettings();
    QDialog::accept();
}

void ColumnsForm::moveToInvisible()
{
    int row = listWidgetVisible->currentRow();
    if (row != -1) {
        QListWidgetItem *ci = listWidgetVisible->takeItem(row);
        listWidgetInvisible->addItem(ci);
    }
}

void ColumnsForm::moveToVisible()
{
    int row = listWidgetInvisible->currentRow();
    if (row != -1) {
        QListWidgetItem *ci = listWidgetInvisible->takeItem(row);
        listWidgetVisible->addItem(ci);
    }
}

void ColumnsForm::moveUp()
{
    int row = listWidgetVisible->currentRow();
    if (row >= 1) {
        QListWidgetItem *item = listWidgetVisible->takeItem(row);
        listWidgetVisible->insertItem(row - 1, item);
        listWidgetVisible->setCurrentRow(row - 1);
    }
}

void ColumnsForm::moveDown()
{
    int row = listWidgetVisible->currentRow();
    if (row + 1 < listWidgetVisible->count()) {
        QListWidgetItem *item = listWidgetVisible->takeItem(row);
        listWidgetVisible->insertItem(row + 1, item);
        listWidgetVisible->setCurrentRow(row + 1);
    }
}

void ColumnsForm::saveSettings()
{
    QList<int> visibleColumns;
    for (int row = 0; row < listWidgetVisible->count(); ++row) {
        QListWidgetItem *item = listWidgetVisible->item(row);
        QVariant data = item->data(RealIndexDataRole);
        assert(data.isValid());
        bool ok;
        int realIndex = data.toInt(&ok);
        assert(ok);
        visibleColumns.append(realIndex);
    }
    QList<int> invisibleColumns;
    for (int row = 0; row < listWidgetInvisible->count(); ++row) {
        QListWidgetItem *item = listWidgetInvisible->item(row);
        QVariant data = item->data(RealIndexDataRole);
        assert(data.isValid());
        bool ok;
        int realIndex = data.toInt(&ok);
        assert(ok);
        invisibleColumns.append(realIndex);
    }

    m_settings->columnsInfo()->setSorting(visibleColumns, invisibleColumns);
}

void ColumnsForm::restoreSettings()
{
    const ColumnsInfo *inf = m_settings->columnsInfo();

    const QList<int> visibleColumns = inf->visibleColumns();
    QListIterator<int> it(visibleColumns);
    while (it.hasNext()) {
        int realIndex = it.next();
        const QString name = inf->columnName(realIndex);
        QListWidgetItem *item = new QListWidgetItem(name);
        item->setData(RealIndexDataRole, realIndex);
        listWidgetVisible->addItem(item);
    }

    const QList<int> invisibleColumns = inf->invisibleColumns();
    it = invisibleColumns;
    while (it.hasNext()) {
        int realIndex = it.next();
        const QString name = inf->columnName(realIndex);
        QListWidgetItem *item = new QListWidgetItem(name);
        item->setData(RealIndexDataRole, realIndex);
        listWidgetInvisible->addItem(item);
    }
}