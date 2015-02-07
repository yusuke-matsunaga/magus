#ifndef IRSTRINGCONST_H
#define IRSTRINGCONST_H

/// @file IrStringConst.h
/// @brief IrStringConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrStringConst IrStringConst.h "IrStringConst.h"
/// @brief 文字列定数を表すノード
//////////////////////////////////////////////////////////////////////
class IrStringConst :
  public IrNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] type 型
  /// @param[in] val 値
  IrStringConst(const Type* type,
		const char* val);

  /// @brief デストラクタ
  virtual
  ~IrStringConst();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 静的評価可能か調べる．
  ///
  /// 要するに定数式かどうかということ
  virtual
  bool
  is_static() const;

  /// @brief 文字列を返す．
  virtual
  const char*
  string_val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  const char* mVal;

};

END_NAMESPACE_YM_YMSL

#endif // IRSTRINGCONST_H
