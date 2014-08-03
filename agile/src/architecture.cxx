//-----------------------------------------------------------------------------
//  architecture.cxx:
//  Implementation for architecture class, responsible for joining layers
//  Author: Luke de Oliveira (luke.deoliveira@yale.edu)
//-----------------------------------------------------------------------------

#include "agile/include/architecture.hh"


architecture::architecture(int num_layers) 
: n_layers(num_layers), stack(num_layers)
{
}
//----------------------------------------------------------------------------
architecture::architecture(std::initializer_list<int> il, problem_type type) 
: n_layers(0), stack(0)
{
    int prev = *(il.begin());

    for (auto value = (il.begin() + 1); value != il.end(); ++value)
    {
        if (value != (il.end() - 1))
        {
            stack.emplace_back(new layer(prev, *value, sigmoid));
        }
        else
        {
            if (type == multiclass)
            {
            stack.emplace_back(new layer(prev, *value, softmax));
            }
            else if (type == classify)
            {
            stack.emplace_back(new layer(prev, *value, sigmoid));
            }
            else
            {
            stack.emplace_back(new layer(prev, *value, linear));    
            }           
        }
        ++n_layers;
        prev = *value;
    }
}
//----------------------------------------------------------------------------
architecture::architecture(const std::vector<int> &v, problem_type type)
: n_layers(0), stack(0)
{
    int prev = *(v.begin());

    for (auto value = (v.begin() + 1); value != v.end(); ++value)
    {
        if (value != (v.end() - 1))
        {
            stack.emplace_back(new layer(prev, *value, sigmoid));
        }
        else
        {
            if (type == multiclass)
            {
            stack.emplace_back(new layer(prev, *value, softmax));
            }
            else if (type == classify)
            {
            stack.emplace_back(new layer(prev, *value, sigmoid));
            }
            else
            {
            stack.emplace_back(new layer(prev, *value, linear));    
            }           
        }
        ++n_layers;
        prev = *value;
    }
}
//----------------------------------------------------------------------------
architecture::architecture(const architecture &arch) 
: n_layers(0), stack(0)
{
    for (auto &entry : arch.stack)
    {
        stack.emplace_back(entry->clone());
        ++n_layers;
    }
}
//----------------------------------------------------------------------------
architecture& architecture::operator =(const architecture &arch)
{
    clear();
    for (auto &entry : arch.stack)
    {
        stack.emplace_back(entry->clone());
        ++n_layers;
    }
    return *this;
}
//----------------------------------------------------------------------------
architecture::~architecture()
{
}
//----------------------------------------------------------------------------
void architecture::add_layer(int n_inputs, int n_outputs, layer_type type)
{
    ++n_layers;
    stack.emplace_back(new layer(n_inputs, n_outputs, type));
}
//----------------------------------------------------------------------------
void architecture::resize(unsigned int size)
{
    stack.resize(size);
    n_layers = size;
}
//----------------------------------------------------------------------------
void architecture::pop_back()
{
    stack.pop_back();
    --n_layers;
}
//----------------------------------------------------------------------------
unsigned int architecture::size()
{
    return n_layers;
}
//----------------------------------------------------------------------------
void architecture::clear()
{
    n_layers = 0;
    stack.clear();
}
//----------------------------------------------------------------------------
std::unique_ptr<layer>& architecture::at(const unsigned int &idx)
{
    if (idx >= n_layers)
    {
        throw std::out_of_range(
            "Accessing layer in 'Class: architecture' beyond contained range.");
    }
    return stack.at(idx);
}
//----------------------------------------------------------------------------
agile::vector architecture::predict(const agile::vector &v)
{
    stack.at(0)->charge(v);
    for (unsigned int i = 1; i < n_layers; ++i)
    {
        stack.at(i)->charge(stack.at(i - 1)->fire());
    }
    return stack.at(n_layers - 1)->fire();
}
//----------------------------------------------------------------------------
// correct() functions are for supervised training!
void architecture::correct(const agile::vector &in, 
    const agile::vector &target)
{
    agile::vector error = predict(in) - target;
    // std::cout << predict(in) << std::endl;
    // std::cout << "error: " << error << std::endl;
    int l = n_layers - 1;
    stack.at(l)->backpropagate(error);

    for (l = (n_layers - 2); l >= 0; --l)
    {
        stack.at(l)->backpropagate( stack.at(l + 1)->dump_below() );
    }
}
//----------------------------------------------------------------------------
// correct() functions are for supervised training!
void architecture::correct(const agile::vector &in, 
    const agile::vector &target, double weight)
{
    agile::vector error = predict(in) - target;
    // std::cout << predict(in) << std::endl;
    // std::cout << "error: " << error << std::endl;
    int l = n_layers - 1;
    stack.at(l)->backpropagate(error, weight);

    for (l = (n_layers - 2); l >= 0; --l)
    {
        stack.at(l)->backpropagate( stack.at(l + 1)->dump_below(), weight);
    }
}
//----------------------------------------------------------------------------
// encode() functions are for unsupervised training!
// QUESTION
void architecture::encode(const agile::vector &in, const unsigned int &which, 
    bool noisify)
{
    if (which == 0)
    {
        stack.at(0)->encode(in, noisify);
        return;
    }
    stack.at(0)->charge(in);

    for (unsigned int l = 1; l < which; ++l)
    {
        stack.at(l)->charge(stack.at(l - 1)->fire());
    }
    agile::vector v = stack.at(which - 1)->fire();
    stack.at(which)->encode(v, noisify);
}
//----------------------------------------------------------------------------
// encode() functions are for unsupervised training!
void architecture::encode(const agile::vector &in, const unsigned int &which, 
    double weight, bool noisify)
{
    // which is the epoch #
    if (which == 0)
    {
        stack.at(0)->encode(in, weight, noisify);
        return;
    }
    stack.at(0)->charge(in);

    for (unsigned int l = 1; l < which; ++l)
    {
        stack.at(l)->charge(stack.at(l - 1)->fire());
    }
    agile::vector v = stack.at(which - 1)->fire();
    stack.at(which)->encode(v, weight, noisify);
}
//----------------------------------------------------------------------------
double architecture::encoding_mse(const agile::matrix &A, const unsigned int &which)
{
    double MSE = 0.0;
    if (which == 0)
    {
        for (int i = 0; i < A.rows(); ++i)
        {
            agile::vector t(A.row(i));
            t -= stack.at(0)->reconstruct(A.row(i));
            MSE += t.array().square().sum();
        }
        return MSE / A.rows();
    }

    for (int i = 0; i < A.rows(); ++i)
    {
        stack.at(0)->charge(A.row(i));
        for (unsigned int l = 1; l < which; ++l)
        {
            stack.at(l)->charge(stack.at(l - 1)->fire());
        }
        agile::vector v = stack.at(which - 1)->fire();
        v -= stack.at(which)->reconstruct(v);
        MSE += v.array().square().sum();
    }
    return MSE / A.rows();    
}
//----------------------------------------------------------------------------
void architecture::set_batch_size(int size)
{
    if (n_layers < 1)
    {
        throw std::logic_error(
            "can't set batch size-- network has no layers.");
    }
    for (auto &layer : stack)
    {
        layer->set_batch_size(size);
    }
}
void architecture::set_learning(const numeric &value)
{
    if (n_layers < 1)
    {
        throw std::logic_error(
            "can't set learning rate-- network has no layers.");
    }
    for (auto &layer : stack)
    {
        layer->set_learning(value);
    }
}
void architecture::set_momentum(const numeric &value)
{
    if (n_layers < 1)
    {
        throw std::logic_error(
            "can't set momentum-- network has no layers.");
    }
    for (auto &layer : stack)
    {
        layer->set_momentum(value);
    }
}
void architecture::set_regularizer(const numeric &value)
{
    if (n_layers < 1)
    {
        throw std::logic_error(
            "can't set regularizer-- network has no layers.");
    }
    for (auto &layer : stack)
    {
        layer->set_regularizer(value);
    }
}
// calls set_contractive() for all layers in the neural net
void architecture::set_contractive() // Added by Lien
{
    if (n_layers < 1)
    {
        throw std::logic_error(
            "can't set contractive autoencoder-- network has no layers.");
    }
    for (auto &layer : stack)
    {
        std::cout << "Setting contractive" << std::endl;
        layer->set_contractive();
    }
}
//----------------------------------------------------------------------------
