#include "twapshowdetail.h"
#include "XTTradestruct.h"
#include "modeldatatwap.h"

CTwapShowDetail::CTwapShowDetail(QObject *parent)
{
    _tabView = nullptr;
    _orderMgr = nullptr;
    _model = nullptr;
}

void    CTwapShowDetail::initData(const std::string& twapRef, QOrderMgr* p)
{
    _twapRef = twapRef;
    _orderMgr = p;

    initControls();
}

void    CTwapShowDetail::initControls()
{
    _tabView = new QTableView();
    _tabView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QObject::connect(_tabView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(on_tableview_doubleClicked(const QModelIndex&)) );

    //[1]
    _model = new ModelTwapOrder();
    _tabView->setModel(_model);
    _model->setTwapMgr(_twapRef, _orderMgr);

    //[2]标题
    QString output = "TWAP 相关订单";
    output += ", ID ";
    output += _twapRef.c_str();
    setWindowTitle(output);

    //[3]布局
    QHBoxLayout* h = new QHBoxLayout();
    h->addWidget(_tabView);
    this->setLayout(h);
}

void CTwapShowDetail::on_tableview_doubleClicked(const QModelIndex &index)
{
    tagXTOrderField* p = _model->getDataByRow(index);
    if(nullptr == p)
    {
        return;
    }

    //如果状态为XT_OST_Accepted
    if(p->OrderStatus == XT_OST_Accepted)
    {
        if(_orderMgr)
        {
            //弹出对话框确认
            QString strMsg;
            strMsg.sprintf("是否对委托单<font color='red'>[%d]</font>进行撤单?", p->OrderRef);
            QMessageBox::StandardButton rb = QMessageBox::warning(nullptr, "撤单提醒",strMsg, QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
            if(rb == QMessageBox::Yes)
            {
                tagXTInputOrderActionField oField;
                memset(&oField, 0, sizeof(oField));
                oField.BrokerID = p->BrokerID;
                strcpy(oField.UserID, p->UserID);
                oField.ExCode = p->ExCode;
                strcpy(oField.ActionRef, p->OrderRef);
                oField.ActionFlag = XT_AF_Delete;
                strcpy(oField.OrderRef, p->OrderRef);
                oField.FrontID = p->FrontID;
                oField.SessionID = p->SessionID;
                strcpy(oField.OrderSysID, p->OrderSysID);

                _orderMgr->CancelOrder(&oField);
            }
        }

    }
    
}


