
#include "SmartParkDlg.h"
#include "trademodule.h"
#include "parkordermgr.h"

CSmartParkDlg::CSmartParkDlg(QWidget* parent /* = nullptr*/)
    :QDialog (parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    initControls();

    configControls();
}

CSmartParkDlg::~CSmartParkDlg()
{

}

void  CSmartParkDlg::setData(tagInputData* pData, tagInputDetail* pDetail)
{
    m_oInputData = *pData;
    m_oInputDetail = *pDetail;

    reconfigControls();
}

//控件，布局
void  CSmartParkDlg::initControls()
{
    //第一区域
    QHBoxLayout* hm1 = new QHBoxLayout();
    {
        _lblPlace = new QLabel(tr("下单到 :"));
        _cboxPlace = new QComboBox();
        _cboxPlace->addItem(tr("服务器 "));
        hm1->addWidget(_lblPlace);
        hm1->addWidget(_cboxPlace);
        hm1->addStretch();
    }


    //第二区域
    _gboxInfo = new QGroupBox(tr("信息"));
    {
        //第一行
        _lblInfo = new QLabel(tr(""));
        QHBoxLayout* h2 = new QHBoxLayout();
        h2->addWidget(_lblInfo);
        //第二行
        _lblPrice = new QLabel(tr("价格: "));
        _spinPrice = new QDoubleSpinBox();
        _lblVolume = new QLabel(tr("数量: "));
        _spinVolume = new QSpinBox();
        QHBoxLayout* h3 = new QHBoxLayout();
        h3->addWidget(_lblPrice);
        h3->addWidget(_spinPrice);
        h3->addWidget(_lblVolume);
        h3->addWidget(_spinVolume);
        h3->addStretch();
        //Group
        QVBoxLayout* v1 = new QVBoxLayout();
        v1->addLayout(h2);
        v1->addLayout(h3);

        _gboxInfo->setLayout(v1);
    }

    //第三区域
    _gboxCondition = new QGroupBox(tr("条件"));
    {
        QGridLayout* g = new QGridLayout();

        //第一行
        _rbtnCondition = new QRadioButton();
        _cboxCondition = new QComboBox();
        _cboxOperator = new QComboBox();
        _spinConditionPrice = new QDoubleSpinBox();
        QHBoxLayout* h1 = new QHBoxLayout();
        h1->addWidget(_cboxCondition);
        h1->addWidget(_cboxOperator);
        h1->addWidget(_spinConditionPrice);
        g->addWidget(_rbtnCondition, 0, 0);
        g->addLayout(h1, 0, 1);

        //第二行
        _rbtnExchContinue = new QRadioButton();
        _lblExchContinue = new QLabel(tr("交易所开盘时(白盘，夜盘第一次进入连续竞价时)"));
        g->addWidget(_rbtnExchContinue, 1, 0);
        g->addWidget(_lblExchContinue, 1, 1);

        //第三行
        _rbtnExchAuction = new QRadioButton();
        _lblExchAuction = new QLabel(tr("集合竞价时"));
        g->addWidget(_rbtnExchAuction, 2, 0);
        g->addWidget(_lblExchAuction, 2, 1);

        //第四行
        _rbtnTime = new QRadioButton();
        _lblTime = new QLabel(tr("时间到达 "));
        _tedtTime = new QTimeEdit();
        g->addWidget(_rbtnTime, 3, 0);
        QHBoxLayout* h2 = new QHBoxLayout();
        h2->addWidget( _lblTime );
        h2->addWidget( _tedtTime );
        h2->addStretch();
        g->addLayout(h2, 3, 1);

        //第五行
        _rbtnHuman = new QRadioButton();
        _lblHuman = new QLabel(tr("预埋，手动发出"));
        g->addWidget(_rbtnHuman, 4, 0);
        g->addWidget(_lblHuman, 4, 1);
        g->setSpacing(10);

        //设置layout
        _gboxCondition->setLayout(g);

        //加入互斥
        _gbtnSelect = new QButtonGroup();
        _gbtnSelect->addButton(_rbtnCondition, 0);
        _gbtnSelect->addButton(_rbtnExchContinue, 1);
        _gbtnSelect->addButton(_rbtnExchAuction, 2);
        _gbtnSelect->addButton(_rbtnTime, 3);
        _gbtnSelect->addButton(_rbtnHuman, 4);
    }

    //第四区域
    QHBoxLayout* hm2 = new QHBoxLayout();
    {
        _btnOK = new QPushButton(tr("确定"));
        _btnCancel = new QPushButton(tr("取消"));
        hm2->addStretch();
        hm2->addWidget(_btnOK);
        hm2->addWidget(_btnCancel);
    }

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addLayout(hm1);
    mainLayout->addWidget(_gboxInfo);
    mainLayout->addWidget(_gboxCondition);
    mainLayout->addLayout(hm2);
    mainLayout->setSpacing(10);
    this->setLayout(mainLayout);

    //[5]消息
    QObject::connect(_btnOK, SIGNAL(clicked()), this,  SLOT(on_okbtn_clicked()) );
    QObject::connect(_btnCancel, SIGNAL(clicked()), this,  SLOT(on_cancelbtn_clicked()) );
}

