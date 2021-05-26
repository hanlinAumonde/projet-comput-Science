#include <cmath>

#include "qclavier.h"

QClavier::QClavier(QCalculateur &calculateur)
    : calculateur_{calculateur},
      sizeButtonPolicy_{QSizePolicy::Minimum, QSizePolicy::Minimum}

{
    setFixedWidth(700);
    sizeButtonPolicy_.setHorizontalStretch(0);
    sizeButtonPolicy_.setVerticalStretch(0);
    sizeButtonPolicy_.setHeightForWidth((new QPushButton())->sizePolicy().hasHeightForWidth());

    mainLayout_ = new QVBoxLayout{};
    QHBoxLayout *subLayout_1_ = new QHBoxLayout{};
    QHBoxLayout *subLayout_2_ = new QHBoxLayout{};
    QHBoxLayout *subLayout_3_ = new QHBoxLayout{};
    QHBoxLayout *subLayout_4_ = new QHBoxLayout{};

    subWidget_1_ = new QWidget{};
    subWidget_2_ = new QWidget{};
    subWidget_3_ = new QWidget{};
    subWidget_4_ = new QWidget{};

    mainLayout_->setSpacing(0);
    setLayout(mainLayout_);

    subWidget_1_->setLayout(subLayout_1_);
    subWidget_2_->setLayout(subLayout_2_);
    subWidget_3_->setLayout(subLayout_3_);
    subWidget_4_->setLayout(subLayout_4_);

    subLayout_1_->setContentsMargins(5, 5, 5, 5);
    subLayout_2_->setContentsMargins(5, 5, 5, 5);
    subLayout_3_->setContentsMargins(5, 5, 5, 5);
    subLayout_4_->setContentsMargins(5, 5, 5, 5);

    mainLayout_->addWidget(subWidget_4_);
    mainLayout_->addWidget(subWidget_3_);
    mainLayout_->addWidget(subWidget_2_);
    mainLayout_->addWidget(subWidget_1_);

    creerPileClavier();
    creerSimpleClavier();
    creerFonctionClavier();
    creerEvaluationClavier();
    creerCondtionClavier();
    creerLogiqueClavier();
    creerUserClavier();
}

void QClavier::creerSimpleClavier()
{
    QGridLayout *simpleLayout = new QGridLayout{};
    simpleClavier_ = new QFrame{this};
    simpleClavier_->setLayout(simpleLayout);
    simpleClavier_->setFrameStyle(QFrame::Box | QFrame::Sunken);
    simpleClavier_->setMidLineWidth(0);
    // simpleLayout->setSizeConstraint(QLayout::SetFixedSize);
    subWidget_1_->layout()->addWidget(simpleClavier_);

    for (size_t i = 0; i <= 9; i++)
    {
        creerButton(QString::number(i), SLOT(addTextClicked()));
    }
    creerButton(tr("["), SLOT(addTextClicked()));
    creerButton(tr("]"), SLOT(addTextClicked()));
    creerButton(tr("."), SLOT(addTextClicked()));
    creerButton(tr("__"), SLOT(addTextClicked()));

    creerButton(tr("DEL"), SLOT(deleteClicked()));

    creerButton(tr("+"), SLOT(autoOperatorClicked()));
    creerButton(tr("-"), SLOT(autoOperatorClicked()));
    creerButton(tr("*"), SLOT(autoOperatorClicked()));
    creerButton(tr("/"), SLOT(autoOperatorClicked()));

    creerButton(tr("ENTREE"), SLOT(enterClicked()));

    for (size_t i = 1; i <= 9; i++)
    {
        int rang = (i - 1) / 3;
        int colonne = (i - 1) % 3;
        simpleLayout->addWidget(keyboardMap_.at(QString::number(i)), rang, colonne);
    }

    simpleLayout->addWidget(keyboardMap_["0"], 3, 0);
    simpleLayout->addWidget(keyboardMap_["["], 3, 1);
    simpleLayout->addWidget(keyboardMap_["]"], 3, 2);

    simpleLayout->addWidget(keyboardMap_["__"], 0, 3);
    simpleLayout->addWidget(keyboardMap_["DEL"], 0, 4);
    simpleLayout->addWidget(keyboardMap_["+"], 1, 3);
    simpleLayout->addWidget(keyboardMap_["-"], 1, 4);
    simpleLayout->addWidget(keyboardMap_["*"], 2, 3);
    simpleLayout->addWidget(keyboardMap_["/"], 2, 4);
    simpleLayout->addWidget(keyboardMap_["."], 3, 3);
    simpleLayout->addWidget(keyboardMap_["ENTREE"], 3, 4);
}

