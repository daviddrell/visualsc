#ifndef SCGRAPHICSVIEW_H
#define SCGRAPHICSVIEW_H

#include "scgraphicsview_global.h"
#include "scdatamodel.h"
#include "sceditcontroller.h"
#include <QObject>
#include <QGraphicsView>
#include "stateattributes.h"
#include "stateboxgraphic.h"
#include "selectablelinesegmentgraphic.h"

#include <QMap>


class SCGRAPHICSVIEWSHARED_EXPORT SCGraphicsView : QWidget
{
      Q_OBJECT


public:
    SCGraphicsView(QWidget *parent, SCDataModel * dm);

    QGraphicsView * getQGraphicsView();

signals:

private slots:

    void handleNewState(SCState * newState);

private:

    // private data

    QGraphicsScene  _scene;
    QGraphicsView   _view;
    SCDataModel    *_dm;
    QMap<SCState*,StateBoxGraphic*> _mapStateToGraphic;


    //  private methods

    void increaseSizeOfAllAncestors (SCState * state);


};

#endif // SCGRAPHICSVIEW_H
