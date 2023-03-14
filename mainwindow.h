#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv.hpp>
#include <opencv2/imgproc.hpp>
#include "imageviewer.h"
#include <crackdetection/functions.h>
#include <QMessageBox>
#include <QClipboard>
#include <crackdetection/islands.h>
#include <boundingbox.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void changeText();
	void changeContrastDivisor(int value);

	void change_preprocessing_treshold_divisor(int value);
	void change_contrast_subtractor_divisor(int value);
	void change_preprocessing_treshold_adder(int value);
	void change_contrast_hsv_substractor_divisor(int value);

	void automaticUpdate();
	void manualUpdate();
	void sliderConstrastDivisorReleased();
	void exportParameters();
	void switchOverlay();
	void save();
	void load();
	Mat colorizeIslandImage(cv::Mat& labelImage, int nLabels);


private:
	Ui::MainWindow *ui;

	ImageViewer* imageviewer;
	ImageViewer* imageviewerContrast;
	ImageViewer* imageviewerCombined;
	float contrastDivisor;
	cv::Mat inputImage;
	cv::Mat contrastImage;
	cv::Mat contrastImageFilled;
	cv::Mat combinedImage;

	float saved_contrast_divisor = 100;
	float saved_contrast_subtractor_divisor = 100;
	float saved_contrast_hsv_substractor_divisor = 0;
	float saved_preprocessing_treshold_divisor = 30;
	float saved_preprocessing_treshold_adder = 0;

signals:
	void triggerAutomaticUpdate();
	void triggerManualUpdate();

};

#endif // MAINWINDOW_H
