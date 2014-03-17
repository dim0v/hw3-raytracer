#ifndef PROGRESSFORM_H
#define PROGRESSFORM_H

#include <QtWidgets/QWidget>
#include <QtCore>

namespace Ui {
class ProgressForm;
}

class ProgressForm : public QWidget
{
    Q_OBJECT

    QTimer *timer;
    QElapsedTimer time;
public:
    explicit ProgressForm(QWidget *parent = 0);
    ~ProgressForm();

public slots:
    void updateProgress(float progress);
    void updateTimeLabels();
    void finish();

private:
    Ui::ProgressForm *ui;
};

#endif // PROGRESSFORM_H
