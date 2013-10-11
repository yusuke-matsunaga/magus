#ifndef SMTFUNTERM_H
#define SMTFUNTERM_H

/// @file SmtFunTerm.h
/// @brief SmtFunTerm のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtTermImpl.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtFunTerm SmtFunTerm.h "SmtFunTerm.h"
/// @brief SmtTerm のリストを表すクラス
//////////////////////////////////////////////////////////////////////
class SmtFunTerm :
  public SmtTermImpl
{
  friend class SmtMgr;
private:

  /// @brief コンストラクタ
  /// @param[in] fun 関数
  /// @param[in] input_list 入力のリスト
  SmtFunTerm(const SmtFun* fun,
	     const vector<const SmtTerm*>& input_list);

  /// @brief デストラクタ
  virtual
  ~SmtFunTerm();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief kFunTerm 型の場合に関数を返す．
  virtual
  const SmtFun*
  function() const;

  /// @brief kFunTerm 型の場合に入力数を返す．
  virtual
  ymuint
  input_num() const;

  /// @brief kFunTerm 型の場合に入力を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  virtual
  const SmtTerm*
  input(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数
  const SmtFun* mFunction;

  // 入力数
  ymuint32 mInputNum;

  // 入力の配列
  // 実際には必要なサイズを確保する．
  const SmtTerm* mInputList[1];

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTFUNTERM_H
