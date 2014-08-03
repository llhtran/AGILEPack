//-----------------------------------------------------------------------------
//  layer.hh:
//  Header for layer class, responsible for s(Wx+b) calculation
//  Author: Luke de Oliveira (luke.deoliveira@yale.edu)
//-----------------------------------------------------------------------------

#ifndef LAYER_HH
#define LAYER_HH

#include <iostream>
#include "agile/include/basedefs.hh"
#include "agile/include/activation.hh"

//-----------------------------------------------------------------------------
//  A simple layer factory for this and all derived versions
//-----------------------------------------------------------------------------
/**
 * @brief A layer factory class.
 * @details Usage is simple, one simply has 
 * 
 * @code 
 * layer *l_new = layer_factory<layer>(12, 4);
 * @endcode
 * 
 * @tparam T The type of layer. Can be the class layer and all inheriting types.
 * @tparam class ...Args Some variadic number of arguments needed 
 * by the class constructor indicated by T.
 * @return A pointer to a new instance of T.
 */
template <class T, class ...Args>
typename std::enable_if<!std::is_array<T>::value, T*>::type
layer_factory(Args&& ...args)
{
    return dynamic_cast<T*>(new T(std::forward<Args>(args)...));
}

//-----------------------------------------------------------------------------
//  Hacky things for yaml-cpp friendship
//-----------------------------------------------------------------------------

class layer;
class architecture;
class autoencoder;

namespace YAML
{
    template <>
    struct convert<layer>;
}

namespace YAML
{
    template <>
    struct convert<architecture>;
}

// QUESTION: Where does this class come from, exactly? What is this declaration for?
namespace agile
{
    class neural_net;
}

//-----------------------------------------------------------------------------
//  layer class implementation
//-----------------------------------------------------------------------------

/**
 * @brief Base class for layers in AGILEPack
 * @details Provides an  abstraction layer for the underlying operations in a 
 * neural network. In the basic case, a layer is paramaterized by a matrix
 * \f$W\f$, a vector \f$b\f$, and an activation function \f$\sigma(\cdot)\f$.
 * The basic operation takes vectors of one dimensionality, and transforms them 
 * to a new dimensionality. Formally, consider an input vector 
 * \f$x\in\mathbb{R}^{D}\f$, and a layer parameterized by \f$W\in\mathbb{R}^{d\times D}\f$
 * , \f$b\in\mathbb{R}^{d}\f$, and an activation function \f$\sigma\f$. The underlying
 * operation is 
 * \f[
 * y = \sigma(Wx + b)
 * \f]
 */
class layer
{
public:
//-----------------------------------------------------------------------------
//  Construction, destruction, and copying
//-----------------------------------------------------------------------------
    explicit layer(int n_inputs = 0, 
        int n_outputs = 0, layer_type type = linear);

    layer(const layer &L);
    layer(layer &&L);
    layer(layer *L);
    virtual layer& operator= (const layer &L);
    virtual layer& operator= (layer &&L);
    virtual ~layer() = default;

    void construct(int n_inputs, int n_outputs, layer_type type);

    virtual agile::types::paradigm get_paradigm()
    {
        return m_paradigm;
    }

    virtual void resize_input(int n_inputs);
    virtual void resize_output(int n_outputs);

    int num_inputs()
    {
        return m_inputs;
    }
    int num_outputs()
    {
        return m_outputs;
    }
//-----------------------------------------------------------------------------
//  Training methods for inter-layer communication
//-----------------------------------------------------------------------------
    virtual void reset_weights(numeric bound);
    void charge(const agile::vector& v); 
    agile::vector fire(); // Fire the charge.
    agile::vector dump_below();


    void backpropagate(const agile::vector &v);
    void backpropagate(const agile::vector &v, double weight);

//-----------------------------------------------------------------------------
//  3 functions for the use of contractive autoencoders by Lien Tran
//-----------------------------------------------------------------------------
    // calculates the Jacobian matrix
    agile::matrix get_jacobian();
    // save to W * x + b formula to prepare for derivations
    void charge_jacobian();
    // pass W * x + b through activation function
    agile::vector fire_jacobian();

    void update();
    void update(double weight);

//-----------------------------------------------------------------------------
//  Parameter Setting methods
//-----------------------------------------------------------------------------
    virtual void set_batch_size(int size)
    {
        if (ctr > 0)
        {
            update();
            m_batch_size = size;
        }
        else
        {
            m_batch_size = size;
        }
    }
    virtual void set_learning(const numeric &value)
    {
        learning = value;
    }
    virtual void set_momentum(const numeric &value)
    {
        momentum = value;
    }
    virtual void set_regularizer(const numeric &value)
    {
        if (!contractive)
            regularizer = value;
        else
            throw std::logic_error("Autoencoder cannot be regularized and\
                contractive at the same time.");
    }
//-----------------------------------------------------------------------------
//  set_contractive() - By Lien Tran
//  HOW TO USE:
//  Call at the interface file with the neural_net class after having set up layers
//  The net will then call set_contractive at the layer level for all layers
//  This activates the contractive autoencoder in the net
//  Can NOT set_contractive() and set_regularizer() 
//  A neural net can only be one of the two types
//-----------------------------------------------------------------------------
    virtual void set_contractive()
    {
        if (regularizer == 0)
            contractive = true;
        else
            throw std::logic_error("Autoencoder cannot be regularized and\
                contractive at the same time.");
    }
    void set_layer_type(const layer_type &type)
    {
        m_layer_type = type;
    }
    layer_type get_layer_type()
    {
        return m_layer_type;
    }

