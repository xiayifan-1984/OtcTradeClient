
#include "otcpositiondata.h"
#include "datamodule.h"
#include "XTCodec.h"
#include <string>
#include "./module/otcoptionmodule.h"
#include "./module/quotemodule.h"
#include "./util/stool.h"
#include "configmodule.h"


static inline int posDirectionSign(XTPosiDirectionType dir)
{
    if(dir == XT_PD_Net or dir == XT_PD_Long) return 1;
    if(dir == XT_PD_Short) return -1;
    return 0;
}

int getTotalPostionByInstrument(tagXTInstrument& curCode)
{
    return 0;
}

ViewOtcPosition::ViewOtcPosition(QWidget* parent /* = nullptr*/)
{
    initControls();

    this->resize(1400, 300);
    this->setProperty("name","viewotcposition");

    setWidgetData();

    setAttribute(Qt::WA_DeleteOnClose);

    QString strDefault = "ViewOtcPosition";
    this->setWindowTitle(strDefault);

    m_curPrice = 0.0;
    auto p = GetConfigModule();
    double vol = p ? (p->GetRiskVol(_curExCode)):  0.2;
    double riskFreeRate = p ? (p->s).riskFreeRate : 0.01;
    m_otcOptFuture.setRiskFreeRate(riskFreeRate);
    m_otcOptFuture.setRiskVol(vol);
}

ViewOtcPosition::~ViewOtcPosition()
{

}

void    ViewOtcPosition::initControls()
{
    //[1]
    _tblWidget = new QTableWidget();
    _tblWidget->setObjectName("otcpostiontable");

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
    arr <<"表彰手数" << "现货报价" << "Delta" << "应避手数" << "时间价值";
    arr <<"当日损益";


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
    m_pTimer->start(2000);
}


//======================================================================================================================================================================================================================
//======================================================================================================================================================================================================================
void    ViewOtcPosition::initRow(int rowno)
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
    pItem->setText("-3,374");
    pItem->setBackgroundColor(greenClr);
    pItem->setTextColor(redClr);
    _tblWidget->setItem(rowno, 19, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText("8320");
    pItem->setBackgroundColor(greenClr);
    pItem->setTextColor(blackClr);
    _tblWidget->setItem(rowno, 20, pItem);
}

