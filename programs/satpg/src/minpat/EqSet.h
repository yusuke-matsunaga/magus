#ifndef EQSET_H
#define EQSET_H

/// @file EqSet.h
/// @brief EqSet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class EqSet EqSet.h "EqSet.h"
/// @brief 同値類を求めるために細分化を行うクラス
//////////////////////////////////////////////////////////////////////
class EqSet
{
public:

  /// @brief コンストラクタ
  EqSet();

  /// @brief デストラクタ
  ~EqSet();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化を行う．
  /// @param[in] elem_list 要素番号のリスト
  void
  init(const vector<ymuint>& elem_list);

  /// @brief 細分化を行う．
  /// @param[in] elem_list 一方の部分集合の要素のリスト
  void
  refinement(const vector<ymuint>& elem_list);

  /// @brief 同値類候補数を返す．
  ymuint
  class_num() const;

  /// @brief 同値類候補を変える．
  /// @param[in] pos 位置番号 (0 <= pos < class_num())
  /// @param[out] elem_list 同値類の要素番号を収めるリスト
  void
  class_list(ymuint pos,
	     vector<ymuint>& elem_list) const;

  /// @brief 内容を出力する．
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  /// @brief 一つの要素に対応するデータ構造
  struct Elem
  {
    // 番号
    ymuint mId;

    // 次の同値類候補を指すリンク
    Elem* mLink;

    // 次の代表を指すリンク
    Elem* mNextTop;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 最終的な処理を行う．
  void
  finalize() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素番号の最大値
  ymuint mMaxId;

  // 要素の配列
  vector<Elem> mElemArray;

  // 代表候補リストの先頭
  Elem* mTop;

  // 作業用の配列
  vector<bool> mMarkArray;

  // 代表候補の配列
  mutable
  vector<Elem*> mTopArray;

  // finalize が必要なときに true となるフラグ
  mutable
  bool mNeedFinalize;

};

END_NAMESPACE_YM_SATPG

#endif // EQSET_H
