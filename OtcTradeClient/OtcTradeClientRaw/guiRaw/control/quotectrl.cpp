
#include "quotectrl.h"
#include "quotemodule.h"
#include "datamodule.h"
#include "XTCodec.h"

#define     QUOTE_HEADER_HEIGHT     (28)
#define     QUOTE_ITEM_HEIGHT       (20)

CQuoteCtrl::CQuoteCtrl(QWidget* parent /* = nullptr */)
    :QWidget (parent)
{
    memset(&m_oCurInstrument, 0, sizeof(m_oCurInstrument) );
    memset(&m_oQuoteData, 0, sizeof(m_oQuoteData) );
    memset(&m_oNameTable, 0, sizeof(m_oNameTable) );

    initControls();

    setMouseTracking(true);      //设置为不按下鼠标键触发moveEvent

    const QFont oldfont = this->font();
    m_boldFont = oldfont;
    m_boldFont.setBold(true);

    m_bregistered = false;
}

CQuoteCtrl::~CQuoteCtrl()
{

}

void        CQuoteCtrl::initControls()
{
    m_pTimer = new QTimer(this);
    QObject::connect(m_pTimer, SIGNAL(timeout()), this, SLOT(ontimer())  );
    m_pTimer->start(500);

    setMinimumSize(120, 230);
}

void        CQuoteCtrl::getClickPrice(int idx, double& dprice)
{
    dprice =0.0;

    tagXTSnapshot* pFuture =  &m_oQuoteData;

    if (idx >=0 && idx <=4)
    {
        dprice = pFuture->SellItem[4-idx].Price;
    }
    if (idx >=5 && idx <=9)
    {
        dprice = pFuture->BuyItem[idx-5].Price;
    }
}

void        CQuoteCtrl::setCurCommodity(tagXTInstrument& ExCode)
{
    QExchange* p = GetDataModule()->GetExchange(ExCode.ExchID);
    if(p)
    {
        m_oCurInstrument = ExCode;
        p->GetOneNameTable(&ExCode, &m_oNameTable);
        memset(&m_oQuoteData, 0, sizeof(m_oQuoteData) );
    }
    else
    {
        memset(&m_oCurInstrument, 0, sizeof(m_oCurInstrument) );
        memset(&m_oQuoteData, 0, sizeof(m_oQuoteData) );
        memset(&m_oNameTable, 0, sizeof(m_oNameTable) );
    }

    m_bregistered = false;
    ontimer();

    this->update();
}

bool    CQuoteCtrl::IsSmallFloat(double din)
{
    if ( din< 1e-10 && din > -1e-10)
    {
        return true;
    }
    return false;
}

void        CQuoteCtrl::  ontimer()
{
    if(m_oCurInstrument.ExchID >0)
    {
        int iret = GetQuoteModule()->GetSnapShot(m_oCurInstrument, &m_oQuoteData);
        if(iret <= 0 )
        {
            if(!m_bregistered)
            {
                GetQuoteModule()->SetCodeList(&m_oCurInstrument, 1);
                m_bregistered = true;
            }
            return;
        }

        //规整5档行情，如果行情只有1档，则按tick生成剩余4档
        double prtick = m_oNameTable.PriceTick;
        double baseprice = m_oQuoteData.BuyItem[0].Price;
        if(baseprice >0 || baseprice <0)
        {
            for(int i=1; i<5; i++)
            {
                if(IsSmallFloat(m_oQuoteData.BuyItem[i].Price) )
                {
                    m_oQuoteData.BuyItem[i].Price = baseprice - i*prtick;
                }
            }
        }
        baseprice = m_oQuoteData.SellItem[0].Price;
        if(baseprice >0 || baseprice <0)
        {
            for(int i=1; i<5; i++)
            {
                if(IsSmallFloat(m_oQuoteData.SellItem[i].Price) )
                {
                    m_oQuoteData.SellItem[i].Price = baseprice + i*prtick;
                }
            }
        }

    }
    this->update();

}

//==========================================================================================================================================================================================================
void    CQuoteCtrl::paintEvent(QPaintEvent*  )
{
    QPainter painter(this);

    painter.save();

    //[0]
    QRect rect(0, 0, width(), height() );
    /*
    QLinearGradient ( qreal x1, qreal y1, qreal x2, qreal y2 )
    //其中x1,y1表示渐变起始坐标, x2,y2表示渐变终点坐标
    //如果只有x相等,则表示垂直线性渐变,如果只有y相等,则表示平行线性渐变,否则就是斜角线性渐变
    QLinearGradient linearGradient(0, 0, 0, rect.bottom());
    linearGradient.setColorAt(0,  QColor(244,244,244) );
    linearGradient.setColorAt(0.5, QColor(216,216,216) );
    linearGradient.setColorAt(1.0, QColor(208,208,208) );
    painter.fillRect(rect, QBrush(linearGradient));
    */

    //[1]
    painter.setPen(QColor(212,212,212));
    painter.drawLine(rect.left(), rect.top()+1, rect.right()-1, rect.top()+1 );
    painter.drawLine(rect.right()-1, rect.top(), rect.right()-1, rect.bottom() );
    painter.drawLine(rect.right()-1, rect.bottom(), rect.left(), rect.bottom() );
    painter.drawLine(rect.left(), rect.bottom(), rect.left(), rect.top() );

    //[2]
    QRect rcHead( rect.left() +1, rect.top() +1, rect.right()-1, rect.top() + QUOTE_HEADER_HEIGHT +1 );
    drawHead(rcHead, painter);

    //[3]
    QRect rcQuote(rect.left() +1, rect.top() + QUOTE_HEADER_HEIGHT +2, rect.right()-1, rect.bottom()-1 );
    drawQuote(rcQuote, painter);

    //[4]
    painter.restore();

}

