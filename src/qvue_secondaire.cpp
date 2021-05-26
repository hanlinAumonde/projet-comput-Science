#include <QMenu>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>

#include "qvue_secondaire.h"
#include "controleur.h"
#include "operateur_evaluatif.h"

/*
** LECTURE OPTIONNELLE! Cette classe ne sert qu'à gérer les 3 vues secondaires, pas le design du programme
*/

QVueSecondaire::QVueSecondaire(QCalculateur &calculateur) : calculateur_{calculateur}
{
    creerFileMenu();
    creerEditMenu();
}

void QVueSecondaire::creerFileMenu()
{
    fileMenu_ = addMenu("&Fichier");

    QAction *saveAction = new QAction("&Sauvegarder", &calculateur_);
    QAction *openAction = new QAction("&Ouvrir", &calculateur_);
    QAction *quitAction = new QAction("&Quitter", &calculateur_);

    fileMenu_->addAction(saveAction);
    fileMenu_->addAction(openAction);
    fileMenu_->addAction(quitAction);

    connect(quitAction, &QAction::triggered, &calculateur_, &QMainWindow::close);
}

void QVueSecondaire::creerEditMenu()
{
    editMenu_ = addMenu("&Édition");

    setUpParametreAction();

    variableAction_ = new QAction("&Variables");
    editMenu_->addAction(variableAction_);
    connect(variableAction_, SIGNAL(triggered()), this, SLOT(setUpVariableAction()));

    programmeAction_ = new QAction("&Programmes", &calculateur_);
    editMenu_->addAction(programmeAction_);
    connect(programmeAction_, SIGNAL(triggered()), this, SLOT(setUpProgrammeAction()));
}

void QVueSecondaire::setUpParametreAction()
{
    QAction *parametreAction = new QAction("&Paramètres", &calculateur_);
    QVBoxLayout *parametreLayout = new QVBoxLayout{};
    parametreWidget_ = new QWidget{};
    parametreWidget_->setWindowTitle("Comp'UT - Paramètres");

    editMenu_->addAction(parametreAction);
    parametreWidget_->setLayout(parametreLayout);

    QFrame *parametreFrame = new QFrame{};
    QVBoxLayout *layout1 = new QVBoxLayout{};
    QLabel *label1 = new QLabel("Nombre d'éléments de la pile affichés :");
    parametreRange_ = new QSpinBox{};

    parametreRange_->setRange(1, 10);
    parametreRange_->setValue(calculateur_.rowCount_);
    layout1->addWidget(label1);
    layout1->addWidget(parametreRange_);

    QVBoxLayout *layout2 = new QVBoxLayout{};
    QLabel *label2 = new QLabel{"Cochez les claviers que vous voulez afficher :"};
    userClavier_ = new QCheckBox{"Clavier de l'utilisateur"};
    avanceClavier_ = new QCheckBox{"Clavier avancé"};
    simpleClavier_ = new QCheckBox{"Clavier simple"};
    userClavier_->setChecked(true);
    avanceClavier_->setChecked(true);
    simpleClavier_->setChecked(true);

    layout2->addWidget(label2);
    layout2->addWidget(userClavier_);
    layout2->addWidget(avanceClavier_);
    layout2->addWidget(simpleClavier_);

    QPushButton *appliquerButton = new QPushButton{"Appliquer"};

    parametreLayout->addLayout(layout1);
    parametreLayout->addSpacing(15);
    parametreLayout->addLayout(layout2);
    parametreLayout->addSpacing(10);
    parametreLayout->addWidget(appliquerButton);

    connect(appliquerButton, SIGNAL(clicked()), this, SLOT(modifierPileEtClavier()));
    connect(parametreAction, &QAction::triggered, parametreWidget_, &QMainWindow::show);
}

