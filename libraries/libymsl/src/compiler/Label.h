#ifndef LABEL_H
#define LABEL_H

/// @file Label.h
/// @brief Label のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class Label Label.h "Label.h"
/// @brief ラベルを表すクラス
//////////////////////////////////////////////////////////////////////
class Label
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  Label(ShString name = ShString());

  /// @brief デストラクタ
  ~Label();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  ShString
  name() const;

  /// @brief 配置済みか調べる．
  bool
  is_placed() const;

  /// @brief 配置する
  /// @param[in] next_node 直後のノード
  void
  place(IrNode* next_node);

  /// @brief 参照する．
  /// @param[in] rerer_node 参照しているノード
  void
  add_refer(IrNode* refer_node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // 直後のノード
  IrNode* mNextNode;

  // このラベルを参照しているノードのリスト
  vector<IrNode*> mRefList;

};

END_NAMESPACE_YM_YMSL

#endif // LABEL_H
