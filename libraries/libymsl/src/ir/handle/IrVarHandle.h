#ifndef IRVARHANDLE_H
#define IRVARHANDLE_H

/// @file IrVarHandle.h
/// @brief IrVarHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrIndexHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrVarHandle IrVarHandle.h "IrVarHandle.h"
/// @brief Var を保持する IrHandle
//////////////////////////////////////////////////////////////////////
class IrVarHandle :
  public IrIndexHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 変数名
  /// @param[in] value_type 型
  /// @param[in] module_index モジュールインデックス
  /// @param[in] local_index ローカルインデックス
  /// @param[in] global グローバル変数の時 true とするフラグ
  IrVarHandle(ShString name,
	      const Type* value_type,
	      ymuint module_index,
	      ymuint local_index,
	      bool global);

  /// @brief デストラクタ
  virtual
  ~IrVarHandle();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  HandleType
  handle_type() const;

  /// @brief 静的評価可能か調べる．
  ///
  /// 要するに定数式かどうかということ
  virtual
  bool
  is_static() const;

  /// @brief グローバル変数の時に true を返す．
  ///
  /// kVar のみ有効
  bool
  is_global() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // グローバルフラグ
  bool mGlobal;

};

END_NAMESPACE_YM_YMSL

#endif // IRVARHANDLE_H
