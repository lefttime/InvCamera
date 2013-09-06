#ifndef UTIL_HPP
#define UTIL_HPP

#include <QImage>
#include <QVector3D>

#include <Inventor/SbVec.h>
#include <Inventor/fields/SoSFImage.h>

namespace Util {
  SbVec3f toSbVec3f( const QVector3D& );
  QVector3D toVector3D( const SbVec3f& );

  void toSoSFImage( const QImage& image, SoSFImage& soSFImage );
};

#endif // UTIL_HPP