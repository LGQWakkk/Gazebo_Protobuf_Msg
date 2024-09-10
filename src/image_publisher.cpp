#include <gazebo/gazebo.hh>
#include <gazebo/transport/transport.hh>
#include <opencv2/opencv.hpp>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <gazebo/gazebo_client.hh>
#include <iostream>

//�Զ�����Ϣ��ʽ
#include "../build/my_image.pb.h"

//�������Ҫ���ж��� �������ʾ�Ҳ���ConstMyImagePtr����
typedef const boost::shared_ptr<const MyImage> ConstMyImagePtr;

using namespace cv;
using namespace std;

//��OpenCVͼ��ת��Ϊu8 vector
void ConvertImageToBytes(const cv::Mat& img, std::vector<uint8_t>& data) {
    int width = img.cols;//ͼ����
    int height = img.rows;//ͼ��߶�
    int channels = img.channels();//ͼ��ͨ����
    data.resize(width * height * channels);//�������ݴ�С ʹ��ԭʼ��ʽ����ͼ��
    std::memcpy(data.data(), img.data, data.size());//��ͼ���Ƶ�buffer��
}

int main(int argc, char** argv) {
    gazebo::client::setup(argc, argv);//��ʼ��Gazebo�ͻ���
    printf("Hello, Gazebo!\n");

    gazebo::transport::NodePtr node(new gazebo::transport::Node());//ͼ�񷢲��߽ڵ�
    node->Init();

    gazebo::transport::PublisherPtr pub = node->Advertise<MyImage>("/image");//��������ͼ����Ϣ����

    //��ȡ OpenCV ͼ�����ڷ���
    //ע�⾡��ʹ�þ���·��������п��ܶ�ȡʧ��!!!
    Mat image = imread("/home/wakkk/Gazebo/proto_msg_test/demo.jpg", IMREAD_COLOR);//�޸����ͼ���ַ
    if (image.empty()) {
        cerr << "Failed to load image" << endl;
        return -1;
    }else{
        cout << "Image loaded successfully" << endl;
    }

    std::vector<uint8_t> image_data;//���ڷ���Ҫ���͵�ͼ�񻺳���
    ConvertImageToBytes(image, image_data);//ͼ���ʽת��

    MyImage image_message;//������Ϣ
    image_message.set_width(image.cols);
    image_message.set_height(image.rows);
    image_message.set_data(image_data.data(), image_data.size());

    while(1)
    {
        printf("Publishing image message\n");
        pub->Publish(image_message);//���Ϸ���ͼ��
        gazebo::common::Time::MSleep(500);
    }
    //���ֵ�һ��Сbug: ��һ�η��͵�ͼ�����֮���ǰ�ɫ�հ�ͼ�� ����һ֡�ᶪʧ

    gazebo::client::shutdown();//clear
    return 0;
}

