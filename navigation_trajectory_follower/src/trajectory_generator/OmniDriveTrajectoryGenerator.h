/******************************************************************************
 * Copyright (c) 2011
 * GPS GmbH
 *
 * Author:
 * Alexey Zakharov
 *
 *
 * This software is published under a dual-license: GNU Lesser General Public
 * License LGPL 2.1 and BSD license. The dual-license implies that users of this
 * code may choose which terms they prefer.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * * Neither the name of GPS GmbH nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License LGPL as
 * published by the Free Software Foundation, either version 2.1 of the
 * License, or (at your option) any later version or the BSD license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License LGPL and the BSD license for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License LGPL and BSD license along with this program.
 *
 ******************************************************************************/

#ifndef OMNIDRIVETRAJECTORYGENERATOR_H
#define	OMNIDRIVETRAJECTORYGENERATOR_H

#include "ITrajectoryGenerator.h"

namespace KDL {
    class Trajectory_Composite;
    class Path_Composite;
}

class FrameWithId;

/**
 * @brief Implementation of the interface class for trajectory generation for omnidrive mobile base.
 */

class OmniDriveTrajectoryGenerator : public ITrajectoryGenerator {
public:

    /**
     * @brief Constructor.
     */
    OmniDriveTrajectoryGenerator();

    /**
     * @brief Copy constructor.
     */
    OmniDriveTrajectoryGenerator(const OmniDriveTrajectoryGenerator& orig);

    /**
     * @brief Destructor.
     */
    virtual ~OmniDriveTrajectoryGenerator();

    /**
     * @brief Converts vector of FrameWithId to KDL::Path_Composite.
     * @param[in] path - input path as a vector of FrameWithId.
     * @param[out] pathComposite - converted path to KDL::Path_Composite.
     */
    void computePathComposite(const std::vector<FrameWithId>& path, KDL::Path_Composite& pathComposite);

    /**
     * @brief Calculates rotation.
     * @param[in] path - input path as a vector of FrameWithId
     * @param[out] pathWithRotation - output path with orientation in each path point.
     */
    void interpolateRotation(const std::vector<FrameWithId>& path, std::vector<FrameWithId>& pathWithRotation);

    /**
     * @brief Implementation if an interface for computing a trajectory.
     * @param[in] path - input path, KDL::Path_Composite
     * @param[out] trajectory - computed trajectory
     */
    void computeTrajectroy(const KDL::Path_Composite& path, KDL::Trajectory_Composite& trajectory);

    /**
     * @brief Computes trajectory given a path as a vector of FrameWithId.
     * @param[in] path - input path, a vector of FrameWithId
     * @param[out] trajectory - computed trajectory
     */
    void computeTrajectroy(const std::vector<FrameWithId> path, KDL::Trajectory_Composite& trajectory);

private:

};

#endif	/* OMNIDRIVETRAJECTORYGENERATOR_H */

