#include "Util.hpp"

SbVec3f Util::toSbVec3f( const QVector3D& vec )
{
  return SbVec3f(vec.x(), vec.y(), vec.z());
}

QVector3D Util::toVector3D( const SbVec3f& vec )
{
  return QVector3D(vec[0], vec[1], vec[2]);
}

void Util::toSoSFImage( const QImage& image, SoSFImage& soSFImage )
{
  QRgb* iBuffer = (QRgb*)image.bits();
  if( iBuffer ) {
    SbVec2i32 size( image.width(), image.height() );
    unsigned char* tBuffer = new unsigned char[ size[0]*size[1]*sizeof(QRgb)];

    for( int iy = size[1]-1; iy >=0 ; --iy ) {
      unsigned char* line = &tBuffer[sizeof(QRgb) * size[0] * (size[1] - iy - 1)];
      for( int ix = 0; ix < size[0]; ++ ix ) {
        *line++ = (((unsigned int)*iBuffer) >> 16) & 0xff;
        *line++ = (((unsigned int)*iBuffer) >> 8 ) & 0xff;
        *line++ = ((unsigned int)*iBuffer)         & 0xff;
        *line++ = (((unsigned int)*iBuffer) >> 24) & 0xff;
        ++iBuffer;
      }
    }

    soSFImage.setValue( size,
      sizeof(QRgb),
      SoSFImage::UNSIGNED_BYTE,
      tBuffer,
      SoSFImage::NO_COPY_AND_DELETE );
  }

}