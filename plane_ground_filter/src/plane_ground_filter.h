//
// Created by rdcas on 2021/5/9.
//

#ifndef PLANE_GROUND_FILTER_plane_ground_filter_H
#define PLANE_GROUND_FILTER_plane_ground_filter_H
#include <ros/ros.h>

// For disable PCL complile lib, to use PointXYZIR
#define PCL_NO_PRECOMPILE

#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_types.h>
#include <pcl/conversions.h>
#include <pcl_ros/transforms.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/common/centroid.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/crop_box.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <math.h>
// using eigen lib
#include <Eigen/Dense>

#include <sensor_msgs/PointCloud2.h>
#include <std_msgs/Float32.h>

#define VPoint pcl::PointXYZ

class plane_ground_filter {
private:
    ros::Subscriber sub_point_cloud_;
    ros::Publisher  pub_ground_cloud_, pub_non_ground_cloud_;

    void point_cb(const sensor_msgs::PointCloud2ConstPtr &in_cloud);

public:
    plane_ground_filter(ros::NodeHandle &nh);

};


#endif //PLANE_GROUND_FILTER_plane_ground_filter_H
