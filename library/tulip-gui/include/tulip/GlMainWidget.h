/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

#ifndef Tulip_GLMAINWIDGET_H
#define Tulip_GLMAINWIDGET_H

#include <tulip/GlScene.h>

#include <QtOpenGL/qgl.h>
#include <QtCore/qpoint.h>
#include <QtGui/qaction.h>
#include <QtOpenGL/QGLFramebufferObject>

#include "tulip/View.h"

class QGLFramebufferObject;

namespace tlp {

class GlCompositeHierarchyManager;

/** \addtogroup Tulip_Widgets */
/*@{*/
/** \brief Widget of visualisation
 *
 * This widget provide a simple system to display visualisation
 * If you only want to visualise a graph, you can call the setGraph function
 * But in other cases you can directly use the GlScene of the GlMainWidget
 * \see GlScene
 *
 * After scene construction you can perform some operation on GlMainWidgte :
 * - Selection with doSelect() and selectGlEntities()
 * - Image output with getImage(), createPicture(), outputSVG() and outputEPS()
 * - Texture output with createTexture()
 * - others operation on GlScene and QGlWidget
 *
 */

class TLP_QT_SCOPE GlMainWidget : public QGLWidget {
  Q_OBJECT

public:
  /**
    * \brief Configure the rendering process ( see render function)
    * \see render
    **/
  enum RenderingOption {
    RenderScene=0x1, /** Force to render the graph even if there is a previous buffered render. You need to call this option if the graph is updated to regenerate the buffer. If not set try to use the last buffered graph render, if there is no valid buffer this flag is forced. **/
    SwapBuffers=0x2 /** Call the swapBuffer functions at the end of the rendering process. If the disabled it's up to you to call the swapBuffer function. **/
  };
  Q_DECLARE_FLAGS ( RenderingOptions, RenderingOption )

  /**
   * Construct the GlMainWidget
   */
  GlMainWidget(QWidget *parent,View *view=NULL);
  ~GlMainWidget();

  /**
   * Get the scene of this glMainWidget
   */
  tlp::GlScene* getScene() {
    return &scene;
  }

  /** \brief select nodes and edges in a region of the screen
   *
   *  select all nodes and edges lying in the area of the screen of given width and height,
   *  and with its upper-left corner at (x,y)
   *  \param sNode filled by the method with the nodes found in the region
   *  \param sEdge filled by the method with the edges found in the region
   */
  void pickNodesEdges(const int x, const int y,
                      const int width, const int height,
                      std::vector<SelectedEntity> &selectedNode, std::vector<SelectedEntity> &seletedEdge,
                      tlp::GlLayer* layer=NULL);
  /** \brief select a node or edge at a point
   *  select either a node or edge at point (x,y)
   *  \param type tells what has been found: NODE, EDGE
   *  \return true if something has been found, false otherwise
   */
  bool pickNodesEdges(const int x, const int y,
                      SelectedEntity &selectedEntity,
                      tlp::GlLayer* layer=NULL);

  /**
   * @deprecated this function should not be used anymore, please use pickNodesEdges instead.
   **/
  _DEPRECATED void doSelect(const int x, const int y,
                            const int width, const int height,
                            std::vector<tlp::node> &sNode, std::vector<tlp::edge> &sEdge,
                            tlp::GlLayer* layer=NULL){
    std::vector<SelectedEntity> nodes;
    std::vector<SelectedEntity> edges;
    pickNodesEdges(x,y,width,height,nodes,edges,layer);
    for(std::vector<SelectedEntity>::iterator it=nodes.begin();it!=nodes.end();++it){
      sNode.push_back(node((*it).getComplexEntityId()));
    }
    for(std::vector<SelectedEntity>::iterator it=edges.begin();it!=edges.end();++it){
      sEdge.push_back(edge((*it).getComplexEntityId()));
    }
  }

  /**
   * @deprecated this function should not be used anymore, please use pickNodesEdges instead.
   **/
  _DEPRECATED bool doSelect(const int x, const int y,
                            tlp::ElementType &type,
                            tlp::node &n,tlp::edge &e,
                            tlp::GlLayer* layer=NULL){
    SelectedEntity entity;
    bool foundEntity=pickNodesEdges(x,y,entity,layer);
    if(!foundEntity)
      return false;

    if(entity.getEntityType()==SelectedEntity::NODE_SELECTED){
      n=node(entity.getComplexEntityId());
      type=NODE;
    }else{
      e=edge(entity.getComplexEntityId());
      type=EDGE;
    }

    return true;
  }

  /**
   * \brief EPS output of a GlGraph
   */
  bool outputEPS(int size, int doSort, const char *filename);
  /**
   * \brief SVG output of a GlGraph
   */
  bool outputSVG(int size, const char* filename);

  /**
   * Compute real texture size with given height and width
   */
  static void getTextureRealSize(int width, int height, int &textureRealWidth, int &textureRealHeight);

  /**
   * Take a snapshot of the Widget and put it in an OpenGl texture
   */
  QGLFramebufferObject *createTexture(const std::string &textureName,int width, int height);
  /**
   * Take a snapshot of the Widget and put it in a picture
   */
  void createPicture(const std::string &pictureName,int width, int height,bool center=true);

