#ifndef YM_SMTLIBV2_SMTID_H
#define YM_SMTLIBV2_SMTID_H

/// @file ym_smtlibv2/SmtId.h
/// @brief SmtId のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtId SmtId.h "ym_smtlibv2/SmtId.h"
/// @brief 識別子を表すクラス
///
/// シンタックスは <symbol> <numeral>*
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
  ymint32
  index(ymuint pos) const = 0;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // YM_SMTLIBV2_SMTID_H