void       CQuoteCtrl:: drawHead(QRect& rcHead, QPainter& painter)
{
    //[0]
    QLinearGradient linearGradient(0, 0, rcHead.right(), 0);
    linearGradient.setColorAt(0, QColor(235,132,29) );
    linearGradient.setColorAt(0.5,  QColor(241,187,37) );
    linearGradient.setColorAt(1.0,  QColor(254,249,186) );
    painter.fillRect(rcHead, QBrush(linearGradient));
    //painter.fillRect(rcHead, QColor(255,230,153));

    //[0.1]
    const QFont oldfont = painter.font();
    painter.setFont(m_boldFont);

    //[1]
    painter.setPen(QColor(0,0,0));

    QString strcode = tr(" ");
    if(m_oCurInstrument.ExchID >0)
    {
        std::string str = m_oCurInstrument.Code;
        strcode += XTCodec::AfGbk_ToQString(str);
    }
    else
    {
        strcode += tr("-");
    }
    painter.drawText( rcHead.left(), rcHead.top(), rcHead.width(), rcHead.height(), Qt::AlignLeft|Qt::AlignVCenter, strcode);

    /*
    //[2]
    QString strName;
    //painter.setPen(QColor(240,248,136));
    if(m_oCurInstrument.ExchID >0)
    {
        std::string str = m_oNameTable.Name;
        strName = XTCodec::AfGbk_ToQString(str);
    }
    else
    {
        strName = tr("-");
    }
    painter.drawText( rcHead.left()+rcHead.width()/2, rcHead.top(), rcHead.width()/2, rcHead.height(), Qt::AlignRight|Qt::AlignVCenter, strName );
    */

    //[0.2]
    painter.setFont(oldfont);
}

