#include "cumbia-viewtrend-ctx-menu-action.h"
#include <cucontrolsreader_abs.h>
#include <QApplication>
#include <cucontext.h>
#include "qutrendwidget.h"

class CuViewTrendContextMenuActionPluginPrivate  {
public:
    QList<QAction *>m_actions;
    const CuContext *m_ctx;
    QPoint m_pos;
};

CuViewTrendContextMenuActionPlugin::CuViewTrendContextMenuActionPlugin(QObject *parent) : QObject(parent)
{
    d = new CuViewTrendContextMenuActionPluginPrivate;
    d->m_ctx = nullptr;
}

CuViewTrendContextMenuActionPlugin::~CuViewTrendContextMenuActionPlugin()
{
    delete d;
}

void CuViewTrendContextMenuActionPlugin::setup(QWidget *widget, const CuContext *cuctx)
{
    d->m_pos = widget->rect().topRight();
    d->m_ctx = cuctx;
    if(d->m_actions.isEmpty() && cuctx) {
        QAction *a  = new QAction("View Trend", this);
        connect(a, SIGNAL(triggered()), this, SLOT(onActionTriggered()));
        d->m_actions << a;
    }
}

QList<QAction *> CuViewTrendContextMenuActionPlugin::getActions() const
{
    return d->m_actions;
}

int CuViewTrendContextMenuActionPlugin::order() const
{
    return -10;
}

void CuViewTrendContextMenuActionPlugin::onActionTriggered()
{
    if(d->m_ctx) {
        QuTrendWidget *t = new QuTrendWidget(nullptr, d->m_ctx);
        t->move(d->m_pos);
        t->show();
    }
    else
        perr("CuCopySourceContextMenuActionPlugin.onActionTriggered: neither a reader nor a writer are configured");
}
