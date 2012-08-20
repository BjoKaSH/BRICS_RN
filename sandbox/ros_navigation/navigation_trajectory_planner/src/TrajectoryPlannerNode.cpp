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

#include "navigation_trajectory_planner/TrajectoryPlanner.h"
#include "navigation_trajectory_planner/ConversionUtils.h"

#include <costmap_2d/costmap_2d_ros.h>
#include <pluginlib/class_loader.h>
#include <navigation_trajectory_msgs/Trajectory.h>
#include <nav_msgs/Odometry.h>
#include <nav_core/base_global_planner.h>
#include <pluginlib/class_loader.h>
#include <tf/transform_datatypes.h>

#include <kdl/frames.hpp>
#include <kdl/path_composite.hpp>
#include <kdl/trajectory_composite.hpp>

#include <string>
#include <iostream>

using namespace std;

TrajectoryPlanner* plannerNodePtr = NULL;
ros::Publisher* trajectoryPublisherPtr = NULL;
costmap_2d::Costmap2DROS* costmap = NULL;


//ros::Publisher velPublisher;

void publishTrajectory(navigation_trajectory_msgs::Trajectory& trajectory) {

    
    
    if (!trajectory.trajectory.empty()) {
        ROS_INFO("Trajectory published, size: %lu values", trajectory.trajectory.size());
        for (unsigned int i = 0; i < trajectory.trajectory.size(); i++) {
            nav_msgs::Odometry odom = trajectory.trajectory[i];
            

            tf::Quaternion bt;
        }
        trajectoryPublisherPtr->publish(trajectory);
    } else {
        ROS_WARN("Trajectory planner returned empty trajectory, skipping.");
    }

}

void goalCallback(const geometry_msgs::PoseStamped& goal) {

    if (plannerNodePtr != NULL) {
        ROS_INFO("Trajectory planner received new goal, computing a trajectory");

        ConversionUtils conversion;

        KDL::Frame goalKDL;
        conversion.poseRosToKdl(goal.pose, goalKDL);



        // getting actual robot pose from the costmap
        tf::Stamped<tf::Pose> globalPose;
        costmap->getRobotPose(globalPose);
        geometry_msgs::PoseStamped globalPoseMsgs;
        tf::poseStampedTFToMsg(globalPose, globalPoseMsgs);

        // converting actual pose to KDL data type.
        // note: everything should be in the global frame
        KDL::Frame initial;
        conversion.poseRosToKdl(globalPoseMsgs.pose, initial);

        // Since planner uses ROS implementation, we need a frame id
        plannerNodePtr->setPathFrameId(goal.header.frame_id);

        // Computing a path, given a goal pose
        KDL::Path_Composite path;
        plannerNodePtr->computePath(initial, goalKDL, path);

        // Computing a trajectory, given the path
        KDL::Trajectory_Composite trajectory;

        plannerNodePtr->computeTrajectory(path, trajectory);


        // Finally, publishing a trajectory
        navigation_trajectory_msgs::Trajectory trajectoryMsgs;
        const double dt = 0.2; //TODO move this to configuration;

        ROS_INFO("Trajectory sampled with ratio: %f Hz", 1.0 / dt);

        conversion.trajectoryKdlToRos(trajectory, trajectoryMsgs.trajectory, dt);
        trajectoryMsgs.header.frame_id = goal.header.frame_id;

        publishTrajectory(trajectoryMsgs);
    }

}

int main(int argc, char **argv) {

    // initializing ROS node
    std::string name = "trajectory_planner";
    ros::init(argc, argv, name);

    // TODO:: read topic names and configuration parameters from yaml files
    // Instantiating a costmap
    tf::TransformListener tf;
    costmap_2d::Costmap2DROS globalCostmap("global_costmap", tf);
    costmap = &globalCostmap;
    ROS_INFO("Initialize costmap size: %d, %d", globalCostmap.getSizeInCellsX(), globalCostmap.getSizeInCellsY());

    // Reading configuration parameters
    ros::NodeHandle node = ros::NodeHandle("~/");
    string globalTrajectoryPlanner;
    node.param("global_costmap/trajectory_planner", globalTrajectoryPlanner, string("navfn/NavfnROS"));

    // Initializing subscribers and publishers
    ros::NodeHandle globalNode = ros::NodeHandle("");
    ros::Publisher trajectoryPublisher;
    trajectoryPublisher = globalNode.advertise<navigation_trajectory_msgs::Trajectory > ("globalTrajectory", 1);
    trajectoryPublisherPtr = &trajectoryPublisher;


   // velPublisher = globalNode.advertise<geometry_msgs::Twist > ("cmd_vel1", 1);

    ros::Subscriber goalSubscriber;
    goalSubscriber = globalNode.subscribe("goal", 1, &goalCallback);

    // Loading a planner plugin
    nav_core::BaseGlobalPlanner* pathPlanner;
    //TODO: read planners names from configuration file
    pluginlib::ClassLoader<nav_core::BaseGlobalPlanner> bgpLoader("nav_core", "nav_core::BaseGlobalPlanner");
    pathPlanner = bgpLoader.createClassInstance(globalTrajectoryPlanner);
    pathPlanner->initialize(bgpLoader.getName(globalTrajectoryPlanner), &globalCostmap);

    // Instantiating trajectory planner
    TrajectoryPlanner trajectoryPlanner(pathPlanner);
    plannerNodePtr = &trajectoryPlanner;

    // Starting execution thread
    ros::spin();

    return 0;
}
