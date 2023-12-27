#include "widget.h"
#include "ui_widget.h"
#include <iostream>
#include <math.h>
#include <QSizePolicy>
#include <QDebug>


// --------------------------
// Set stop time here
// --------------------------
#define ENDOFTIME 25
#define SAMPLINGTIMEMSEC 10

// --------------------------
// Set stop time here
// --------------------------

typedef std::vector<float> vec;
typedef std::vector<std::vector<float>> mat;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

	// Create dir for logs
	if (!QDir("logs").exists()) {
		QDir().mkdir("logs");
	}

    // Set window size
	this->setMinimumSize(640, 480);


    // Add main layout with two plots
    widgetLayout = new QVBoxLayout(this);
    mainlayout = new QGridLayout(this);
    titleLayout = new QHBoxLayout(this);
    widgetLayout->addLayout(titleLayout);
    widgetLayout->addLayout(mainlayout);
    inputPlot = new QCustomPlot(this);
    outputPlot = new QCustomPlot(this);
    auto xTitle = new QLabel("Входной сигнал по обратной связи");
    auto yTitle = new QLabel("Динамическая аналоговая модель №1 ");
    xTitle->setStyleSheet("font-weight: bold; font-size: 18px; color: black");
    yTitle->setStyleSheet("font-weight: bold; font-size: 18px; color: black");
    xTitle->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    yTitle->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    xTitle->setFixedHeight(20);
    yTitle->setFixedHeight(20);
    titleLayout->addWidget(xTitle);
    titleLayout->addWidget(yTitle);
    mainlayout->addWidget(inputPlot,  0, 0);
    mainlayout->addWidget(outputPlot, 0, 1);
	inputPlot->addGraph();
	outputPlot->addGraph();

	startButton = new QPushButton("Start");
	connect(startButton, &QPushButton::clicked, this, [=](){
		startTime = 0;
		relativeTime = 0;
		inputPlot->graph(0)->data()->clear();
		outputPlot->graph(0)->data()->clear();
		timer->start();
	});
	mainlayout->addWidget(startButton, 1, 0);

	saveButton = new QPushButton("Save");
	connect(saveButton, &QPushButton::clicked, this, [=](){
		QString timestamp = QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss");
		inputPlot->savePng(QString("./logs/%1_input.png").arg(timestamp));
		outputPlot->savePng(QString("./logs/%1_output.png").arg(timestamp));
	});
	mainlayout->addWidget(saveButton, 1, 1);
    this->setLayout(widgetLayout);

    // Give the axes some labels:
    inputPlot->xAxis->setLabel("t");
    inputPlot->yAxis->setLabel("input");
    outputPlot->xAxis->setLabel("t");
    outputPlot->yAxis->setLabel("output");

    // --------------------------
    // Change ranges if you need
    // --------------------------
    // Set axes ranges so see all data:
    inputPlot->xAxis->setRange(0, ENDOFTIME);
    inputPlot->yAxis->setRange(-1.2, 1.2);
    outputPlot->xAxis->setRange(0, ENDOFTIME);
    outputPlot->yAxis->setRange(-1.2, 1.2);

    initStateSpace();
    initDynamicAnalogModel();
    initDynamicAnalogModel();

    initDynamicDiscreteModel1();
    initDynamicDiscreteModel2();
    initDynamicDiscreteModel3();

    initStaticDiscreteModel1();
    initStaticDiscreteModel2();
    initStaticDiscreteModel3();

    timer = new QTimer(this);
    timer->setTimerType(Qt::TimerType::PreciseTimer);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));

    // --------------------------
    // Set sampling time here
    // --------------------------
    timer->setInterval(SAMPLINGTIMEMSEC);
    // --------------------------
    // Set sampling time here
    // --------------------------

    m_gain = new Gain(0.0);
    m_integrator = new Integrator(-1.0);
}

Widget::~Widget()
{
    delete ui;
    delete inputPlot;
    delete outputPlot;
    delete timer;
    delete mainlayout;

    delete m_gain;
    delete m_integrator;

    delete dynamicDiscreteModel1;
    delete dynamicDiscreteModel2;
    delete dynamicDiscreteModel3;

    delete staticDiscreteModel1;
    delete staticDiscreteModel2;
    delete staticDiscreteModel3;

    delete stateSpace;
}

void Widget::initStateSpace()
{

    // ------------------------------------------------
    // | [1] STATE SPACE                              |
    // ------------------------------------------------

    vec initConditions {0.0, 0.0, 0.0};
    mat A {{-4, -4, -1},{1, 0, 0},{0, 1, 0}};
    vec B {1, 0, 0};
    vec C {0, 0, 1};
    vec D {1};
    stateSpace = new StateSpace(initConditions, A, B, C, D);
}



void Widget::initStaticDiscreteModel1()
{
    // ------------------------------------------------
    // | [2] STATIC DISCRETE MODEL - 1                |
    // ------------------------------------------------

    vec initConditions {0.0, 0.0, 0.0};
    mat A{{0.4013, -0.5514, -0.1340},{0.1340, 0.9374, -0.0154},{0.0154, 0.1956, 0.9989}};
    vec B {0.1340, 0.0154, 0.0011};
    vec C {0, 0, 1};
    vec D {1};
    staticDiscreteModel1 = new StaticDiscreteModel(initConditions, A, B, C, D);
}

void Widget::initStaticDiscreteModel2()
{
    // ------------------------------------------------
    // | [3] STATIC DISCRETE MODEL - 2                |
    // ------------------------------------------------

    vec initConditions {0.0, 0.0, 0.0};
    mat A{{0.8731, -0.1253, -0.0312},{0.0312, 0.9979, -0.0005},{0.0005, 0.0333, 1.0000}};
    vec B {0.0312, 0.0005, 0.0006};
    vec C {0, 0, 1};
    vec D {1};
    staticDiscreteModel2 = new StaticDiscreteModel(initConditions, A, B, C, D);
}

