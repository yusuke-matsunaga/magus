#ifndef YMUTILS_STREAMODO_H
#define YMUTILS_STREAMODO_H

/// @file YmUtils/StreamODO.h
/// @brief StreamODO のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "YmUtils/ODO.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class StreamODO StreamODO.h "YmUtils/StreamODO.h"
/// @ingroup YmUtils
/// @brief ostream を用いた ODO の継承クラス
//////////////////////////////////////////////////////////////////////
class StreamODO :
  public ODO
{
public:

  /// @brief コンストラクタ
  /// @param[in] s 出力先のストリーム
  StreamODO(ostream& s);

  /// @brief デストラクタ
  virtual
  ~StreamODO();


public:
  //////////////////////////////////////////////////////////////////////
  // ODO の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データを書き出す．
  /// @param[in] buff データを収めた領域のアドレス
  /// @param[in] n データサイズ
  /// @return 実際に書き出した量を返す．
  virtual
  ssize_t
  write(const ymuint8* buff,
	ymuint64 n);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力先のストリーム
  ostream& mS;

};

END_NAMESPACE_YM

#endif // YMUTILS_STREAMODO_H
