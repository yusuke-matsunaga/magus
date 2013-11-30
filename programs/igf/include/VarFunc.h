#ifndef VARFUNC_H
#define VARFUNC_H

/// @file VarFunc.h
/// @brief VarFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "InputFunc.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
/// @class VarFunc VarFunc.h "VarFunc.h"
/// @brief 変数だけの変換関数
//////////////////////////////////////////////////////////////////////
class VarFunc :
  public InputFunc
{
public:

  /// @brief コンストラクタ
  /// @param[in] var_list 変数のリスト
  VarFunc(const vector<ymuint32>& var_list);

  /// @brief デストラクタ
  virtual
  ~VarFunc();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力数を返す．
  virtual
  ymuint
  output_num() const;

  /// @brief 関数値を求める．
  virtual
  ymuint32
  eval(const RegVect* rv) const;

  /// @brief 内容を表示する．
  virtual
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数のリスト
  vector<ymuint32> mVarList;

};

END_NAMESPACE_YM_IGF

#endif // VARFUNC_H
