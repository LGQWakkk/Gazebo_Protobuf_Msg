#include <gazebo/gazebo.hh>
#include <gazebo/transport/transport.hh>
#include <opencv2/opencv.hpp>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <gazebo/gazebo_client.hh>
#include <iostream>

//自定义消息格式
#include "../build/my_image.pb.h"

//这个宏需要自行定义 否则会提示找不到ConstMyImagePtr定义
typedef const boost::shared_ptr<const MyImage> ConstMyImagePtr;

using namespace cv;
using namespace std;

//将OpenCV图像转换为u8 vector
void ConvertImageToBytes(const cv::Mat& img, std::vector<uint8_t>& data) {
    int width = img.cols;//图像宽度
    int height = img.rows;//图像高度
    int channels = img.channels();//图像通道数
    data.resize(width * height * channels);//计算数据大小 使用原始格式发送图像
    std::memcpy(data.data(), img.data, data.size());//将图像复制到buffer中
}

int main(int argc, char** argv) {
    gazebo::client::setup(argc, argv);//初始化Gazebo客户端
    printf("Hello, Gazebo!\n");

    gazebo::transport::NodePtr node(new gazebo::transport::Node());//图像发布者节点
    node->Init();

    gazebo::transport::PublisherPtr pub = node->Advertise<MyImage>("/image");//这里设置图像消息名称

    //读取 OpenCV 图像用于发送
    //注意尽量使用绝对路径否则很有可能读取失败!!!
    Mat image = imread("/home/wakkk/Gazebo/proto_msg_test/demo.jpg", IMREAD_COLOR);//修改你的图像地址
    if (image.empty()) {
        cerr << "Failed to load image" << endl;
        return -1;
    }else{
        cout << "Image loaded successfully" << endl;
    }

    std::vector<uint8_t> image_data;//用于放置要发送的图像缓冲区
    ConvertImageToBytes(image, image_data);//图像格式转换

    MyImage image_message;//建立消息
    image_message.set_width(image.cols);
    image_message.set_height(image.rows);
    image_message.set_data(image_data.data(), image_data.size());

    while(1)
    {
        printf("Publishing image message\n");
        pub->Publish(image_message);//不断发送图像
        gazebo::common::Time::MSleep(500);
    }
    //发现的一个小bug: 第一次发送的图像接收之后是白色空白图像 即第一帧会丢失

    gazebo::client::shutdown();//clear
    return 0;
}

