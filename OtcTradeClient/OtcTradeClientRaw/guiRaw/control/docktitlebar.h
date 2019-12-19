
#ifndef DOCKTITLEBAR_DEFINE_H
#define DOCKTITLEBAR_DEFINE_H

#include <QtWidgets>
#include <vector>

#define     TITLEBUTTON_CLOSE       1           //关闭
#define     TITLEBUTTON_PIN         2           //固定针
#define     TITLEBUTTON_MENU        3           //菜单
#define     TITLEBUTTON_CONFIG      4           //配置

class CDockTitlebar:public QWidget
{
    Q_OBJECT
public:
    CDockTitlebar(QWidget* parent = nullptr);
    ~CDockTitlebar();

public:
    void        setTitle(const QString strTitle);
    void        addStockID(int id);             //添加内置图标，即上述TITLEBUTTON_
    void        addText(int id, QString strText);   //添加自定义按钮
    void        setReady();

public:
    void        ShowHideButton(int id, bool bShow = true);

Q_SIGNALS:
    void        titleButtonClick(int id);          //自定义按钮的点击消息，会通过这个信号发出。id 代表按钮的ID

private slots:
    void        on_btn_clicked();

protected:
    void        initControls();
    void        setStyle();
    void        reLayout();

private:
    QString         m_strTitle;

private:
    typedef struct
    {
        char            type;       //0表示内置图标 1表示外面添加的
        int             id;         //内置的ID有特殊约定，外添加的无约定
        QString         text;       //文字
        QPushButton*    real;       //实际的控件(指针可以为空)
    }tagTitleButton;

    std::vector<tagTitleButton>     m_arrButtons;

    QLabel*         m_pLabelTitle;

};


/*
// 自定义的Dock TitleBar, 主要完成如下功能:
1, 整体灰色的背景
2, 放置 /向下/配置/固定/关闭 三个按钮
3, 左边显示 标题(可以由外部设置)
4, 靠右的区域，可以添加其他的功能按钮， 按钮的样式为link
*/


#endif
