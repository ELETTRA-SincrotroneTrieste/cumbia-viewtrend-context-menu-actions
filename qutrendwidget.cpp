#include "qutrendwidget.h"
#include <qutrendplot.h>
#include <quwatcher.h>
#include <cucontext.h>
#include <QLabel>
#include <QGridLayout>
#include <cumacros.h>
#include <QDialog>

// spectrum  - 3D?
#include <cupluginloader.h>
#include <qutimearray3dplotplugin_i.h>
#include <QPushButton>

class QuTrendDialogPrivate {
public:
    QWindow *m_surface;
};

QuTrendWidget::QuTrendWidget(QWidget *parent, const CuContext *ctx) : QWidget(parent) {
    d = new QuTrendDialogPrivate;
    d->m_surface = nullptr;
    resize(700, 500);
    setAttribute(Qt::WA_DeleteOnClose, true);
    QGridLayout *lo = new QGridLayout(this);
    QLabel *l = new QLabel("source", this);
    l->setObjectName("lsrc");
    QFont f = l->font();
    f.setBold(true);
    l->setFont(f);
    l->setAlignment(Qt::AlignHCenter);
    QuTrendPlot *plot = nullptr;
    if(ctx && ctx->cumbia())
        plot = new QuTrendPlot(this, ctx->cumbia(), *ctx->getReaderFactoryI());
    else if(ctx && ctx->cumbiaPool())
        plot = new QuTrendPlot(this, ctx->cumbiaPool(), ctx->getControlsFactoryPool());
    lo->addWidget(l, 0, 0, 1, 7);
    QPushButton *b = new QPushButton("Configure", this);
    lo->addWidget(b, 0, 6, 1, 1);
    if(plot) {
        lo->addWidget(plot, lo->rowCount(), 0, 5, lo->columnCount());
        connect(plot, SIGNAL(newData(const CuData&)), this, SLOT(onNewData(const CuData&)));
        connect(findChild<QPushButton *>(), SIGNAL(clicked()), this, SLOT(showConfig()));
        if(ctx->getReader()) plot->setSource(ctx->getReader()->source());
        // label text and window title
        l->setText(plot->source());
        setWindowTitle(l->text() + " - trend");
    }
}

QuTrendWidget::~QuTrendWidget() {
    delete d;
}

void QuTrendWidget::onNewData(const CuData &da) {
    QGridLayout *glo = findChild<QGridLayout *>();
    if(da["err"].toBool()) {

    }
    if(da.has("data_format_str", "vector")) {
        if(!d->m_surface) {
            QHBoxLayout *hlo = new QHBoxLayout(this);
            CuContext *ctx = nullptr;
            QuTrendPlot *p = findChild<QuTrendPlot *>();
            if(p) {
                p->setVisible(false); // hide scalar trend plot
                glo->removeWidget(p); // remove scalar trend plot from layout
                glo->addLayout(hlo, glo->rowCount(), 0, 6, glo->columnCount()); // add hor layout for 3D plot
                ctx = p->getContext();
                CuPluginLoader ploader;
                QObject *pl_obj;
                QuTimeArray3DPlotPlugin_I *pi = ploader.get<QuTimeArray3DPlotPlugin_I>("qutimearray3dplotplugin.so", &pl_obj);
                if(pi) {
                    d->m_surface = pi->create("QuTimeArray3DPlot", 0);
                    //            d->m_surface->setFlags(d->m_surface->flags() ^ Qt::FramelessWindowHint);
                    d->m_surface->setProperty("maxNumRows", 100);
                    d->m_surface->resize(800, 800);
                    QWidget *container = QWidget::createWindowContainer(d->m_surface, this);
                    hlo->addWidget(container);
                    findChild<QPushButton *>()->setCheckable(true);
                    disconnect(findChild<QPushButton *>(), SIGNAL(clicked()), this, SLOT(showConfig()));
                    connect(findChild<QPushButton *>(), SIGNAL(toggled(bool)), this, SLOT(showConfig(bool)));
                    QuWatcher *w = nullptr;
                    if(ctx->cumbia() && ctx->getReaderFactoryI())
                        w = new QuWatcher(this, ctx->cumbia(), *ctx->getReaderFactoryI());
                    else if(ctx->cumbiaPool())
                        w = new QuWatcher(this, ctx->cumbiaPool(), ctx->getControlsFactoryPool());
                    if(w && ctx->getReader()) {
                        connect(w, SIGNAL(newData(CuData)), this, SLOT(onNewData(CuData)));
                        w->setSource(ctx->getReader()->source());
                    }
                }
                delete p;
            }
        }
        double x;
        CuVariant ts = da["timestamp_ms"];
        ts.getType() == CuVariant::LongInt ? x = static_cast<qint64>(ts.toLongInt()) : x = ts.toDouble();
        const std::vector<double> &v = da["value"].toDoubleVector();
        QVector<double> qv(v.begin(), v.end()), qvx;
        for(int i = 0; i < qv.size(); i++) qvx.push_back(i);
        printf("QuTrendWidget::onNewData: invoking addTimeArray wit date time %s siz %d\n",
               QDateTime::fromMSecsSinceEpoch(x).toString().toStdString().c_str(), qv.size());
        QMetaObject::invokeMethod(d->m_surface, "addTimeArray", Q_ARG(QString, da["src"].toString().c_str()),
                Q_ARG(double, x),
                Q_ARG(QVector<double>, qvx), Q_ARG(QVector<double>, qv));
    }
}

void QuTrendWidget::showConfig(bool show) {
    if(d->m_surface && show) {
        CuPluginLoader ploader;
        QObject *pl_obj;
        QuTimeArray3DPlotPlugin_I *pi = ploader.get<QuTimeArray3DPlotPlugin_I>("qutimearray3dplotplugin.so", &pl_obj);
        if(pi) {
            QWidget *m_configW = pi->plotConfigurationWidget(d->m_surface);
            m_configW->setObjectName("3dconfw");
            QHBoxLayout *hlo = findChild<QHBoxLayout *>();
            hlo->addWidget(m_configW);
        }
    }
    else if(!show && d->m_surface && findChild<QWidget *>("3dconfw")) {
        printf("QuTrendWidget::showConfig: deleting 3dconfw\n");
        delete findChild<QWidget *>("3dconfw");
    }
    else if(findChild<QuTrendPlot *>()) {
        QDialog *di = findChild<QuTrendPlot *>()->createConfigureDialog();
        di->exec();
        delete di;
    }
}

