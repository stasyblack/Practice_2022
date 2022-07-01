#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
#define CV_RETR_EXTERNAL 0 // найти только крайние внешние контуры
#define CV_RETR_LIST     1 // найти все контуры и разместить их списком
#define CV_RETR_CCOMP    2 // найти все контуры и разместить их в виде 2-уровневой иерархии
#define CV_RETR_TREE     3 // найти все контуры и разместить их в иерархии вложенных контуров

#define CV_CHAIN_CODE               0 // цепной код Фридмана
#define CV_CHAIN_APPROX_NONE        1 // все точки цепного кода переводятся в точки
#define CV_CHAIN_APPROX_SIMPLE      2 // сжимает горизонтальные, вертикальные и диагональные сегменты и оставляет только их конечные точки
#define CV_CHAIN_APPROX_TC89_L1     3 // применяется алгоритм
#define CV_CHAIN_APPROX_TC89_KCOS   4 // аппроксимации Teh-Chin
#define CV_LINK_RUNS                5 // алгоритм только для CV_RETR_LIST

void recogniseStickersByThreshold(cv::Mat image, std::vector<cv::Mat>& stickers) {
	cv::Mat image_hsv;
	std::vector< std::vector<cv::Point> > contours;
	cv::cvtColor(image, image_hsv, cv::COLOR_BGR2HSV); // Преобразуем в hsv
	cv::Mat tmp_img(image.size(), CV_8U);
	// Выделение подходящих по цвету областей. Цвет задается константой :)
	cv::inRange(image_hsv, cv::Scalar(key_light - 2, key_sat - 20, key_hue - 35),
		cv::Scalar(key_light + 2, key_sat + 20, key_hue + 35), tmp_img);
	// "Замазать" огрехи в при выделении по цвету
	cv::dilate(tmp_img, tmp_img, cv::Mat(), cv::Point(-1, -1), 3);
	cv::erode(tmp_img, tmp_img, cv::Mat(), cv::Point(-1, -1), 1);
	//Выделение непрерывных областей
	cv::findContours(tmp_img, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	for (uint i = 0; i < contours.size(); i++) {
		cv::Mat sticker;
		//Для каждой области определяем ограничивающий прямоугольник
		cv::Rect rect = cv::boundingRect(contours[i]);
		image(rect).copyTo(sticker);
		stickers.push_back(sticker); //Добавить к массиву распознанных стикеров
	}
}

int main() {
	cv::Mat image(cv::Size(256, 256), CV_8UC3);
	for (int x = 0; x < image.size().width; ++x)
	{
		for (int y = 0; y < image.size().height; ++y) {
			image.at < cv::Vec3b >(y, x)[0] = x; // Blue
			image.at < cv::Vec3b >(y, x)[1] = y; // Green
			image.at < cv::Vec3b >(y, x)[2] = (x + y) / 2; // Red
		}
	}

	cv::VideoCapture cam(0);
	while (1) {
		cv::Mat img;
		cam >> img;
		cv::imshow("image", img);
		std::cout << "bjhsvdbhj\n";
		cv::waitKey(1);
	}
	recogniseStickersByThreshold(image, );
	return 0;
}