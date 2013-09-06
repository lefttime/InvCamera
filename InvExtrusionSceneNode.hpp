#ifndef INVEXTRUSIONSCENENODE_HPP
#define INVEXTRUSIONSCENENODE_HPP

#include "InvShapeSceneNode.hpp"

#include <Qt3D/qvector3darray.h>

class AbstractTextureImage;

class InvExtrusionSceneNode : public InvShapeSceneNode
{
public:

  InvExtrusionSceneNode( const QVector3DArray& trajectory,
                         const QVector<qreal>& scales );
  ~InvExtrusionSceneNode();

  void setTextureImage( AbstractTextureImage* textureImage );

protected:

  virtual void processCallback( SoAction* );

private:

  class InvExtrusionSceneNodePrivate;
  QScopedPointer<InvExtrusionSceneNodePrivate>         _pd;

};

#endif // INVEXTRUSIONSCENENODE_HPP