  /**
   * Take a snapshot of the Widget and return it
   */
  QImage createPicture(int width, int height, bool center=true);

  /**
   * Function to do picking on entities.  It just calls
   * selectEntities on the GlScene instance.
   */
  bool pickGlEntities(const int x, const int y,
                      const int width, const int height,
                      std::vector<SelectedEntity>
                      &pickedEntities,
                      tlp::GlLayer* layer=NULL);
  /**
   * Function to do picking on entities.  It just calls
   * selectEntities on the GlScene instance with a small window of
   * twelve pixels.
   */
  bool pickGlEntities(const int x, const int y,
                      std::vector<SelectedEntity>
                      &pickedEntities,
                      tlp::GlLayer* layer=NULL);

  /**
   * @deprecated this function should not be used anymore, please use pickGlEntities instead.
   */
  _DEPRECATED bool selectGlEntities(const int x, const int y,
                                    const int width, const int height,
                                    std::vector<GlSimpleEntity*> &pickedEntities,
                                    tlp::GlLayer* layer=NULL){
    std::vector<SelectedEntity> entities;
    pickGlEntities(x,y,width,height,entities,layer);
    bool foundEntity=false;
    for(std::vector<SelectedEntity>::iterator it=entities.begin();it!=entities.end();++it){
      if((*it).getEntityType()==SelectedEntity::SIMPLE_ENTITY_SELECTED){
        pickedEntities.push_back((*it).getSimpleEntity());
        foundEntity=true;
      }
    }
    return foundEntity;
  }

  /**
   * @deprecated this function should not be used anymore, please use pickGlEntities instead.
   */
  _DEPRECATED bool selectGlEntities(const int x, const int y,
                                    std::vector<GlSimpleEntity*> &pickedEntities,
                                    tlp::GlLayer* layer=NULL){
    std::vector<SelectedEntity> entities;
    pickGlEntities(x,y,entities,layer);
    bool foundEntity=false;
    for(std::vector<SelectedEntity>::iterator it=entities.begin();it!=entities.end();++it){
      if((*it).getEntityType()==SelectedEntity::SIMPLE_ENTITY_SELECTED){
        pickedEntities.push_back((*it).getSimpleEntity());
        foundEntity=true;
      }
    }
    return foundEntity;
  }

  /**
   * Grab the image of this widget
   */
  virtual QImage grabFrameBuffer(bool withAlpha = false);


  /**
   * Extend makeCurrent function of QGLWidget to inform TextureManager and DisplayListManager of context changement
   */
  virtual void makeCurrent();

  /**
   * Resize openGL view
   */
  void  resizeGL(int w, int h);

  /**
   * Compute interactors before drawing
   */
  void  computeInteractors();

  /**
   * Draw interactors
   */
  void  drawInteractors();

  /**
    * \brief This function performs all the rendering process of the graph.
    *   Use this function only for advanced purpose, if you want to perform simple rendering use the draw or redraw functions instead.
    * \param options Configure the rendering process, see the RenderingOption documentation for more informations on each rendering option effect.
    * \see RenderingOption
    **/
  void render(RenderingOptions options=RenderingOptions(RenderScene|SwapBuffers), bool checkVisibility=true);

private:

  void  setupOpenGlContext();
  void createRenderingStore(int width, int height);

  tlp::GlScene scene;
  QRegion _visibleArea;
  View *view;
  int widthStored;
  int heightStored;
  char *renderingStore;
  bool frameBufferStored;
  bool useFramebufferObject;
  QGLFramebufferObject *glFrameBuf;
  static bool inRendering;
  int renderingNumber;

public slots:
  /**
   * Draw the graph, the augmented dispaly and the interactors
   */
  void draw(bool graphChanged=true);
  /**
   * That function is given for optimisation purpose. If the hardware enable it,
   * it enables to redraw only the Augmented display and the interactors and not the graph
   * it is really usefull for interactors such as zoom box etc..
   * Warning, if you change the graph or the porperties of element (Colors, size, etc...)
   * applying that fonction will not display the change, in that case, use the draw function.
   */
  void redraw();


  void closeEvent(QCloseEvent *e);

  /**
   * @brief Convinience function that call center function on the current scene and draw the view.
   * Same thing than
   * @code
   * getScene()->centerScene();
   * draw();
   * @endcode
  **/
  void centerScene();

  void emitGraphChanged();

protected slots:
  void paintEvent( QPaintEvent* );

signals:
  /**
   * This signal is emit when the GlMainWidget will be deleted
   */
  void closing(GlMainWidget *, QCloseEvent *);

  /**
   * This signal is emit when GlMainWidget::redraw() is call
   */
  void viewRedrawn(GlMainWidget *glWidget);
  /**
   * This signal is emit when GlMainWidget::draw() is call
   */
  void viewDrawn(GlMainWidget *glWidget,bool graphChanged);

  void glResized(int w,int h);

  void graphChanged();

public :

  /**
   * This function return the first QGLWidget created
   * This function is use to share OpenGL context
   */
  static QGLWidget* getFirstQGLWidget();

  static void clearFirstQGLWidget();

private :
  static QGLWidget *firstQGLWidget;

};

}

/*@}*/
#endif