void Widget::initStaticDiscreteModel3()
{
    // ------------------------------------------------
    // | [4] STATIC DISCRETE MODEL - 3                |
    // ------------------------------------------------

    vec initConditions {0.0, 0.0, 0.0};
    mat A{{0.9606, -0.0393, -0.0098},{0.0098, 0.9998, -0.00005},{0.00005, 0.0100, 1.0000}};
    vec B {0.3252, 0.0349, 0.0024};
    vec C {0, 0, 1};
    vec D {1};
    staticDiscreteModel3 = new StaticDiscreteModel(initConditions, A, B, C, D);
}

void Widget::initDynamicDiscreteModel1()
{
    // ------------------------------------------------
    // | [5] DYNAMIC DISCRETE MODEL - 1               |
    // ------------------------------------------------

    vec initConditions {1, 0};
    mat A{{0.9950, 0.1997},{-0.0499, 0.9950}};
    vec B {0, 0};
    vec C {1, 0};
    vec D {0};
    dynamicDiscreteModel1 = new DynamicDiscreteModel(initConditions, A, B, C, D);
}

void Widget::initDynamicDiscreteModel2()
{
    // ------------------------------------------------
    // | [6] DYNAMIC DISCRETE MODEL - 2               |
    // ------------------------------------------------

    vec initConditions {1, 0};
    mat A{{0.9999, 0.0333},{-0.0083, 0.9999}};
    vec B {0, 0};
    vec C {1, 0};
    vec D {0};
    dynamicDiscreteModel2 = new DynamicDiscreteModel(initConditions, A, B, C, D);
}

void Widget::initDynamicDiscreteModel3()
{
    // ------------------------------------------------
    // | [7] DYNAMIC DISCRETE MODEL - 3               |
    // ------------------------------------------------

    vec initConditions {1, 0};
    mat A{{1.0000, 0.0100},{-0.0025, 1.0000}};
    vec B {0, 0};
    vec C {1, 0};
    vec D {0};
    dynamicDiscreteModel3 = new DynamicDiscreteModel(initConditions, A, B, C, D);
}

void Widget::initDynamicAnalogModel()
{
    // ------------------------------------------------
    // | [8] DYNAMIC ANALOG MODEL                     |
    // ------------------------------------------------

    vec initConditions {1, 0};
    mat A{{0.0, 1.0},{-0.25, 0.0}};
    vec B {0.0, 0.0};
    vec C {1.0, 0.0};
    vec D {0.0};
    dynamicAnalogModel = new DynamicAnalogModel(initConditions, A, B, C, D);
}

void Widget::setInputPlot(qint64 time, float signal)
{
    inputPlot->graph(0)->addData(time/1000.0, signal);
    inputPlot->replot();
}

void Widget::setOutputPlot(qint64 time, float signal)
{
    outputPlot->graph(0)->addData(time/1000.0, signal);
    outputPlot->replot();
}

void Widget::update() {

    // float signal = staticDiscreteModel1->getOutput();
    // float signal = std::sin(relativeTime/ 1000.0);
    float signal = 10;
    if (startTime == 0) {
		startTime = QDateTime::currentMSecsSinceEpoch();

		inputPlot->graph(0)->addData(0.0, 0);
		outputPlot->graph(0)->addData(0.0, 0);
		return;
	} else {
		dt = relativeTime;
		relativeTime = QDateTime::currentMSecsSinceEpoch() - startTime;
		dt = relativeTime - dt;
	}


    float signal_dt = static_cast<float>(dt/1000.0);
    float outputSignal = 0.0;

    switch (mode) {

    case STATE_SPACE:
        outputSignal = stateSpace->getOutput();
        stateSpace->update(signal, signal_dt);
        break;

    case INTEGRATOR:

        break;

    case STATIC_ANALOG_MODEL:
        break;

    case STATIC_DISCRETE_MODEL_1:
        outputSignal = staticDiscreteModel1->getOutput();
        staticDiscreteModel1->update(signal);
        break;

    case STATIC_DISCRETE_MODEL_2:
        outputSignal = staticDiscreteModel2->getOutput();
        staticDiscreteModel2->update(signal);
        break;

    case STATIC_DISCRETE_MODEL_3:
        outputSignal = staticDiscreteModel3->getOutput();
        staticDiscreteModel3->update(signal);
        break;

    case DYNAMIC_ANALOG_MODEL:
        outputSignal = dynamicAnalogModel->getOutput();
        dynamicAnalogModel->update(outputSignal, signal_dt);
        break;

    case DYNAMIC_DISCRETE_MODEL_1:
        outputSignal = dynamicDiscreteModel1->getOutput();
        dynamicDiscreteModel1->update(outputSignal);
        break;

    case DYNAMIC_DISCRETE_MODEL_2:
        outputSignal = dynamicDiscreteModel2->getOutput();
        dynamicDiscreteModel2->update(outputSignal);
        break;

    case DYNAMIC_DISCRETE_MODEL_3:
        outputSignal = dynamicDiscreteModel3->getOutput();
        dynamicDiscreteModel3->update(outputSignal);
        break;
    }


//    setInputPlot(relativeTime, signal);
    setInputPlot(relativeTime, outputSignal);
    setOutputPlot(relativeTime, outputSignal);

    // test integrator
    // m_integrator->update(signal, float(signal_dt));
    // setOutputPlot(relativeTime, m_integrator->state());
    // end test

    if (relativeTime / 1000.0 > ENDOFTIME) {
        timer->blockSignals(true);
        timer->stop();
        timer->blockSignals(false);
    }
}