void    CSmartParkDlg::configControls()
{
    //[0]默认选择条件单
    _rbtnCondition->setChecked(true);

    //[1]条件左边的Combox
    _cboxCondition->addItem(tr("最新价"), QVariant( int(1) ));
    _cboxCondition->addItem(tr("买一价"), QVariant( int(2) ));
    _cboxCondition->addItem(tr("卖一价"), QVariant( int(3) ));

    //[2]条件比较操作符
    _cboxOperator->addItem(tr(" > "), QVariant( int(1) ));
    _cboxOperator->addItem(tr(" >= "), QVariant( int(2) ));
    _cboxOperator->addItem(tr(" < "), QVariant( int(3) ));
    _cboxOperator->addItem(tr(" <= "), QVariant( int(4) ));

    //[4]外观
    _lblInfo->setProperty("name", "info");
    this->setProperty("name", "smartparkdlg");
}

void   CSmartParkDlg::reconfigControls()
{
    //[1] 拼字符串
    QString strText;
    {
        int usertype=0;
        int broker =0;
        char szuser[32]={0};

        m_oInputData.curMgr->WhoIs(usertype, broker, szuser);

        strText.sprintf("账户: %s   合约: %s ", szuser, m_oInputData.ExCode.Code);
        if(m_oInputData.direction == XT_D_Buy)
        {
            strText += " 买, ";
        }
        else
        {
            strText += " 卖, ";
        }

        if(m_oInputData.offset == XT_OF_Open)
        {
            strText += " 开仓, ";
        }
        else if(m_oInputData.offset == XT_OF_Close)
        {
            strText += " 平仓, ";
        }
        else if(m_oInputData.offset == XT_OF_CloseToday)
        {
            strText += " 平今, ";
        }
        else if(m_oInputData.offset == XT_OF_CloseToday)
        {
            strText += " 平今, ";
        }
        else if(m_oInputData.offset == XT_OF_Auto)
        {
            strText += " 自动, ";
        }

        if(m_oInputData.hedge == XT_HF_Speculation)
        {
            strText += " 投机 ";
        }
        else if(m_oInputData.hedge == XT_HF_Hedge)
        {
            strText += " 套保 ";
        }
        else if(m_oInputData.hedge == XT_HF_Arbitrage)
        {
            strText += " 套利 ";
        }
    }

    _lblInfo->setText(strText);

    //[2]设定数量
    _spinVolume->setValue(m_oInputData.volume);
    _spinVolume->setMinimum(1);
    _spinVolume->setMaximum(1000);
    _spinVolume->setSingleStep(1);

    //[3]spinPrice默认值
    if(m_oInputDetail.dyn_valid)
    {
        _spinPrice->setMinimum(m_oInputDetail.lowerlimitprice);
        _spinPrice->setMaximum(m_oInputDetail.upperlimitprice);

        _spinPrice->setSingleStep(m_oInputDetail.pricetick);
        _spinPrice->setDecimals(m_oInputDetail.showdot);
        _spinPrice->setValue(m_oInputData.price);
    }
    else
    {
        _spinPrice->setMinimum(0.0);
        _spinPrice->setMaximum(10000);
        _spinPrice->setSingleStep(0.01);
        _spinPrice->setDecimals(2);
    }

    //[4]条件 的最右值
    if(m_oInputDetail.dyn_valid)
    {
        _spinConditionPrice->setMinimum(m_oInputDetail.lowerlimitprice);
        _spinConditionPrice->setMaximum(m_oInputDetail.upperlimitprice);

        _spinConditionPrice->setSingleStep(m_oInputDetail.pricetick);
        _spinConditionPrice->setDecimals(m_oInputDetail.showdot);
        _spinConditionPrice->setValue(m_oInputData.price);
    }
    else
    {
        _spinConditionPrice->setMinimum(0.0);
        _spinConditionPrice->setMaximum(10000);
        _spinConditionPrice->setSingleStep(0.01);
        _spinConditionPrice->setDecimals(2);
    }

    //[5] 时间控件
    _tedtTime->setDisplayFormat("HH:mm:ss");
    _tedtTime->setTime(QTime::currentTime());

    //[6] 如果是买， 默认 操作符选择为>=：  如果是卖，默认操作符选择为<=:
    if(m_oInputData.direction == XT_D_Buy)
    {
        _cboxOperator->setCurrentIndex(1);
    }
    else
    {
        _cboxOperator->setCurrentIndex(3);
    }
}