void QVueSecondaire::setUpVariableAction()
{
    variableWidget_ = new QWidget{};
    variableWidget_->setFixedSize(500, 250);
    variableWidget_->show();
    variableWidget_->setWindowTitle("Comp'UT - Variables");

    QGridLayout *variableLayout = new QGridLayout{};

    // Affichage de la valeur de la variable
    QLabel *affichageLabel = new QLabel("Valeur de la variable :");

    variableWidget_->setLayout(variableLayout);
    variableLayout->addWidget(affichageLabel, 0, 0, 1, 2);

    QComboBox *variableNom = new QComboBox();
    QLabel *valeurAffichage = new QLabel("Rien");
    identifieurMap_t *mp = calculateur_.controleur_.getIdentifieurMap();

    variableNom->addItem("");
    for (auto itr = mp->begin(); itr != mp->end(); itr++)
    {
        if (itr->second->getLitterale()->getType() != TypeLitterale::PROGRAMME)
        {
            variableNom->addItem(itr->first);
        }
    }
    variableLayout->addWidget(variableNom, 1, 0);
    variableLayout->addWidget(valeurAffichage, 1, 1);
    valeurAffichage->setContentsMargins(15, 2, 2, 2);

    connect(variableNom, &QComboBox::currentTextChanged, [=](const QString &newValue) {
        newValue != "" ? valeurAffichage->setText(mp->at(newValue)->getLitterale()->affichage())
                       : valeurAffichage->setText("Rien");
    });

    // Supprimer une variable
    QLabel *forgetVariableLabel = new QLabel("Supprimer une variable :");
    variableNom_ = new QComboBox(variableNom);
    QPushButton *forgetButton = new QPushButton("Supprimer");

    variableNom_->addItem("");
    for (auto itr = mp->begin(); itr != mp->end(); itr++)
    {
        if (itr->second->getLitterale()->getType() != TypeLitterale::PROGRAMME)
        {
            variableNom_->addItem(itr->first);
        }
    }

    variableLayout->addWidget(forgetVariableLabel, 2, 0, 1, 2);
    variableLayout->addWidget(variableNom_, 3, 0);
    variableLayout->addWidget(forgetButton, 3, 1);

    connect(forgetButton, SIGNAL(clicked()), this, SLOT(forgetVariable()));

    // Stocker une variable
    QLabel *stockerVariableLabel = new QLabel("Stocker une variable :");
    varNom_ = new QLineEdit();
    varValeur_ = new QLineEdit();
    QPushButton *stockerButton = new QPushButton("Stocker");

    varNom_->setPlaceholderText("Nom");
    varValeur_->setPlaceholderText("Valeur");

    variableLayout->addWidget(stockerVariableLabel, 4, 0, 1, 2);
    variableLayout->addWidget(varNom_, 5, 0);
    variableLayout->addWidget(varValeur_, 5, 1);
    variableLayout->addWidget(stockerButton, 6, 0, 1, 2, Qt::AlignCenter);

    connect(stockerButton, SIGNAL(clicked()), this, SLOT(stockerVariable()));
}

