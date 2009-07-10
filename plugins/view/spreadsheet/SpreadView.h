// +-------------------------------------------------------------------------+
// | Tulip Software                                                          |
// | Author:  Morgan Mathiaut                                                |
// +-------------------------------------------------------------------------+
// | License:                                                                |
// |                                                                         |
// | Tulip is free software; you can redistribute it and/or modify           |
// | it under the terms of the GNU General Public License as published by    |
// | the Free Software Foundation; either version 2 of the License, or       |
// | (at your option) any later version.                                     |
// |                                                                         |
// | Tulip is distributed in the hope that it will be useful,                |
// | but WITHOUT ANY WARRANTY; without even the implied warranty of          |
// | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
// | GNU General Public License for more details.                            |
// |                                                                         |
// | You should have received a copy of the GNU General Public License       |
// | along with QSimpleSheet; if not, write to the Free Software             |
// | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA|
// |                                                                         |
// +-------------------------------------------------------------------------+

#ifndef Tulip_SPREADVIEW_H
#define Tulip_SPREADVIEW_H

#include <QtGui/QWidget>

#include <tulip/AbstractView.h>

#include "SpreadWidget.h"

namespace tlp {

  class Graph;

  class SpreadView : public AbstractView
  {
    Q_OBJECT

  public:

    SpreadView();
    virtual ~SpreadView() {}

    QWidget *construct(QWidget *parent);
    void setData(Graph *graph,DataSet dataSet);
    void getData(Graph **graph,DataSet *dataSet);
    Graph *getGraph();

    void installInteractor(QAction*) {}

    virtual void createPicture(const std::string&, int, int);


  protected :

    SpreadWidget *mainWidget;

  public slots :

    void draw();
    void refresh();
    void init();
    void setGraph(Graph *graph);

  };

}

#endif
