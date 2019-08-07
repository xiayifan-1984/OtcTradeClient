#include "condordertab.h"

CondOrderTab::CondOrderTab(QWidget *parent)
{
    _condOrderModel = nullptr;
    initControls();
}

void CondOrderTab::initControls()
{
    _condOrderModel = new QStandardItemModel();
    setHeadersForCondOrder();
    this->setModel(_condOrderModel);
    fillRow(0);
}

void CondOrderTab::setHeadersForCondOrder()
{
    QStandardItem* pItem = new QStandardItem();
    pItem->setText("ID Number");
    pItem->setEditable(false);
    _condOrderModel->setHorizontalHeaderItem(0, pItem);

    pItem = new QStandardItem();
    pItem->setText("Code");
    pItem->setEditable(false);
    _condOrderModel->setHorizontalHeaderItem(1, pItem);

    pItem = new QStandardItem();
    pItem->setText("Status");
    pItem->setEditable(false);
    _condOrderModel->setHorizontalHeaderItem(2, pItem);

    pItem = new QStandardItem();
    pItem->setText("trigger");
    pItem->setEditable(false);
    _condOrderModel->setHorizontalHeaderItem(3, pItem);

    pItem = new QStandardItem();
    pItem->setText("Price");
    pItem->setEditable(false);
    _condOrderModel->setHorizontalHeaderItem(4, pItem);

    pItem = new QStandardItem();
    pItem->setText("Direction");
    pItem->setEditable(false);
    _condOrderModel->setHorizontalHeaderItem(5, pItem);

    pItem = new QStandardItem();
    pItem->setText("OpenOrClose");
    pItem->setEditable(false);
    _condOrderModel->setHorizontalHeaderItem(6, pItem);

    pItem = new QStandardItem();
    pItem->setText("TriggerTime");
    pItem->setEditable(false);
    _condOrderModel->setHorizontalHeaderItem(7, pItem);

    pItem = new QStandardItem();
    pItem->setText("OrderRef");
    pItem->setEditable(false);
    _condOrderModel->setHorizontalHeaderItem(8, pItem);

    pItem = new QStandardItem();
    pItem->setText("InsertTime");
    pItem->setEditable(false);
    _condOrderModel->setHorizontalHeaderItem(9, pItem);
}

void CondOrderTab::fillRow(int rowIdx)
{
    int col = 0;

    QStandardItem* pItem = new QStandardItem();
    pItem->setText("13200");
    pItem->setEditable(false);
    pItem->setTextAlignment(Qt::AlignCenter);
    _condOrderModel->setItem(rowIdx, col, pItem);

    pItem = new QStandardItem();
    pItem->setText("jd1910");
    pItem->setEditable(false);
    pItem->setTextAlignment(Qt::AlignCenter);
    _condOrderModel->setItem(rowIdx, col+1, pItem);

    pItem = new QStandardItem();
    pItem->setText("已报");
    pItem->setEditable(false);
    pItem->setTextAlignment(Qt::AlignCenter);
    _condOrderModel->setItem(rowIdx, col+2, pItem);

    pItem = new QStandardItem();
    pItem->setText(">=");
    pItem->setEditable(false);
    pItem->setTextAlignment(Qt::AlignCenter);
    _condOrderModel->setItem(rowIdx, col+3, pItem);

    pItem = new QStandardItem();
    pItem->setText("3800");
    pItem->setEditable(false);
    pItem->setTextAlignment(Qt::AlignCenter);
    _condOrderModel->setItem(rowIdx, col+4, pItem);

    pItem = new QStandardItem();
    pItem->setText("Buy");
    pItem->setEditable(false);
    pItem->setTextAlignment(Qt::AlignCenter);
    _condOrderModel->setItem(rowIdx, col+5, pItem);

    pItem = new QStandardItem();
    pItem->setText("BuyOrSell");
    pItem->setEditable(false);
    pItem->setTextAlignment(Qt::AlignCenter);
    _condOrderModel->setItem(rowIdx, col+6, pItem);

    pItem = new QStandardItem();
    pItem->setText("09:42:11");
    pItem->setEditable(false);
    pItem->setTextAlignment(Qt::AlignCenter);
    _condOrderModel->setItem(rowIdx, col+7, pItem);

    pItem = new QStandardItem();
    pItem->setText("1249710117");
    pItem->setEditable(false);
    pItem->setTextAlignment(Qt::AlignCenter);
    _condOrderModel->setItem(rowIdx, col+8, pItem);

    pItem = new QStandardItem();
    pItem->setText("");
    pItem->setEditable(false);
    pItem->setTextAlignment(Qt::AlignCenter);
    _condOrderModel->setItem(rowIdx, col+9, pItem);
}
