#include "otcriskcalc.h"
#include "configmodule.h"
#include "selectmaincontract.h"
#include "quotemodule.h"
#include "datamodule.h"
#include "XTCodec.h"
#include <QString>
#include "otcpositiondata.h"
#include "stool.h"

OtcRiskCalc::OtcRiskCalc(QWidget *parent)
{
    m_vol = 0.2;
    m_otcOptData.setRiskVol(m_vol);
    m_otcOptData.setRiskFreeRate(0.045);
    m_position = 0;
    memset(&m_curCode, 0, sizeof(tagXTInstrument));
    m_riskCombs.resize(9);
    for(auto& ele:m_riskCombs)
        ele = 0.0;

    initControls();
    setWidgetData();
}

void OtcRiskCalc::initControls()
{
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
}

void OtcRiskCalc::setWidgetData()
{
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

void OtcRiskCalc::initRow001()
{
    QTableWidgetItem* pItem = new QTableWidgetItem();
    pItem->setText("选择合约");
    _tblWidget->setItem(0, 0, pItem);
    pItem->setBackgroundColor(QColor(50,150,255));

    pItem = new QTableWidgetItem();
    pItem->setText("OTC持仓");
    _tblWidget->setItem(0, 1, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText("持仓量");
    pItem->setBackgroundColor(QColor(200,100,255));
    _tblWidget->setItem(0, 3, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText("");
    pItem->setTextColor(QColor(236,41,103));
    pItem->setBackgroundColor(QColor(102,255,255));
    _tblWidget->setItem(0, 4, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText("避险波动率");
    pItem->setTextColor(QColor(0,0,103));
    pItem->setBackgroundColor(QColor(255,0,0));
    _tblWidget->setItem(0, 5, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText(tr(""));
    _tblWidget->setItem(0, 6, pItem);
    _tblWidget->setSpan(0,7,1, 2);

    pItem = new QTableWidgetItem();
    pItem->setText(tr(""));
    _tblWidget->setItem(0, 7, pItem);
    _tblWidget->setSpan(0,7,1, 2);

    pItem = new QTableWidgetItem();
    pItem->setText("");
    pItem->setTextColor(QColor(236,41,103));
    _tblWidget->setItem(0, 9, pItem);
}

void OtcRiskCalc::initRow002()
{
    QTableWidgetItem* pItem = new QTableWidgetItem();
    pItem->setText("挂单");
    pItem->setBackgroundColor(QColor(0,255,0));
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

void OtcRiskCalc::initRow003()
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

void OtcRiskCalc::initRow004()
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
        pItem->setBackgroundColor(QColor(255,204,153));
        QString str;
        str.sprintf("%.1lf", arrData[i]);
        pItem->setText(str);
        pItem->setTextAlignment(Qt::AlignCenter);
        _tblWidget->setItem(3, i+1, pItem);
    }
}

void OtcRiskCalc::initRow005()
{
    double arrData[9]=
    {
        0.0,0.0,0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };

    QTableWidgetItem* pItem = new QTableWidgetItem();
    pItem->setText("超缺避");
    pItem->setTextAlignment(Qt::AlignCenter);
    pItem->setTextColor(QColor(236,41,103));
    //pItem->setBackgroundColor(QColor(255,204,153));
    pItem->setBackgroundColor(QColor(102,255,255));
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
        pItem->setBackgroundColor(QColor(102,255,255));
        QString str;
        str.sprintf("%.1lf", arrData[i]);
        pItem->setText(str);
        pItem->setTextAlignment(Qt::AlignCenter);
        _tblWidget->setItem(4, i+1, pItem);
    }
}

void OtcRiskCalc::onWidget_itemDoubleClicked(QTableWidgetItem *pItem)
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

            initRow002();
            initRow003();
            initRow004();
            initRow005();
        }
    }
    if(row ==0 && col ==1)
    {

    }
    if(row == 0 && col == 6)
    {
        if(m_curCode.ExchID<=0) return;
        QInputDialog dlg(this);
        dlg.setWindowTitle("Input Volatility");
        dlg.setLabelText("Please input：");
        dlg.setInputMode(QInputDialog::TextInput);


        if(dlg.exec() == QInputDialog::Accepted)
        {
           qDebug() << dlg.doubleValue();
          // m_vol = dlg.doubleValue();
           m_vol = dlg.textValue().toDouble();
           m_otcOptData.setRiskVol(m_vol);

           QTableWidgetItem* pItem = new QTableWidgetItem();
           pItem = new QTableWidgetItem();
           pItem->setText(QString::number(m_vol,'f',2));
           pItem->setTextColor(QColor(0,0,103));
           pItem->setBackgroundColor(QColor(255,255,0));
           _tblWidget->setItem(0, 6, pItem);

           updateRiskAfterNewParaInput();
        }
    }
    if(row == 0 && col == 4)
    {
        if(m_curCode.ExchID<=0) return;
        QInputDialog dlg(this);
        dlg.setWindowTitle("Input Volume");
        dlg.setLabelText("Please input：");
        dlg.setInputMode(QInputDialog::TextInput);


        if(dlg.exec() == QInputDialog::Accepted)
        {
           qDebug() << dlg.doubleValue();
           m_position = dlg.textValue().toDouble();

           QTableWidgetItem* pItem = new QTableWidgetItem();
           pItem = new QTableWidgetItem();
           pItem->setText(QString::number(m_position,'f',2));
           pItem->setTextColor(QColor(0,0,103));
           pItem->setBackgroundColor(QColor(255,255,255));
           _tblWidget->setItem(0, 4, pItem);
           updateRiskAfterNewParaInput();
        }
    }
    if(row == 2 && col >0 && col<10)
    {
        if(m_curCode.ExchID<=0) return;
        QInputDialog dlg(this);
        dlg.setWindowTitle("Input Price");
        dlg.setLabelText("Please input：");
        dlg.setInputMode(QInputDialog::TextInput);

        if(dlg.exec() == QInputDialog::Accepted)
        {
           qDebug() << dlg.doubleValue();
           auto price = dlg.textValue().toDouble();

           QTableWidgetItem* pItem = new QTableWidgetItem();
           pItem = new QTableWidgetItem();
           pItem->setText(QString::number(price,'f',2));
           pItem->setTextColor(QColor(0,0,103));
           _tblWidget->setItem(row, col, pItem);
           m_riskCombs[col-1] = price;
           updateRiskComb(price, row, col);
        }
    }
}

