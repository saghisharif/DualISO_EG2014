//
// VCL header
//

#ifndef __VCL_MATLAB_H__
#define __VCL_MATLAB_H__

#include <string>
#include <vector>

#include <matio.h>
#include <Eigen/Dense>


namespace Vcl
{

//
// NOTE: The addition of the Eigen Vector functions have introduced an ambiguity.
//       It's not the worst thing that could happen, but keep an eye out for the cases where it causes issues.
//

class VclMatio
{
public:
    VclMatio();
    VclMatio(const std::string& filename, bool forReading=true);
    ~VclMatio();
    
    // Reading
    bool openForReading(const std::string& filename);
    bool readValueNamed(const std::string& name, float& value);
    bool readValueNamed(const std::string& name, double& value);
    bool readEigenVectorNamed(const std::string& name, Eigen::VectorXf& value);
    bool readEigenVectorNamed(const std::string& name, Eigen::VectorXd& value);
    bool readEigenVector2dNamed(const std::string& name, std::vector<Eigen::VectorXf>& value);
    bool readEigenVector2dNamed(const std::string& name, std::vector<Eigen::VectorXd>& value);
    bool readEigenMatrix2dNamed(const std::string& name, Eigen::MatrixXf& value);
    bool readEigenMatrix2dNamed(const std::string& name, Eigen::MatrixXd& value);
    bool readEigenMatrix3dNamed(const std::string& name, std::vector<Eigen::MatrixXf>& value);
    bool readEigenMatrix3dNamed(const std::string& name, std::vector<Eigen::MatrixXd>& value);
    
    // Writing
    bool openForWriting(const std::string& filename);
    bool writeValueNamed(const std::string& name, const float& value);
    bool writeValueNamed(const std::string& name, const double& value);
    bool writeEigenVectorNamed(const std::string& name, const Eigen::VectorXf& value);
    bool writeEigenVectorNamed(const std::string& name, const Eigen::VectorXd& value);
    bool writeEigenVector2dNamed(const std::string& name, const std::vector<Eigen::VectorXf>& value);
    bool writeEigenVector2dNamed(const std::string& name, const std::vector<Eigen::VectorXd>& value);
    bool writeEigenMatrix2dNamed(const std::string& name, const Eigen::MatrixXf& value);
    bool writeEigenMatrix2dNamed(const std::string& name, const Eigen::MatrixXd& value);
    bool writeEigenMatrix3dNamed(const std::string& name, const std::vector<Eigen::MatrixXf>& value);
    bool writeEigenMatrix3dNamed(const std::string& name, const std::vector<Eigen::MatrixXd>& value);
    
    // Info
    std::vector<std::string> variableNames();
    bool isValue(const std::string& name);
    bool isFloat(const std::string& name);
    bool isRowVec(const std::string& name);
    bool isColVec(const std::string& name);
    bool isMatrix2d(const std::string& name);
    bool isMatrix3d(const std::string& name);
    
    // Closing
    bool close();
    
private:
    mat_t* m_matfp;
    std::string m_filename;
    
    bool matFileOpen(const mat_t* mat);
};

}

#endif
