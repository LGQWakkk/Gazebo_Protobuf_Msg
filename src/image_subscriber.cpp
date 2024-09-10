#include <gazebo/gazebo.hh>
#include <gazebo/transport/transport.hh>
#include <opencv2/opencv.hpp>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <gazebo/gazebo_client.hh>
#include <iostream>

//自定义消息
#include "../build/my_image.pb.h"
//自定义消息指针
typedef const boost::shared_ptr<const MyImage> ConstMyImagePtr;

using namespace std;

//收到图像消息回调函数
void onImageReceived(ConstMyImagePtr& msg) {
    cout << "Received image" << endl;
    cout << "Width: " << msg->width() << ", Height: " << msg->height() << endl;
    //建立opencv图像对象
    cv::Mat image(msg->height() , msg->width() , CV_8UC3); // 假设图像为 CV_8UC3 格式，如果不同需要调整
    memcpy(image.data, msg->data().c_str(), msg->data().size());//图像数据拷贝
    cv::imshow("Received Image", image);//show the image
    cv::waitKey(1);//delay to show
}

int main(int argc, char** argv) {
    gazebo::client::setup(argc, argv);
    printf("Hello, Gazebo!\n");

    gazebo::transport::NodePtr node(new gazebo::transport::Node());
    node->Init();

    gazebo::transport::SubscriberPtr sub = node->Subscribe("/image", onImageReceived);//这里修改订阅消息名称

    while (true) {
        gazebo::common::Time::MSleep(10);//不断运行以接收消息
    }

    gazebo::client::shutdown();//clear
    return 0;
}

