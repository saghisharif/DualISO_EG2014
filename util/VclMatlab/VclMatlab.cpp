#include <iostream>
#include <cstring>

#include "VclMatlab.h"

//
// NOTE:
// This is not the prettiest code ever!
// Because the Matio library is rather lame, one needs rewind the variable iterator
// pointer each time one accesses a variable.  Therefore, each function is required
// to rewind the pointer when it starts to make sure all is well.
//

namespace Vcl
{


VclMatio::VclMatio()
    : m_matfp(NULL)
    , m_filename("")
{ }


VclMatio::VclMatio(const std::string& filename, bool forReading)
    : m_filename(filename)
{
    if (forReading)
        openForReading(m_filename);
    else
        openForWriting(m_filename);
}


VclMatio::~VclMatio()
{
    close();
}


// Reading
bool VclMatio::openForReading(const std::string& filename)
{
    close();
    m_matfp = Mat_Open(filename.c_str(), MAT_ACC_RDONLY);
    if (m_matfp == NULL)
        return false;
    return true;
}


bool VclMatio::readValueNamed(const std::string& name, float& value)
{
    value = 0.0f;
    if (!matFileOpen(m_matfp))
        return false;
    
    // Make sure the variable exists, is a single value, and is floating point
    if (!isFloat(name))
        return false;
    if (!isValue(name))
        return false;

    // Read the data into memory and do one last check
    Mat_Rewind(m_matfp);
    matvar_t* matvarRead = Mat_VarRead(m_matfp, name.c_str());
    
    // Assign the actual value to our return variable
    if (matvarRead->data_type == MAT_T_SINGLE)
    {
        value = *(float*)matvarRead->data;
    }
    else if (matvarRead->data_type == MAT_T_DOUBLE)
    {
        value = (float)*(double*)matvarRead->data;
    }

    Mat_VarFree(matvarRead);
    return true;
}


bool VclMatio::readValueNamed(const std::string& name, double& value)
{
    value = 0.0;
    if (!matFileOpen(m_matfp))
        return false;
    
    // Make sure the variable exists, is a single value, and is floating point
    if (!isFloat(name))
        return false;
    if (!isValue(name))
        return false;

    // Read the data into memory
    Mat_Rewind(m_matfp);
    matvar_t* matvarRead = Mat_VarRead(m_matfp, name.c_str());
    
    // Assign the actual value to our return variable
    if (matvarRead->data_type == MAT_T_SINGLE)
    {
        value = (double)*(float*)matvarRead->data;
    }
    else if (matvarRead->data_type == MAT_T_DOUBLE)
    {
        value = *(double*)matvarRead->data;
    }

    Mat_VarFree(matvarRead);
    return true;
}


bool VclMatio::readEigenVectorNamed(const std::string& name, Eigen::VectorXf& value)
{
    value.resize(0);
    if (!matFileOpen(m_matfp))
        return false;

    // Make sure the variable exists and is floating point
    if (!isFloat(name))
        return false;

    // Read the data into memory
    Mat_Rewind(m_matfp);
    matvar_t* matvarRead = Mat_VarRead(m_matfp, name.c_str());

    // Make sure the dimensionality is correct
    size_t dim = 0;
    if (isRowVec(name))
        dim = matvarRead->dims[1];
    else if (isColVec(name))
        dim = matvarRead->dims[0];
    else
        return false;

    // Assign the actual value to our return variable
    value.resize(dim);
    if (matvarRead->data_type == MAT_T_SINGLE)
    {
        for (int x = 0; x < dim; x++)
            value(x) = ((float*)matvarRead->data)[x];
    }
    else if (matvarRead->data_type == MAT_T_DOUBLE)
    {
        for (int x = 0; x < dim; x++)
            value(x) = ((double*)matvarRead->data)[x];
    }

    Mat_VarFree(matvarRead);
    return true;
}


bool VclMatio::readEigenVectorNamed(const std::string& name, Eigen::VectorXd& value)
{
    value.resize(0);
    if (!matFileOpen(m_matfp))
        return false;

    // Make sure the variable exists and is floating point
    if (!isFloat(name))
        return false;

    // Read the data into memory
    Mat_Rewind(m_matfp);
    matvar_t* matvarRead = Mat_VarRead(m_matfp, name.c_str());

    // Make sure the dimensionality is correct
    size_t dim = 0;
    if (isRowVec(name))
        dim = matvarRead->dims[1];
    else if (isColVec(name))
        dim = matvarRead->dims[0];
    else
        return false;

    // Assign the actual value to our return variable
    value.resize(dim);
    if (matvarRead->data_type == MAT_T_SINGLE)
    {
        for (int x = 0; x < dim; x++)
            value(x) = ((float*)matvarRead->data)[x];
    }
    else if (matvarRead->data_type == MAT_T_DOUBLE)
    {
        for (int x = 0; x < dim; x++)
            value(x) = ((double*)matvarRead->data)[x];
    }

    Mat_VarFree(matvarRead);
    return true;
}


bool VclMatio::readEigenVector2dNamed(const std::string& name, std::vector<Eigen::VectorXf>& value)
{
    value.clear();
    if (!matFileOpen(m_matfp))
        return false;

    // Make sure the variable exists, is floating point, and is a 2d matrix
    if (!isFloat(name))
        return false;
    if (!isMatrix2d(name))
        return false;

    // Read the data into memory
    Mat_Rewind(m_matfp);
    matvar_t* matvarRead = Mat_VarRead(m_matfp, name.c_str());

    // Assign the actual value to our return variable
    const size_t dimX = matvarRead->dims[0];
    const size_t dimY = matvarRead->dims[1];
    // Allocate space
    value.resize(dimY);
    for (size_t y = 0; y < dimY; ++y)
        value[y].resize(dimX);
    if (matvarRead->data_type == MAT_T_SINGLE)
    {
        for (int x = 0; x < dimX; x++)
        {
            for (int y = 0; y < dimY; y++)
            {
                (value[y])(x) = ((float*)matvarRead->data)[x + y*dimX];
            }
        }
    }
    else if (matvarRead->data_type == MAT_T_DOUBLE)
    {
        for (int x = 0; x < dimX; x++)
        {
            for (int y = 0; y < dimY; y++)
            {
                (value[y])(x) = ((double*)matvarRead->data)[x + y*dimX];
            }
        }
    }

    Mat_VarFree(matvarRead);
    return true;
}


bool VclMatio::readEigenVector2dNamed(const std::string& name, std::vector<Eigen::VectorXd>& value)
{
    value.clear();
    if (!matFileOpen(m_matfp))
        return false;

    // Make sure the variable exists, is floating point, and is a 2d matrix
    if (!isFloat(name))
        return false;
    if (!isMatrix2d(name))
        return false;

    // Read the data into memory
    Mat_Rewind(m_matfp);
    matvar_t* matvarRead = Mat_VarRead(m_matfp, name.c_str());

    // Assign the actual value to our return variable
    const size_t dimX = matvarRead->dims[0];
    const size_t dimY = matvarRead->dims[1];
    // Allocate space
    value.resize(dimY);
    for (size_t y = 0; y < dimY; ++y)
        value[y].resize(dimX);
    if (matvarRead->data_type == MAT_T_SINGLE)
    {
        for (int x = 0; x < dimX; x++)
        {
            for (int y = 0; y < dimY; y++)
            {
                (value[y])(x) = ((float*)matvarRead->data)[x + y*dimX];
            }
        }
    }
    else if (matvarRead->data_type == MAT_T_DOUBLE)
    {
        for (int x = 0; x < dimX; x++)
        {
            for (int y = 0; y < dimY; y++)
            {
                (value[y])(x) = ((double*)matvarRead->data)[x + y*dimX];
            }
        }
    }

    Mat_VarFree(matvarRead);
    return true;
}


bool VclMatio::readEigenMatrix2dNamed(const std::string& name, Eigen::MatrixXf& value)
{
    value.resize(0,0);
    if (!matFileOpen(m_matfp))
        return false;

    // Make sure the variable exists, is floating point, and is a 2d matrix
    if (!isFloat(name))
        return false;
    if (!isMatrix2d(name))
        return false;
    
    // Read the data into memory
    Mat_Rewind(m_matfp);    
    matvar_t* matvarRead = Mat_VarRead(m_matfp, name.c_str());
    
    // Assign the actual value to our return variable
    const size_t dimX = matvarRead->dims[0];
    const size_t dimY = matvarRead->dims[1];
    value.resize(dimX, dimY);
    if (matvarRead->data_type == MAT_T_SINGLE)
    {
        for (int x = 0; x < dimX; x++)
        {
            for (int y = 0; y < dimY; y++)
            {
                value(x,y) = ((float*)matvarRead->data)[x + y*dimX];
            }
        }
    }
    else if (matvarRead->data_type == MAT_T_DOUBLE)
    {
        for (int x = 0; x < dimX; x++)
        {
            for (int y = 0; y < dimY; y++)
            {
                 value(x,y) = ((double*)matvarRead->data)[x + y*dimX];
            }
        }
    }

    Mat_VarFree(matvarRead);
    return true;
}


bool VclMatio::readEigenMatrix2dNamed(const std::string& name, Eigen::MatrixXd& value)
{
    value.resize(0,0);
    if (!matFileOpen(m_matfp))
        return false;

    // Make sure the variable exists, is floating point, and is a 2d matrix
    if (!isFloat(name))
        return false;
    if (!isMatrix2d(name))
        return false;
    
    // Read the data into memory
    Mat_Rewind(m_matfp);
    matvar_t* matvarRead = Mat_VarRead(m_matfp, name.c_str());
    
    // Assign the actual value to our return variable
    const size_t dimX = matvarRead->dims[0];
    const size_t dimY = matvarRead->dims[1];
    value.resize(dimX, dimY);
    if (matvarRead->data_type == MAT_T_SINGLE)
    {
        for (int x = 0; x < dimX; x++)
        {
            for (int y = 0; y < dimY; y++)
            {
                value(x,y) = ((float*)matvarRead->data)[x + y*dimX];
            }
        }
    }
    else if (matvarRead->data_type == MAT_T_DOUBLE)
    {
        for (int x = 0; x < dimX; x++)
        {
            for (int y = 0; y < dimY; y++)
            {
                 value(x,y) = ((double*)matvarRead->data)[x + y*dimX];
            }
        }
    }

    Mat_VarFree(matvarRead);
    return true;
}


bool VclMatio::readEigenMatrix3dNamed(const std::string& name, std::vector<Eigen::MatrixXf>& value)
{
    value.clear();
    if (!matFileOpen(m_matfp))
        return false;

    // Make sure the variable exists, is floating point, and is a 3d matrix
    if (!isFloat(name))
        return false;
    if (!isMatrix3d(name))
        return false;
    
    // Read the data into memory
    Mat_Rewind(m_matfp);
    matvar_t* matvarRead = Mat_VarRead(m_matfp, name.c_str());
   
    const size_t dimX = matvarRead->dims[0];
    const size_t dimY = matvarRead->dims[1];
    const size_t dimZ = matvarRead->dims[2];

    // Allocate space
    value.resize(dimZ);
    for (size_t i = 0; i < dimZ; ++i)
        value[i].resize(dimX, dimY);

    // Assign the actual value to our return variable
    if (matvarRead->data_type == MAT_T_SINGLE)
    {
        for (size_t z = 0; z < dimZ; z++)
        {
            size_t iIdx = z*dimX*dimY;
            for (size_t x = 0; x < dimX; x++)
                for (size_t y = 0; y < dimY; y++)
                    value[z](x, y) = ((float*)matvarRead->data)[x + y*dimX + iIdx];
        }
    }
    else if (matvarRead->data_type == MAT_T_DOUBLE)
    {
        for (size_t z = 0; z < dimZ; z++)
        {
            size_t iIdx = z*dimX*dimY;
            for (size_t x = 0; x < dimX; x++)
                for (size_t y = 0; y < dimY; y++)
                    value[z](x, y) = ((double*)matvarRead->data)[x + y*dimX + iIdx];
        }
    }

    Mat_VarFree(matvarRead);
    return true;
}


bool VclMatio::readEigenMatrix3dNamed(const std::string& name, std::vector<Eigen::MatrixXd>& value)
{
    value.clear();
    if (!matFileOpen(m_matfp))
        return false;

    // Make sure the variable exists, is floating point, and is a 3d matrix
    if (!isFloat(name))
        return false;
    if (!isMatrix3d(name))
        return false;

    // Read the data into memory
    Mat_Rewind(m_matfp);
    matvar_t* matvarRead = Mat_VarRead(m_matfp, name.c_str());

    const size_t dimX = matvarRead->dims[0];
    const size_t dimY = matvarRead->dims[1];
    const size_t dimZ = matvarRead->dims[2];

    // Allocate space
    value.resize(dimZ);
    for (size_t i = 0; i < dimZ; ++i)
        value[i].resize(dimX, dimY);

    // Assign the actual value to our return variable
    if (matvarRead->data_type == MAT_T_SINGLE)
    {
        for (size_t z = 0; z < dimZ; z++)
        {
            size_t iIdx = z*dimX*dimY;
            for (size_t x = 0; x < dimX; x++)
                for (size_t y = 0; y < dimY; y++)
                    value[z](x, y) = ((float*)matvarRead->data)[x + y*dimX + iIdx];
        }
    }
    else if (matvarRead->data_type == MAT_T_DOUBLE)
    {
        for (size_t z = 0; z < dimZ; z++)
        {
            size_t iIdx = z*dimX*dimY;
            for (size_t x = 0; x < dimX; x++)
                for (size_t y = 0; y < dimY; y++)
                    value[z](x, y) = ((double*)matvarRead->data)[x + y*dimX + iIdx];
        }
    }

    Mat_VarFree(matvarRead);
    return true;
}


// Writing
bool VclMatio::openForWriting(const std::string& filename)
{
    close();
    m_matfp = Mat_CreateVer(filename.c_str(), NULL, MAT_FT_MAT73);
    if (m_matfp == NULL)
    {
        std::cerr << "Unable to create mat file: " << filename << std::endl;
        return false;
    }
    return true;
}


bool VclMatio::writeValueNamed(const std::string& name, const float& value)
{
    matvar_t* matvar;
    size_t dims[2] = {1, 1};
    matvar = Mat_VarCreate(name.c_str(), MAT_C_DOUBLE, MAT_T_SINGLE, 2, dims, (void*)&value, MAT_F_DONT_COPY_DATA);
    if (matvar == NULL)
        return false;
    Mat_VarWrite(m_matfp, matvar, MAT_COMPRESSION_NONE);
    Mat_VarFree(matvar);
    return true;
}


bool VclMatio::writeValueNamed(const std::string& name, const double& value)
{
    matvar_t* matvar;
    size_t dims[2] = {1, 1};
    matvar = Mat_VarCreate(name.c_str(), MAT_C_DOUBLE, MAT_T_DOUBLE, 2, dims, (void*)&value, MAT_F_DONT_COPY_DATA);
    if (matvar == NULL)
        return false;
    Mat_VarWrite(m_matfp, matvar, MAT_COMPRESSION_NONE);
    Mat_VarFree(matvar);
    return true;
}


bool VclMatio::writeEigenVectorNamed(const std::string& name, const Eigen::VectorXf& value)
{
    matvar_t* matvar;
    size_t dims[2] = { value.rows(), 1 };
    matvar = Mat_VarCreate(name.c_str(), MAT_C_DOUBLE, MAT_T_SINGLE, 2, dims, (void*)value.data(), MAT_F_DONT_COPY_DATA);
    if (matvar == NULL)
        return false;
    Mat_VarWrite(m_matfp, matvar, MAT_COMPRESSION_NONE);
    Mat_VarFree(matvar);
    return true;
}


bool VclMatio::writeEigenVectorNamed(const std::string& name, const Eigen::VectorXd& value)
{
    matvar_t* matvar;
    size_t dims[2] = { value.rows(), 1 };
    matvar = Mat_VarCreate(name.c_str(), MAT_C_DOUBLE, MAT_T_DOUBLE, 2, dims, (void*)value.data(), MAT_F_DONT_COPY_DATA);
    if (matvar == NULL)
        return false;
    Mat_VarWrite(m_matfp, matvar, MAT_COMPRESSION_NONE);
    Mat_VarFree(matvar);
    return true;
}


bool VclMatio::writeEigenVector2dNamed(const std::string& name, const std::vector<Eigen::VectorXf>& value)
{
    matvar_t* matvar;
    // Copy data to matrix
    Eigen::MatrixXf tmp;
    tmp.resize(value[0].rows(), value.size());
    for (size_t i = 0; i < tmp.rows(); ++i)
        for (size_t j = 0; j < tmp.cols(); ++j)
            tmp(i, j) = (value[j])(i);
    size_t dims[2] = { tmp.rows(), tmp.cols() };
    matvar = Mat_VarCreate(name.c_str(), MAT_C_DOUBLE, MAT_T_SINGLE, 2, dims, (void*)tmp.data(), MAT_F_DONT_COPY_DATA);
    if (matvar == NULL)
        return false;
    Mat_VarWrite(m_matfp, matvar, MAT_COMPRESSION_NONE);
    Mat_VarFree(matvar);
    return true;
}


bool VclMatio::writeEigenVector2dNamed(const std::string& name, const std::vector<Eigen::VectorXd>& value)
{
    matvar_t* matvar;
    // Copy data to matrix
    Eigen::MatrixXd tmp;
    tmp.resize(value[0].rows(), value.size());
    for (size_t i = 0; i < tmp.rows(); ++i)
        for (size_t j = 0; j < tmp.cols(); ++j)
            tmp(i, j) = (value[j])(i);
    size_t dims[2] = { tmp.rows(), tmp.cols() };
    matvar = Mat_VarCreate(name.c_str(), MAT_C_DOUBLE, MAT_T_DOUBLE, 2, dims, (void*)tmp.data(), MAT_F_DONT_COPY_DATA);
    if (matvar == NULL)
        return false;
    Mat_VarWrite(m_matfp, matvar, MAT_COMPRESSION_NONE);
    Mat_VarFree(matvar);
    return true;
}


bool VclMatio::writeEigenMatrix2dNamed(const std::string& name, const Eigen::MatrixXf& value)
{
    matvar_t* matvar;
    size_t dims[2] = {value.rows(), value.cols()};
    matvar = Mat_VarCreate(name.c_str(), MAT_C_DOUBLE, MAT_T_SINGLE, 2, dims, (void*)value.data(), MAT_F_DONT_COPY_DATA);
    if (matvar == NULL)
        return false;
    Mat_VarWrite(m_matfp, matvar, MAT_COMPRESSION_NONE);
    Mat_VarFree(matvar);
    return true;
}


bool VclMatio::writeEigenMatrix2dNamed(const std::string& name, const Eigen::MatrixXd& value)
{
    matvar_t* matvar;
    size_t dims[2] = {value.rows(), value.cols()};
    matvar = Mat_VarCreate(name.c_str(), MAT_C_DOUBLE, MAT_T_DOUBLE, 2, dims, (void*)value.data(), MAT_F_DONT_COPY_DATA);
    if (matvar == NULL)
        return false;
    Mat_VarWrite(m_matfp, matvar, MAT_COMPRESSION_NONE);
    Mat_VarFree(matvar);
    return true;
}


bool VclMatio::writeEigenMatrix3dNamed(const std::string& name, const std::vector<Eigen::MatrixXf>& value)
{
    matvar_t* matvar;
    size_t dims[3] = {value[0].rows(), value[0].cols(), value.size()};

    // Makes a copy of the data before writing it to the file.  Not good (see next function for example of how to maybe do it better).
    std::vector<float> allData;
    allData.resize(value[0].rows()*value[0].cols()*value.size());
    for (size_t i = 0; i < value.size(); i++)
        for (size_t j = 0; j < value[i].cols(); j++)
            for (size_t k = 0; k < value[i].rows(); k++)
                allData[k + j*value[i].rows() + i*value[i].cols()*value[i].rows()] = value[i](k, j);
    matvar = Mat_VarCreate(name.c_str(), MAT_C_DOUBLE, MAT_T_SINGLE, 3, dims, (void*)allData.data(), 0);
    if (matvar == NULL)
        return false;
    Mat_VarWrite(m_matfp, matvar, MAT_COMPRESSION_NONE);
    Mat_VarFree(matvar);
    return true;
}


bool VclMatio::writeEigenMatrix3dNamed(const std::string& name, const std::vector<Eigen::MatrixXd>& value)
{
    matvar_t* matvar;
    size_t dims[3] = { value[0].rows(), value[0].cols(), value.size() };

    // Makes a copy of the data before writing it to the file.  Not good (see next function for example of how to maybe do it better).
    std::vector<double> allData;
    allData.resize(value[0].rows()*value[0].cols()*value.size());
    for (size_t i = 0; i < value.size(); i++)
        for (size_t j = 0; j < value[i].cols(); j++)
            for (size_t k = 0; k < value[i].rows(); k++)
                allData[k + j*value[i].rows() + i*value[i].cols()*value[i].rows()] = value[i](k, j);
    matvar = Mat_VarCreate(name.c_str(), MAT_C_DOUBLE, MAT_T_DOUBLE, 3, dims, (void*)allData.data(), 0);
    if (matvar == NULL)
        return false;
    Mat_VarWrite(m_matfp, matvar, MAT_COMPRESSION_NONE);
    Mat_VarFree(matvar);
    return true;
    
    /*
    // Attempts to write the data without copying.  Doesn't work yet...
    // http://groups.inf.ed.ac.uk/vision/MAJECKA/Detector/Resources/libraries/matio-1.3.3/test/test_mat.c
    matvar = Mat_VarCreate(name.c_str(), MAT_C_DOUBLE, MAT_T_DOUBLE, 3, dims, NULL, 0);
    Mat_VarWriteInfo(m_matfp, matvar);
    for (int i = 0; i < value.size(); i++)
    {
        int start[2] = {0,0};
        int stride[2] = {1,1};
        int edge[2] = {3,5};
        Mat_VarWriteData(m_matfp, matvar, (void*)value[i].data(), start, stride, edge);
    }
    */
    
    Mat_VarFree(matvar);
    return true;
}


// Info
std::vector<std::string> VclMatio::variableNames()
{
    std::vector<std::string> vNames;
    
    matvar_t* matvar;
    Mat_Rewind(m_matfp);
    while ((matvar = Mat_VarReadNextInfo(m_matfp)) != NULL) 
    {
        vNames.push_back(matvar->name);
        Mat_VarFree(matvar);
        matvar = NULL;
    }
    return vNames;
}


bool VclMatio::isValue(const std::string& name)
{
    if (!matFileOpen(m_matfp))
        return false;
    
    // Make sure the variable exists and is the proper rank
    bool good = true;
    Mat_Rewind(m_matfp);
    matvar_t* matvar = Mat_VarReadInfo(m_matfp, name.c_str());
    if (matvar == NULL)
    {
        std::cerr << "Variable named '" << name << "' does not exist." << std::endl;
        good = false;
    }
    if (matvar->rank == 2 && (matvar->dims[0] != 1 || matvar->dims[1] != 1))
        good = false;
    if (matvar->rank >= 3)
        good = false;
    
    Mat_VarFree(matvar);
    return good;
}


bool VclMatio::isFloat(const std::string& name)
{
    if (!matFileOpen(m_matfp))
        return false;

    // Make sure the variable exists and is floating point (or unknown)
    bool good = true;
    Mat_Rewind(m_matfp);
    matvar_t* matvar = Mat_VarReadInfo(m_matfp, name.c_str());
    if (matvar == NULL)
    {
        std::cerr << "Variable named '" << name << "' does not exist." << std::endl;
        good = false;
    }
    if (matvar->data_type != MAT_T_DOUBLE && matvar->data_type != MAT_T_SINGLE && matvar->data_type != MAT_T_UNKNOWN)
    {
        std::cerr << "Variable type for '" << name << "' is not floating point." << std::endl;
        good = false;
    }
    
    Mat_VarFree(matvar);
    return good;
}


bool VclMatio::isRowVec(const std::string& name)
{
    if (!matFileOpen(m_matfp))
        return false;

    // Make sure the variable exists and is the proper rank
    bool good = true;
    Mat_Rewind(m_matfp);
    matvar_t* matvar = Mat_VarReadInfo(m_matfp, name.c_str());
    if (matvar == NULL)
    {
        std::cerr << "Variable named '" << name << "' does not exist." << std::endl;
        good = false;
    }
    if (matvar->rank != 2 || matvar->dims[0] != 1)
        good = false;

    Mat_VarFree(matvar);
    return good;
}


bool VclMatio::isColVec(const std::string& name)
{
    if (!matFileOpen(m_matfp))
        return false;

    // Make sure the variable exists and is the proper rank
    bool good = true;
    Mat_Rewind(m_matfp);
    matvar_t* matvar = Mat_VarReadInfo(m_matfp, name.c_str());
    if (matvar == NULL)
    {
        std::cerr << "Variable named '" << name << "' does not exist." << std::endl;
        good = false;
    }
    if (matvar->rank != 2 || matvar->dims[1] != 1)
        good = false;

    Mat_VarFree(matvar);
    return good;
}


bool VclMatio::isMatrix2d(const std::string& name)
{
    if (!matFileOpen(m_matfp))
        return false;
    
    // Make sure the variable exists and is the proper rank
    bool good = true;
    Mat_Rewind(m_matfp);
    matvar_t* matvar = Mat_VarReadInfo(m_matfp, name.c_str());
    if (matvar == NULL)
    {
        std::cerr << "Variable named '" << name << "' does not exist." << std::endl;
        good = false;
    }
    if (matvar->rank == 2 && matvar->dims[0] == 1 && matvar->dims[1] == 1)
        good = false;
    if (matvar->rank >= 3)
        good = false;
    
    Mat_VarFree(matvar);
    return good;
}


bool VclMatio::isMatrix3d(const std::string& name)
{
    if (!matFileOpen(m_matfp))
        return false;
    
    // Make sure the variable exists and is the proper rank
    bool good = true;
    Mat_Rewind(m_matfp);
    matvar_t* matvar = Mat_VarReadInfo(m_matfp, name.c_str());
    if (matvar == NULL)
    {
        std::cerr << "Variable named '" << name << "' does not exist." << std::endl;
        good = false;
    }
    if (matvar->rank < 3)
        good = false;
    
    Mat_VarFree(matvar);
    return good;
}


// Closing
bool VclMatio::close()
{
    Mat_Close(m_matfp);
    m_matfp = NULL;
    return true;
}


// Private functions
bool VclMatio::matFileOpen(const mat_t* mat)
{
    if (mat == NULL)
    {
        std::cerr << "No mat file opened for reading." << std::endl;
        return false;
    }
    return true;
}


}
