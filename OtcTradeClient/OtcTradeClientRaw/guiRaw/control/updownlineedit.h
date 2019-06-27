
#ifndef UPDOWNLINEEDITDEFINE_H
#define UPDOWNLINEEDITDEFINE_H

#include <QtWidgets>
class UpDownLineEdit: public QLineEdit
{
    Q_OBJECT

public:
    explicit UpDownLineEdit(QWidget* parent = nullptr);
    ~UpDownLineEdit();

Q_SIGNALS:
    void        editupkey();
    void        editdownkey();

protected:
    virtual void keyPressEvent(QKeyEvent *e);

};

/*
此类的主要功能是:
在edit获取焦点的时候，可以响应Up, Down Key的消息，并将消息emit
曾尝试在parent中通过eventfilter,没有成功。所以采用这种方式。
*/


#endif
