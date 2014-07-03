//-----------------------------------------------------------------------------
//  activation.hh:
//  Header for neural activation functions
//  Author: Luke de Oliveira (luke.deoliveira@yale.edu)
//-----------------------------------------------------------------------------

#include "agile/include/activation.hh"

//----------------------------------------------------------------------------
agile::vector agile::functions::rect_lin_unit(const agile::vector &v)
{
    agile::vector w(v);
    for (int row = 0; row < v.rows(); ++row)
    {
        w(row) = std::max(w(row), 0.0);
    }
    return std::move(w);
}
//----------------------------------------------------------------------------
agile::vector agile::functions::rect_lin_unit_deriv(const agile::vector &v)
{
    agile::vector w(v);
    for (int row = 0; row < v.rows(); ++row)
    {
        w(row) = (w(row) > 0) ? 1.0 : 0.0;
    }
    return std::move(w);
}
//----------------------------------------------------------------------------
agile::vector agile::functions::exp_sigmoid(const agile::vector &v)
{
    agile::vector w(v);
    for (int row = 0; row < v.rows(); ++row)
    {
        w(row) = 1 / (1 + exp(-w(row)));
    }
    return std::move(w);
}
//----------------------------------------------------------------------------
// this is for s'(x) = s(x) * (1 - s(x))
agile::vector agile::functions::exp_sigmoid_deriv(const agile::vector &v)
{
    agile::vector w(v);
    for (int row = 0; row < v.rows(); ++row)
    {
        w(row) = w(row) * (1 - w(row));
    }
    return std::move(w);
}
//----------------------------------------------------------------------------
agile::vector agile::functions::softmax(const agile::vector &v)
{
    agile::vector w(v);
    for (int row = 0; row < v.rows(); ++row)
    {
        w(row) = exp(w(row));
    }
    w /= w.sum();
    return std::move(w);
}
//----------------------------------------------------------------------------
agile::vector agile::functions::add_noise(const agile::vector &v, 
    double level)
{
    std::normal_distribution <numeric> distribution(0.0, level);

    agile::vector w(v);
    for (int row = 0; row < v.rows(); ++row)
    {
        w(row) += distribution(agile::mersenne_engine());;
    }
    return std::move(w);
}
//----------------------------------------------------------------------------