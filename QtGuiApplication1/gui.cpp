#include "gui.h"

Gui::Gui(int argc, char *argv[], osg::ref_ptr<CustomFigure> figure)
{
	_figure = figure;
	execute = true;

	std::tr1::shared_ptr<QWidget> tmpWindow(new QWidget);
	_window = tmpWindow;
	_window->setWindowTitle("GUI");

	QHBoxLayout * layoutA = new QHBoxLayout;
	QLabel * labelA = new QLabel;
	labelA->setText("A");
	labelAVal = new QLabel;
	CustomSlider * sliderA = new CustomSlider;
	sliderA->setOrientation(Qt::Horizontal);
	sliderA->setRange(-10, 10);
	sliderA->setSliderPosition(0);
	auto lambdaA = [figure](int a) { figure->SetKA(a); };
	QObject::connect(sliderA, &QSlider::valueChanged, sliderA, lambdaA);
	layoutA->addWidget(labelA);
	layoutA->addWidget(labelAVal);
	layoutA->addWidget(sliderA);

	QHBoxLayout * layoutB = new QHBoxLayout;
	QLabel * labelB = new QLabel;
	labelB->setText("B");
	labelBVal = new QLabel;
	CustomSlider * sliderB = new CustomSlider;
	sliderB->setOrientation(Qt::Horizontal);
	sliderB->setRange(-10, 10);
	sliderB->setSliderPosition(0);
	auto lambdaB = [figure](int b) { figure->SetKB(b); };
	QObject::connect(sliderB, &QSlider::valueChanged, sliderB, lambdaB);
	layoutB->addWidget(labelB);
	layoutB->addWidget(labelBVal);
	layoutB->addWidget(sliderB);

	QObject::connect(sliderA, &QSlider::sliderReleased, sliderA, [figure]() { figure->SetDefault(); });
	QObject::connect(sliderA, &QSlider::sliderReleased, sliderA, &CustomSlider::SetDefault);
	QObject::connect(sliderB, &QSlider::sliderReleased, sliderB, [figure]() { figure->SetDefault(); });
	QObject::connect(sliderB, &QSlider::sliderReleased, sliderB, &CustomSlider::SetDefault);


	QVBoxLayout * layout = new QVBoxLayout;
	layout->addLayout(layoutA);
	layout->addLayout(layoutB);
	_window->setLayout(layout);
	_window->show();

	bool execute = true;
	upd = std::make_unique<std::thread>(Gui::updateData, this, figure);
}

Gui::~Gui()
{
	execute = false;
	upd->join();
}

void Gui::updateData(Gui * gui,  osg::ref_ptr<CustomFigure> figure)
{
	//_sleep(2000);
	while (gui->execute)
	{
		gui->labelAVal->setText(QString::fromStdString(std::to_string(figure->GetA())));
		gui->labelBVal->setText(QString::fromStdString(std::to_string(figure->GetB())));
		_sleep(100);
	}
}