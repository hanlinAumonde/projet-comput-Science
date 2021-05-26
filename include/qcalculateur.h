#ifndef __QCALCULATEUR_H__
#define __QCALCULATEUR_H__

#include <QMainWindow>
#include <QWidget>
#include <QTableWidget>
#include <QTableView>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <memory>
#include <vector>
#include <queue>
#include <map>

#include "controleur.h"
#include "qclavier.h"
#include "qvue_secondaire.h"

class QVueSecondaire;
class QClavier;

class QCalculateur : public QMainWindow
{
private:
    Q_OBJECT

    friend class QVueSecondaire;
    friend class QClavier;

    unsigned int rowCount_;
    QString currentText_;

    QWidget *mainWidget_;
    QVBoxLayout *mainLayout_;

    std::unique_ptr<QVueSecondaire> menubar_;
    QLineEdit *message_;
    QTableWidget *vuePile_;
    QLineEdit *commandeBar_;
    std::unique_ptr<QClavier> clavier_;

    Controleur &controleur_;

public:
    QCalculateur();
    QLineEdit &getCommandBar() { return *commandeBar_; }
    Controleur &getControleur() { return controleur_; }
    void setMessage(const QString &s) { message_->setText(s); }
    bool calculerAutoOperateur(QString op = "");
    void renderVuePile();
    void creerVuePile();

signals:
    void returnPressed() const;

private slots:
    void enterClicked();

protected:
    void keyPressEvent(QKeyEvent *event);
    bool event(QEvent* event) override;
};

#endif // __QCALCULATEUR_H__
