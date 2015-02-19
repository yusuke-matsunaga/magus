#ifndef IRFUNCBLOCK_H
#define IRFUNCBLOCK_H

/// @file IrFuncBlock.h
/// @brief IrFuncBlock のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrCodeBlock.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrFuncBlock IrFuncBlock.h "IrFuncBlock.h"
/// @brief 関数の中間表現
//////////////////////////////////////////////////////////////////////
class IrFuncBlock :
  public IrCodeBlock
{
public:

  /// @brief コンストラクタ
  /// @param[in] toplevel トップレベルブロック
  /// @param[in] name 関数名
  /// @param[in] type 関数の型
  /// @param[in] arg_list 引数のリスト
  /// @param[in] arg_init_list 引数のデフォルト値のリスト
  IrFuncBlock(IrToplevel& toplevel,
	      ShString name,
	      const Type* type,
	      const vector<const IrVar*>& arg_list,
	      const vector<IrNode*>& arg_init_list);

  /// @brief デストラクタ
  ~IrFuncBlock();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief トップレベルのブロックを返す．
  virtual
  IrToplevel&
  toplevel();

  /// @brief 関数名を返す．
  ShString
  name() const;

  /// @brief 関数の型を返す．
  const Type*
  type() const;

  /// @brief インデックスを返す．
  ymuint
  index() const;

  /// @brief インデックスを設定する．
  void
  set_index(ymuint index);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // トップレベルブロック
  IrToplevel& mToplevel;

  // 関数名
  ShString mName;

  // 関数の型
  const Type* mType;

  // 引数のリスト
  vector<const IrVar*> mArgList;

  // 引数のデフォルト値のリスト
  vector<IrNode*> mArgInitList;

  // インデックス
  ymuint mIndex;

};

END_NAMESPACE_YM_YMSL

#endif // IRFUNCBLOCK_H
