#ifndef SMTID_H
#define SMTID_H

/// @file SmtId.h
/// @brief SmtId のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "smtlibv2_nsdef.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtId SmtId.h "SmtId.h"
/// @brief 識別子を表すクラス
///
/// シンタックスは <symbol> <numeral>*
/// <numeral> は正の整数なので ymuint32 で表す．
/// SmtId の中でユニークな ID 番号を持つ．
//////////////////////////////////////////////////////////////////////
class SmtId
{
  friend class SmtIdMgr;

protected:

  /// @brief デストラクタ
  virtual
  ~SmtId() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  virtual
  ymuint
  id() const = 0;

  /// @brief 名前を返す．
  virtual
  ShString
  name() const = 0;

  /// @brief インデックスリストの要素数を返す．
  /// @note インデックスリストを持たない場合は 0 を返す．
  virtual
  ymuint
  index_size() const = 0;

  /// @brief インデックスを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < index_size() )
  /// @note インデックスを持たない場合や pos が index_size() より大きい場合はエラー(アボート)となる．
  virtual
  ymuint32
  index(ymuint pos) const = 0;

};


/// @relates SmtId
/// @brief 内容を表す文字列を返す．
extern
string
id_str(const SmtId* id);

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTID_H
