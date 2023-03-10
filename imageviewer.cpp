#include "imageviewer.h"

/*
ImageViewer::ImageViewer(QWidget *parent)
   : QWidget(parent), ui(new Ui::ImageViewer)
{


}*/


ImageViewer::ImageViewer(QString _image, QWidget *parent)    : QWidget(parent), ui(new Ui::ImageViewer)
{
	ui->setupUi(this);

	ui->scrollArea->setBackgroundRole(QPalette::Dark);

	// set the scrollArea as imageLabel
	ui->imageLabel->setParent(this);
	delete ui->scrollArea->takeWidget();
	ui->scrollAreaWidgetContents = nullptr;
	ui->scrollArea->setWidget(ui->imageLabel);

	// actions to the buttons

	setFilePath(_image);
	loadImage();
}

ImageViewer::ImageViewer(QPixmap _pixmap, QWidget *parent) :  QWidget(parent), ui(new Ui::ImageViewer), t_image(_pixmap)
{
	ui->setupUi(this);

	ui->scrollArea->setBackgroundRole(QPalette::Dark);

	// set the scrollArea as imageLabel
	ui->imageLabel->setParent(this);
	delete ui->scrollArea->takeWidget();
	ui->scrollAreaWidgetContents = nullptr;
	ui->scrollArea->setWidget(ui->imageLabel);

	// actions to the buttons

	setScaledImage();
	ui->scrollArea->ensureWidgetVisible(ui->imageLabel);
}

ImageViewer::~ImageViewer()
{
	delete ui;
}


void ImageViewer::setFilePath(const QString& filePath)
{
	t_filePath = filePath;
}

void ImageViewer::setPixMap(QPixmap _pixmap)
{
	t_image = _pixmap;
	setScaledImage();
}

void ImageViewer::enableRightClick()
{
	rightclick_enabled = true;
}

void ImageViewer::disableRightClick()
{
	rightclick_enabled = false;
}

bool ImageViewer::loadImage()
{
	if(t_filePath.isEmpty())
	{
		return false;
	}

	if(t_image.load(t_filePath))
	{
		t_scaleFactor = 1.0;
		setScaledImage();
		return true;
	}
	return false;
}

void ImageViewer::setScaledImage()
{
	QPixmap scaledPixmap = this->scaledImage();
	if(!scaledPixmap.isNull())
	{
		ui->imageLabel->setFixedSize(scaledPixmap.size());
		ui->imageLabel->setPixmap(scaledPixmap);
	}
}


QPixmap ImageViewer::scaledImage()
{
	QSize scaledSize = getScaledSize();
	QPixmap scaledPixmap;
	if(!t_image.isNull()) {
		scaledPixmap = t_image.scaled(scaledSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}

	resize(sizeHint());
	ui->scrollArea->resize(ui->scrollArea->sizeHint());


	return scaledPixmap;
}

QSize ImageViewer::getScaledSize()
{
	QSize scaledSize = size();
	scaledSize *= t_scaleFactor;
	scaledSize.setWidth(scaledSize.width() - 5);
	scaledSize.setHeight(scaledSize.height() - 5);
	return scaledSize;
}


void ImageViewer::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
	setScaledImage();
}

void ImageViewer::wheelEvent(QWheelEvent *event)
{
	int delta = event->delta();
	if(QApplication::keyboardModifiers() & Qt::ControlModifier)
	{
		if(delta > 0)
		{
			zoomIn();
		}
		else
		{
			zoomOut();
		}
	}
}

void ImageViewer::mouseReleaseEvent(QMouseEvent *qevent)
{
	if (qevent->button() == Qt::MouseButton::RightButton)
	{
		if(rightclick_enabled)
		{
			QString filename = QFileDialog::getSaveFileName();
			if(!filename.isEmpty())
			{
				t_image.save(filename);
			}
		}
	}
}

void ImageViewer::zoomIn()
{
	t_scaleFactor += 0.3;
	setScaledImage();
}

void ImageViewer::zoomOut()
{
	t_scaleFactor -= 0.3;
	setScaledImage();
}

void ImageViewer::fitToWindow()
{
	t_scaleFactor = 1.0;
	setScaledImage();
}
