// SPDX-FileCopyrightText: 2023 Idiap Research Institute <contact@idiap.ch>
//
// SPDX-FileContributor: Jeremy Maceiras  <jeremy.maceiras@idiap.ch>
//
// SPDX-License-Identifier: GPL-3.0-only

#include "ilqr_planner/system/SpacetimeKeypoint.h"

namespace ilqr_planner {
namespace sys {

Eigen::VectorXd SpacetimeKeypoint::getState() const {
    Eigen::VectorXd state = PosOrnKeypoint::getState();
    Eigen::VectorXd state_augmented = Eigen::VectorXd::Zero(state.rows() + 1);
    state_augmented << state, continuous_time_;
    return state_augmented;
}

Eigen::VectorXd SpacetimeKeypoint::diff(const Eigen::VectorXd& state) const {
    Eigen::VectorXd residual = PosOrnKeypoint::diff(state.head(state.rows() - 1));
    Eigen::VectorXd residual_augmented = Eigen::VectorXd::Zero(residual.rows() + 1);
    residual_augmented << residual, continuous_time_ - state(state.rows() - 1);
    return residual_augmented;
}

}  // namespace sys
}  // namespace ilqr_planner
