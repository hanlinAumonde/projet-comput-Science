#include <QString>

#include "operateur_evaluatif.h"
bool estOperateurPredefini(const QString &s)
{
    if (s == "IFT" || s == "IFTE" || s == "WHILE" || s == "EVAL" || s == "STO" || s == "DIV" ||
        s == "MOD" || s == "NEG" || s == "NUM" || s == "DEN" || s == "POW" || s == "SIN" || s == "COS" ||
        s == "TAN" || s == "ARCSIN" || s == "ARCCOS" || s == "ARCTAN" || s == "SQRT" || s == "EXP" ||
        s == "LN" || s == "AND" || s == "OR" || s == "NOT" || s == "DUP" || s == "DROP" ||
        s == "CLEAR" || s == "UNDO" || s == "REDO" || s == "FORGET" || s == "IM")
    {
        return true;
    }
    return false;
}

// IFT opérateur
IfThen::IfThen()
    : OperateurEvaluatif{2, "IFT"} {}

void IfThen::traitement()
{
    Litterale *argument_2 = pile_->top().cloneOnHeapGeneral();
    pile_->pop();
    Litterale *argument_1 = pile_->top().cloneOnHeapGeneral();
    pile_->pop();
    if (argument_1->isNull())
    {
        return;
    }
    argument_2->evaluer();
}

// IFTE opérateur
IfThenElse::IfThenElse()
    : OperateurEvaluatif{3, "IFTE"} {}

void IfThenElse::traitement()
{
    Litterale *argument_3 = pile_->top().cloneOnHeapGeneral();
    pile_->pop();
    Litterale *argument_2 = pile_->top().cloneOnHeapGeneral();
    pile_->pop();
    Litterale *argument_1 = pile_->top().cloneOnHeapGeneral();
    pile_->pop();
    if (!argument_1->isNull())
    {
        argument_2->evaluer();
    }
    else
    {
        argument_3->evaluer();
    }
}

// WHILE opérateur
While::While()
    : OperateurEvaluatif{std::numeric_limits<unsigned int>::max(), "WHILE"} {}

void While::traitement()
{
}

// EVAL opérateur
Evaluer::Evaluer()
    : OperateurEvaluatif{1, "EVAL"} {}

void Evaluer::traitement()
{
    Litterale &top = pile_->top();
    if (top.getType() != TypeLitterale::PROGRAMME && top.getType() != TypeLitterale::EXPRESSION)
    {
        throw CalculateurException(("Opérateur EVAL ne traite qu'une littérale expression ou une littérale programme, " +
                                    top.affichage().toStdString() + " trouvé!")
                                       .c_str());
    }
    if (top.getType() == TypeLitterale::PROGRAMME)
    {
        auto programme = top.cloneOnHeapGeneral();
        pile_->pop();
        programme->evaluer();
        return;
    }
    QString atome = top.affichage().mid(1, top.affichage().length() - 2);
    auto pos = identifieurMap_->find(atome);
    if (pos != identifieurMap_->end())
    {
        pile_->pop();
        if (pos->second->getLitterale()->getType() == TypeLitterale::PROGRAMME)
        {
            pos->second->getLitterale()->evaluer();
            return;
        }
        pile_->push(*(pos->second->evaluer()->cloneOnHeapGeneral()));
    }
    else
    {
        throw CalculateurException((top.affichage().toStdString() + " n'est pas une variable!").c_str());
    }
}

// STO opérateur
Stocker::Stocker()
    : OperateurEvaluatif{2, "STO"} {}

void Stocker::traitement()
{
    Litterale *argument_2 = pile_->top().cloneOnHeapGeneral();
    pile_->pop();
    Litterale *argument_1 = pile_->top().cloneOnHeapGeneral();
    pile_->pop();
    if (argument_1->getType() == TypeLitterale::EXPRESSION)
    {
        throw CalculateurException("La première littérale ne doit pas être une littérale expression !");
    }
    if (argument_2->getType() != TypeLitterale::EXPRESSION)
    {
        throw CalculateurException("La seconde littérale doit être une littérale expression !");
    }
    QString atome = argument_2->affichage().mid(1, argument_2->affichage().length() - 2);
    if (estOperateurPredefini(atome))
    {
        throw CalculateurException(("L'opérateur " + atome.toStdString() + " est prédifini !").c_str());
    }

    auto pos = identifieurMap_->find(atome);
    if (pos != identifieurMap_->end())
    {
        (*identifieurMap_)[atome]->setLitterale(argument_1);
    }
    else
    {
        LitteraleExpression *variable = new LitteraleExpression{"'" + atome + "'"};
        variable->setLitterale(argument_1);
        (*identifieurMap_)[atome] = variable;
    }
}

// FORGET opérateur
Forget::Forget(const QString &atome)
    : OperateurEvaluatif{1, "FORGET"}, atome_{atome} {}

void Forget::traitement()
{
    auto pos = identifieurMap_->find(atome_);
    if (pos != identifieurMap_->end())
    {
        identifieurMap_->erase(atome_);
    }
    else
    {
        throw CalculateurException(("Atome " + atome_.toStdString() + " n'est pas stocké!").c_str());
    }
}
