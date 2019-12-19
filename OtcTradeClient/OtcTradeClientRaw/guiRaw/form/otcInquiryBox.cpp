
#include "otcInquiryBox.h"
#include "./module/otcoptionmodule.h"
#include <string>
#include <algorithm>
#include "datamodule.h"
#include "XTBase.h"
#include "configmodule.h"
#include "quotemodule.h"
#include "XTCodec.h"
using namespace std;

extern int getTotalPostionByInstrument(tagXTInstrument& curCode);

ViewInquiryBox::ViewInquiryBox(QWidget* parent /* = nullptr*/)
{
    initControls();

    this->setProperty("name","viewinquirybox");

    auto pConf = GetConfigModule();
    m_otcOptFuture.setRiskFreeRate(0.045);
    m_otcOptFuture.setRiskVol(0.2);
    setWidgetData();
}

ViewInquiryBox::~ViewInquiryBox()
{

}

void    ViewInquiryBox::initControls()
{
    //[1]
    _tblWidget = new QTableWidget();
    _tblWidget->setObjectName("inquirytable");

    _tblWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //禁止编辑
    _tblWidget->verticalHeader()->setVisible(false);   //隐藏列表头
    _tblWidget->horizontalHeader()->setHighlightSections(false); //点击表时不对表头行光亮（获取焦点）

    //[2]
    QVBoxLayout* v1 = new QVBoxLayout();

    v1->addWidget(_tblWidget);
    this->setLayout(v1);

    //[3]设置表头
    const QFont oldfont = _tblWidget->font();
    QFont newfont(oldfont);
    newfont.setBold(true);

    QStringList arr;
    arr <<"客户名称" << "标的名称" << "标的代码" << "交易数量"<<"名目本金" << "形式";
    arr <<"客户方向" << "执行价" << "起始价格" << "交易日" << "到期日";
    arr <<"商品波动度" << "期权市价" << "期权理论价" << "避险波动度" << "承作opt数";
    arr <<"表彰手数" << "现货报价" << "Delta" << "应避手数" << "需补缴资金";
    arr <<"    ";


    _tblWidget->setColumnCount(22);
    _tblWidget->setHorizontalHeaderLabels(arr);

    for(int i=0; i<22; i++)
    {
        if(i==8 || i ==9)
        {
            _tblWidget->setColumnWidth(i, 100);
        }
        else
        {
            _tblWidget->setColumnWidth(i, 70);
        }
    }

    //[4]
    QObject::connect(_tblWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)),  this, SLOT(onWidget_itemDoubleClicked(QTableWidgetItem*)) );

    m_pTimer = new QTimer(this);
    QObject::connect(m_pTimer, SIGNAL(timeout()), this, SLOT(onTimer()));
    m_pTimer->start(1000);
}


