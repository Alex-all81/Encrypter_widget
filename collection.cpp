//+------------------------------------------------------------------+
//+------------------------------------------------------------------+
#include <QVariant>
#include "encrypter.h"
#include "collection.h"

namespace Encrypt
{
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
bool Collection::addPreset(Cipher cipher, ParamMap params, QString name)
  {
   EncrypterBase<QString,QChar> *newitem=nullptr;
   if(!name.isEmpty() && m_presets.find(name)!=m_presets.end())
      return false;
   switch(cipher)    //Cesar,Affine,Atbash,CesarAbs
     {
      case Cipher::Cesar:
      case Cipher::CesarAbs:
      case Cipher::Affine:
        {
         if(params.size()<1)
            return false;
         newitem = new CipherCesar();
         static_cast<CipherCesar*>(newitem)->setDiapazones(Cipher::CesarAbs==cipher?&m_diapall:getDiapazones());
         break;
        }
      case Cipher::Atbash:
         if(params.size()<1)
            return false;
         newitem = new CipherAtbash();
         static_cast<CipherCesar*>(newitem)->setDiapazones(getDiapazones());
         //        newitem->setParams(params);
         break;
      case Cipher::KeywordCesar:
         newitem = new CipherKeywordCesar();
         static_cast<CipherKeywordCesar*>(newitem)->setDiapazones(getDiapazones());
         break;
      default:
         return false;
         break;
     }
   if(newitem)
     {
      newitem->setParams(params);
      newitem->actualize();
      m_presets[name]=newitem;
      return true;
     }
   return false;
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+  
void Collection::init()
  {
   ParamMap param;
//   QString ru("àÿ");
//   QString RU("Àß");
   QString en("az");
   QString EN=("AZ");
   QVector<uint> ucs4=en.toUcs4();
   m_diapazones.push_back({ucs4[0],ucs4[1]});
   ucs4=EN.toUcs4();
   m_diapazones.push_back({ucs4[0],ucs4[1]});
   ucs4[0]=1072;
   ucs4[1]=1103;
   m_diapazones.push_back({ucs4[0],ucs4[1]});
   ucs4[0]=1040;
   ucs4[1]=1071;
   m_diapazones.push_back({ucs4[0],ucs4[1]});
   param["shift"]=3;
   addPreset(Cipher::Cesar,param,"Cesar classic");   
   addPreset(Cipher::Atbash,param,"Atbash");
   param["mult"]=3;
   addPreset(Cipher::Affine,param,"Affine");
   param.remove("mult");
   param["keyword"]="gosniias";
   addPreset(Cipher::KeywordCesar,param,"Keyword Cesar");
  }

}// Encrypt
