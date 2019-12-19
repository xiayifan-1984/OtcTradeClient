
#include "docktitlebar.h"
#include "iconhelper.h"
#include <QDebug>



CDockTitlebar::CDockTitlebar(QWidget* parent /*= nullptr*/)
    :QWidget(parent)
{
    initControls();
}

CDockTitlebar::~CDockTitlebar()
{

}

void    CDockTitlebar::initControls()
{
    this->setMinimumSize(100, 24);

    this->setStyleSheet("background-color:rgb(228,229,232); border: 0px;");
    setAttribute(Qt::WA_TranslucentBackground, true);

    //[1]
    m_pLabelTitle = new QLabel();

    //[2]默认有2个按钮
    tagTitleButton oInfo;
    oInfo.type =0;
    oInfo.id = TITLEBUTTON_CLOSE;
    oInfo.real = new QPushButton();
    m_arrButtons.push_back(oInfo);

    oInfo.type =0;
    oInfo.id = TITLEBUTTON_PIN;
    oInfo.real = new QPushButton();
    m_arrButtons.push_back(oInfo);

    this->setProperty("name", "titlebar");

}

void    CDockTitlebar::setTitle(const QString strTitle)
{
    m_strTitle = strTitle;

    if(m_pLabelTitle)
    {
        m_pLabelTitle->setText(m_strTitle);
    }
}
void    CDockTitlebar::addStockID(int id)
{
    tagTitleButton oInfo;
    oInfo.type =0;
    oInfo.id = id;
    oInfo.real = new QPushButton();
    m_arrButtons.push_back(oInfo);
}

void    CDockTitlebar::addText(int id, QString strText)
{
    QPushButton* pBtn = new QPushButton();
    pBtn->setText(strText);

    tagTitleButton oInfo;
    oInfo.type =1;
    oInfo.id = id;
    oInfo.real = pBtn;
    oInfo.text = strText;

    m_arrButtons.push_back(oInfo);

    //绑定消息
    pBtn->setProperty("btnid", id);
    QObject::connect( pBtn, SIGNAL(clicked()), this, SLOT(on_btn_clicked()) );
}

void    CDockTitlebar::ShowHideButton(int id, bool bShow /* = true*/ )
{
    std::vector<tagTitleButton>::iterator itb = m_arrButtons.begin();
    while( itb != m_arrButtons.end() )
    {
        if(itb->id == id)
        {
            if(bShow)
                itb->real->show();
            else
                itb->real->hide();
            return;
        }
        itb++;
    }

}

void    CDockTitlebar::setStyle()
{
    if(m_pLabelTitle)
    {
        m_pLabelTitle->setStyleSheet("background-color:transparent; border: 0px; color:rgb(145,145,145);font-weight:bold;");
    }
    for( auto& oInfo: m_arrButtons)
    {
        if(oInfo.real)
        {
            if(oInfo.type == 0)
            {
                if(oInfo.id == TITLEBUTTON_CLOSE)
                    IconHelper::instance()->setIcon(oInfo.real, QChar(0xf00d));
                else if(oInfo.id == TITLEBUTTON_PIN)
                    IconHelper::instance()->setIcon(oInfo.real, QChar(0xf08d));
                else if(oInfo.id == TITLEBUTTON_CONFIG)
                    IconHelper::instance()->setIcon(oInfo.real, QChar(0xf013));
                else if(oInfo.id == TITLEBUTTON_MENU)
                    IconHelper::instance()->setIcon(oInfo.real, QChar(0xf103));

                oInfo.real->setStyleSheet("background-color:transparent; border: 0px; min-width:12px; padding:0px;");
            }
            else
            {
                oInfo.real->setStyleSheet("background-color:transparent; border: 0px; min-width:36px; padding:0px; color:blue;text-decoration:underline;font-weight:bold;");
                oInfo.real->setCursor(Qt::PointingHandCursor);
            }
        }
    }
}

void    CDockTitlebar::reLayout()
{
    //[1]
    QHBoxLayout* h2 = new QHBoxLayout();
    for(auto& oInfo : m_arrButtons )
    {
        h2->insertWidget(0, oInfo.real);    //倒序放入
    }
    h2->addSpacing(6);     //在最右边，加5像素的空位SpacerItem

    h2->setSpacing(6);     //指在内部，各个元素之间，保持6个像素的空隙

    //[2]
    QHBoxLayout* h1 = new QHBoxLayout();
    h1->addSpacing(10);
    h1->addWidget(m_pLabelTitle);
    h1->addStretch();

    QHBoxLayout* mainlayout = new QHBoxLayout();
    mainlayout->addLayout(h1);
    mainlayout->addLayout(h2);

    mainlayout->setMargin(0);
    mainlayout->setSpacing(0);
    this->setLayout(mainlayout);
}

void    CDockTitlebar::setReady()
{
    //[2]设置风格
    setStyle();

    //[3]加入layout
    reLayout();
}

void    CDockTitlebar::on_btn_clicked()
{
    QPushButton *rb = qobject_cast<QPushButton *>(sender());
    QVariant vt = rb->property("btnid");
    int id = vt.toInt();

    emit titleButtonClick(id);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////









