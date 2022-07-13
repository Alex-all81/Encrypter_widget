//+------------------------------------------------------------------+
//+------------------------------------------------------------------+
#ifndef COLLECTION_H
#define COLLECTION_H
#include <map>
#include <limits>
#include <QByteArray>
#include <QVariant>
#include "encrypter.h"

namespace Encrypt
{

using CesarCipher = CipherCesar;
using AffineCipher = CipherCesar;
using CesarCipherAbsolute = CipherCesar;
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
class Collection
  {
public:
   using Presets =   std::map<QString,EncrypterBase<QString,QChar>*>;
protected:
   Presets           m_presets;
   Diapazones        m_diapazones;
   Diapazones m_diapall= {{1,std::numeric_limits<unicode_t>::max()-1}};
                     Collection()=default;
                    ~Collection()=default;
public:
   static Collection* getCollection()
     {
      static Collection *instance=nullptr;
      if(!instance)
         instance = new Collection;
      return instance;
     }
   bool              addPreset(Cipher cipher, ParamMap params, QString name=nullptr);
   void              init();
   Diapazones *      getDiapazones() {return &m_diapazones;}
   std::map<QString,EncrypterBase<QString,QChar>*>* getPresets() {return &m_presets;}

  };
}// Encrypt
#endif // COLLECTION_H


//+------------------------------------------------------------------+
