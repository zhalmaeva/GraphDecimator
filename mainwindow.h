#pragma once

#include <QMainWindow>
#include "qcustomplot.h"
#include "datadecimator.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    using storage_t = DataDecimator::storage_t;
    using decimate_func_type = std::function<storage_t(const storage_t&, double)>;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    const std::vector<decimate_func_type> func { DataDecimator::douglas_peucker , DataDecimator::test };
    size_t currentAlg { 0 };

    Ui::MainWindow *ui;
    storage_t storage { new storage_t::value_type };

    bool replotGraph(QCustomPlot* plot, int id, const storage_t& data);
    void decGraph();

    bool read_from_file(const QString&);

    void setRanges(QCustomPlot* plot, const QCPRange& xRange, const QCPRange& yRange);
    void graphInit(QCustomPlot* plot);

public slots:
    void setPathClicked(bool checked = false);
    void setDateTimeClicked(bool checked = false);
    void changeAlgorithm(int index);
};
