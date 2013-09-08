#ifndef YM_UTILS_STREAMIDO_H
#define YM_UTILS_STREAMIDO_H

/// @file ym_utils/StreamIDO.h
/// @brief StreamIDO のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/IDO.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class StreamIDO StreamIDO.h "ym_utils/StreamIDO.h"
/// @ingroup YmUtils
/// @brief istream を用いた IDO の継承クラス
//////////////////////////////////////////////////////////////////////
class StreamIDO :
  public IDO
{
public:

  /// @brief コンストラクタ
  /// @param[in] s 入力ストリーム
  StreamIDO(istream& s);

  /// @brief デストラクタ
  virtual
  ~StreamIDO();


public:
  //////////////////////////////////////////////////////////////////////
  // IDO の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データを読み込む．
  /// @param[in] buff 読み込んだデータを格納する領域の先頭アドレス．
  /// @param[in] n 読み込むデータサイズ
  /// @return 実際に読み込んだ量を返す．
  virtual
  ssize_t
  read(ymuint8* buff,
       ymuint64 n);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ストリーム
  istream& mS;

};

END_NAMESPACE_YM

#endif // YM_UTILS_STREAMIDO_H
