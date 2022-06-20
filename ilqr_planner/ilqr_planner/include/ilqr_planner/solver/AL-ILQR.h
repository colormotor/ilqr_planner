/**
    This package provides a C++ iLQR library that comes with its python bindings.
    It allows you to solve iLQR optimization problem on any robot as long as you
    provide an [URDF file](http://wiki.ros.org/urdf/Tutorials) describing the
    kinematics chain of the robot. For debugging purposes it also provide a 2D
    planar robots class that you can use. You can also apply a spatial
    transformation to compute robot task space information in the base frame of
    your choice (e.g. object frame).

    Copyright (c) 2022 Idiap Research Institute, http://www.idiap.ch/
    Written by Jeremy Maceiras <jeremy.maceiras@idiap.ch>

    This file is part of ilqr_planner.

    ilqr_planner is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 3 as
    published by the Free Software Foundation.

    ilqr_planner is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ilqr_planner. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <eigen3/Eigen/Dense>
#include <memory>
#include <tuple>
#include <vector>
#include "ilqr_planner/system/System.h"
#include "ilqr_planner/utils/CallbackMessage.h"

namespace ilqr_planner {
namespace solver {

struct Constraint {
    Eigen::MatrixXd A;
    Eigen::VectorXd b;
};

class AL_ILQR {
    /**
     * Iterative LQR with Augmented Lagrangian
     */
public:
    /**
     *  Constructor
     *      s <const std::shared_ptr<sys::System> &> pointer to the abstraction of the object that we want to control
     *      inequality: Set of inequality constraints
     *      initLambda: Initial value of Lagrange multipliers
     */
    AL_ILQR(const std::shared_ptr<sys::System>& s, const std::vector<Constraint>& inequality, const std::vector<Eigen::VectorXd>& initLambda);

    /**
     * Solve the AL-ILQR problem, return X,U
     *      U0: Initial control commands (0 a good choice)
     *      nb_iter: Number of iteration
     *      horizon: T of the system
     *      lag_update_step: The Lagrange multipliers update frequency
     *      penalty: "weight" of the constraints
     *      scaling_factor: If you want evolve the weight of constraints
     *      line_search: If true line search is performed @ each iteration
     *      early_stop: If true stop optimization when cost is not moving anymore.
     */
    std::tuple<std::vector<Eigen::VectorXd>, std::vector<Eigen::VectorXd>, std::vector<Eigen::VectorXd>> solve(const std::vector<Eigen::VectorXd>& U0,
                                                                                                               int nb_iter,
                                                                                                               int lag_update_step,
                                                                                                               double penalty,
                                                                                                               double scaling_factor,
                                                                                                               bool line_search = true,
                                                                                                               bool early_stop = false,
                                                                                                               CallBackMessage* cb = nullptr);

private:
    /**
     * Compute the loss @ time k with respect of the constraitns
     */
    Eigen::VectorXd augmentedLossK(const Eigen::VectorXd& xk, const Eigen::VectorXd& uk, int k, const Eigen::VectorXd& lambdak, const Eigen::VectorXd& Ck, const Eigen::MatrixXd& Ik);

    /**
     * Compute the constraints at time k
     */
    std::tuple<Eigen::MatrixXd, Eigen::VectorXd> constraints(const Eigen::VectorXd& xk, const Eigen::VectorXd& uk, int k);

    std::shared_ptr<sys::System> s;
    std::vector<Constraint> inequality;
    std::vector<Eigen::VectorXd> multipliers;
};
}  // namespace solver
}  // namespace ilqr_planner