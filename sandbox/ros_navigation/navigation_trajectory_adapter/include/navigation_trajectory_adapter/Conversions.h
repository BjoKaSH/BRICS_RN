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

#ifndef CONVERSIONS_H
#define	CONVERSIONS_H

#include <nav_msgs/Odometry.h>
#include <nav_msgs/Path.h>
#include "navigation_trajectory_adapter/FrameWithId.h"
#include "navigation_trajectory_adapter/TwistWithId.h"

namespace conversions {

    inline void odometryRosToFrame(const nav_msgs::Odometry& odometry, FrameWithId& pose) {
        const geometry_msgs::Quaternion& orientation = odometry.pose.pose.orientation;
        const geometry_msgs::Point& position = odometry.pose.pose.position;
        pose.id = odometry.header.frame_id;
        pose.M = KDL::Rotation::Quaternion(orientation.x, orientation.y, orientation.z, orientation.w);
        pose.p = KDL::Vector(position.x, position.y, position.z);
    }

    inline void odometryRosToTwist(const nav_msgs::Odometry& odometry, TwistWithId& twist) {
        const geometry_msgs::Vector3& angular = odometry.twist.twist.angular;
        const geometry_msgs::Vector3& linear = odometry.twist.twist.linear;
        twist.id = odometry.header.frame_id;
        twist.rot = KDL::Vector(angular.x, angular.y, angular.z);
        twist.vel = KDL::Vector(linear.x, linear.y, linear.z);
    }

    inline void poseRosToFrame(const geometry_msgs::Pose& poseRos, const std::string& id, FrameWithId& pose) {
        const geometry_msgs::Quaternion& orientation = poseRos.orientation;
        const geometry_msgs::Point& position = poseRos.position;
        pose.id = id;
        pose.M = KDL::Rotation::Quaternion(orientation.x, orientation.y, orientation.z, orientation.w);
        pose.p = KDL::Vector(position.x, position.y, position.z);
    }

    inline void poseStampedRosToFrame(const geometry_msgs::PoseStamped& poseStamped, FrameWithId& pose) {
        poseRosToFrame(poseStamped.pose, poseStamped.header.frame_id, pose);
    }

    inline void frameToPoseRos(const FrameWithId& pose, geometry_msgs::Pose& poseRos, std::string& id) {
        id = pose.id;
        pose.M.GetQuaternion(poseRos.orientation.x,
                poseRos.orientation.y,
                poseRos.orientation.z,
                poseRos.orientation.w);
        poseRos.position.x = pose.p.x();
        poseRos.position.y = pose.p.y();
        poseRos.position.z = pose.p.z();
    }

    inline void frameToPoseStampedRos(const FrameWithId& pose, geometry_msgs::PoseStamped& poseStamped) {
        frameToPoseRos(pose, poseStamped.pose, poseStamped.header.frame_id);
    }

    inline void pathToPathRos(const std::vector<FrameWithId>& path, std::vector <geometry_msgs::PoseStamped>& pathRos) {
        if (!path.empty()) {
            std::vector<FrameWithId>::const_iterator it = path.begin();
            geometry_msgs::PoseStamped pose;
            while (it != path.end()) {
                frameToPoseStampedRos(*it, pose);
                pathRos.push_back(pose);
                ++it;
            }
        }
    }

    inline void pathToPathRos(const std::vector<FrameWithId>& path, nav_msgs::Path& pathRos) {
        if (!path.empty()) {
            std::vector<FrameWithId>::const_iterator it = path.begin();
            pathRos.header.frame_id = it->id;
            pathToPathRos(path, pathRos.poses);
        }
    }

    inline void pathRosToPath(const std::vector <geometry_msgs::PoseStamped>& pathRos, std::vector<FrameWithId>& path) {
        if (!pathRos.empty()) {
            std::vector <geometry_msgs::PoseStamped>::const_iterator it = pathRos.begin();
            FrameWithId pose;
            while (it != pathRos.end()) {
                poseStampedRosToFrame(*it, pose);
                path.push_back(pose);
                ++it;
            }
        }
    }

    inline void pathRosToPath(const nav_msgs::Path& pathRos, std::vector<FrameWithId>& path) {
        pathRosToPath(pathRos.poses, path);
    }

}
#endif	/* CONVERSIONS_H */
