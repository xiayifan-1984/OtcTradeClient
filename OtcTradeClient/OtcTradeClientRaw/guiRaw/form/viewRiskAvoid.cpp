
#include "viewRiskAvoid.h"
#include "selectmaincontract.h"
#include "datamodule.h"
#include "XTCodec.h"
#include "viewOtcPosition.h"
#include <QString>
#include "configmodule.h"
#include <QDebug>
#include <algorithm>
#include <cmath>
#include <utility>
#include <trademoduleimpl.h>
#include "XTQuoteStruct.h"
#include "quotemodule.h"
#include "./module/otcoptionmodule.h"
#include "./util/stool.h"

extern QMdiArea * G_midArea;

#define	GET_VALID_PRICE(P,T) (((SHOPT_NPRICE(P))/((int)(T*10000)))*T);

shared_ptr<KBInputBox> tradeBox;
bool isRiskWidgetPositonInitialized = false;
pair<pair<long, long>, bool> riskWidgetPostions[8];

QOrderMgr* GetOrderMgrbyInstrument(tagXTInstrument& curCode)
{
    auto pTradeMod = static_cast<QTradeModuleImpl*>(GetTradeModule());
    if(!pTradeMod) return nullptr;

    QExchange* pExchange = GetDataModule()->GetExchange(curCode.ExchID);
    if(!pExchange) return nullptr;

    tagXTNameTable oNameCode;
    pExchange->GetOneNameTable(&curCode, &oNameCode);
    qDebug()<< "Exchange = " << curCode.ExchID << " instrument name " << oNameCode.Name;

    vector<tagOneTradeUser>  arrUser;
    GetConfigModule()->GetTradeUserBy(oNameCode.ProductID, arrUser);
    qDebug() << "Users number = " << arrUser.size();
    auto pMgr = arrUser.size()>0 ? pTradeMod->GetMgr(arrUser[0].type, arrUser[0].broker, arrUser[0].user): nullptr;
    return pMgr;
}

int posDirectionSign(XTPosiDirectionType dir)
{
    if(dir == XT_PD_Net or dir == XT_PD_Long) return 1;
    if(dir == XT_PD_Short) return -1;
    return 0;
}

inline int orderDirectionSign(XTPosiDirectionType dir)
{
    if(dir == XT_D_Buy or dir == XT_D_BuyCollateral or dir == XT_D_BuyMargin) return 1;
    if(dir == XT_D_Sell or dir == XT_D_SellCollateral or dir == XT_D_SellMargin) return -1;
    return 0;
}

inline bool isOrderShow(XTOrderStatusType orderStatus)
{
    if(orderStatus == XT_OST_Accepted or orderStatus == XT_OST_CancelSubmitted) return true;
    return false;
}

ViewRiskAvoid::ViewRiskAvoid(QWidget* parent /* = nullptr*/)
{
    QConfigModule*  pConf = GetConfigModule();
    m_vol = pConf ? (pConf->g).vol : 0.2;
    m_otcOptData.setRiskVol(m_vol);
    m_otcOptData.setRiskFreeRate(pConf?(pConf->g).riskFreeRate:0.045);
    m_position = 0;
    m_widgetPosition = -1;
    memset(&m_curCode, 0, sizeof(tagXTInstrument));

    initControls();

    this->setProperty("name","viewriskavoid");

    setWidgetData();
    init8WidgetsPostions();
}

ViewRiskAvoid::~ViewRiskAvoid()
{
    if(m_widgetPosition>=0)
    {
        riskWidgetPostions[m_widgetPosition].second = false;
    }
    qDebug() << "widget postion " << m_widgetPosition << " is released !";
}

void    ViewRiskAvoid::initControls()
{
    //[1]
    _tblWidget = new QTableWidget();
    _tblWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //禁止编辑
    _tblWidget->verticalHeader()->setVisible(false);   //隐藏列表头
    _tblWidget->horizontalHeader()->setVisible(false); //隐藏行表头

    //[2]
    QVBoxLayout* v1 = new QVBoxLayout();

    v1->addWidget(_tblWidget);
    this->setLayout(v1);

    //[3]
    //itemDoubleClicked(QTableWidgetItem *item)
    QObject::connect(_tblWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)),  this, SLOT(onWidget_itemDoubleClicked(QTableWidgetItem*)) );
    m_pTimer = new QTimer(this);
    QObject::connect(m_pTimer, SIGNAL(timeout()), this, SLOT(ontimer()));
    m_pTimer->start(500);
}

