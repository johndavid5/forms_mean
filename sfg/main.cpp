#include <QApplication>
#include <QLabel>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	QLabel* label = new QLabel
	(
	 "<h2 style=\"color: purple; border: 2px;\">Hello SEC Forms!</h2>\n"
	 "<h2 style=\"color: green; font-family: courier; border: 2px;\">Let off some steam, Bennett!</h2>\n"
	);
	label->show();
	return app.exec();
}/* main() */