//======================================================================================================================================================================================================================
//======================================================================================================================================================================================================================
void    ViewInquiryBox::initRow(int rowno)
{
    QColor  pinkClr(252,213,180);
    QColor  greenClr(147,209,80);
    QColor  redClr(255,0, 0);
    QColor  blackClr(0,0,0);

    QTableWidgetItem* pItem = new QTableWidgetItem();
    pItem->setText("上海筑欣");
    pItem->setBackgroundColor(pinkClr);
    _tblWidget->setItem(rowno, 0, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText("焦炭1505");
    pItem->setBackgroundColor(greenClr);
    _tblWidget->setItem(rowno, 1, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText("J1505");
    pItem->setBackgroundColor(pinkClr);
    _tblWidget->setItem(rowno, 2, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText("21,000.000");
    pItem->setBackgroundColor(greenClr);
    pItem->setTextColor(redClr);
    _tblWidget->setItem(rowno, 3, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText("c");
    pItem->setBackgroundColor(pinkClr);
    pItem->setTextColor(redClr);
    _tblWidget->setItem(rowno, 4, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText("-1");
    pItem->setBackgroundColor(pinkClr);
    pItem->setTextColor(redClr);
    _tblWidget->setItem(rowno, 5, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText("1,050");
    pItem->setBackgroundColor(pinkClr);
    pItem->setTextColor(redClr);
    _tblWidget->setItem(rowno, 6, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText("1037");
    pItem->setBackgroundColor(greenClr);
    pItem->setTextColor(redClr);
    _tblWidget->setItem(rowno, 7, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText("2014年12月5日");
    pItem->setBackgroundColor(greenClr);
    pItem->setTextColor(redClr);
    _tblWidget->setItem(rowno, 8, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText("2015年1月5日");
    pItem->setBackgroundColor(pinkClr);
    pItem->setTextColor(redClr);
    _tblWidget->setItem(rowno, 9, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText("11.20%");
    pItem->setBackgroundColor(pinkClr);
    pItem->setTextColor(redClr);
    _tblWidget->setItem(rowno, 10, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText("0.87");
    pItem->setBackgroundColor(greenClr);
    pItem->setTextColor(blackClr);
    _tblWidget->setItem(rowno, 11, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText("1.01");
    pItem->setBackgroundColor(greenClr);
    pItem->setTextColor(blackClr);
    _tblWidget->setItem(rowno, 12, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText("15%");
    pItem->setBackgroundColor(pinkClr);
    pItem->setTextColor(blackClr);
    _tblWidget->setItem(rowno, 13, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText("20000");
    pItem->setBackgroundColor(pinkClr);
    pItem->setTextColor(redClr);
    _tblWidget->setItem(rowno, 14, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText("200");
    pItem->setBackgroundColor(greenClr);
    pItem->setTextColor(blackClr);
    _tblWidget->setItem(rowno, 15, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText("1011");
    pItem->setBackgroundColor(greenClr);
    pItem->setTextColor(blackClr);
    _tblWidget->setItem(rowno, 16, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText("-7.47%");
    pItem->setBackgroundColor(greenClr);
    pItem->setTextColor(QColor(0,0,255));
    _tblWidget->setItem(rowno, 17, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText("-14.9");
    pItem->setBackgroundColor(greenClr);
    pItem->setTextColor(redClr);
    _tblWidget->setItem(rowno, 18, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText("326200");
    pItem->setBackgroundColor(greenClr);
    pItem->setTextColor(blackClr);
    _tblWidget->setItem(rowno, 19, pItem);

    QLabel* label=new QLabel;
    label->setPixmap(QPixmap(":/image/dotrade.png"));
    _tblWidget->setCellWidget(rowno, 20, label);
}

void ViewInquiryBox::fillRow(int rowno, const otcOptPosition &opt)
{
    QColor  pinkClr(252,213,180);
    QColor  greenClr(147,209,80);
    QColor  redClr(255,0, 0);
    QColor  blackClr(0,0,0);

  /*  std::string str = XTCodec::AfUtf8_ToString(strcode);
    tagXTInstrument oExCode;
    memset(&oExCode, 0, sizeof(oExCode));
    strncpy(oExCode.Code, str.c_str(), 30);*/

    string inst(opt.under_code);
    transform(inst.begin(), inst.end(), inst.begin(), ::tolower);
    auto dataModule = GetDataModule();
    if(!dataModule) return;

    tagXTInstrument xtInst;
    memset(&xtInst, 0, sizeof(xtInst));
    strncpy(xtInst.Code, inst.c_str(), 30);
    if(dataModule->VerifyExCode(xtInst) != 2) return;


    int itemidx = 0;
    double curPrice = GetQuoteModule()->GetLastPrice(xtInst);
    curPrice = curPrice <=0.0 ? 1:curPrice;
    auto pConf = GetConfigModule();
    auto volOfInst = pConf ? (pConf->GetRiskVol(xtInst)):0.2;

    m_otcOptFuture.setRiskVol(volOfInst);
    auto topt = opt;
    m_otcOptFuture.update(curPrice, topt);

    // 客户名称
    QTableWidgetItem* pItem = new QTableWidgetItem();
    pItem->setText(opt.user_name.c_str());
    pItem->setBackgroundColor(pinkClr);
    _tblWidget->setItem(rowno, itemidx++, pItem);

    tagXTNameTable oNameCode;
    QExchange* pExchange = GetDataModule()->GetExchange(xtInst.ExchID);
    double priceTick = 0.0001;
    int    showDot = 2;
    int multi = 1;
    if(pExchange)
    {
        pExchange->GetOneNameTable(&xtInst, &oNameCode);
        showDot = oNameCode.ShowDot;
        priceTick = oNameCode.PriceTick!=0.0 ? oNameCode.PriceTick:priceTick;
        multi = oNameCode.VolumeMultiple;
        //priceTick = oNameCode.PriceTick;
    }
    //获取名称
    char szname[256]={0};
    GetDataModule()->GetOneName(xtInst, szname);
    std::string str = szname;
    QString strName = XTCodec::AfGbk_ToQString(str);

    //标的名称
    pItem = new QTableWidgetItem();
    pItem->setText(strName);
    pItem->setBackgroundColor(greenClr);
    _tblWidget->setItem(rowno, itemidx++, pItem);

    //标的代码
    pItem = new QTableWidgetItem();
    pItem->setText(opt.under_code.c_str());
    pItem->setBackgroundColor(pinkClr);
    _tblWidget->setItem(rowno, itemidx++, pItem);

    //交易数量
    pItem = new QTableWidgetItem();
    pItem->setText(QString::number(opt.volume, 'f', 2));
    pItem->setBackgroundColor(pinkClr);
    _tblWidget->setItem(rowno, itemidx++, pItem);

    //名目本金
    pItem = new QTableWidgetItem();
    pItem->setText(QString::number(curPrice*opt.volume*opt.multi, 'f', showDot));
    pItem->setBackgroundColor(greenClr);
    pItem->setTextColor(redClr);
    _tblWidget->setItem(rowno, itemidx++, pItem);

    //形式
    pItem = new QTableWidgetItem();
    pItem->setText(opt.callput == 0 ? "c":"p");
    pItem->setBackgroundColor(pinkClr);
    pItem->setTextColor(redClr);
    _tblWidget->setItem(rowno, itemidx++, pItem);

    //客户方向
    pItem = new QTableWidgetItem();
    pItem->setText(opt.direction == 0 ? "买":"卖");
    pItem->setBackgroundColor(pinkClr);
    pItem->setTextColor(redClr);
    _tblWidget->setItem(rowno, itemidx++, pItem);

    //执行价
    pItem = new QTableWidgetItem();
    pItem->setText(QString::number(opt.exec_price));
    pItem->setBackgroundColor(pinkClr);
    pItem->setTextColor(redClr);
    _tblWidget->setItem(rowno, itemidx++, pItem);

    //起始价格
    pItem = new QTableWidgetItem();
    pItem->setText(QString::number(opt.under_price));
    pItem->setBackgroundColor(greenClr);
    pItem->setTextColor(redClr);
    _tblWidget->setItem(rowno,itemidx++, pItem);

    //交易日
    pItem = new QTableWidgetItem();
    pItem->setText(QString::number(opt.trade_date));
    pItem->setBackgroundColor(greenClr);
    pItem->setTextColor(redClr);
    _tblWidget->setItem(rowno, itemidx++, pItem);

    //到期日
    pItem = new QTableWidgetItem();
    pItem->setText(QString::number(opt.enddate));
    pItem->setBackgroundColor(pinkClr);
    pItem->setTextColor(redClr);
    _tblWidget->setItem(rowno, itemidx++, pItem);

    //商品波动度
    pItem = new QTableWidgetItem();
    pItem->setText(QString::number(opt.under_sigma));
    pItem->setBackgroundColor(pinkClr);
    pItem->setTextColor(redClr);
    _tblWidget->setItem(rowno, itemidx++, pItem);

    //期权市价
    pItem = new QTableWidgetItem();
    pItem->setText(QString::number(opt.price));
    pItem->setBackgroundColor(greenClr);
    pItem->setTextColor(blackClr);
    _tblWidget->setItem(rowno, itemidx++, pItem);

    //理论价格
    double theoryPrice = m_otcOptFuture.getBsPrice();
    pItem = new QTableWidgetItem();
    pItem->setText(QString::number(theoryPrice, 'f', 2));
    pItem->setBackgroundColor(greenClr);
    pItem->setTextColor(blackClr);
    _tblWidget->setItem(rowno,itemidx++, pItem);

    //避险波动率
    pItem = new QTableWidgetItem();
    pItem->setText(QString::number(pConf?(pConf->GetRiskVol(xtInst)) : 0.2));
    pItem->setBackgroundColor(QColor(255,255,0));
    pItem->setTextColor(blackClr);
    _tblWidget->setItem(rowno, itemidx++, pItem);

    //承作数
    pItem = new QTableWidgetItem();
    pItem->setText(QString::number(opt.volume));
    pItem->setBackgroundColor(pinkClr);
    pItem->setTextColor(redClr);
    _tblWidget->setItem(rowno, itemidx++, pItem);

    //表彰手数
    pItem = new QTableWidgetItem();
    pItem->setText(QString::number(opt.volume*opt.multi/multi));
    pItem->setBackgroundColor(greenClr);
    pItem->setTextColor(blackClr);
    _tblWidget->setItem(rowno, itemidx++, pItem);

    //现货报价
    pItem = new QTableWidgetItem();
    auto pp = GetQuoteModule()->GetLastPrice(xtInst);
    pItem->setText(QString::number(pp,'f', showDot));
    pItem->setBackgroundColor(greenClr);
    pItem->setTextColor(blackClr);
    _tblWidget->setItem(rowno, itemidx++, pItem);

    //delta
    double delta = m_otcOptFuture.getDelta();

    pItem = new QTableWidgetItem();
    pItem->setText(QString::number(delta));
    pItem->setBackgroundColor(greenClr);
    pItem->setTextColor(QColor(0,0,255));
    _tblWidget->setItem(rowno, itemidx++, pItem);

    //应避手数
    double avoidAmount = delta*opt.multi*curPrice*opt.volume;
    double posAmount = getTotalPostionByInstrument(xtInst)*curPrice*multi;
    pItem = new QTableWidgetItem();
    pItem->setText(QString::number((avoidAmount)/curPrice/multi*(-1.0), 'f', 2));
    pItem->setBackgroundColor(greenClr);
    pItem->setTextColor(redClr);
    _tblWidget->setItem(rowno, itemidx++, pItem);

    //需补缴资金
    double customerProfit = opt.direction == 0 ? 0.0 : opt.price - theoryPrice;
    double marginToCall = customerProfit >= 0.0 ? 0 : std::abs(customerProfit);
    pItem = new QTableWidgetItem();
    pItem->setText(QString::number(marginToCall, 'f', 2));
    pItem->setBackgroundColor(greenClr);
    pItem->setTextColor(blackClr);
    _tblWidget->resizeColumnToContents(itemidx);
    _tblWidget->setItem(rowno, itemidx++, pItem);
}

void ViewInquiryBox::onTimer()
{
    setWidgetData();
}

void    ViewInquiryBox::setWidgetData()
{
    //[1]
  /*  _tblWidget->setRowCount(2);

    //[2]
    initRow(0);
    initRow(1);

    for(int i=0; i<2; i++)
    {
        _tblWidget->setRowHeight(i,25);
    }*/
    m_otcPositions = GetOtcOptionModule()->getAllOtcPositions();
    if(m_otcPositions.size()<=0)
    {
        _tblWidget->setRowCount(1);
        return;
    }
    _tblWidget->setRowCount(m_otcPositions.size()+1);
    int i = 0;
    for(auto p = m_otcPositions.begin(); p!= m_otcPositions.end();++p)
    {
        fillRow(i, *p);
        ++i;
    }
    for(int j = 0; j<i; ++j)
    {
        _tblWidget->setRowHeight(j,25);
    }
}


void   ViewInquiryBox::onWidget_itemDoubleClicked(QTableWidgetItem* pItem)
{
    //@@@@@@@@@@
    int row = pItem->row();
    int col = pItem->column();

    qDebug() << "itemDoubleClicked" << row << " ][" << col;

    auto& opt = m_otcPositions[row];

    string inst(opt.under_code);
    auto dataModule = GetDataModule();
    if(!dataModule) return;

    tagXTInstrument xtInst;
    memset(&xtInst, 0, sizeof(xtInst));
    strncpy(xtInst.Code, inst.c_str(), 30);
    if(dataModule->VerifyExCode(xtInst) != 2) return;

    QInputDialog dlg;
    QString code(opt.under_code.c_str());
    dlg.setLabelText("Input vol - " + code);

    dlg.setInputMode(QInputDialog::TextInput);

    auto p1 = GetConfigModule();
    if(p1)
    {
        dlg.setTextValue(QString::number(p1->GetRiskVol(xtInst), 'f',2));
    }

    if(dlg.exec() == QInputDialog::Accepted)
    {
        auto vol = dlg.textValue().toDouble();
        auto pConf = GetConfigModule();
        if(!pConf) return;
        pConf->InsertRiskVol(xtInst, vol);
    }

}




