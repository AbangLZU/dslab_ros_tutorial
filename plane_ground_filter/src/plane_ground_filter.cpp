//
// Created by rdcas on 2021/5/9.
//

#include "plane_ground_filter.h"

using namespace std;


void plane_ground_filter::point_cb(const sensor_msgs::PointCloud2ConstPtr &in_cloud_ptr)
{
    pcl::PointCloud<VPoint>::Ptr laserCloudIn(new pcl::PointCloud<VPoint>());
    pcl::fromROSMsg(*in_cloud_ptr, *laserCloudIn);

    pcl::PointCloud<VPoint>::Ptr croped_cloud(new pcl::PointCloud<VPoint>);
    pcl::CropBox<VPoint> crop_box(true);

    // crop with a rectangle
    Eigen::Vector4f min_point(-30.0, -15.0, -5.0, 1);
    Eigen::Vector4f max_point(30.0, 15.0, 5.0, 1);
    crop_box.setMin(min_point);
    crop_box.setMax(max_point);
    crop_box.setInputCloud(laserCloudIn);
    crop_box.filter(*croped_cloud);

    // downsample with voxel grid
    pcl::VoxelGrid<VPoint> voxel_filter;
    voxel_filter.setInputCloud(croped_cloud);
    voxel_filter.setLeafSize(0.3, 0.3, 0.3);
    pcl::PointCloud<VPoint>::Ptr ds_cloud(new pcl::PointCloud<VPoint>);
    voxel_filter.filter(*ds_cloud);

    // segment with RANSAC
    pcl::ModelCoefficients::Ptr coefficients (new pcl::ModelCoefficients ());
    pcl::PointIndices::Ptr inliers(new pcl::PointIndices());
    pcl::SACSegmentation<VPoint> seg;
    seg.setOptimizeCoefficients(true);
    seg.setModelType(pcl::SACMODEL_PLANE);
    seg.setMethodType(pcl::SAC_RANSAC);
    seg.setMaxIterations(40);
    seg.setDistanceThreshold(0.3);
    seg.setInputCloud(ds_cloud);
    seg.segment(*inliers, *coefficients);

	pcl::PointCloud<VPoint>::Ptr plane_cloud(new pcl::PointCloud<VPoint>());
	pcl::PointCloud<VPoint>::Ptr non_ground_cloud(new pcl::PointCloud<VPoint>);

    for (auto ind : inliers->indices) {
        plane_cloud->points.push_back(ds_cloud->points[ind]);
    }

    pcl::ExtractIndices<VPoint> extractor;
    extractor.setIndices(inliers);
    extractor.setNegative(true);
    extractor.setInputCloud(ds_cloud);
    extractor.filter(*non_ground_cloud);

    // pub ground cloud
    sensor_msgs::PointCloud2 cloud_msg;
    pcl::toROSMsg(*plane_cloud, cloud_msg);
    cloud_msg.header = in_cloud_ptr->header;
	pub_ground_cloud_.publish(cloud_msg);

    // publish non ground cloud
	sensor_msgs::PointCloud2 non_ground_msg;
	pcl::toROSMsg(*non_ground_cloud, non_ground_msg);
	non_ground_msg.header = in_cloud_ptr->header;
	pub_non_ground_cloud_.publish(non_ground_msg);
}

plane_ground_filter::plane_ground_filter(ros::NodeHandle &nh) {

	string input_cloud_topic, non_ground_topic;

	nh.getParam("input_cloud", input_cloud_topic);
	nh.getParam("non_ground_cloud", non_ground_topic);
    sub_point_cloud_ = nh.subscribe(input_cloud_topic, 10, &plane_ground_filter::point_cb, this);
    pub_ground_cloud_ = nh.advertise<sensor_msgs::PointCloud2>("/ground_cloud", 1);
	pub_non_ground_cloud_ = nh.advertise<sensor_msgs::PointCloud2>(non_ground_topic, 1);

    ros::spin();
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "plane_ground_filter");

    ros::NodeHandle nh("~");

    plane_ground_filter core(nh);
    return 0;
}

