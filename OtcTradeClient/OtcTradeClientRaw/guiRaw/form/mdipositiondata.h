#ifndef MDIPOSITIONDATADEFINE_H
#define MDIPOSITIONDATADEFINE_H

#include <QtWidgets>

class TransactionEventArgs;
class ModelNormalPosition;
class ModelCompositePosition;
class CMDIPositionData : public QWidget
{
    Q_OBJECT

public:
    explicit CMDIPositionData(QWidget *parent = nullptr);
    ~CMDIPositionData();

protected:
    void                initControls();

protected:
    void                closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void                on_combox_indexChange(int index);
    void                onTransactEvent(TransactionEventArgs* e);

private:
    QLabel*                     _label;
    QComboBox*                  _combox;

    QTabWidget*                 _tabWidget;

    QTableView*                 _tblNormalView;
    ModelNormalPosition*        _csmNormalModel;

    QTableView*                 _tblCompositeView;
    ModelCompositePosition*     _csmCompositeModel;

};







#endif
