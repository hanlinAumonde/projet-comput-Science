#ifndef __QVUE_SECONDAIRE_H__
#define __QVUE_SECONDAIRE_H__

#include <QMenuBar>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>

#include "qcalculateur.h"

/*
** LECTURE OPTIONNELLE! Ce class ne se sert qu'aux 3 vues secondaires, pas le désign du programme
*/

class QCalculateur;

class QVueSecondaire : public QMenuBar
{
private:
    Q_OBJECT

    QCalculateur &calculateur_;
    QSpinBox *parametreRange_;

    QMenu *fileMenu_;
    QMenu *editMenu_;

    QWidget *parametreWidget_;
    QWidget *variableWidget_;
    QWidget *programmeWidget_;

    QCheckBox *userClavier_;
    QCheckBox *avanceClavier_;
    QCheckBox *simpleClavier_;

    QAction *variableAction_;
    QAction *programmeAction_;
    QComboBox *variableNom_;
    QComboBox *programmeNom_;
    QLineEdit *varNom_;
    QLineEdit *varValeur_;
    QLineEdit *proNom_;
    QLineEdit *proValeur_;

public:
    QVueSecondaire(QCalculateur &calculateur);

private:
    void creerFileMenu();
    void creerEditMenu();

    void setUpParametreAction();

private slots:
    void setUpVariableAction();
    void setUpProgrammeAction();

    void forgetVariable();
    void forgetProgramme();
    void stockerVariable();
    void stockerProgramme();

    void modifierPileEtClavier();
};

#endif // __QVUE_SECONDAIRE_H__
