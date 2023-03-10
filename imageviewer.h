#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include <QWheelEvent>
#include <QFileDialog>
#include "ui_imageviewer.h"

class ImageViewer : public QWidget
{
	Q_OBJECT

public:
	explicit ImageViewer(QString _image, QWidget *parent = 0);
	explicit ImageViewer(QPixmap _pixmap, QWidget *parent = 0);
	~ImageViewer();


	// functions to load the image into the ImageViewer
	bool loadImage();
	void setFilePath(const QString& filepath);
	void setPixMap(QPixmap _pixmap);
	void enableRightClick();
	void disableRightClick();

private slots:
	// buttonActions for zooming
	void zoomIn();
	void zoomOut();
	void fitToWindow();

protected:
	// events for resizing window or using the mouswheel
	void resizeEvent(QResizeEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
	void mouseReleaseEvent(QMouseEvent * qevent) override;

private:
	// functions to scale the image
	QPixmap scaledImage();
	void setScaledImage();
	QSize getScaledSize();

	// variables for the image
	Ui::ImageViewer *ui;
	QPixmap t_image;
	double t_scaleFactor = 1.0;
	QString t_filePath;
	bool rightclick_enabled = true;

};

#endif // IMAGEVIEWER_H
