#ifndef ICONHELPER_DEFINE_H
#define ICONHELPER_DEFINE_H

#include <QtCore>
#include <QtGui>
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
#include <QtWidgets>
#endif

class IconHelper : public QObject
{
    Q_OBJECT
public:
    static IconHelper*  instance();
    explicit IconHelper(QObject* parent = nullptr);

public:
    void        setIcon(QLabel *lab, const QChar &str, quint32 size = 12);
    void        setIcon(QAbstractButton *btn, const QChar &str, quint32 size = 12);
    QPixmap     getPixmap(const QColor &color, const QChar &str, quint32 size = 12,
                          quint32 pixWidth = 15, quint32 pixHeight = 15, int flags = Qt::AlignCenter);

private:
    static QScopedPointer<IconHelper> self;
    QFont       iconFont;             //图形字体

};









#endif
