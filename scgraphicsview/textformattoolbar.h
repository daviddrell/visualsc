#ifndef TEXTFORMATTOOLBAR_H
#define TEXTFORMATTOOLBAR_H

#include "scgraphicsview_global.h"
#include <QWidget>
#include <QToolBar>
#include <QAction>
#include <QFontComboBox>
#include <QComboBox>

class SCGRAPHICSVIEWSHARED_EXPORT TextFormatToolBar  : public QToolBar
{
    Q_OBJECT

public:
    TextFormatToolBar(QWidget * parent=NULL);

private slots:

    void handleFontChange();
    void handleBoldChanged();
    void handleCurrentFontChanged();


private:


    QAction *boldAction;
    QAction *italicAction;
    QFontComboBox *fontCombo;
    QComboBox *fontSizeCombo;
};

#endif // TEXTFORMATTOOLBAR_H