void   ViewRiskAvoid::onWidget_itemDoubleClicked(QTableWidgetItem* pItem)
{
    int row = pItem->row();
    int col = pItem->column();

    qDebug() << "itemDoubleClicked" << row << " ][" << col;
    if(row ==0 && col ==0)
    {
        SelectMainContract dlg;
        //dlg.setWindowFlag(Qt::WindowStaysOnTopHint);
        dlg.setModal(true);
        if(dlg.exec() == QDialog::Accepted)
        {
            tagXTInstrument oCode;
            dlg.getSelectCommodity(&oCode);
            setInstrument(oCode);
            qDebug() << "instrument selected " << oCode.ExchID << " ][" << oCode.Code;
            auto pQuoteMod = GetQuoteModule();
            if(pQuoteMod)
            {
                pQuoteMod->SetCodeList(&m_curCode, 1);
            }
            //获取名称
            char szname[256]={0};
            GetDataModule()->GetOneName(oCode, szname);
            std::string str = szname;
            QString strName = XTCodec::AfGbk_ToQString(str);
            pItem->setText(strName);
        }
    }
    if(row ==0 && col ==1)
    {
        ViewOtcPosition* widget = new ViewOtcPosition();
        widget->setCurCommodity(m_curCode);
        if(G_midArea)
        {
            G_midArea->addSubWindow(widget);
        }
       // widget->setWindowFlags(widget->windowFlags()|Qt::WindowStaysOnTopHint);
        widget->show();
    }
    if(row ==2 && col ==5)
    {
       // auto child = getSingleTradeBox();
        qDebug() << "show Input box";
        if(!tradeBox)
        {
            tradeBox = make_shared<KBInputBox>();
        }
        m_tradeBox = tradeBox;

        if(m_tradeBox.lock())
        {
            qDebug() << "valid input box";
            auto ptr = m_tradeBox.lock();
            ptr->setWindowFlag(Qt::WindowStaysOnTopHint);
            ptr->inputCommodity(m_curCode);
            ptr->setWindowTitle(tr("KBInputbox"));
            ptr->show();
        }
    }
    if(row == 0 && col == 6)
    {
        if(m_curCode.ExchID<=0) return;
        QInputDialog dlg(this);
        dlg.setWindowTitle("Input Volatility");
        dlg.setLabelText("Please input：");
        dlg.setInputMode(QInputDialog::TextInput);
      /*  dlg.setInputMode(QInputDialog::DoubleInput);
        dlg.setDoubleDecimals(2);
        dlg.setDoubleRange(0.0, 100.0);*/

        auto p1 = GetConfigModule();
        if(p1)
        {
           // dlg.setDoubleValue(p1->getRiskVol(m_curCode));
            dlg.setTextValue(QString::number(p1->getRiskVol(m_curCode), 'f',2));
        }


        if(dlg.exec() == QInputDialog::Accepted)
        {
           qDebug() << dlg.doubleValue();
          // m_vol = dlg.doubleValue();
           m_vol = dlg.textValue().toDouble();
           m_otcOptData.setRiskVol(m_vol);
           auto p = GetConfigModule();
           if(p)
           {
               p->insertRiskVol(m_curCode, m_vol);
           }
        }
    }
}

void ViewRiskAvoid::ontimer()
{
    if(m_curCode.ExchID == 0) return;
    tagXTSnapshot quoteSnap;
    auto pQuoteMod = GetQuoteModule();
    double curPrice = 0.1;
    if(pQuoteMod)
    {
        qDebug() << "valid quote model";
        auto res = pQuoteMod->GetSnapShot(m_curCode, &quoteSnap);
        qDebug() << "res = " << res;
        if(res>0)
        {
            curPrice = quoteSnap.LastPrice;
        }
    }

    //update vol
    auto pConf = GetConfigModule();
    if(!pConf) return;
    m_vol = pConf->getRiskVol(m_curCode);
    m_otcOptData.setRiskVol(m_vol);
    // calc one gamma
    QString ts(m_curCode.Code);
    std::string inst(XTCodec::AfUtf8_ToString(ts));
    inst = stool::strToUpper(inst);
    auto otcOpts = GetOtcOptionModule()->getOptPostionsByInst(inst);
    double totalOneGamma = 0.0;
    for(auto p = otcOpts.begin(); p != otcOpts.end(); ++p)
    {
        m_otcOptData.update(curPrice, *p);
        totalOneGamma += m_otcOptData.getOneGamma();
    }


    updateRow001(m_position, totalOneGamma);
    updateRow002_003_004_005(curPrice);
}

