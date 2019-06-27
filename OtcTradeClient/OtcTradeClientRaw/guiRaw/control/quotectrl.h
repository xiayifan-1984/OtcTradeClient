#ifndef QUOTECTRLDEFINE_H
#define QUOTECTRLDEFINE_H

#include <QtWidgets>
#include "XTBase.h"
#include "XTStaticData.h"
#include "XTQuoteStruct.h"

class CQuoteCtrl: public QWidget
{
    Q_OBJECT

public:
    explicit CQuoteCtrl(QWidget* parent = nullptr);
    ~CQuoteCtrl();

public:
    void        setCurCommodity(tagXTInstrument& ExCode);
    void        getClickPrice(int idx, double& dprice);
    virtual QSize minimumSizeHint() const;

Q_SIGNALS:
    void        quoteClick(int idx);

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

protected:
    void        initControls();
    void        drawHead(QRect& rcHead, QPainter& painter);
    void        drawQuote(QRect& rcQuote, QPainter& painter);
    int         checkPos(const QPoint& curPos);
    bool        IsSmallFloat(double din);

private slots:
    void        ontimer();

protected:
    QTimer*     m_pTimer;

    tagXTInstrument			m_oCurInstrument;
    tagXTSnapshot			m_oQuoteData;
    tagXTNameTable          m_oNameTable;

    QPoint                  m_startpos;
    QPoint                  m_endpos;

    QFont                   m_boldFont;
    bool                    m_bregistered;

};




#endif
