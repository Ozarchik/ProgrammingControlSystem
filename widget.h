#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QVector>
#include <QDateTime>
#include "qcustomplot.h"
#include <QHBoxLayout>
#include "blocks/gain/gain.h"
#include "blocks/DynamicAnalogModel/DynamicAnalogModel.h"
#include "blocks/DynamicDiscreteModel/DynamicDiscreteModel.h"
#include "blocks/StaticDiscreteModel/StaticDiscreteModel.h"
#include "blocks/StateSpace/statespace.h"


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:


    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void initStateSpace();
    void initDynamicAnalogModel();

    void initDynamicDiscreteModel1();
    void initDynamicDiscreteModel2();
    void initDynamicDiscreteModel3();

    void initStaticDiscreteModel1();
    void initStaticDiscreteModel2();
    void initStaticDiscreteModel3();

    void setInputPlot(qint64 time, float signal);
    void setOutputPlot(qint64 time, float signal);

public slots:
    void update();

private:
    Ui::Widget *ui;

    QVBoxLayout* widgetLayout;
    QGridLayout *mainlayout;
    QHBoxLayout* titleLayout;
    QCustomPlot *inputPlot;
    QCustomPlot *outputPlot;
	QPushButton* startButton;
	QPushButton* saveButton;

	qint64 startTime = 0;
	qint64 relativeTime = 0;
	qint64 dt;

    QTimer * timer;

    // --------------------------
    // Add pointer to the object here
    // --------------------------
    Gain* m_gain;
    Integrator* m_integrator;


    DynamicAnalogModel* dynamicAnalogModel;
    DynamicDiscreteModel* dynamicDiscreteModel1;
    DynamicDiscreteModel* dynamicDiscreteModel2;
    DynamicDiscreteModel* dynamicDiscreteModel3;

    StaticDiscreteModel* staticDiscreteModel1;
    StaticDiscreteModel* staticDiscreteModel2;
    StaticDiscreteModel* staticDiscreteModel3;
    StateSpace* stateSpace;

    // System* system;

    enum SIMULATE_MODE {
        STATE_SPACE,
        INTEGRATOR,

        STATIC_DISCRETE_MODEL_1,
        STATIC_DISCRETE_MODEL_2,
        STATIC_DISCRETE_MODEL_3,

        DYNAMIC_DISCRETE_MODEL_1,
        DYNAMIC_DISCRETE_MODEL_2,
        DYNAMIC_DISCRETE_MODEL_3,

        DYNAMIC_ANALOG_MODEL,
        STATIC_ANALOG_MODEL
    };

    SIMULATE_MODE mode;



    // --------------------------
    // Add pointer to the object here
    // --------------------------
};

#endif // WIDGET_H
