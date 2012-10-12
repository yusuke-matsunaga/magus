#ifndef YM_UTILS_STREAMBINO_H
#define YM_UTILS_STREAMBINO_H

/// @file ym_utils/StreamBinO.h
/// @brief StreamBinO のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/BinO.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class StreamBinO StreamBinO.h "ym_utils/StreamBinO.h"
/// @ingroup YmUtils
/// @brief ostream を用いた BinO の継承クラス
//////////////////////////////////////////////////////////////////////
class StreamBinO :
  public BinO
{
public:

  /// @brief コンストラクタ
  /// @param[in] s 出力先のストリーム
  StreamBinO(ostream& s);

  /// @brief デストラクタ
  virtual
  ~StreamBinO();


public:
  //////////////////////////////////////////////////////////////////////
  // BinO の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データを書き出す．
  /// @param[in] n データサイズ
  /// @param[in] buff データを収めた領域のアドレス
  /// @return 実際に書き出した量を返す．
  virtual
  ymuint
  write(ymuint64 n,
	const ymuint8* buff);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力先のストリーム
  ostream& mS;

};

END_NAMESPACE_YM

#endif // YM_UTILS_STREAMBINO_H
