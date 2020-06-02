/********************************************************************************
** Form generated from reading UI file 'debugquote.ui'
**
** Created by: Qt User Interface Compiler version 5.9.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEBUGQUOTE_H
#define UI_DEBUGQUOTE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_debugquote
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QComboBox *comboBox;
    QListWidget *listWidget;
    QTableView *tableView;

    void setupUi(QWidget *debugquote)
    {
        if (debugquote->objectName().isEmpty())
            debugquote->setObjectName(QStringLiteral("debugquote"));
        debugquote->resize(630, 337);
        horizontalLayout = new QHBoxLayout(debugquote);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        comboBox = new QComboBox(debugquote);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        verticalLayout->addWidget(comboBox);

        listWidget = new QListWidget(debugquote);
        listWidget->setObjectName(QStringLiteral("listWidget"));

        verticalLayout->addWidget(listWidget);


        horizontalLayout->addLayout(verticalLayout);

        tableView = new QTableView(debugquote);
        tableView->setObjectName(QStringLiteral("tableView"));

        horizontalLayout->addWidget(tableView);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 3);

        retranslateUi(debugquote);

        QMetaObject::connectSlotsByName(debugquote);
    } // setupUi

    void retranslateUi(QWidget *debugquote)
    {
        debugquote->setWindowTitle(QApplication::translate("debugquote", "Quote", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class debugquote: public Ui_debugquote {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEBUGQUOTE_H
