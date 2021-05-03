#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QDateTime>
#include <qcustomplot.h>

#include <iostream>
#include <datadecimator.h>

void messageBox(const QString& text)
{
    QMessageBox mb;
    mb.setText(text);
    mb.show();
    mb.exec();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lePath->setText("../GraphDecimator/Data1.txt");

    graphInit(ui->plot1);
    graphInit(ui->plot2);

    ui->dteStart->setCalendarPopup(true);
    ui->dteEnd->setCalendarPopup(true);

    connect(ui->selectAlg, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::changeAlgorithm);
    connect(ui->btnSetPath, &QPushButton::clicked, this, &MainWindow::setPathClicked);
    connect(ui->btnSetDateTime, &QPushButton::clicked, this, &MainWindow::setDateTimeClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::replotGraph(QCustomPlot* plot, int id, const storage_t& data)
{
    auto graph = plot->graph(id);
    graph->data()->clear();
    graph->setData(data);
    bool xValid, yValid;
    auto xRange = graph->getKeyRange(xValid);
    auto yRange = graph->getValueRange(yValid);
    if( !xValid || !yValid ) {
        return true;
    }
    setRanges(plot, xRange, yRange);
    plot->replot();
    return true;
}

void MainWindow::decGraph()
{
    QTime timer;
    auto storage2 = func[currentAlg](storage, (ui->plot1->yAxis->range().upper - ui->plot1->yAxis->range().lower) / 30);
    timer.start();

    replotGraph(ui->plot2, 0, storage2);

    ui->plottingTime2->setText(QString::number(timer.elapsed() / 1000.0) + "s");
    ui->nData2->setText(QString::number(ui->plot2->graph(0)->data()->size()));
}

bool MainWindow::read_from_file(const QString& path)
{
    FILE* file = fopen(path.toLocal8Bit().data(), "r");
    if(file == nullptr) {
        return false;
    }
    size_t  l;
    char*   line { nullptr };
    double  value;
    while(!feof(file)) {
        getline(&line, &l, file);
        struct tm t { };
        sscanf(line, "%d.%d.%d %d:%d:%d\t%lf", &t.tm_mday, &t.tm_mon, &t.tm_year, &t.tm_hour, &t.tm_min, &t.tm_sec, &value);
        t.tm_year -= 1900;
        --t.tm_mon;
        storage->add({ static_cast<double>(mktime(&t)), value });
    }
    return true;
}

void MainWindow::setRanges(QCustomPlot* plot, const QCPRange &xRange, const QCPRange &yRange)
{
    plot->xAxis->setRange(xRange);
    plot->yAxis->setRange(yRange);
}

void MainWindow::graphInit(QCustomPlot *plot)
{
    plot->addGraph();
    plot->xAxis->setTicker(QSharedPointer<QCPAxisTickerDateTime>(new QCPAxisTickerDateTime));
}

void MainWindow::setPathClicked(bool checked)
{

    storage.data()->clear();
    QTime timer;
    timer.start();
    bool readingResponse = read_from_file(ui->lePath->text());
    if(!readingResponse) {
        messageBox("File doesn't exists");
    }
    else {
        ui->timeReading->setText(QString::number(timer.elapsed() / 1000.0) + "s");


        timer.start();

        replotGraph(ui->plot1, 0, storage);

        ui->plottingTime1->setText(QString::number(timer.elapsed() / 1000.0) + "s");
        ui->nData1->setText(QString::number(ui->plot1->graph(0)->data()->size()));

        auto startDateTime  { QDateTime::fromTime_t(ui->plot1->xAxis->range().lower) };
        auto endDateTime    { QDateTime::fromTime_t(ui->plot1->xAxis->range().upper) };

        ui->dteStart->setDateTimeRange(startDateTime, endDateTime);
        ui->dteEnd->setDateTimeRange(startDateTime, endDateTime);
        ui->dteEnd->setDateTime(endDateTime);

        decGraph();

    }
}

void MainWindow::setDateTimeClicked(bool checked)
{
    ui->plot1->xAxis->setRange(ui->dteStart->dateTime().toTime_t(), ui->dteEnd->dateTime().toTime_t());
    ui->plot1->replot();
    ui->plot2->xAxis->setRange(ui->dteStart->dateTime().toTime_t(), ui->dteEnd->dateTime().toTime_t());
    ui->plot2->replot();
}

void MainWindow::changeAlgorithm(int index)
{
    if(static_cast<size_t>(index) < func.size()) {
        currentAlg = index;
    }
    if(!func.empty()) {
        decGraph();
    }
}
