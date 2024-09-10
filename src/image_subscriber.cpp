#include <gazebo/gazebo.hh>
#include <gazebo/transport/transport.hh>
#include <opencv2/opencv.hpp>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <gazebo/gazebo_client.hh>
#include <iostream>

//�Զ�����Ϣ
#include "../build/my_image.pb.h"
//�Զ�����Ϣָ��
typedef const boost::shared_ptr<const MyImage> ConstMyImagePtr;

using namespace std;

//�յ�ͼ����Ϣ�ص�����
void onImageReceived(ConstMyImagePtr& msg) {
    cout << "Received image" << endl;
    cout << "Width: " << msg->width() << ", Height: " << msg->height() << endl;
    //����opencvͼ�����
    cv::Mat image(msg->height() , msg->width() , CV_8UC3); // ����ͼ��Ϊ CV_8UC3 ��ʽ�������ͬ��Ҫ����
    memcpy(image.data, msg->data().c_str(), msg->data().size());//ͼ�����ݿ���
    cv::imshow("Received Image", image);//show the image
    cv::waitKey(1);//delay to show
}

int main(int argc, char** argv) {
    gazebo::client::setup(argc, argv);
    printf("Hello, Gazebo!\n");

    gazebo::transport::NodePtr node(new gazebo::transport::Node());
    node->Init();

    gazebo::transport::SubscriberPtr sub = node->Subscribe("/image", onImageReceived);//�����޸Ķ�����Ϣ����

    while (true) {
        gazebo::common::Time::MSleep(10);//���������Խ�����Ϣ
    }

    gazebo::client::shutdown();//clear
    return 0;
}

