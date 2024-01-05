#include "widget.h"
#include "ui_widget.h"
#include <iostream>
#include <math.h>
#include <QSizePolicy>
#include <QDebug>

// Длительность симуляции
#define END_OF_TIME 50

// Шаг дискретизации
#define SAMPLING_TIME_MSEC 10
// 200 мс - 5 Гц
//  33 мс - 30 Гц
//  10 мс - 100 Гц

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
    this->setMinimumSize(1600, 800);


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
    inputPlot->xAxis->setRange(0, END_OF_TIME);
    outputPlot->xAxis->setRange(0, END_OF_TIME);

    // ********************************************************
    // **************** Для динамической модели ***************
    // ********************************************************
    inputPlot->yAxis->setRange(-1.25, 1.25);
    outputPlot->yAxis->setRange(-1.25, 1.25);


    // ********************************************************
    // **************** Для статической модели ****************
    // ********************************************************
    // inputPlot ->yAxis->setRange(9, 21);
    // outputPlot->yAxis->setRange(9, 21);


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
    timer->setInterval(SAMPLING_TIME_MSEC);
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


// ------------------------------------------------
// | [1] STATE SPACE                              |
// ------------------------------------------------
void Widget::initStateSpace()
{
    vec initConditions {0.0, 0.0, 0.0};
    mat A {{-4, -4, -1},{1, 0, 0},{0, 1, 0}};
    vec B {1, 0, 0};
    vec C {0, 0, 1};
    vec D {1};
    stateSpace = new StateSpace(initConditions, A, B, C, D);
}



// ------------------------------------------------
// | [2] STATIC DISCRETE MODEL - 1                |
// ------------------------------------------------
void Widget::initStaticDiscreteModel1()
{
    vec initConditions {0.0, 0.0, 0.0};
    mat A{{0.401298, -0.551447, -0.134015},
          {0.134015,  0.937360, -0.015385},
          {0.015385,  0.195559,  0.998904}};
    vec B {0.134015,  0.015386,  0.001096};
    vec C {0, 0, 1};
    vec D {1};
    staticDiscreteModel1 = new StaticDiscreteModel(initConditions, A, B, C, D);
}


// ------------------------------------------------
// | [3] STATIC DISCRETE MODEL - 2                |
// ------------------------------------------------
void Widget::initStaticDiscreteModel2()
{
    vec initConditions {0.0, 0.0, 0.0};
    mat A{{0.873134, -0.125265, -0.031183},
          {0.031183,  0.997868, -0.000531},
          {0.000531,  0.033309,  0.999994}};
    vec B {0.031183,  0.000531,  0.0000059};
    vec C {0, 0, 1};
    vec D {1};
    staticDiscreteModel2 = new StaticDiscreteModel(initConditions, A, B, C, D);
}


// ------------------------------------------------
// | [4] STATIC DISCRETE MODEL - 3                |
// ------------------------------------------------
void Widget::initStaticDiscreteModel3()
{

    vec initConditions {0.0, 0.0, 0.0};
    mat A{{0.960594, -0.039257, -0.009801},
          {0.009801, 0.999802, -0.000049},
          {0.000049, 0.009999, 0.999999}};
    vec B {0.0098019, 0.0000493, 0.0000001};
    vec C {0, 0, 1};
    vec D {1};
    staticDiscreteModel3 = new StaticDiscreteModel(initConditions, A, B, C, D);
}


// ------------------------------------------------
// | [5] DYNAMIC DISCRETE MODEL - 1               |
// ------------------------------------------------
void Widget::initDynamicDiscreteModel1()
{
    vec initConditions {1, 0};
    mat A{{0.995004, 0.199667},{-0.049917, 0.995004}};
    vec B {0, 0};
    vec C {1, 0};
    vec D {0};
    dynamicDiscreteModel1 = new DynamicDiscreteModel(initConditions, A, B, C, D);
}


// ------------------------------------------------
// | [6] DYNAMIC DISCRETE MODEL - 2               |
// ------------------------------------------------
void Widget::initDynamicDiscreteModel2()
{
    vec initConditions {1, 0};
    mat A{{0.99986, 0.033332},{-0.0083329, 0.99986}};
    vec B {0, 0};
    vec C {1, 0};
    vec D {0};
    dynamicDiscreteModel2 = new DynamicDiscreteModel(initConditions, A, B, C, D);
}