void OtcRiskCalc::setInstrument(tagXTInstrument &input)
{
    memset(&m_curCode, 0, sizeof(tagXTInstrument));
    memcpy(&m_curCode, &input, sizeof(tagXTInstrument));
}

void OtcRiskCalc::updateRiskComb(double price, int row, int col)
{
    if(price == 0.0) return;
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

    QString ts(m_curCode.Code);
    std::string inst(XTCodec::AfUtf8_ToString(ts));
    qDebug()<<ts;
    inst = stool::strToUpper(inst);
    auto loginName = stool::loginName();
    //auto otcOpts = GetOtcOptionModule()->getOptPostionsByInstAndUser(inst, loginName);
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
    double totalAvoid = totolAvoidAmout(price)/instMulti/price*(-1.0);
    auto lackVolume = (totolAvoidAmout(price)/instMulti/price*(-1.0)-m_position);

    QTableWidgetItem* pItem = new QTableWidgetItem();
    QString str = QString::number(totalAvoid, 'f', 2);
    pItem->setText(str);

    if(totalAvoid<0)
    {
        pItem->setTextColor(QColor(236,41,103));
    }
    pItem->setTextAlignment(Qt::AlignCenter);
    _tblWidget->setItem(row+1, col, pItem);

    pItem = new QTableWidgetItem();
    str = QString::number(lackVolume, 'f', 2);
    pItem->setText(str);
    if(lackVolume<0)
    {
      pItem->setTextColor(QColor(236,41,103));
    }
    pItem->setTextAlignment(Qt::AlignCenter);
    _tblWidget->setItem(row+2, col, pItem);
}

void OtcRiskCalc::updateRiskAfterNewParaInput()
{
    const int row = 2;
    int col = 1;
    for(; col < 10; ++col)
    {
        updateRiskComb(m_riskCombs[col-1], row, col);
    }
}
