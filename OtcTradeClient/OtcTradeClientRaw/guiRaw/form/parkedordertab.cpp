#include "parkedordertab.h"

ParkedOrderTab::ParkedOrderTab(QWidget *parent)
{
    _parkedOrderModel = nullptr;
    initControls();
}

void ParkedOrderTab::initControls()
{
    _parkedOrderModel = new QStandardItemModel();
    this->setModel(_parkedOrderModel);
    setHeadersForParkedOrder();
}

void ParkedOrderTab::setHeadersForParkedOrder()
{
    QStandardItem* pItem = new QStandardItem();
    pItem->setText("ID Number");
    _parkedOrderModel->setHorizontalHeaderItem(0, pItem);

    pItem = new QStandardItem();
    pItem->setText("Code");
    _parkedOrderModel->setHorizontalHeaderItem(1, pItem);

    pItem = new QStandardItem();
    pItem->setText("Status");
    _parkedOrderModel->setHorizontalHeaderItem(2, pItem);

    pItem = new QStandardItem();
    pItem->setText("Offset");
    _parkedOrderModel->setHorizontalHeaderItem(3, pItem);

    pItem = new QStandardItem();
    pItem->setText("BuyOrSell");
    _parkedOrderModel->setHorizontalHeaderItem(4, pItem);

    pItem = new QStandardItem();
    pItem->setText("Price");
    _parkedOrderModel->setHorizontalHeaderItem(5, pItem);


    pItem = new QStandardItem();
    pItem->setText("ParkTime");
    _parkedOrderModel->setHorizontalHeaderItem(6, pItem);

    pItem = new QStandardItem();
    pItem->setText("OrderRef");
    _parkedOrderModel->setHorizontalHeaderItem(7, pItem);

    pItem = new QStandardItem();
    pItem->setText("InsertTime");
    _parkedOrderModel->setHorizontalHeaderItem(8, pItem);

}
