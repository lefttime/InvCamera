#include "AbstractTextureImage.hpp"

class AbstractTextureImage::AbstractTextureImagePrivate
{
public:

  AbstractTextureImagePrivate( AbstractTextureImage* me ) : m_self( me ) {
  }

  AbstractTextureImage*         m_self;
};

AbstractTextureImage::AbstractTextureImage( QObject* parent )
  : QObject( parent ), _pd( new AbstractTextureImagePrivate( this ) )
{

}

AbstractTextureImage::~AbstractTextureImage()
{

}
