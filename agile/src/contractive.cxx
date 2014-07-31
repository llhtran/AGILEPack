//-----------------------------------------------------------------------------
//  contractive.cxx:
//  Implementation of contractive autoencoder class, inherits from layer class
//  Author: Lien Tran (lien.tran@yale.edu)
//-----------------------------------------------------------------------------

#include "agile/include/contractive.hh"

contractive::contractive(int n_inputs, int n_outputs, 
    layer_type encoder_type, layer_type decoder_type) :
layer(n_inputs, n_outputs, encoder_type), 
decoder(n_outputs, n_inputs, decoder_type),
m_paradigm(agile::types::contractive)

{
}
//----------------------------------------------------------------------------
contractive::contractive(const contractive &L) :
layer(L), 
decoder(L.decoder)
{
}
//----------------------------------------------------------------------------
contractive::contractive(contractive *L) :
layer(*L), 
decoder(L->decoder)
{
}
//----------------------------------------------------------------------------
contractive& contractive::operator= (const contractive &L)
{
    m_inputs = (L.m_inputs);
    m_outputs = (L.m_outputs);
    m_batch_size = (L.m_batch_size);

    W = (L.W);
    W_old = (L.W_old);
    W_change = (L.W_change);

    b = (L.b);
    b_old = (L.b_old);
    b_change = (L.b_change);

    m_out = (L.m_out);
    m_in = (L.m_in);

    learning = (L.learning);
    momentum = (L.momentum);
    regularizer = (L.regularizer);
    m_paradigm = (L.m_paradigm);
    m_layer_type = (L.m_layer_type);
    decoder = (L.decoder);

    return *this;
}
//----------------------------------------------------------------------------
contractive& contractive::operator= (contractive &&L)
{
    m_inputs = std::move(L.m_inputs);
    m_outputs = std::move(L.m_outputs);
    m_batch_size = std::move(L.m_batch_size);

    W = std::move(L.W);
    W_old = std::move(L.W_old);
    W_change = std::move(L.W_change);

    b = std::move(L.b);
    b_old = std::move(L.b_old);
    b_change = std::move(L.b_change);

    m_out = std::move(L.m_out);
    m_in = std::move(L.m_in);

    learning = std::move(L.learning);
    momentum = std::move(L.momentum);
    regularizer = std::move(L.regularizer);

    m_layer_type = std::move(L.m_layer_type);
    m_paradigm = (L.m_paradigm);
    decoder = std::move(L.decoder);

    return *this;
}
//----------------------------------------------------------------------------
void contractive::construct(int n_inputs, int n_outputs, 
    layer_type encoder_type, layer_type decoder_type)
{
    layer::construct(n_inputs, n_outputs, encoder_type);
    decoder.construct(n_outputs, n_inputs, decoder_type);
    m_paradigm = agile::types::contractive;
}
//----------------------------------------------------------------------------
void contractive::resize_input(int n_inputs)
{
    decoder.resize_output(n_inputs);
    m_inputs = n_inputs;
    W.resize(m_outputs, n_inputs);
    W_change.resize(m_outputs, n_inputs);
    W_old.resize(m_outputs, n_inputs);
    m_in.resize(n_inputs, Eigen::NoChange);
    reset_weights(sqrt((numeric)6 / (numeric)(m_inputs + m_outputs)));
}
//----------------------------------------------------------------------------
void contractive::resize_output(int n_outputs)
{
    decoder.resize_input(n_outputs);
    m_outputs = n_outputs;
    W.resize(n_outputs, m_inputs);
    W_change.resize(n_outputs, m_inputs);
    W_old.resize(n_outputs, m_inputs);
    b.resize(n_outputs, Eigen::NoChange);
    b_change.resize(n_outputs, Eigen::NoChange);
    b_old.resize(n_outputs, Eigen::NoChange);
    m_out.resize(n_outputs, Eigen::NoChange);
    reset_weights(sqrt((numeric)6 / (numeric)(m_inputs + m_outputs)));
}
//----------------------------------------------------------------------------
void contractive::reset_weights(numeric bound)
{
    layer::reset_weights(bound);
    decoder.reset_weights(bound);
}
//----------------------------------------------------------------------------
void contractive::encode(const agile::vector &v, bool noisify)
{
    // NEEDS WORK
    agile::vector error = reconstruct(v, noisify) - v;  
    decoder.backpropagate(error);
    backpropagate(decoder.dump_below());
}
//----------------------------------------------------------------------------
void contractive::encode(const agile::vector &v, double weight, bool noisify)
{
    // NEEDS WORK
    agile::vector error = reconstruct(v, noisify) - v;  
    decoder.backpropagate(error, weight);
    backpropagate(decoder.dump_below(), weight);
}
//----------------------------------------------------------------------------
agile::vector contractive::reconstruct(const agile::vector &v, bool noisify)
{
    if (noisify)
    {
        this->charge(agile::functions::add_noise(v));
    }
    else
    {
        this->charge(v);
    }
    // decoder is a layer class
    decoder.charge(this->fire());
    return decoder.fire();
}
//----------------------------------------------------------------------------
agile::vector contractive::get_encoding(const agile::vector &v)
{
    this->charge(v);
    return std::move(this->fire());
}
//----------------------------------------------------------------------------
agile::vector contractive::decode(const agile::vector &v)
{
    decoder.charge(v);
    return decoder.fire();
}
//----------------------------------------------------------------------------
contractive::~contractive()
{
}   
//----------------------------------------------------------------------------
YAML::Emitter& operator << (YAML::Emitter& out, const contractive &L) 
{
    out << YAML::BeginMap;
    out << YAML::Key << "inputs"      << YAML::Value << L.m_inputs;
    out << YAML::Key << "outputs"     << YAML::Value << L.m_outputs;
    out << YAML::Key << "learning"    << YAML::Value << L.learning;
    out << YAML::Key << "momentum"    << YAML::Value << L.momentum;
    out << YAML::Key << "regularizer" << YAML::Value << L.regularizer;
    out << YAML::Key << "batchsize"   << YAML::Value << L.m_batch_size;
    out << YAML::Key << "activation";

    if (L.m_layer_type == linear)
    {
        out << YAML::Value << "linear";
    }
    else if (L.m_layer_type == softmax)
    {
        out << YAML::Value << "softmax";
    }
    else if (L.m_layer_type == rectified)
    {
        out << YAML::Value << "rectified";
    }
    else
    {
        out << YAML::Value << "sigmoid";
    }

    out << YAML::Key << "weights" << YAML::Value << agile::stringify(L.W);
    out << YAML::Key << "bias"    << YAML::Value << agile::stringify(L.b);
    out << YAML::EndMap;
    return out;
}
//----------------------------------------------------------------------------