    agile::matrix get_weights()
    {
        return W;
    }
//-----------------------------------------------------------------------------
//  Access for YAML serialization
//-----------------------------------------------------------------------------
    friend YAML::Emitter& operator << (YAML::Emitter& out, const layer &L);

    friend YAML::Emitter& operator << (YAML::Emitter& out, 
        const architecture &arch);

    friend struct YAML::convert<layer>;
    friend struct YAML::convert<architecture>;

    friend class architecture;
    friend class agile::neural_net;


//-----------------------------------------------------------------------------
//  Stupid virtuals for derived classes
//----------------------------------------------------------------------------- 
    virtual agile::vector reconstruct(const agile::vector &v, 
        bool noisify = true) 
    {
        throw std::logic_error("layer::reconstruct() called on class\
         layer -- only valid for class autoencoder");
    }
    virtual void encode(const agile::vector &v, bool noisify = true) 
    {
        throw std::logic_error("layer::encode() called on class\
         layer -- only valid for class autoencoder");
    }

    virtual void encode(const agile::vector &v, double weight, bool noisify = true) 
    {
        throw std::logic_error("layer::encode() called on class\
         layer -- only valid for class autoencoder");
    }

    virtual agile::vector get_encoding(const agile::vector &v)
    {
        throw std::logic_error("layer::get_encoding() called on class\
         layer -- only valid for class autoencoder");
    }
    virtual agile::vector decode(const agile::vector &v)
    {
        throw std::logic_error("layer::decode() called on class\
         layer -- only valid for class autoencoder");
    }
    
protected:
    // to do / think about more since im out of control
    // agile::matrix get_jacobian_gradient(agile::vector v);
//-----------------------------------------------------------------------------
//  Protected Members
//  Lien added the Jacobian, jacobian penalty and contractive members
//  and their corresponding constructors in the .cxx file
//-----------------------------------------------------------------------------
    int m_inputs,     // number of inputs to the layer
        m_outputs,    // number of outputs leaving the layer
        m_batch_size, // number of examples to consider when updating gradient
        ctr;          // number of examples we've considered so far

    agile::matrix W,       // current weight matrix
                  W_old,   // previous weight matrix
                  W_change,// the change to make to W
                  Jacobian;// the Jacobian penalty, 
                           // used for contractive autoencoder functions

    agile::vector b,            // bias vector
                  b_old,        // previous bias vector
                  b_change,     // change to make to b
                  m_out,        // untransformed layer output
                  m_in,         // input to the layer
                  delta,        // intermediate derivative
                  m_dump_below; // quantity to feed to a lower layer

    numeric learning,    // learning rate
            momentum,    // momentum (gradient smoothing) parameter
            regularizer, // l2 regularization scalar
            jacobian_penalty; // jacobian_penalty

    bool contractive;   // whether its a contractive autoencoder

    layer_type m_layer_type; // what type of layer (linear, sigmoid, etc.)
    agile::types::paradigm m_paradigm; //type of pre-training

private:
    virtual layer* clone()
    {
        return new layer(*this);   
    }

};

//-----------------------------------------------------------------------------
//  Typedef the stack of layers for the architecture class
//-----------------------------------------------------------------------------
namespace agile
{
    typedef std::vector<std::unique_ptr<layer>> layer_stack;
}

//-----------------------------------------------------------------------------
//  YAML Serialization Structure 
//  (look at https://code.google.com/p/yaml-cpp/wiki/Tutorial)
//-----------------------------------------------------------------------------
namespace YAML 
{
    template<>
    struct convert<layer> 
    {
        static Node encode(const layer& L)
        {
            Node node;
            node["class"] = "layer";
            node["inputs"] = L.m_inputs;
            node["outputs"] = L.m_outputs;
            node["learning"] = L.learning;
            node["momentum"] = L.momentum;
            node["regularizer"] = L.regularizer;
            node["batchsize"] = L.m_batch_size;

            if (L.m_layer_type == linear)
            {
                node["activation"] = "linear";
            }
            else if (L.m_layer_type == softmax)
            {
                node["activation"] = "softmax";
            }
            else if (L.m_layer_type == rectified)
            {
                node["activation"] = "rectified";
            }
            else
            {
                node["activation"] = "sigmoid";
            }

            node["weights"] = agile::stringify(L.W);
            node["bias"] = agile::stringify(L.b);
            // node["decoder"] = layer::_reveal(L);

            return node;
        }

        static bool decode(const Node& node, layer& L) 
        {
            L.m_paradigm = agile::types::Basic;
            L.m_inputs = node["inputs"].as<int>();
            L.m_outputs = node["outputs"].as<int>();
            L.learning = node["learning"].as<double>();
            L.momentum = node["momentum"].as<double>();
            L.regularizer = node["regularizer"].as<double>();
            L.m_batch_size = node["batchsize"].as<int>();

            L.m_in.conservativeResize(L.m_inputs);
            L.m_out.conservativeResize(L.m_outputs);



            std::string tmp_str = node["activation"].as<std::string>();
            if (tmp_str == "linear")
            {
                L.m_layer_type = linear;
            }
            else if (tmp_str == "sigmoid")
            {
                L.m_layer_type = sigmoid;
            }
            else if (tmp_str == "rectified")
            {
                L.m_layer_type = rectified;
            }
            else
            {
                L.m_layer_type = softmax;
            }

            L.W = agile::destringify(node["weights"].as<std::string>());
            L.b = agile::destringify(node["bias"].as<std::string>());

            return true;
        }
    };
} // end ns YAML


#endif