// ------------------------------------------------
// | [7] DYNAMIC DISCRETE MODEL - 3               |
// ------------------------------------------------
void Widget::initDynamicDiscreteModel3()
{
    vec initConditions {1, 0};
    mat A{{0.99999, 0.01},{-0.0025, 0.99999}};
    vec B {0, 0};
    vec C {1, 0};
    vec D {0};
    dynamicDiscreteModel3 = new DynamicDiscreteModel(initConditions, A, B, C, D);
}


// ------------------------------------------------
// | [8] DYNAMIC ANALOG MODEL                     |
// ------------------------------------------------
void Widget::initDynamicAnalogModel()
{


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


    // float signal_dt = static_cast<float>(dt/1000.0);
    float signal_dt = (static_cast<float>(dt)/1000.0);
    float outputSignal = 0.0;


//  1. STATE_SPACE [no freq]
//  3. STATIC_DISCRETE_MODEL  [1/2/3] [5 33 100] Hz
//  3. DYNAMIC_DISCRETE_MODEL [1/2/3] [5 33 100] Hz
//  4. DYNAMIC_ANALOG_MODEL           [5 33 100] Hz

    mode = DYNAMIC_DISCRETE_MODEL_3;

    switch (mode) {
    case INTEGRATOR: break;
    case STATIC_ANALOG_MODEL: break;

    case STATE_SPACE:
        outputSignal = stateSpace->getOutput();
        stateSpace->update(signal, signal_dt);
        setInputPlot(relativeTime, signal);
        setOutputPlot(relativeTime, outputSignal);
        break;

    case STATIC_DISCRETE_MODEL_1:
        outputSignal = staticDiscreteModel1->getOutput();
        staticDiscreteModel1->update(signal);
        setInputPlot(relativeTime, signal);
        setOutputPlot(relativeTime, outputSignal);
        break;

    case STATIC_DISCRETE_MODEL_2:
        outputSignal = staticDiscreteModel2->getOutput();
        staticDiscreteModel2->update(signal);
        setInputPlot(relativeTime, signal);
        setOutputPlot(relativeTime, outputSignal);
        break;

    case STATIC_DISCRETE_MODEL_3:
        outputSignal = staticDiscreteModel3->getOutput();
        staticDiscreteModel3->update(signal);
        setInputPlot(relativeTime, signal);
        setOutputPlot(relativeTime, outputSignal);
        break;

    case DYNAMIC_DISCRETE_MODEL_1:
        outputSignal = dynamicDiscreteModel1->getOutput();
        dynamicDiscreteModel1->update(outputSignal);
        setInputPlot(relativeTime, outputSignal);
        setOutputPlot(relativeTime, outputSignal);
        break;

    case DYNAMIC_DISCRETE_MODEL_2:
        outputSignal = dynamicDiscreteModel2->getOutput();
        dynamicDiscreteModel2->update(outputSignal);
        setInputPlot(relativeTime, outputSignal);
        setOutputPlot(relativeTime, outputSignal);
        break;

    case DYNAMIC_DISCRETE_MODEL_3:
        outputSignal = dynamicDiscreteModel3->getOutput();
        dynamicDiscreteModel3->update(outputSignal);
        setInputPlot(relativeTime, outputSignal);
        setOutputPlot(relativeTime, outputSignal);
        break;

    case DYNAMIC_ANALOG_MODEL:
        outputSignal = dynamicAnalogModel->getOutput();
        dynamicAnalogModel->update(outputSignal, signal_dt);
        setInputPlot(relativeTime, outputSignal);
        setOutputPlot(relativeTime, outputSignal);
        break;
    }


//    setInputPlot(relativeTime, signal);


    // test integrator
    // m_integrator->update(signal, float(signal_dt));
    // setOutputPlot(relativeTime, m_integrator->state());
    // end test

    if (relativeTime / 1000.0 > END_OF_TIME) {
        timer->blockSignals(true);
        timer->stop();
        timer->blockSignals(false);
    }
}
