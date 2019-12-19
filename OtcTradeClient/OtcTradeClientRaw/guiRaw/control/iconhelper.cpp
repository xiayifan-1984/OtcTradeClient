#include "iconhelper.h"

QScopedPointer<IconHelper> IconHelper::self;

IconHelper *IconHelper::instance()
{
    if (self.isNull())
    {
        QMutex      mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull())
        {
            self.reset(new IconHelper);
        }
    }

    return self.data();
}

IconHelper::IconHelper(QObject *parent) : QObject(parent)
{
    //判断图形字体是否存在,不存在则加入
    QFontDatabase fontDb;
    if (!fontDb.families().contains("FontAwesome"))
    {
        int fontId = fontDb.addApplicationFont(":/image/fontawesome-webfont.ttf");
        QStringList fontName = fontDb.applicationFontFamilies(fontId);
        if (fontName.count() == 0)
        {
            qDebug() << "load fontawesome-webfont.ttf error";
        }
    }

    if (fontDb.families().contains("FontAwesome"))
    {
        iconFont = QFont("FontAwesome");

#if (QT_VERSION >= QT_VERSION_CHECK(4,8,0))
        iconFont.setHintingPreference(QFont::PreferNoHinting);
#endif
    }
}



void IconHelper::setIcon(QLabel *lab, const QChar &str, quint32 size)
{
    iconFont.setPixelSize(size);
    lab->setFont(iconFont);
    lab->setText(str);
}

void IconHelper::setIcon(QAbstractButton *btn, const QChar &str, quint32 size)
{
    iconFont.setPixelSize(size);
    btn->setFont(iconFont);
    btn->setText(str);
}

QPixmap IconHelper::getPixmap(const QColor &color, const QChar &str, quint32 size,
                              quint32 pixWidth, quint32 pixHeight, int flags)
{
    QPixmap pix(pixWidth, pixHeight);
    pix.fill(Qt::transparent);

    QPainter painter;
    painter.begin(&pix);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(color);

    iconFont.setPixelSize(size);
    painter.setFont(iconFont);
    painter.drawText(pix.rect(), flags, str);
    painter.end();

    return pix;
}

