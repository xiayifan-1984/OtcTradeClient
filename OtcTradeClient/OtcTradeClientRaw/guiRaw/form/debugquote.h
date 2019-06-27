#ifndef DEBUGQUOTE_H
#define DEBUGQUOTE_H

#include <QtWidgets>

namespace Ui {
class debugquote;
}

class QExchange;
class QuoteDataModel;
class DebugQuote : public QWidget
{
    Q_OBJECT

public:
    explicit DebugQuote(QWidget *parent = nullptr);
    ~DebugQuote();

protected:
    void                initControls();

protected:
    void                closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void                on_combox_indexChange(int index);
    void                on_listwidget_selectChange();
    void                ontimer();
private:
    QTimer*             _timer;
    QExchange*          _curExchange;
    QuoteDataModel*    _customModel;
    Ui::debugquote *    ui;
    QIcon               _exchIco;
    QIcon               _productIco;
};



#endif // DEBUGQUOTE_H
