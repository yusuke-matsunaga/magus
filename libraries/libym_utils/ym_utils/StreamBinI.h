#ifndef YM_UTILS_STREAMBINI_H
#define YM_UTILS_STREAMBINI_H

/// @file ym_utils/StreamBinI.h
/// @brief StreamBinI のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/BinI.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class StreamBinI StreamBinI.h "ym_utils/StreamBinI.h"
/// @ingroup YmUtils
/// @brief istream を用いた BinI の継承クラス
//////////////////////////////////////////////////////////////////////
class StreamBinI :
  public BinI
{
public:

  /// @brief コンストラクタ
  /// @param[in] s 入力ストリーム
  StreamBinI(istream& s);

  /// @brief デストラクタ
  virtual
  ~StreamBinI();


public:
  //////////////////////////////////////////////////////////////////////
  // BinI の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データを読み込む．
  /// @param[in] n 読み込むデータサイズ
  /// @param[in] buff 読み込んだデータを格納する領域の先頭アドレス．
  /// @return 実際に読み込んだ量を返す．
  virtual
  ssize_t
  read(ymuint64 n,
       ymuint8* buff);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ストリーム
  istream& mS;

};

END_NAMESPACE_YM

#endif // YM_UTILS_STREAMBINI_H
