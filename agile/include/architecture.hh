//-----------------------------------------------------------------------------
//  architecture.hh:
//  Header for architecture class, responsible for joining layers
//  Author: Luke de Oliveira (luke.deoliveira@yale.edu)
//-----------------------------------------------------------------------------

#ifndef ARCHITECTURE_HH
#define ARCHITECTURE_HH 

#include "agile/include/layer.hh"
#include "agile/include/autoencoder.hh"


//-----------------------------------------------------------------------------
//  Generic enum for initializer list construction
//-----------------------------------------------------------------------------

enum problem_type { regress, classify, multiclass };

//-----------------------------------------------------------------------------
//  Hacky things for yaml-cpp friendship
//-----------------------------------------------------------------------------
class architecture;

namespace YAML
{
    template <>
    struct convert<architecture>;
}

//-----------------------------------------------------------------------------
//  architecture class implementation
//-----------------------------------------------------------------------------
class architecture
{
public:
//-----------------------------------------------------------------------------
//  Construction, destruction, and copying
//-----------------------------------------------------------------------------
    explicit architecture(int num_layers = 0);
    explicit architecture(std::initializer_list<int> il, 
        problem_type type = regress);
    explicit architecture(const std::vector<int> &v, 
        problem_type type = regress);

    ~architecture();
    architecture(const architecture &arch);

    template <class T>
    architecture& operator =(const T &L);

    template <class T>
    architecture& operator =(T *L);

    virtual architecture& operator =(const architecture &arch);

//-----------------------------------------------------------------------------
//  Layer Manipulation and access methods
//----------------------------------------------------------------------------- 
    template <class T>
    void add_layer(const T &L);

    void add_layer(int n_inputs = 0, int n_outputs = 0, 
        layer_type type = linear);

    template <class T>
    void add_layer(T *L);

    // THIS TAKES OWNERSHIP
    template <class T>
    void emplace_back(T *L);

    template <class T, class ...Args>
    void add_layer(Args&& ...args);

    std::unique_ptr<layer>& at(const unsigned int &idx);

    template <class T>
    T* cast_layer(const unsigned int &idx);

    void pop_back();
    void clear();
    unsigned int size();
    void resize(unsigned int size);

//-----------------------------------------------------------------------------
//  Global parameter setting
//-----------------------------------------------------------------------------

    void set_batch_size(int size);
    void set_learning(const numeric &value);
    void set_momentum(const numeric &value);
    void set_regularizer(const numeric &value);
    void set_contractive(); // added by Lien

//-----------------------------------------------------------------------------
//  Prediction and training methods
//-----------------------------------------------------------------------------
    agile::vector predict(const agile::vector &v);
    
    void correct(const agile::vector &in, const agile::vector &target);
    void correct(const agile::vector &in, const agile::vector &target, 
        double weight);

    void encode(const agile::vector &in, const unsigned int &which, 
        bool noisify = true);

    void encode(const agile::vector &in, const unsigned int &which, 
        double weight, bool noisify = true);

    double encoding_mse(const agile::matrix &A, const unsigned int &which);

//-----------------------------------------------------------------------------
//  Access for YAML serialization
//-----------------------------------------------------------------------------
    friend struct YAML::convert<architecture>;

protected:
//-----------------------------------------------------------------------------
//  Protected Members
//-----------------------------------------------------------------------------
    unsigned int n_layers;    // number of network layers
    agile::layer_stack stack; // the stack of layers
                              // basically vector of unique ptrs to layer class
};

template <class T>
architecture& architecture::operator =(const T &L)
{
    clear();
    stack.emplace_back(new T(L));
    ++n_layers;
    return *this;
}

template <class T>
architecture& architecture::operator =(T *L)
{
    clear();
    stack.emplace_back((T*)L);
    ++n_layers;
    return *this;
}

template <class T>
void architecture::add_layer(const T &L)
{
    stack.emplace_back(new T(L));
    ++n_layers;
}

template <class T>
void architecture::add_layer(T *L)
{
    ++n_layers;
    stack.emplace_back(new T(L));
}

template <class T>
void architecture::emplace_back(T *L)
{
    ++n_layers;
    // unsafe cast?
    stack.emplace_back(std::move((T*)(L)));
}

template <class T, class ...Args>
void architecture::add_layer(Args&& ...args)
{
    ++n_layers;
    stack.emplace_back(new T(std::forward<Args>(args)...));
}

template <class T>
T* architecture::cast_layer(const unsigned int &idx)
{
    return dynamic_cast<T*>(stack.at(idx).get());
}


//-----------------------------------------------------------------------------
//  YAML Serialization Structure 
//  (look at https://code.google.com/p/yaml-cpp/wiki/Tutorial)
//-----------------------------------------------------------------------------
namespace YAML 
{

template<>
struct convert<architecture> 
{
    static Node encode(const architecture &arch)
    {
        Node node;
        for (unsigned int i = 0; i < arch.stack.size(); ++i)
        {
            std::string layer_index = "layer_" + std::to_string(i);
            node["layer_access"].push_back(layer_index);
            if (arch.stack.at(i)->get_paradigm() == agile::types::Autoencoder)
            {
                node[layer_index] = *(dynamic_cast<autoencoder*>(
                    arch.stack.at(i).get()));
            }
            else
            {
                node[layer_index] = *(arch.stack.at(i).get());
            }
        }
        return node;
    }

    static bool decode(const Node& node, architecture &arch) 
    {

        arch.clear();

        auto layer_names = node["layer_access"];

        for (unsigned int i = 0; i < layer_names.size(); ++i)
        {
            std::string layer_id = layer_names[i].as<std::string>(); 
            std::string class_type = node[layer_id]["class"].as<std::string>();

            if (class_type == "autoencoder")
            {
                arch.emplace_back(
                    new autoencoder(node[layer_id].as<autoencoder>()));
            }
            else if (class_type == "layer")
            {
                arch.emplace_back(new layer(node[layer_id].as<layer>()));
            }
            else
            {
                throw std::logic_error(
                    "class " + class_type + " not recognized.");
            }

                     
        }

        return true;
    }
};
//----------------------------------------------------------------------------
}


#endif