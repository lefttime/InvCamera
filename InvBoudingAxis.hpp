#ifndef INVBOUDINGAXIS_HPP
#define INVBOUDINGAXIS_HPP

#include "InvShapeSceneNode.hpp"

class InvBoudingAxis : public InvShapeSceneNode
{
public:

  InvBoudingAxis();
  virtual ~InvBoudingAxis();

  void setValues( const SbBox3f& values );
  void setExtent( const SbBox3f& extent );

private:

  class InvBoudingAxisPrivate;
  QScopedPointer<InvBoudingAxisPrivate>         _pd;

};

#endif // INVBOUDINGAXIS_HPP
