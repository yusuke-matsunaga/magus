#ifndef SORTTEMPLBASE_H
#define SORTTEMPLBASE_H

/// @file SortTemplBase.h
/// @brief SortTemplBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SortTempl.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SortTemplBase SortTemplBase.h "SortTemplBase.h"
/// @brief SortTempl の実装クラスの基底クラス
//////////////////////////////////////////////////////////////////////
class SortTemplBase :
  public SortTempl
{
  friend class SortTemplMgr;

protected:

  /// @brief コンストラクタ
  /// @param[in] body 本体
  /// @param[in] param_num パラメータ数
  SortTemplBase(SortElem* body,
		ymuint param_num);

  /// @brief デストラクタ
  virtual
  ~SortTemplBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief パラメータ型の場合に true を返す．
  virtual
  bool
  is_param() const;

  /// @brief パラメータ番号を返す．
  /// @note is_param() == true の時のみ意味を持つ．
  virtual
  ymuint
  param_id() const;

  /// @brief 名前を返す．
  /// @note is_param() == false の時のみ意味を持つ．
  virtual
  const SmtId*
  name() const;

  /// @brief パラメータ数を返す．
  /// @note is_param() == false の時のみ意味を持つ．
  virtual
  ymuint
  param_num() const;

  /// @brief 要素数を返す．
  /// @note is_param() == false の時のみ意味を持つ．
  virtual
  ymuint
  elem_num() const;

  /// @brief 要素の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < elem_num() )
  /// @note is_param() == false の時のみ意味を持つ．
  virtual
  const SortTempl*
  elem(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 型の本体
  SortElem* mBody;

  // パラメータ数
  ymuint32 mParamNum;

  // ハッシュ表で用いるリンクポインタ
  SortTemplBase* mLink;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SORTTEMPLBASE_H
