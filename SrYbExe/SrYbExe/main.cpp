#include "srybexe.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
// not in Qt5
//    QApplication::setGraphicsSystem("raster");
    QApplication a(argc, argv);

// 	// splash screen
// 	QPixmap pixmap("H:\\Photos\\2011-11-01\\2011-10-30-19-09 DSC_7758.JPG");
// 	QSplashScreen splash(pixmap.scaledToWidth(600));
// 	splash.show();

	SrYbExe w;
	w.show();

// 	// end splash 
// 	splash.finish(&w);

	return a.exec();
}

