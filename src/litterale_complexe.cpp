#include <iostream>
#include <complex>

#include "litterale_entiere.h"
#include "litterale_reelle.h"
#include "litterale_complexe.h"

LitteraleComplexe::LitteraleComplexe(LitteraleNombre *re, LitteraleNombre *im)
    : partRe_{re}, partIm_{im}
{
    typeLitterale_ = TypeLitterale::COMPLEXE;
}

LitteraleComplexe *LitteraleComplexe::simplifier()
{
    partRe_ = partRe_->simplifier();
    partIm_ = partIm_->simplifier();
    return this;
}

LitteraleNombre *LitteraleComplexe::cloneOnHeapNombre() const
{
    LitteraleNombre *re = partRe_->cloneOnHeapNombre();
    LitteraleNombre *im = partIm_->cloneOnHeapNombre();
    return new LitteraleComplexe{re, im};
}

LitteraleNombre *LitteraleComplexe::puissance(LitteraleReelle &l)
{
    double valeur = l.getValeur();
    if (valeur != std::floor(valeur))
    {
        throw CalculateurException("Pas possible de traiter POW sur index non-entier d'un complexe");
    }
    LitteraleNombre *result = new LitteraleEntier{1};
    for (size_t i = 0; i < static_cast<long long int>(valeur); i++)
    {
        result = (*result) * (*this);
    }
    return result->simplifier();
}

LitteraleNombre *LitteraleComplexe::traiterUnaireSpeciale(QString operateur)
{
    if (operateur == "IM")
    {
        return (new LitteraleComplexe{
                    (partIm_->traiterUnaireSpeciale("NEG")), partRe_->traiterUnaireSpeciale("NEG")})
            ->simplifier();
    }
    if (operateur == "NEG")
    {
        return (new LitteraleComplexe{
                    (partRe_->traiterUnaireSpeciale("NEG")), partRe_->traiterUnaireSpeciale("NEG")})
            ->simplifier();
    }
    Litterale::traiterUnaireSpeciale(operateur);
    return nullptr;
}

const QString LitteraleComplexe::affichage(QString f) const
{
    if (partIm_->isNull())
    {
        return f.append(partRe_->affichage());
    }
    if (partRe_->isNull())
    {
        return f.append(partIm_->affichage()) + QString("i");
    }
    if (partIm_->isPos())
    {
        return f.append(partRe_->affichage() + QString{"+"} + partIm_->affichage() + QString{"i"});
    }
    return f.append(partRe_->affichage() + partIm_->affichage() + QString{"i"});
}

LitteraleNombre *LitteraleComplexe::operator+(LitteraleNombre &l)
{
    if (l.getType() != TypeLitterale::COMPLEXE)
    {
        return *this + *l.convertToComplexe();
    }
    LitteraleComplexe &l_cast = dynamic_cast<LitteraleComplexe &>(l);
    return (new LitteraleComplexe{*partRe_ + *(l_cast.partRe_),
                                  *partIm_ + *(l_cast.partIm_)})
        ->simplifier();
}

LitteraleNombre *LitteraleComplexe::operator-(LitteraleNombre &l)
{
    if (l.getType() != TypeLitterale::COMPLEXE)
    {
        return *this - *l.convertToComplexe();
    }
    LitteraleComplexe &l_cast = dynamic_cast<LitteraleComplexe &>(l);
    return (new LitteraleComplexe{*partRe_ - *(l_cast.partRe_),
                                  *partIm_ - *(l_cast.partIm_)})
        ->simplifier();
}

LitteraleNombre *LitteraleComplexe::operator*(LitteraleNombre &l)
{
    if (l.getType() != TypeLitterale::COMPLEXE)
    {
        return *this * *l.convertToComplexe();
    }
    LitteraleComplexe &l_cast = dynamic_cast<LitteraleComplexe &>(l);

    LitteraleNombre *a = *partRe_ * (*l_cast.partRe_);
    LitteraleNombre *b = *partIm_ * (*l_cast.partIm_);
    return (new LitteraleComplexe{*(*partRe_ * (*l_cast.partRe_)) - *(*partIm_ * (*l_cast.partIm_)),
                                  *(*partRe_ * (*l_cast.partIm_)) + *(*partIm_ * (*l_cast.partRe_))})
        ->simplifier();
}

LitteraleNombre *LitteraleComplexe::operator/(LitteraleNombre &l)
{
    if (l.getType() != TypeLitterale::COMPLEXE)
    {
        return *this + *l.convertToComplexe();
    }
    LitteraleComplexe &l_cast = dynamic_cast<LitteraleComplexe &>(l);

    LitteraleNombre *den = *((*l_cast.partRe_) * (*l_cast.partRe_)) + *((*l_cast.partIm_) * (*l_cast.partIm_));
    LitteraleNombre *num_1 = *(*partRe_ * (*l_cast.partRe_)) + *(*partIm_ * (*l_cast.partIm_));
    LitteraleNombre *num_2 = *(*partIm_ * (*l_cast.partRe_)) - *(*partRe_ * (*l_cast.partIm_));
    return (new LitteraleComplexe{*num_1 / *den, *num_2 / *den})->simplifier();
}
