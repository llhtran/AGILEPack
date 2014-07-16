//-----------------------------------------------------------------------------
//  dataframe.cxx:
//  Implementation for dataset handling, almost in the vain of the R language
//  Author: Luke de Oliveira (luke.deoliveira@yale.edu)
//-----------------------------------------------------------------------------

#include "include/dataframe.hh"
#include <iostream>
// including library for math parsing
#include "include/exprtk.hpp"

namespace agile
{
//-----------------------------------------------------------------------------
//  Constructors, assignment, etc.
//-----------------------------------------------------------------------------
dataframe::dataframe(std::string filename, bool colnames)
: m_scaled(false), m_cols(0), m_rows(0)
{
    if (filename != "")
    {
        std::ifstream input;
        input.open(filename);
        if (colnames)
        {
            m_columns_set = true;
            std::string line;
            std::getline( input, line );

            std::stringstream ss( line );
            std::string field;
            std::size_t ctr = 0;
            while (getline( ss, field, ',' ))
            {
                std::stringstream fs( field );

                column_names[agile::no_quotes(agile::trim(fs.str()))] = ctr;
                ++ctr;
            }
        }
        input >> data;
        m_rows = data.size();
        m_cols = data[0].size();
        input.close();
    }
    else
    {
        m_columns_set = false;
    }
}
dataframe::dataframe(const dataframe &D) 
: column_names(D.column_names), data(D.data), m_columns_set(D.m_columns_set), 
m_scaled(false), m_cols(D.m_cols), m_rows(D.m_rows)
{
}
//----------------------------------------------------------------------------
dataframe::dataframe(dataframe &&D)
: column_names(std::move(D.column_names)), data(std::move(D.data)),
 m_columns_set(std::move(D.m_columns_set)), m_scaled(false), 
 m_cols(std::move(D.m_cols)), m_rows(std::move(D.m_rows))
{
}
//----------------------------------------------------------------------------
dataframe& dataframe::operator=(const dataframe &D)
{
    column_names = (D.column_names);
    data = (D.data);
    m_columns_set = (D.m_columns_set);
    m_cols = (D.m_cols);
    m_rows = (D.m_rows);
    m_scaled = (D.m_scaled);
    return *this;
}
//----------------------------------------------------------------------------
dataframe& dataframe::operator=(dataframe &&D)
{
    column_names = std::move(D.column_names);
    data = std::move(D.data);
    m_columns_set = std::move(D.m_columns_set);
    m_cols = std::move(D.m_cols);
    m_rows = std::move(D.m_rows);
    m_scaled = std::move(D.m_scaled);
    return *this;

}
//----------------------------------------------------------------------------
dataframe::~dataframe()
{
}

//-----------------------------------------------------------------------------
//  Loading and writing
//-----------------------------------------------------------------------------
void dataframe::from_csv(std::string filename, bool colnames)
{
    std::ifstream input(filename);
    data.clear();
    if (colnames)
    {
        m_columns_set = true;
        std::string line;
        std::getline( input, line );

        std::stringstream ss( line );
        std::string field;
        std::size_t ctr = 0;
        while (getline( ss, field, ',' ))
        {
            std::stringstream fs( field );
            column_names[agile::no_quotes(agile::trim(fs.str()))] = ctr;
            ++ctr;
        }
    }
    else
    {
        m_columns_set = false;
    }
    input >> data;
    m_rows = data.size();
    m_cols = data[0].size();
    input.close();
}
//----------------------------------------------------------------------------
void dataframe::to_csv(std::string filename, bool write_colnames)
{
    std::ofstream output(filename);
    if (write_colnames && m_columns_set)
    {
        output << knit(get_column_names()) << "\n";
    }
    for (auto &row : data)
    {
        output << knit(row) << "\n";
    }
}
//----------------------------------------------------------------------------
std::ostream& operator << ( std::ostream& os, dataframe &data )
{
    if (data.m_columns_set)
    {
        os << knit(data.get_column_names()) << "\n";
    }
    for (auto &row : data.data)
    {
        os << knit(row) << "\n";
    }
    return os;
}
//----------------------------------------------------------------------------
data_t& dataframe::raw()
{
    return data;
}



//-----------------------------------------------------------------------------
//  Size / other Information
//-----------------------------------------------------------------------------
std::size_t dataframe::rows()
{
    return m_rows;
}
//----------------------------------------------------------------------------
std::size_t dataframe::columns()
{
    return m_cols;
}
//----------------------------------------------------------------------------
std::vector<std::string> dataframe::get_column_names()
{
    if (!m_columns_set)
    {
        throw std::runtime_error("column names not set.");
    }
    std::vector<std::string> v(m_cols);
    for (auto &var : column_names)
    {
        v.at(var.second) = agile::trim(var.first);
        //std::cout << "here is " + agile::trim(var.first) << std::endl;
    }
    return std::move(v);
}

std::size_t dataframe::get_column_idx(const std::string &name)
{
    try
    {
        return column_names.at(name);    
    }
    catch(std::out_of_range &e)
    {
        throw std::out_of_range("no variable named \'" + name + "\' present.");
    }   
}
//----------------------------------------------------------------------------
void dataframe::set_column_names(std::vector<std::string> v)
{
    // std::cout << "callingit \n";
    if (m_columns_set)
    {
        throw std::runtime_error("column names already set.");
    }
    std::size_t ctr = 0;
    std::string temp;
    for (auto &e : v)
    {
        temp = agile::trim(e);
        column_names[temp] = ctr;
        // to be able to access branch names for later parsing
        variable_names[ctr] = temp;
        ++ctr;
    }
    m_columns_set = true;
    m_cols = v.size();
}

//-----------------------------------------------------------------------------
//  Element Access
//-----------------------------------------------------------------------------

record_t& dataframe::at(const std::size_t &idx)
{
    return data.at(idx);
}
//----------------------------------------------------------------------------
double& dataframe::at(const std::size_t &idx, const std::string &colname)
{
    try
    {
        return data.at(idx).at(column_names.at(colname));
    }
    catch(std::out_of_range &e)
    {
        std::string wha(
            "tried to access non-existent column \'" + colname + "\'.");
        throw std::out_of_range(wha);
    }
    
}
std::map<std::string, double> dataframe::at(const std::size_t &idx, const std::vector<std::string> cols)
{
    std::map<std::string, double> retval;
    for (auto &colname : cols)
    {
        try
        {
            retval[colname] = data.at(idx).at(column_names.at(colname));
        }
        catch(std::out_of_range &e)
        {
            std::string wha(
                "tried to access non-existent column \'" + colname + "\'.");
            throw std::out_of_range(wha);
        }
    }
    return retval;
}
//----------------------------------------------------------------------------
record_t& dataframe::operator[](const std::size_t &idx)
{
    return data[idx];
}

//-----------------------------------------------------------------------------
//  Additions
//-----------------------------------------------------------------------------

void dataframe::push_back(const record_t &r)
{
    if ((r.size() != m_cols) && (m_cols > 0))
    {
        std::string wha("vectors to be push_back()'d must be the same size");
        throw dimension_error(wha);
    }
    data.push_back(r);
    ++m_rows;
}
//----------------------------------------------------------------------------
void dataframe::push_back(record_t &&r)
{
    if ((r.size() != m_cols) && (m_cols > 0))
    {
        std::string wha("vectors to be push_back()'d must be the same size");
        throw dimension_error(wha);
    }
    // for (auto &entry : r)
    // {
    //     std::cout << entry << "   ";
    // }
    // std::cout << std::endl;
    data.push_back(std::move(r));
    ++m_rows;
}
//----------------------------------------------------------------------------
void dataframe::push_back(std::initializer_list<double> il)
{
    data.emplace_back(il);
}
    // void pop_back();
//----------------------------------------------------------------------------
void dataframe::append(const dataframe &D)
{
    if ((D.m_cols != m_cols) && (m_cols != 0))
    {
        std::string wha(
            "cannot append dataframes with differing numbers of columns.");
        
        throw dimension_error(wha);
    }
    if (m_cols == 0)
    {
        m_cols = D.m_cols;
    }
    for (auto &row : D.data)
    {
        push_back(row);
    }
    if ((!m_columns_set) && D.m_columns_set)
    {
        m_columns_set = true;
        column_names = D.column_names;
    }
}
//----------------------------------------------------------------------------
void dataframe::append(dataframe &&D)
{
    if ((D.m_cols != m_cols) && (m_cols != 0))
    {
        std::string wha(
            "cannot append dataframes with differing numbers of columns.");
        
        throw dimension_error(wha);
    }
    if (m_cols == 0)
    {
        m_cols = std::move(D.m_cols);
    }
    for (auto &row : D.data)
    {
        push_back(std::move(row));
    }
    if ((!m_columns_set) && D.m_columns_set)
    {
        m_columns_set = std::move(D.m_columns_set);
        column_names = std::move(D.column_names);
    }
}

//-----------------------------------------------------------------------------
//  Derived variables - Lien Tran
//-----------------------------------------------------------------------------
void dataframe::add_derived_var(const std::string derived_name, const std::string formula)
{
    std::cout << "We got to this add new columns, bitches!" << std::endl;
    column_names[derived_name] = m_cols;
    m_cols++;

    std::cout << "This is the formula " + formula << std::endl;

    // this map keeps track of branches that are used in the expression
    std::map<std::size_t, std::string> used_var;

    // expression type needed to compute mathematical expression to be parsed
    // part of exprtk library
    typedef exprtk::expression<double> expression_t;

    // number of used variables in expression
    int n_var = 0;
    // parse through all variables names and see which ones
    // are used in the math expression, save it to map
    for (int i = 0; i < m_cols - 1; i++)
    {
        // if the variable is in formula
        // add it to the used_var map
        // variable_names is a map part of dataframe that keeps track of all branches
        if ((formula.find(variable_names[i])) != std::string::npos)
        {
            std::cout << "This variable is used in expression: " + variable_names[i] << std::endl;
            used_var[n_var] = variable_names[i];
            ++n_var;
        }
    }

    // symbol table needed to set up necessary values for computation
    // part of exprtk library
    typedef exprtk::symbol_table<double> symbol_table_t;

    // this is the class to call on for computation
    expression_t computed_expression;
    // go through every row of the newly added variable to fill in computed value
    for (auto &row : data)
    {
        symbol_table_t symbol_table;
        // adds pi, e to table of values
        symbol_table.add_constants();

        // go through every variable used, fill in its new value, compute,
        // then add the to column of the new derived variable
        for (int i = 0; i < n_var; i++)
        {
            symbol_table.add_variable(used_var[i], row.at(get_column_idx(used_var[i])));
        }

        // register table (a pre-computation step)
        computed_expression.register_symbol_table(symbol_table);

        exprtk::parser<double> parser;
        parser.compile(formula, computed_expression);

        //double y = computed_expression;
        //std::cout << "Computed: " + std::to_string(y) << std::endl; 

        // add value to last column of current row
        row.push_back(computed_expression.value());
    }
}

}



dimension_error::dimension_error(const std::string& what_arg)
: std::runtime_error(what_arg)
{
}