void    ViewOtcPosition::initTotal(int rowno, double avoidSize, double timeValue, double totalProfit)
{
    QColor  pinkClr(252,213,180);
    QColor  greenClr(147,209,80);
    QColor  redClr(255,0, 0);
    QColor  blackClr(0,0,0);

    QTableWidgetItem* pItem = new QTableWidgetItem();
    pItem->setText("总计");
    _tblWidget->setItem(rowno, 0, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText(QString::number(avoidSize, 'f', 2));
    pItem->setTextColor(redClr);
    _tblWidget->setItem(rowno, 19, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText(QString::number(timeValue, 'f',2));
    pItem->setTextColor(redClr);
    _tblWidget->setItem(rowno, 20, pItem);

    pItem = new QTableWidgetItem();
    pItem->setText(QString::number(totalProfit, 'f', 2));
    pItem->setTextColor(blackClr);
    _tblWidget->setItem(rowno, 21, pItem);
}

void ViewOtcPosition::fillRow(int rowno, const otcOptPosition& opt, double& avoidSize, double& timeValue, double& totalProfit)
{
    QColor  pinkClr(252,213,180);
    QColor  greenClr(147,209,80);
    QColor  redClr(255,0, 0);
    QColor  blackClr(0,0,0);
    int itemidx = 0;
    double curPrice = GetQuoteModule()->GetLastPrice(_curExCode);
    curPrice = curPrice <=0.0 ? 1 : curPrice;
    auto pConf = GetConfigModule();
    m_otcOptFuture.setRiskVol(pConf?(pConf->GetRiskVol(_curExCode)) : 0.2);

    // 客户名称
    QTableWidgetItem* pItem = new QTableWidgetItem();
    pItem->setText(opt.user_name.c_str());
    pItem->setBackgroundColor(pinkClr);
    _tblWidget->setItem(rowno, itemidx++, pItem);

    tagXTNameTable oNameCode;
    QExchange* pExchange = GetDataModule()->GetExchange(_curExCode.ExchID);
    double priceTick = 0.0001;
    int    showDot = 2;
    int multi = 1;
    if(pExchange)
    {
        pExchange->GetOneNameTable(&_curExCode, &oNameCode);
        showDot = oNameCode.ShowDot;
        priceTick = oNameCode.PriceTick!=0.0 ? oNameCode.PriceTick:priceTick;
        multi = oNameCode.VolumeMultiple != 0 ? oNameCode.VolumeMultiple : multi;
        //priceTick = oNameCode.PriceTick;
    }

    double futurePos = getTotalPostionByInstrument(_curExCode);
    //获取名称
    char szname[256]={0};
    GetDataModule()->GetOneName(_curExCode, szname);
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
    pItem->setText(QString::number(pConf?(pConf->GetRiskVol(_curExCode)) : 0.2));
    pItem->setBackgroundColor(pinkClr);
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
    pItem->setText(QString::number(curPrice,'f', showDot));
    pItem->setBackgroundColor(greenClr);
    pItem->setTextColor(blackClr);
    _tblWidget->setItem(rowno, itemidx++, pItem);

    //delta
    auto topt = opt;
    m_otcOptFuture.update(curPrice, topt);
    double delta = m_otcOptFuture.getDelta();

    pItem = new QTableWidgetItem();
    pItem->setText(QString::number(delta));
    pItem->setBackgroundColor(greenClr);
    pItem->setTextColor(QColor(0,0,255));
    _tblWidget->setItem(rowno, itemidx++, pItem);

    //应避手数
    double avoidAmount = delta*opt.multi*curPrice*opt.volume*(-1.0);
    double posAmount = futurePos*curPrice*multi;
    pItem = new QTableWidgetItem();
    pItem->setText(QString::number((avoidAmount)/curPrice/multi, 'f', 2));
    pItem->setBackgroundColor(greenClr);
    pItem->setTextColor(redClr);
    _tblWidget->setItem(rowno, itemidx++, pItem);

    avoidSize += (avoidAmount)/curPrice/multi;

    //时间价值
    double theta = m_otcOptFuture.getTheta();
    pItem = new QTableWidgetItem();
    pItem->setText(QString::number(theta*opt.volume*opt.multi, 'f', 2));
    pItem->setBackgroundColor(greenClr);
    pItem->setTextColor(redClr);
    _tblWidget->setItem(rowno, itemidx++, pItem);

    timeValue += theta*opt.volume*opt.multi;

    //当日损益
    double tradeDir = opt.direction == 0 ? -1.0: 1.0;
    double optProfit = tradeDir*(opt.price - theoryPrice)*opt.multi*opt.volume;
    double futureProfit = futurePos*(curPrice - opt.under_price)*multi;
    pItem = new QTableWidgetItem();
    pItem->setText(QString::number(optProfit + futureProfit, 'f',2));
    pItem->setBackgroundColor(greenClr);
    pItem->setTextColor(blackClr);
    _tblWidget->resizeColumnToContents(itemidx);
    _tblWidget->setItem(rowno, itemidx++, pItem);

    totalProfit += optProfit + futureProfit;
}

void ViewOtcPosition::onTimer()
{
    qDebug()<< "update otc table every 500ms!";
    setCurCommodity(_curExCode);
}

void    ViewOtcPosition::setWidgetData()
{
    //[1]
    QString ts(_curExCode.Code);
    std::string inst(XTCodec::AfUtf8_ToString(ts));
    qDebug()<<ts;
    inst = stool::strToUpper(inst);
    auto loginName = stool::loginName();
    auto otcOpts = GetOtcOptionModule()->getOptPostionsByInst(inst);
    if(otcOpts.size()<=0)
    {
        _tblWidget->setRowCount(1);
        return;
    }

    _tblWidget->setRowCount(otcOpts.size()+1);
    int i = 0;
    double avoidSize = 0.0;
    double timeValue = 0.0;
    double totalProfit = 0.0;
    for(auto p = otcOpts.begin(); p!= otcOpts.end();++p)
    {
        fillRow(i, *p, avoidSize, timeValue, totalProfit);
        ++i;
    }
    for(int j = 0; j<i; ++j)
    {
        _tblWidget->setRowHeight(j,25);
    }
    initTotal(otcOpts.size(), avoidSize, timeValue, totalProfit);
//    _tblWidget->setRowCount(1);

}

void    ViewOtcPosition::setCurCommodity(tagXTInstrument&  oExCode)
{
    memcpy(&_curExCode, &oExCode, sizeof(tagXTInstrument));

    QString strDefault = "ViewOtcPosition";

    char szname[255]={0};
    int iret = GetDataModule()->GetOneName(_curExCode, szname);
    if(iret >0)
    {
        std::string str = szname;
        QString strName = XTCodec::AfGbk_ToQString(str);

        this->setWindowTitle(strDefault + "[" + strName + "]");
    }
    else
    {
        this->setWindowTitle(strDefault);
    }
    setWidgetData();
}

void ViewOtcPosition::setCurPrice(double p)
{
    m_curPrice = p;
}



