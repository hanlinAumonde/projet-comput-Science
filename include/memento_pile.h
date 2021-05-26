#ifndef __MEMENTO_PILE_H__
#define __MEMENTO_PILE_H__
#include <map>
#include "pile.h"
#include "litterale_expression.h"

using identifieurMap_t = std::map<QString, LitteraleExpression *>;

class MementoPile
{
private:
    Pile *etatPile_;
    identifieurMap_t etatIdentifieurMap_;

public:
    MementoPile(Pile &pile, identifieurMap_t identifieurMap)
    {
        etatPile_ = pile.cloneOnHeapGeneral();
        etatIdentifieurMap_ = identifieurMap;
    }
    Pile &getEtatPile() const { return *etatPile_; }
    identifieurMap_t getIdentifieurMap() const { return etatIdentifieurMap_; }
    ~MementoPile() { delete etatPile_; }
};

class CareTakerPile
{
    std::vector<MementoPile *> historiquePile_;

public:
    void addMementoPile(MementoPile &etat, unsigned int index)
    {
        if (index < getTailleHistorique())
        {
            historiquePile_[index] = &etat;
        }
        else
        {
            historiquePile_.push_back(&etat);
        }
    }
    MementoPile &getMementoPile(unsigned int index) const { return *historiquePile_[index]; }
    unsigned int getTailleHistorique() const { return historiquePile_.size(); }
    void pop() { historiquePile_.pop_back(); }
};
#endif // __MEMENTO_PILE_H__