void ViewRiskAvoid::init8WidgetsPostions()
{
    if(isRiskWidgetPositonInitialized == false)
    {
        for(int i =0; i< 8; ++i)
        {
            riskWidgetPostions[i].second = false;
        }
        auto widgeWidth = 800;
        auto widgetHeight = 200;

        for(int i=0; i< 8; i+=2)
        {
            riskWidgetPostions[i].first.first = 0;
            riskWidgetPostions[i+1].first.first = widgeWidth;
            riskWidgetPostions[i].first.second = i/2*widgetHeight;
            riskWidgetPostions[i+1].first.second = (i+1)/2*widgetHeight;
        }

    }
    isRiskWidgetPositonInitialized = true;
}

std::pair<long, long> ViewRiskAvoid::getRecommendPostion()
{
    for(int i = 0; i<8; ++i)
    {
        if(riskWidgetPostions[i].second == false and isRiskWidgetPositonInitialized)
        {
            riskWidgetPostions[i].second = true;
            m_widgetPosition = i;
            return riskWidgetPostions[i].first;
        }
    }
   return {qrand()%100+500,qrand()%100+100};
}

/*weak_ptr<KBInputBox> ViewRiskAvoid::getSingleTradeBox()
{
    qDebug() << "get trade box";
    if(!m_tradeBox.lock())
    {
        m_tradeBox = tradeBox;
    }
    return m_tradeBox;
}*/

void ViewRiskAvoid::setInstrument(tagXTInstrument &exCode)
{
    m_curCode = exCode;
    QConfigModule*  pConf = GetConfigModule();
    m_vol = pConf ? pConf->getRiskVol(exCode) : 0.2;
    m_otcOptData.setRiskVol(m_vol);
}


