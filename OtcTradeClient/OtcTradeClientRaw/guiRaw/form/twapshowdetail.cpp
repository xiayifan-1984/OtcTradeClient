#include "twapshowdetail.h"
#include "XTTradestruct.h"

TwapShowDetail::TwapShowDetail(QObject *parent)
{
    _tabView = nullptr;
    _user = nullptr;
    _model = nullptr;
}

void TwapShowDetail::setContent(QTableView* pView, ModelSingleOrder* pModel,QOrderMgr* user)
{
    _tabView = pView;
    _user = user;
    _model = pModel;
    QObject::connect(_tabView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(on_tableview_doubleClicked(const QModelIndex&)) );
}

void TwapShowDetail::on_tableview_doubleClicked(const QModelIndex &index)
{
    tagXTOrderField* p = _model->getDataByRow(index);
    if(p)
    {
        //如果状态为XT_OST_Accepted
        if(p->OrderStatus == XT_OST_Accepted)
        {
            //如果combox当前选择的账号，同通知的账号相同，则刷新
            QOrderMgr* _curMgr = _user;
            if(_curMgr)
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

                    _curMgr->CancelOrder(&oField);
                }
            }

        }
    }
}