void   CSmartParkDlg::on_cancelbtn_clicked()
{
    reject();
}

void   CSmartParkDlg::on_okbtn_clicked()
{
    int utype =0;
    int ubroker =0;
    char szuser[32] ={0};
    m_oInputData.curMgr->WhoIs(utype, ubroker, szuser);

    char szkey[32] = {0};
    _snprintf(szkey, 31, "%d_%s", ubroker, szuser);

    ParkCondOrderMgrByUser* pMgr = GetParkedOrderMgr()->findMgrByUser(szkey);
    if(pMgr)
    {
        tagXTParkedOrderInputField oField;
        memset(&oField, 0, sizeof(oField));

        //基本要素
        oField.BrokerID = ubroker;
        strcpy(oField.UserID, szuser);
        oField.ExCode = m_oInputData.ExCode;
        oField.Direction = m_oInputData.direction;
        oField.OffsetFlag[0] = oField.OffsetFlag[1] = m_oInputData.offset;
        oField.HedgeFlag[0] = oField.HedgeFlag[1] = m_oInputData.hedge;
        oField.LimitPrice = _spinPrice->value();
        oField.Volume = _spinVolume->value();

        int checkid = _gbtnSelect->checkedId();
        if(checkid == 0)
        {
            oField.ParkedType = XT_PT_CONDITION;
            oField.StopPrice = _spinConditionPrice->value();

            int opt = _cboxOperator->currentIndex();
            int curidx = _cboxCondition->currentIndex();
            if(curidx == 0) //最新价
            {
                if(opt == 0) // >
                    oField.ConditionType = XT_CC_LastPriceGreaterThanStopPrice;
                else if(opt == 1) // >=
                    oField.ConditionType = XT_CC_LastPriceGreaterEqualStopPrice;
                else if(opt == 2) // <
                    oField.ConditionType = XT_CC_LastPriceLesserThanStopPrice;
                else if(opt == 3) // <=
                    oField.ConditionType = XT_CC_LastPriceLesserEqualStopPrice;
            }
            else if(curidx == 1) //买一价
            {
                if(opt == 0) // >
                    oField.ConditionType = XT_CC_BidPriceGreaterThanStopPrice;
                else if(opt == 1) // >=
                    oField.ConditionType = XT_CC_BidPriceGreaterEqualStopPrice;
                else if(opt == 2) // <
                    oField.ConditionType = XT_CC_BidPriceLesserThanStopPrice;
                else if(opt == 3) // <=
                    oField.ConditionType = XT_CC_BidPriceLesserEqualStopPrice;
            }
            else if(curidx == 2) //卖一价
            {
                if(opt == 0) // >
                    oField.ConditionType = XT_CC_AskPriceGreaterThanStopPrice;
                else if(opt == 1) // >=
                    oField.ConditionType = XT_CC_AskPriceGreaterEqualStopPrice;
                else if(opt == 2) // <
                    oField.ConditionType = XT_CC_AskPriceLesserThanStopPrice;
                else if(opt == 3) // <=
                    oField.ConditionType = XT_CC_AskPriceLesserEqualStopPrice;
            }

        }
        else if(checkid == 1)
        {
            oField.ParkedType = XT_PT_EXCHCONTINUE;
        }
        else if(checkid == 2)
        {
            oField.ParkedType = XT_PT_EXCHAUCTION;
        }
        else if(checkid == 3)
        {
            oField.ParkedType = XT_PT_TIME;
            QTime tt = _tedtTime->time();
            oField.StopTime = tt.hour() *10000 + tt.minute()*100 + tt.second();
        }
        else if(checkid == 4)
        {
            oField.ParkedType = XT_PT_HUMAN;
        }

        QTime now = QTime::currentTime();
        char szParkID[13] = {0};
        _snprintf(szParkID, 12, "p_%02d_%02d_%02d", now.hour(), now.minute(), now.second() );
        strcpy(oField.ParkedOrderID, szParkID);

        pMgr->reqParkedOrderInsert(&oField);
    }

    accept();
}











