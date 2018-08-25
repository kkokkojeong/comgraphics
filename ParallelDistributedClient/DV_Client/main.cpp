#include "dv_client.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	DV_Client w;
	w.show();
	
	/*
	DvClientThread client;
	client.connectServer();
	client.sendInformation();
	client.start();
	*/
	return a.exec();
}
