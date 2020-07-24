#ifndef QUTRENDWIDGET_H
#define QUTRENDWIDGET_H

#include <QWidget>
#include <cucontrolsfactories_i.h>
#include <cucontrolsreader_abs.h>

class CuContext;
class QuTrendDialogPrivate;

class QuTrendWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QuTrendWidget(QWidget *parent, const CuContext *ctx);
    ~QuTrendWidget();

signals:

public slots:
    void onNewData(const CuData& da);
    void showConfig(bool show = true);

private:
    QuTrendDialogPrivate *d;
};

#endif // QUTRENDDIALOG_H
