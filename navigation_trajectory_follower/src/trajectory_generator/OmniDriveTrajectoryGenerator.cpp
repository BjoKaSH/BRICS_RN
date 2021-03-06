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

#include "OmniDriveTrajectoryGenerator.h"

#include <navigation_trajectory_common/FrameWithId.h>
#include <navigation_trajectory_common/TwistWithId.h>
#include <navigation_trajectory_common/Utilities.h>

#include <kdl/velocityprofile_trap.hpp>
#include <kdl/rotational_interpolation_sa.hpp>
#include <kdl/path_line.hpp>
#include <kdl/path_composite.hpp>
#include <kdl/trajectory_segment.hpp>
#include <kdl/trajectory_composite.hpp>
#include <kdl/path.hpp>
#include <iostream>

#include <cmath>

OmniDriveTrajectoryGenerator::OmniDriveTrajectoryGenerator() {
}

OmniDriveTrajectoryGenerator::OmniDriveTrajectoryGenerator(const OmniDriveTrajectoryGenerator& orig) {
}

OmniDriveTrajectoryGenerator::~OmniDriveTrajectoryGenerator() {
}

void OmniDriveTrajectoryGenerator::computePathComposite(const std::vector<FrameWithId>& path, KDL::Path_Composite& pathComposite) {
    std::vector<FrameWithId>::const_iterator it;

    if (path.size() > 1) {

        FrameWithId p1 = path.front();
        FrameWithId p2;

        for (it = path.begin() + 1; it != path.end(); ++it) {
            p2 = *it;
            KDL::Frame f1, f2;
            f1 = p1.getFrame();
            f2 = p2.getFrame();

            KDL::RotationalInterpolation_SingleAxis* rot = new KDL::RotationalInterpolation_SingleAxis();
            //   rot->Vel(0.1,0.01);
            //   rot->Acc(0.1,0.01,0.001);
            KDL::Path_Line* pathLine = new KDL::Path_Line(f1, f2, rot, 0.001);
            pathComposite.Add(pathLine);

            p1 = p2;
        }

    }
}

void OmniDriveTrajectoryGenerator::interpolateRotation(const std::vector<FrameWithId>& path, std::vector<FrameWithId>& pathWithRotation) {

    if (path.size() <= 1)
        return;

    pathWithRotation.clear();

    double r, p, y;
    path.front().getFrame().M.GetRPY(r, p, y);
    double start = y;

    path.back().getFrame().M.GetRPY(r, p, y);
    double end = y;
    double step = utilities::getShortestAngle(end, start) / (path.size() - 1);


    std::vector<FrameWithId>::const_iterator it;
    for (it = path.begin(); it != path.end(); ++it) {

        KDL::Frame f;
        std::string id = it->id;

        f.p = it->getFrame().p;
        f.M = KDL::Rotation::RPY(0, 0, start);

        pathWithRotation.push_back(FrameWithId(f, id));

        start = start + step;
    }

    // pathWithRotation.front() = path.front();
    // pathWithRotation.back() = path.back();
}

void OmniDriveTrajectoryGenerator::computeTrajectroy(const KDL::Path_Composite& path, KDL::Trajectory_Composite& trajectory) {

    const double maxVel = 1.0;
    const double maxAcc = 0.1;

    KDL::VelocityProfile_Trap* velocityProfile = new KDL::VelocityProfile_Trap(maxVel, maxAcc);
    KDL::Path* copyPath = const_cast<KDL::Path_Composite&> (path).Clone(); // Why KDL has no const version of Clone method?
    // They force me to do this!

    velocityProfile->SetProfile(0, copyPath->PathLength());

    KDL::Trajectory_Segment* trajectorySegment = new KDL::Trajectory_Segment(copyPath, velocityProfile);
    trajectory.Add(trajectorySegment);

}

void OmniDriveTrajectoryGenerator::computeTrajectroy(const std::vector<FrameWithId> path, KDL::Trajectory_Composite& trajectory) {
    std::vector<FrameWithId> pathWithRotation;
    interpolateRotation(path, pathWithRotation);

    KDL::Path_Composite pathComposite;
    computePathComposite(pathWithRotation, pathComposite);
    computeTrajectroy(pathComposite, trajectory);
}
