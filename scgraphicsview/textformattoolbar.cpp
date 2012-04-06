#include "textformattoolbar.h"

#include <QVBoxLayout>
#include <QLabel>


TextFormatToolBar::TextFormatToolBar(QWidget * parent) : QToolBar(parent)
{

    boldAction = new QAction(tr("Bold"), this);
    boldAction->setCheckable(true);
    QPixmap pixmap(":/SCGraphicsView/bold.png");
    boldAction->setIcon(QIcon(pixmap));
    boldAction->setShortcut(tr("Ctrl+B"));
    connect(boldAction, SIGNAL(triggered()), this, SLOT(handleBoldChanged()));

    italicAction = new QAction(QIcon(":/SCGraphicsView/italic.png"),   tr("Italic"), this);
    italicAction->setCheckable(true);
    italicAction->setShortcut(tr("Ctrl+I"));
    connect(italicAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    fontCombo = new QFontComboBox();
    connect(fontCombo, SIGNAL(currentFontChanged(QFont)),
            this, SLOT(handleCurrentFontChanged(QFont)));

    fontSizeCombo = new QComboBox;
    fontSizeCombo->setEditable(true);
    for (int i = 8; i < 30; i = i + 2)
        fontSizeCombo->addItem(QString().setNum(i));
    QIntValidator *validator = new QIntValidator(2, 64, this);
    fontSizeCombo->setValidator(validator);
    connect(fontSizeCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(fontSizeChanged(QString)));


    this->addWidget(fontCombo);
    this->addWidget(fontSizeCombo);
    this->addAction(boldAction);
    this->addAction(italicAction);
    this->setEnabled(false);

}

void TextFormatToolBar::handleBoldChanged()
{
}

void TextFormatToolBar::handleFontChange()
{
}

void TextFormatToolBar::handleCurrentFontChanged()
{
}
