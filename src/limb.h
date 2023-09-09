#pragma once
#include <libmath/matrix.h>
#include <libmath/solver/us/unlinearsolver.h>
#include <libmath/solver/us/secant.h>
#include <vector>
#include <defines.h>
#include <string>
#include <robo_exception.h>
#include <functional>

namespace robo
{
    /// @brief Precision
#define ROBO_DOUBLE_PREC
#ifdef ROBO_DOUBLE_PREC
    typedef double real;
#else
    typedef float real;
#endif
}

namespace robo
{
    class Limb
    {
    private:
        /// @brief Current X coordinates of vertexes
        math::Matrix<real> x_curr_;

        /// @brief Current Y coordinates of vertexes
        math::Matrix<real> y_curr_;

        /// @brief Current Z coordinates of vertexes
        math::Matrix<real> z_curr_;

        /// @brief Target X coordinates of vertexes
        math::Matrix<real> x_tar_;

        /// @brief Target Y coordinates of vertexes
        math::Matrix<real> y_tar_;

        /// @brief Target Z coordinates of vertexes
        math::Matrix<real> z_tar_;

        /// @brief Length of limb parts
        math::Matrix<real> L_;

        /// @brief Number of vertexes
        size_t vertex_num_;

        /// @brief Limb counter
        static size_t cnt_;

        /// @brief Limb name
        std::string name_;

        /// @brief Functions, represent system of non-linear equations for computing vertexes coordinates
        std::vector<std::function<real(const math::Matrix<real> &)>> Eq_ =
        {
            [](const math::Matrix<real> &x)
            {
                return (x(0, 0) + x(1, 0) + x(2, 0) - x(3, 0));
            },
            [](const math::Matrix<real> &y)
            {
                return (y(0, 0) + y(1, 0) + y(2, 0) - y(3, 0));
            },
            [](const math::Matrix<real> &z)
            {
                return (z(0, 0) + z(1, 0) + z(2, 0) - z(3, 0));
            }
        };

        /// @brief Solver
        math::UnlinearSolver<real> *solver_ = new math::Secant<double>();

    public:
        /// @brief Default constructor
        Limb(){

        };

        /**
         * @brief Constructor for robot limb
         * @details Links number in limb is 3
         * @param L: Column vector of length of limb parts
         * @param name: Limb name. Default name is "Limb_[idx]""
         */
        Limb(
            const math::Matrix<real> &L,
            const std::string &name = "") : L_(L), vertex_num_(L.rows()), name_(name),
                                            x_curr_(math::Matrix<real>(L.rows() + 1, 1)),
                                            y_curr_(math::Matrix<real>(L.rows() + 1, 1)),
                                            z_curr_(math::Matrix<real>(L.rows() + 1, 1)),
                                            x_tar_(math::Matrix<real>(L.rows() + 1, 1)),
                                            y_tar_(math::Matrix<real>(L.rows() + 1, 1)),
                                            z_tar_(math::Matrix<real>(L.rows() + 1, 1))
        {
            if (L_.rows() != 3)
            {
                throw(ExceptionInvalidValue("Limb: links amount must be 3!"));
            }
            if (L_.cols() != 1)
            {
                throw(ExceptionInvalidValue("Limb: L must be column vector!"));
            }

            if (name_ == "")
            {
                name_ = "Limb_" + std::to_string(cnt_);
            }
            ++cnt_;
        };

        /**
         * @brief Constructor for robot limb
         * @details Links number in limb is 3
         * @param L: Column vector of length of limb parts
         * @param x: Column vector of initial x vertex coordinates
         * @param y: Column vector of initial y vertex coordinates
         * @param z: Column vector of initial z vertex coordinates
         * @param name: Limb name. Default name is "Limb_[idx]""
         */
        Limb(
            const math::Matrix<real> &L,
            const math::Matrix<real> &x,
            const math::Matrix<real> &y,
            const math::Matrix<real> &z,
            const std::string &name = "") : L_(L), vertex_num_(L.rows()), name_(name),
                                            x_curr_(x),
                                            y_curr_(y),
                                            z_curr_(z),
                                            x_tar_(math::Matrix<real>(L.rows() + 1, 1)),
                                            y_tar_(math::Matrix<real>(L.rows() + 1, 1)),
                                            z_tar_(math::Matrix<real>(L.rows() + 1, 1))
        {
            if ((x_curr_.rows() != y_curr_.rows()) || (x_curr_.rows() != z_curr_.rows()))
            {
                throw(ExceptionInvalidValue("Limb: initial coordinates vectors (x,y,z) must be the same length!"));
            }
            if (L_.rows() != 3)
            {
                throw(ExceptionInvalidValue("Limb: links amount must be 3!"));
            }
            if (L_.cols() != 1)
            {
                throw(ExceptionInvalidValue("Limb: L must be column vector!"));
            }
            if ((x_curr_.rows() != L_.rows() + 1) ||
                (y_curr_.rows() != L_.rows() + 1) ||
                (z_curr_.rows() != L_.rows() + 1))
            {
                throw(ExceptionInvalidValue("Limb: incorrect current coordinates length, 4 points must be set!"));
            }
            if ((x_curr_.rows() != 1) ||
                (y_curr_.rows() != 1) ||
                (z_curr_.rows() != 1))
            {
                throw(ExceptionInvalidValue("Limb: current coordinates (x,y,z) must be column vectors!"));
            }

            if (name_ == "")
            {
                name_ = "Limb_" + std::to_string(cnt_);
            }
            ++cnt_;
        }

        /// @brief Destructor
        ~Limb()
        {
            delete solver_;
        };
    };
}