void QClavier::creerPileClavier()
{
    QGridLayout *pileLayout = new QGridLayout{};
    pileClavier_ = new QFrame{this};
    pileClavier_->setLayout(pileLayout);
    pileClavier_->setFrameStyle(QFrame::Box | QFrame::Sunken);
    pileClavier_->setMidLineWidth(0);
    subWidget_1_->layout()->addWidget(pileClavier_);
    // subLayout_1_->addWidget(pileClavier_);

    creerButton(tr("DUP"), SLOT(addTextClicked()));
    creerButton(tr("DROP"), SLOT(addTextClicked()));
    creerButton(tr("SWAP"), SLOT(addTextClicked()));
    creerButton(tr("CLEAR"), SLOT(addTextClicked()));

    creerButton(tr("UNDO"), SLOT(autoOperatorClicked()));
    creerButton(tr("REDO"), SLOT(autoOperatorClicked()));

    pileLayout->addWidget(keyboardMap_["DUP"], 0, 1);
    pileLayout->addWidget(keyboardMap_["DROP"], 1, 1);
    pileLayout->addWidget(keyboardMap_["SWAP"], 2, 1);
    pileLayout->addWidget(keyboardMap_["CLEAR"], 3, 1);
    keyboardMap_["UNDO"]->setSizePolicy(sizeButtonPolicy_);
    keyboardMap_["REDO"]->setSizePolicy(sizeButtonPolicy_);
    pileLayout->addWidget(keyboardMap_["UNDO"], 0, 0, 2, 1);
    pileLayout->addWidget(keyboardMap_["REDO"], 2, 0, 2, 1);
}

void QClavier::creerFonctionClavier()
{
    QGridLayout *fonctionLayout = new QGridLayout{};
    fonctionClavier_ = new QFrame{this};
    QSizePolicy sp = fonctionClavier_->sizePolicy();
    sp.setRetainSizeWhenHidden(true);
    fonctionClavier_->setSizePolicy(sp);
    fonctionClavier_->setLayout(fonctionLayout);
    fonctionClavier_->setFrameStyle(QFrame::Box | QFrame::Sunken);
    fonctionClavier_->setMidLineWidth(0);
    subWidget_2_->layout()->addWidget(fonctionClavier_);

    QStringList fonctionList = {"DIV", "MOD", "NUM", "DEN", "NEG", "SIN", "COS", "TAN", "IM",
                                "ARCSIN", "ARCCOS", "ARCTAN", "SQRT", "POW", "EXP", "LN"};
    for (auto &fonction : fonctionList)
    {
        creerButton(fonction, SLOT(addTextClicked()));
    }

    for (size_t i = 0; i < fonctionList.size() - 2; i++)
    {
        int range = i / 4;
        int colonne = i % 4;
        fonctionLayout->addWidget(keyboardMap_[fonctionList[i]], range, colonne);
    }

    keyboardMap_["EXP"]->setSizePolicy(sizeButtonPolicy_);
    keyboardMap_["LN"]->setSizePolicy(sizeButtonPolicy_);
    fonctionLayout->addWidget(keyboardMap_["POW"], 0, 4);
    fonctionLayout->addWidget(keyboardMap_["SQRT"], 0, 5);
    fonctionLayout->addWidget(keyboardMap_["EXP"], 1, 4, 2, 1);
    fonctionLayout->addWidget(keyboardMap_["LN"], 1, 5, 2, 1);
}

void QClavier::creerLogiqueClavier()
{
    QGridLayout *logiqueLayout = new QGridLayout{};
    logiqueClavier_ = new QFrame{this};
    logiqueClavier_->setLayout(logiqueLayout);
    logiqueClavier_->setFrameStyle(QFrame::Box | QFrame::Sunken);
    logiqueClavier_->setMidLineWidth(0);
    subWidget_3_->layout()->addWidget(logiqueClavier_);

    QStringList logiqueList = {"==", "<", ">", "!=", "=<", ">=", "AND", "OR", "NOT"};
    for (auto &logique : logiqueList)
    {
        creerButton(logique, SLOT(addTextClicked()));
    }

    for (size_t i = 0; i < logiqueList.size(); i++)
    {
        int range = i / 3;
        int colonne = i % 3;
        logiqueLayout->addWidget(keyboardMap_[logiqueList[i]], range, colonne);
    }
}

void QClavier::creerEvaluationClavier()
{
    QGridLayout *evaluationLayout = new QGridLayout{};
    evaluationClavier_ = new QFrame{this};
    evaluationClavier_->setLayout(evaluationLayout);
    evaluationClavier_->setFrameStyle(QFrame::Box | QFrame::Sunken);
    evaluationClavier_->setMidLineWidth(0);
    subWidget_3_->layout()->addWidget(evaluationClavier_);

    QStringList evaluationList = {"EVAL", "STO", "FORGET"};
    for (auto &evaluation : evaluationList)
    {
        creerButton(evaluation, SLOT(addTextClicked()));
    }

    for (size_t i = 0; i < evaluationList.size(); i++)
    {
        int range = i;
        int colonne = 0;
        evaluationLayout->addWidget(keyboardMap_[evaluationList[i]], range, colonne);
    }
}