//======================================================================================================================================================================================================================
//======================================================================================================================================================================================================================
void    ViewRiskAvoid::initRow001()
{
    QTableWidgetItem* pItem = new QTableWidgetItem();
    pItem->setText("选择合约");
    _tblWidget->setItem(0, 0, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText("OTC持仓");
    _tblWidget->setItem(0, 1, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText("持仓量");
    pItem->setBackgroundColor(QColor(102,255,255));
    _tblWidget->setItem(0, 3, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText("");
    pItem->setTextColor(QColor(236,41,103));
    pItem->setBackgroundColor(QColor(102,255,255));
    _tblWidget->setItem(0, 4, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText("避险波动率");
    pItem->setTextColor(QColor(236,41,103));
    pItem->setBackgroundColor(QColor(102,255,255));
    _tblWidget->setItem(0, 5, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText("1倍标准差的Gamma");
    _tblWidget->setItem(0, 7, pItem);
    _tblWidget->setSpan(0,7,1, 2);

    pItem = new QTableWidgetItem();
    pItem->setText("");
    pItem->setTextColor(QColor(236,41,103));
    _tblWidget->setItem(0, 9, pItem);
}

void    ViewRiskAvoid::initRow002()
{
    QTableWidgetItem* pItem = new QTableWidgetItem();
    pItem->setText("挂单");
    pItem->setBackgroundColor(QColor(255,255,0));
    _tblWidget->setItem(1, 0, pItem);

    int arrPrice[9]=
    {
        0,0,0,0, 0, 0, 0, 0, 0
    };
    int arrVol[9]=
    {
        0,0, 0, 0, 0, 0, 0, 0, 0
    };

    for(int i=0; i<9; i++)
    {
        if(arrPrice[i] >0)
        {
            pItem = new QTableWidgetItem();
            QString str;
            if(arrVol[i] >=0)
            {
                str.sprintf("%d+%d", arrPrice[i], arrVol[i]);
            }
            else
            {
                str.sprintf("%d%d", arrPrice[i], arrVol[i]);
            }
            pItem->setText(str);
            _tblWidget->setItem(1, i, pItem);
        }
    }
}

void    ViewRiskAvoid::initRow003()
{
    QTableWidgetItem* pItem = new QTableWidgetItem();
    pItem->setText("期货价格");
    pItem->setBackgroundColor(QColor(192,192,192));
    _tblWidget->setItem(2, 0, pItem);

    int arrData[9]=
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    for(int i=0; i<9; i++)
    {
        pItem = new QTableWidgetItem();
        QString str;
        str.sprintf("%d", arrData[i]);
        pItem->setText(str);

        if(i == 1 || i == 4 || i ==7)
        {
            pItem->setBackgroundColor(QColor(146,208,80));
        }
        pItem->setTextAlignment(Qt::AlignCenter);
        _tblWidget->setItem(2, i+1, pItem);
    }
}

void    ViewRiskAvoid::initRow004()
{
    double arrData[9]=
    {
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };

    QTableWidgetItem* pItem = new QTableWidgetItem();
    pItem->setText("理论应避");
    pItem->setTextAlignment(Qt::AlignCenter);
    pItem->setTextColor(QColor(236,41,103));
    pItem->setBackgroundColor(QColor(255,204,153));
    _tblWidget->setItem(3, 0, pItem);

    for(int i=0; i<9; i++)
    {
        pItem = new QTableWidgetItem();
        pItem->setTextColor(QColor(236,41,103));
        QString str;
        str.sprintf("%.1lf", arrData[i]);
        pItem->setText(str);
        pItem->setTextAlignment(Qt::AlignCenter);
        _tblWidget->setItem(3, i+1, pItem);
    }
}

void    ViewRiskAvoid::initRow005()
{
    double arrData[9]=
    {
        0.0,0.0,0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };

    QTableWidgetItem* pItem = new QTableWidgetItem();
    pItem->setText("超缺避");
    pItem->setTextAlignment(Qt::AlignCenter);
    pItem->setTextColor(QColor(236,41,103));
    pItem->setBackgroundColor(QColor(255,204,153));
    _tblWidget->setItem(4, 0, pItem);

    for(int i=0; i<9; i++)
    {
        pItem = new QTableWidgetItem();
        if(arrData[i] <0)
        {
            pItem->setTextColor(QColor(236,41,103));
        }
        else
        {
            pItem->setTextColor(QColor(0,0,0));
        }
        QString str;
        str.sprintf("%.1lf", arrData[i]);
        pItem->setText(str);
        pItem->setTextAlignment(Qt::AlignCenter);
        _tblWidget->setItem(4, i+1, pItem);
    }
}

void ViewRiskAvoid::updateRow001(int position, double one_gamma)
{
    QTableWidgetItem* pItem = new QTableWidgetItem();

    pItem = new QTableWidgetItem();
    QString str = QString::number(position);
    pItem->setText(str);
    pItem->setTextColor(QColor(236,41,103));
    pItem->setBackgroundColor(QColor(102,255,255));
    _tblWidget->setItem(0, 4, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText(QString::number(m_vol, 'f',2));
    _tblWidget->setItem(0, 6, pItem);

    pItem = new QTableWidgetItem();
    str = QString::number(one_gamma);
    pItem->setText(str);
    pItem->setTextColor(QColor(236,41,103));
    _tblWidget->setItem(0, 9, pItem);
}

void ViewRiskAvoid::updateRow002_003_004_005(double cur_price)
{
    //firstly, update row 3

    double arrData[9]=
    {
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };

    tagXTNameTable oNameCode;
    QExchange* pExchange = GetDataModule()->GetExchange(m_curCode.ExchID);
    double priceTick = 0.0001;
    int    showDot = 2;
    int    instMulti = 1;
    if(pExchange)
    {
        pExchange->GetOneNameTable(&m_curCode, &oNameCode);
        showDot = oNameCode.ShowDot;
        priceTick = oNameCode.PriceTick!=0.0 ? oNameCode.PriceTick:priceTick;
        instMulti = oNameCode.VolumeMultiple;
    }
    qDebug()<< "price = " << cur_price << " ,priceTick = " << priceTick << " , volatility = " << m_vol;

    arrData[4] = GET_VALID_PRICE(cur_price, priceTick);

    arrData[3] = GET_VALID_PRICE(cur_price * (1 - 0.25*(m_vol/16.0)), priceTick);
    arrData[5] = GET_VALID_PRICE(cur_price * (1 + 0.25*(m_vol/16.0)), priceTick);

    arrData[2] = GET_VALID_PRICE(cur_price * (1 - 0.5*(m_vol/16.0)), priceTick);
    arrData[6] = GET_VALID_PRICE(cur_price * (1 + 0.5*(m_vol/16.0)), priceTick);

    arrData[1] = GET_VALID_PRICE(cur_price * (1 - (m_vol/16.0)), priceTick);
    arrData[7] = GET_VALID_PRICE(cur_price * (1 + (m_vol/16.0)), priceTick);

    arrData[0] = GET_VALID_PRICE(cur_price * (1 - 2*(m_vol/16.0)), priceTick);
    arrData[8] = GET_VALID_PRICE(cur_price * (1 + 2*(m_vol/16.0)), priceTick);

    QTableWidgetItem* pItem = new QTableWidgetItem();
    for(int i=0; i<9; i++)
    {
        pItem = new QTableWidgetItem();
        QString str = QString::number(arrData[i], 'f', showDot);
        pItem->setText(str);

        if(i == 1 || i == 4 || i ==7)
        {
            pItem->setBackgroundColor(QColor(146,208,80));
        }
        pItem->setTextAlignment(Qt::AlignCenter);
        _tblWidget->setItem(2, i+1, pItem);
    }

    //update row 2

    std::vector<std::pair<double, int>> row3Data;
    row3Data.resize(9);
    for(auto& p: row3Data)
    {
        p.first = 0.0;
        p.second = 0;
    }
    auto pTradeMod = static_cast<QTradeModuleImpl*>(GetTradeModule());

    if(!pTradeMod)
    {
        qDebug() << " Failed to get trade Model ";
    }
    else
    {
        std::vector<tagXTOrderField> orders_vec;

        vector<char> directionsVec{XT_PD_Net, XT_PD_Long, XT_PD_Short, XT_PD_Covered};
        vector<char> hedgeflagsVec{XT_HF_Others, XT_HF_Speculation, XT_HF_Arbitrage, XT_HF_Hedge, XT_HF_Covered, XT_HF_MarketMaker};

        int position = 0;
        bool isPosUpdated = false;
        auto pMgr = getOrderMgrbyInstrument();
        if(pMgr)
        {
            qDebug() << "account infor received ";
            pMgr->GetOrdersForInstrument(m_curCode, orders_vec);

            //update position
            for(auto dir:directionsVec)
            {
                for(auto hedgeflag:hedgeflagsVec)
                {
                    tagXTInvestorPositionField temp;
                    if(1 == pMgr->GetOnePosition(m_curCode, dir, hedgeflag, temp))
                    {
                        isPosUpdated = true;
                        position += (temp.Position* (posDirectionSign(dir)));
                    }
                }
            }
        }
        qDebug() << "current position = " << m_position << "order size " << orders_vec.size();
        if(isPosUpdated) m_position = position;

        if(orders_vec.size()>0)
        {
            orders_vec.erase(remove_if(orders_vec.begin(), orders_vec.end(),
                                       [](tagXTOrderField& p){return not(isOrderShow(p.OrderStatus) and p.VolumeTotal>0);}), orders_vec.end());


            sort(orders_vec.begin(), orders_vec.end(), [](tagXTOrderField& p1, tagXTOrderField& p2)
                                                         {return p1.LimitPrice <= p2.LimitPrice;});

            vector<pair<double, int>> ordersPriceAndPosition;
            auto pOnePriceBegin = orders_vec.begin();
            auto pOnePriceEnd = pOnePriceBegin;
            while(pOnePriceBegin != orders_vec.end())
            {
                const double price = pOnePriceBegin->LimitPrice;
                int position = 0;
                pOnePriceEnd = find_if(pOnePriceBegin, orders_vec.end(), [price](tagXTOrderField& p){return price != p.LimitPrice;});
                while(pOnePriceBegin != pOnePriceEnd)
                {
                    position += pOnePriceBegin->VolumeTotal*(orderDirectionSign(pOnePriceBegin->Direction));
                    ++pOnePriceBegin;
                }
                ordersPriceAndPosition.push_back({price,  position});
                pOnePriceBegin = pOnePriceEnd;
            }

            auto curPriceIdx = find_if(ordersPriceAndPosition.begin(), ordersPriceAndPosition.end(),
                                     [&cur_price](pair<double, int>& p){return cur_price<=p.first;});
            auto beginIdx = curPriceIdx - ordersPriceAndPosition.begin() >=4 ? curPriceIdx-4:ordersPriceAndPosition.begin();
            auto endIdx = ordersPriceAndPosition.end()-curPriceIdx >=5 ? ordersPriceAndPosition.end()-5 : ordersPriceAndPosition.end();
            auto temp = curPriceIdx;
            int idx = 3;
            while(temp > beginIdx)
            {
                --temp;
                row3Data[idx] = *temp;
                --idx;
            }
            idx = 4;
            while(curPriceIdx < endIdx)
            {
                row3Data[idx] = *curPriceIdx;
                ++curPriceIdx;
                ++idx;
            }
        }

    }
    pItem = new QTableWidgetItem();
    pItem->setText("挂单");
    pItem->setBackgroundColor(QColor(255,255,0));
    _tblWidget->setItem(1, 0, pItem);

    for(int i=0; i<9; i++)
    {
        pItem = new QTableWidgetItem();
        if(row3Data[i].first >0)
        {
            QString str;
            if(row3Data[i].second >=0)
            {
                str = QString::number(row3Data[i].first, 'f', showDot);
                str.append("+");
                str.append(QString::number(row3Data[i].second));
                qDebug() << "+," <<str;
            }
            else
            {
                str = QString::number(row3Data[i].first, 'f', showDot);
                str.append(QString::number(row3Data[i].second));
                qDebug() << "-," <<str;
            }
            pItem->setText(str);
            _tblWidget->setItem(1, i+1, pItem);
        }
        else
        {
            pItem->setText("");
            _tblWidget->setItem(1, i+1, pItem);
        }
    }

    //update row 4

    QString ts(m_curCode.Code);
    std::string inst(XTCodec::AfUtf8_ToString(ts));
    qDebug()<<ts;
    inst = stool::strToUpper(inst);
    auto otcOpts = GetOtcOptionModule()->getOptPostionsByInst(inst);
    auto totolAvoidAmout = [&](double price)
    {
        double result = 0.0;
        for(auto p = otcOpts.begin(); p!= otcOpts.end(); ++p)
        {
            this->m_otcOptData.update(price, *p);
            result += this->m_otcOptData.getRiskAmount();
        }
        return result;
    };
    vector<double> riskAvoidVolume(9,0.0);
    int arrIdx = 0;
    while(arrIdx < 9)
    {
        double diffAmount = totolAvoidAmout(arrData[arrIdx]);
        if(arrData[arrIdx]!=0)
            riskAvoidVolume[arrIdx] = diffAmount;
        ++arrIdx;
    }
    for(int i = 0 ; i<9 ; ++i)
    {
        pItem = new QTableWidgetItem();
        double totalAvoid = riskAvoidVolume[i]/instMulti/arrData[i]*(-1.0); // switch trader direction to custumers'
        QString str = QString::number(totalAvoid, 'f', 2);
        pItem->setText(str);

        if(totalAvoid<0)
        {
            pItem->setTextColor(QColor(236,41,103));
        }
        pItem->setTextAlignment(Qt::AlignCenter);
        _tblWidget->setItem(3, i+1, pItem);

        pItem = new QTableWidgetItem();
        //auto lackVolume = (riskAvoidVolume[i]+m_position*arrData[i]*instMulti)/instMulti/arrData[i];
        auto lackVolume = (riskAvoidVolume[i]/instMulti/arrData[i]*(-1.0)-m_position);
        str = QString::number(lackVolume, 'f', 2);
        pItem->setText(str);
        if(lackVolume<0)
        {
          pItem->setTextColor(QColor(236,41,103));
        }
        pItem->setTextAlignment(Qt::AlignCenter);
        _tblWidget->setItem(4, i+1, pItem);
    }
}

QOrderMgr* ViewRiskAvoid::getOrderMgrbyInstrument()
{
    auto pTradeMod = static_cast<QTradeModuleImpl*>(GetTradeModule());
    if(!pTradeMod) return nullptr;

    QExchange* pExchange = GetDataModule()->GetExchange(m_curCode.ExchID);
    if(!pExchange) return nullptr;

    tagXTNameTable oNameCode;
    pExchange->GetOneNameTable(&m_curCode, &oNameCode);
    qDebug()<< "Exchange = " << m_curCode.ExchID << " instrument name " << oNameCode.Name;

    vector<tagOneTradeUser>  arrUser;
    GetConfigModule()->GetTradeUserBy(oNameCode.ProductID, arrUser);
    qDebug() << "Users number = " << arrUser.size();
    auto pMgr = arrUser.size()>0 ? pTradeMod->GetMgr(arrUser[0].type, arrUser[0].broker, arrUser[0].user): nullptr;
    return pMgr;
}

void    ViewRiskAvoid::setWidgetData()
{
    //[1]
    _tblWidget->setRowCount(5);
    _tblWidget->setColumnCount(10);

    //[2]
    for(int i=0; i<10; i++)
    {
        _tblWidget->setColumnWidth(i, 70);
    }
    for(int i=0; i<5; i++)
    {
        _tblWidget->setRowHeight(i,25);
    }

    initRow001();
    initRow002();
    initRow003();
    initRow004();
    initRow005();

}