void QVueSecondaire::setUpProgrammeAction()
{
    programmeWidget_ = new QWidget{};
    programmeWidget_->setFixedSize(500, 250);
    programmeWidget_->show();
    programmeWidget_->setWindowTitle("Comp'UT - Programmes");

    QGridLayout *programmeLayout = new QGridLayout{};

    // Affichage de la valeur d'un programme
    QLabel *affichageLabel = new QLabel("Valeur du programme :");

    programmeWidget_->setLayout(programmeLayout);
    programmeLayout->addWidget(affichageLabel, 0, 0, 1, 2);

    QComboBox *programmeNom = new QComboBox();
    QLabel *valeurAffichage = new QLabel("Rien");
    identifieurMap_t *mp = calculateur_.controleur_.getIdentifieurMap();

    programmeNom->addItem("");
    for (auto itr = mp->begin(); itr != mp->end(); itr++)
    {
        if (itr->second->getLitterale()->getType() == TypeLitterale::PROGRAMME)
        {
            programmeNom->addItem(itr->first);
        }
    }
    programmeLayout->addWidget(programmeNom, 1, 0);
    programmeLayout->addWidget(valeurAffichage, 1, 1);
    valeurAffichage->setContentsMargins(15, 2, 2, 2);

    connect(programmeNom, &QComboBox::currentTextChanged, [=](const QString &newValue) {
        newValue != "" ? valeurAffichage->setText(mp->at(newValue)->getLitterale()->affichage())
                       : valeurAffichage->setText("Rien");
    });

    // Supprimer un programme
    QLabel *forgetProgrammeLabel = new QLabel("Supprimer un programme :");
    programmeNom_ = new QComboBox(programmeNom);
    QPushButton *forgetButton = new QPushButton("Supprimer");

    programmeNom_->addItem("");
    for (auto itr = mp->begin(); itr != mp->end(); itr++)
    {
        if (itr->second->getLitterale()->getType() == TypeLitterale::PROGRAMME)
        {
            programmeNom_->addItem(itr->first);
        }
    }

    programmeLayout->addWidget(forgetProgrammeLabel, 2, 0, 1, 2);
    programmeLayout->addWidget(programmeNom_, 3, 0);
    programmeLayout->addWidget(forgetButton, 3, 1);

    connect(forgetButton, SIGNAL(clicked()), this, SLOT(forgetProgramme()));

    // Stocker un programme
    QLabel *stockerProgrammeLabel = new QLabel("Stocker un programme :");
    proNom_ = new QLineEdit();
    proValeur_ = new QLineEdit();
    QPushButton *stockerButton = new QPushButton("Stocker");

    proNom_->setPlaceholderText("Nom");
    proValeur_->setPlaceholderText("Valeur");

    programmeLayout->addWidget(stockerProgrammeLabel, 4, 0, 1, 2);
    programmeLayout->addWidget(proNom_, 5, 0);
    programmeLayout->addWidget(proValeur_, 5, 1);
    programmeLayout->addWidget(stockerButton, 6, 0, 1, 2, Qt::AlignCenter);

    connect(stockerButton, SIGNAL(clicked()), this, SLOT(stockerProgramme()));
}

void QVueSecondaire::modifierPileEtClavier()
{
    calculateur_.rowCount_ = parametreRange_->value();
    calculateur_.creerVuePile();
    calculateur_.renderVuePile();
    if (simpleClavier_->isChecked())
    {
        calculateur_.clavier_->subWidget_1_->show();
    }
    else
    {
        calculateur_.clavier_->subWidget_1_->hide();
    }
    if (avanceClavier_->isChecked())
    {
        calculateur_.clavier_->subWidget_2_->show();
        calculateur_.clavier_->subWidget_3_->show();
    }
    else
    {
        calculateur_.clavier_->subWidget_2_->hide();
        calculateur_.clavier_->subWidget_3_->hide();
    }
    if (userClavier_->isChecked())
    {
        calculateur_.clavier_->subWidget_4_->show();
    }
    else
    {
        calculateur_.clavier_->subWidget_4_->hide();
    }
    parametreWidget_->close();
}

void QVueSecondaire::forgetVariable()
{
    if (variableNom_->currentText() != "")
    {
        calculateur_.controleur_.traiterCommandBar(variableNom_->currentText() + " FORGET");
        calculateur_.clavier_->updateUserClavier();
        variableWidget_->close();
    }
}

void QVueSecondaire::stockerVariable()
{
    if (varNom_->text().trimmed() != "" && varValeur_->text().trimmed() != "")
    {
        try
        {
            calculateur_.controleur_.traiterCommandBar(varValeur_->text() + " '" + varNom_->text() + "' " + "STO");
            calculateur_.clavier_->updateUserClavier();
            variableWidget_->close();
        }
        catch (const CalculateurException &e)
        {
            calculateur_.setMessage(e.what());
        }
    }
}

void QVueSecondaire::forgetProgramme()
{
    if (programmeNom_->currentText() != "")
    {
        calculateur_.controleur_.traiterCommandBar(programmeNom_->currentText() + " FORGET");
        calculateur_.clavier_->updateUserClavier();
        programmeWidget_->close();
    }
}

void QVueSecondaire::stockerProgramme()
{
    if (proNom_->text().trimmed() != "" && proValeur_->text().trimmed() != "")
    {
        try
        {
            calculateur_.controleur_.traiterCommandBar(proValeur_->text() + " '" + proNom_->text() + "' " + "STO");
            calculateur_.clavier_->updateUserClavier();
            programmeWidget_->close();
        }
        catch (const CalculateurException &e)
        {
            calculateur_.setMessage(e.what());
        }
    }
}