void       CQuoteCtrl::drawQuote(QRect& rcQuote, QPainter& painter)
{
    //[0]
    painter.fillRect(rcQuote, QColor(240, 240, 240));

    //[1]Sell
    QRect rcItem = rcQuote;
    rcItem.setBottom( rcItem.top() );
    QRect rcGrid;
    for(int i=0; i<5; i++)
    {
        rcItem.setTop( rcItem.bottom() );
        rcItem.setBottom( rcItem.top() + QUOTE_ITEM_HEIGHT );

        if(i ==4)
        {
            painter.fillRect(rcItem, QColor(180,255,180));
        }

        rcGrid = rcItem;
        rcGrid.setRight( rcGrid.left() + rcItem.width()/4  );


        QString strText;
        switch(i)
        {
        case 0: strText = tr(" 卖五");painter.setPen( QColor(109,109,109));break;
        case 1: strText = tr(" 卖四");painter.setPen( QColor(109,109,109));break;
        case 2: strText = tr(" 卖三");painter.setPen( QColor(109,109,109));break;
        case 3: strText = tr(" 卖二");painter.setPen( QColor(109,109,109));break;
        case 4: strText = tr(" 卖一");painter.setPen( QColor(0,0,0));break;
        }

        painter.drawText(rcGrid, Qt::AlignLeft|Qt::AlignVCenter, strText);

        rcGrid.setLeft( rcGrid.right() );
        rcGrid.setRight(rcGrid.left() + rcItem.width()/2 );

        if(m_oQuoteData.SellItem[4-i].Price >0 || m_oQuoteData.SellItem[4-i].Price <0)
        {
            if(m_oNameTable.ShowDot >0)
            {
                char szformat[32]={0};
                _snprintf(szformat,31, "%%.0%dlf", m_oNameTable.ShowDot );
                strText.sprintf(szformat, m_oQuoteData.SellItem[4-i].Price );
            }
            else
            {
                int nprice = m_oQuoteData.SellItem[4-i].Price;
                strText.sprintf("%d", nprice);
            }
        }
        else
        {
            strText = tr("-");
        }

        //[0.1]
        const QFont oldfont = painter.font();
        painter.setFont(m_boldFont);

        painter.drawText(rcGrid, Qt::AlignRight|Qt::AlignVCenter, strText);

        //[0.2]
        painter.setFont(oldfont);

        rcGrid.setLeft(rcGrid.right());
        rcGrid.setRight(rcItem.right()-1);
        painter.setPen(QColor(0, 0, 0));
        if(m_oQuoteData.SellItem[4-i].Volume >0)
        {
            strText.sprintf("%d", m_oQuoteData.SellItem[4-i].Volume );
        }
        else
        {
            strText = tr("-");
        }
        painter.drawText(rcGrid, Qt::AlignRight|Qt::AlignVCenter, strText);

    }


    for(int i=0; i<5; i++)
    {
        rcItem.setTop( rcItem.bottom() );
        rcItem.setBottom( rcItem.top() + QUOTE_ITEM_HEIGHT );

        if(i ==0)
        {
            painter.fillRect(rcItem, QColor(255,220,220));
        }

        rcGrid = rcItem;
        rcGrid.setRight( rcGrid.left() + rcItem.width()/4  );
        painter.setPen( QColor(0,0,0));

        QString strText;
        switch(i)
        {
        case 0: strText = tr(" 买一");painter.setPen( QColor(0,0,0));break;
        case 1: strText = tr(" 买二");painter.setPen( QColor(109,109,109));break;
        case 2: strText = tr(" 买三");painter.setPen( QColor(109,109,109));break;
        case 3: strText = tr(" 买四");painter.setPen( QColor(109,109,109));break;
        case 4: strText = tr(" 买五");painter.setPen( QColor(109,109,109));break;
        }

        painter.drawText(rcGrid, Qt::AlignLeft|Qt::AlignVCenter, strText);

        rcGrid.setLeft( rcGrid.right() );
        rcGrid.setRight(rcGrid.left() + rcItem.width()/2 );

        if(m_oQuoteData.BuyItem[i].Price >0 || m_oQuoteData.BuyItem[i].Price <0)
        {
            if(m_oNameTable.ShowDot >0)
            {
                char szformat[32]={0};
                _snprintf(szformat,31, "%%.0%dlf", m_oNameTable.ShowDot );
                strText.sprintf(szformat, m_oQuoteData.BuyItem[i].Price );
            }
            else
            {
                int nprice = m_oQuoteData.BuyItem[i].Price;
                strText.sprintf("%d", nprice);
            }
        }
        else
        {
            strText = tr("-");
        }

        //[0.1]
        const QFont oldfont = painter.font();
        painter.setFont(m_boldFont);

        painter.drawText(rcGrid, Qt::AlignRight|Qt::AlignVCenter, strText);

        //[0.2]
        painter.setFont(oldfont);

        rcGrid.setLeft(rcGrid.right());
        rcGrid.setRight(rcItem.right()-1);
        if(m_oQuoteData.BuyItem[i].Volume >0)
        {
            strText.sprintf("%d", m_oQuoteData.BuyItem[i].Volume );
        }
        else
        {
            strText = tr("-");
        }
        painter.drawText(rcGrid, Qt::AlignRight|Qt::AlignVCenter, strText);

    }

}

void CQuoteCtrl::mouseMoveEvent(QMouseEvent *event)
{
    QPoint curPos = event->pos();

    QRect rect(0, 0, width(), height() );


    rect.setTop( rect.top() + QUOTE_HEADER_HEIGHT +2 );
    rect.setBottom(  rect.top() + QUOTE_ITEM_HEIGHT*10 );

    QRect rcItem = rect;
    rcItem.setLeft( rcItem.left() + rect.width()/4 );
    rcItem.setRight(rcItem.left() + rect.width()/2 );

    if(rcItem.contains(curPos))
    {
        this->setCursor(Qt::PointingHandCursor);
    }
    else
    {
        this->setCursor(Qt::ArrowCursor);
    }
}

void    CQuoteCtrl::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

}

void    CQuoteCtrl::mousePressEvent(QMouseEvent *event)
{
    //当左键被按下时，保存首坐标.并更新重绘.
    if (event->button() == Qt::LeftButton)
    {
        //点击的时候定下初始位置.
        m_startpos = event->pos();
    }

}

void    CQuoteCtrl::mouseReleaseEvent(QMouseEvent *event)
{
    //当左键释放后，保存尾坐标.
    if (event->button() == Qt::LeftButton)
    {
        m_endpos = event->pos();

        int n1 = checkPos(m_startpos);
        int n2 = checkPos(m_endpos);
        if(n1 == n2 && n1 >= 0)
        {
            emit quoteClick(n1);
        }
    }

}

int         CQuoteCtrl::checkPos(const QPoint& curPos)
{
    QRect rect(0, 0, width(), height() );


    rect.setTop( rect.top() + QUOTE_HEADER_HEIGHT +2 );
    rect.setBottom(  rect.top() + QUOTE_ITEM_HEIGHT*10 );

    QRect rcItem = rect;
    rcItem.setLeft( rcItem.left() + rect.width()/4 );
    rcItem.setRight(rcItem.left() + rect.width()/2 );

    if(!rcItem.contains(curPos))
    {
        return -1;
    }

    int idx = (curPos.y() - rcItem.top() ) / (rcItem.height()/10);
    return idx;
}

QSize   CQuoteCtrl::minimumSizeHint() const
{
    return QSize( 140 , 236);
}