void QClavier::creerCondtionClavier()
{
    QGridLayout *conditionLayout = new QGridLayout{};
    conditionClavier_ = new QFrame{this};
    conditionClavier_->setLayout(conditionLayout);
    conditionClavier_->setFrameStyle(QFrame::Box | QFrame::Sunken);
    conditionClavier_->setMidLineWidth(0);
    subWidget_3_->layout()->addWidget(conditionClavier_);

    QStringList conditionList = {"IFT", "IFTE", "WHILE"};
    for (auto &condtion : conditionList)
    {
        creerButton(condtion, SLOT(addTextClicked()));
    }

    for (size_t i = 0; i < conditionList.size(); i++)
    {
        int range = i;
        int colonne = 0;
        conditionLayout->addWidget(keyboardMap_[conditionList[i]], range, colonne);
    }
}

void QClavier::creerUserClavier()
{
    variableLayout_ = new QGridLayout{};
    userVariableClavier_ = new QFrame{this};
    userVariableClavier_->setLayout(variableLayout_);
    userVariableClavier_->setFrameStyle(QFrame::Box | QFrame::Sunken);
    userVariableClavier_->setMidLineWidth(0);
    subWidget_4_->layout()->addWidget(userVariableClavier_);

    fonctionLayout_ = new QGridLayout{};
    userFonctionClavier_ = new QFrame{this};
    userFonctionClavier_->setLayout(fonctionLayout_);
    userFonctionClavier_->setFrameStyle(QFrame::Box | QFrame::Sunken);
    userFonctionClavier_->setMidLineWidth(0);
    subWidget_4_->layout()->addWidget(userFonctionClavier_);
}

void QClavier::updateUserClavier()
{
    unsigned int maxColonne = 4;
    QLayoutItem *item;
    while ((item = variableLayout_->takeAt(0)) != nullptr)
    {
        delete item->widget();
        delete item;
    }
    while ((item = fonctionLayout_->takeAt(0)) != nullptr)
    {
        delete item->widget();
        delete item;
    }
    int currentVarible = 0, currentFonction = 0;
    for (auto itr = identifieurMap_->begin(); itr != identifieurMap_->end(); itr++)
    {
        QString buttonText = itr->second->affichage().mid(1, itr->second->affichage().length() - 2);
        QPushButton *button = new QPushButton(buttonText);
        connect(button, SIGNAL(clicked()), this, SLOT(evaluerVarEtProgramme()));
        if (itr->second->getLitterale()->getType() == TypeLitterale::PROGRAMME)
        {
            int range = currentFonction / maxColonne;
            int colonne = currentFonction % maxColonne;
            fonctionLayout_->addWidget(button, range, colonne);
            currentFonction++;
        }
        else
        {
            int range = currentVarible / maxColonne;
            int colonne = currentVarible % maxColonne;
            variableLayout_->addWidget(button, range, colonne);
            currentVarible++;
        }
    }
}

void QClavier::creerButton(const QString &text, const char *member)
{
    keyboardMap_[text] = new QPushButton{text};
    connect(keyboardMap_[text], SIGNAL(clicked()), this, member);
}

void QClavier::deleteClicked()
{
    QLineEdit &commandeBar = calculateur_.getCommandBar();
    QString text = commandeBar.text();
    if (text != "")
    {
        commandeBar.setText(text.left(text.length() - 1));
    }
}

void QClavier::autoOperatorClicked()
{
    try
    {
        auto clickedButton = qobject_cast<QPushButton *>(sender());
        calculerAutoOperateur(clickedButton->text());
    }
    catch (const CalculateurException &e)
    {
        calculateur_.setMessage(e.what());
    }
}

bool QClavier::calculerAutoOperateur(QString op)
{
    QLineEdit &commandeBar = calculateur_.getCommandBar();
    calculateur_.getControleur().traiterCommandBar(commandeBar.text() + " " + op);
    calculateur_.renderVuePile();
    calculateur_.setMessage("");
    commandeBar.clear();
    return true;
}

void QClavier::evaluerVarEtProgramme()
{
    try
    {
        auto clickedButton = qobject_cast<QPushButton *>(sender());
        calculateur_.getControleur().traiterCommandBar("'" + clickedButton->text() + "'" + " EVAL");
        calculateur_.renderVuePile();
        calculateur_.setMessage("");
    }
    catch (const CalculateurException &e)
    {
        calculateur_.setMessage(e.what());
    }
}

void QClavier::addTextClicked()
{
    auto clickedButton = qobject_cast<QPushButton *>(sender());
    QLineEdit &commandBar = calculateur_.getCommandBar();
    if (clickedButton->text() == "__")
    {
        commandBar.setText(commandBar.text() + " ");
        return;
    }
    commandBar.setText(commandBar.text() + clickedButton->text());
}

void QClavier::enterClicked()
{
    try
    {
        calculateur_.calculerAutoOperateur();
    }
    catch (const CalculateurException &e)
    {
        calculateur_.setMessage(e.what());
    }
}
