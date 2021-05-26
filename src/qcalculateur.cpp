#include <iostream>
#include <QHeaderView>
#include <QDialog>
#include <QTableWidgetItem>
#include <QScrollBar>
#include <QKeyEvent>
#include <QMenuBar>
#include <QMenu>
#include <cmath>

#include "qcalculateur.h"
#include "exceptions.h"

// Cette fonction ne se sert qu'à configurer vuePile_, lecture optionel
void verticalResizeTableViewToContents(QTableWidget *tableView)
{
    int rowTotalHeight = 0;

    int count = tableView->verticalHeader()->count();
    tableView->verticalHeader()->setDefaultSectionSize(1);
    for (int i = 0; i < count; ++i)
    {
        if (!tableView->verticalHeader()->isSectionHidden(i))
        {
            rowTotalHeight += 25;
            // rowTotalHeight += tableView->verticalHeader()->sectionSize(i);
        }
    }

    if (!tableView->horizontalScrollBar()->isHidden())
    {
        rowTotalHeight += tableView->horizontalScrollBar()->height();
    }

    if (!tableView->horizontalHeader()->isHidden())
    {
        rowTotalHeight += tableView->horizontalHeader()->height();
    }
    tableView->setFixedHeight(rowTotalHeight);
}

void QCalculateur::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return)
        emit returnPressed();
    QMainWindow::keyReleaseEvent(event);
}

QCalculateur::QCalculateur()
    : rowCount_{10}, controleur_{Controleur::getInstance()}, menubar_{std::make_unique<QVueSecondaire>(*this)},
      message_{new QLineEdit{}}, vuePile_{new QTableWidget{this}}, commandeBar_{new QLineEdit{}},
      clavier_{std::make_unique<QClavier>(*this)}
{
    clavier_->setIdentifieurMap(controleur_.getIdentifieurMap());

    //setWindowIcon(QIcon("../projet-comput-master/res/icon/calculateur_icon.png"));

    connect(this, SIGNAL(returnPressed()), this, SLOT(enterClicked()));
    setWindowTitle("Comp'UT");

    mainWidget_ = new QWidget{};
    mainLayout_ = new QVBoxLayout{};
    mainWidget_->setAutoFillBackground(true);
    mainWidget_->setLayout(mainLayout_);
    setCentralWidget(mainWidget_);

    setMenuBar(menubar_.get());

    message_->setReadOnly(true);
    message_->setFocusPolicy(Qt::NoFocus);
    message_->setAlignment(Qt::AlignLeft);

    creerVuePile();

    connect(commandeBar_, SIGNAL(returnPressed()), this, SLOT(enterClicked()));
    mainLayout_->addWidget(message_);
    mainLayout_->addWidget(vuePile_);
    mainLayout_->addWidget(commandeBar_);
    mainLayout_->addWidget(clavier_.get());
}

bool QCalculateur::calculerAutoOperateur(QString op)
{
    controleur_.traiterCommandBar(commandeBar_->text() + " " + op);
    renderVuePile();
    message_->clear();
    commandeBar_->clear();
    return true;
}

void QCalculateur::renderVuePile()
{

    Pile &pile = controleur_.getPile();
    PileIterator pileItr{&pile};
    int count = 1;
    for (auto itr = pileItr.begin(); (itr != pileItr.end()) && (count <= rowCount_); itr++)
    {
        QString temp = (*itr)->affichage();
        if (temp.length() > 50)
        {
            if (temp[temp.length() - 1] == '\'')
            {
                temp = temp.mid(0, 50) + "...'";
            }
            else if (temp[temp.length() - 1] == ']')
            {
                temp = temp.mid(0, 50) + "...]";
            }
        }
        vuePile_->item(rowCount_ - count, 0)->setText(temp);
        count++;
    }

    for (size_t i = count; i <= rowCount_; i++)
    {
        vuePile_->item(rowCount_ - i, 0)->setText("");
    }
    clavier_->updateUserClavier();
}

void QCalculateur::creerVuePile()
{
    vuePile_->setRowCount(rowCount_);
    vuePile_->setColumnCount(1);
    vuePile_->horizontalHeader()->setStretchLastSection(true);
    vuePile_->horizontalHeader()->setVisible(false);
    vuePile_->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    vuePile_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    vuePile_->setFocusPolicy(Qt::NoFocus);
    vuePile_->setSelectionMode(QAbstractItemView::NoSelection);
    verticalResizeTableViewToContents(vuePile_);
    QStringList labelList;
    for (size_t i = rowCount_; i >= 1; i--)
    {
        labelList.append(QString::number(i));
    }
    vuePile_->setVerticalHeaderLabels(labelList);
    for (size_t i = 0; i < rowCount_; i++)
    {
        vuePile_->setItem(i, 0, new QTableWidgetItem{});
    }
}

void QCalculateur::enterClicked()
{
    try
    {
        if (isWindowModified())
        {
            setWindowTitle("Comp'UT[*]");
            setWindowModified(false);
            return;
        }
        setWindowTitle("Comp'UT[*]");
        setWindowModified(true);
        calculerAutoOperateur();
    }
    catch (const CalculateurException &e)
    {
        message_->setText(e.what());
    }
}

bool QCalculateur::event(QEvent *ev)
{
    if (ev->type() == QEvent::LayoutRequest)
    {
        setFixedSize(sizeHint());
    }
    return QMainWindow::event(ev);
}
