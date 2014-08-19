#ifndef VARHEAP_H
#define VARHEAP_H

/// @file VarHeap.h
/// @brief VarHeap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"
#include "Variable.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
/// @class VarHeap VarHeap.h "VarHeap.h"
/// @brief 変数のヒープ木
//////////////////////////////////////////////////////////////////////
class VarHeap
{
public:

  /// @brief コンストラクタ
  /// @param[in] num ノード数
  VarHeap(ymuint num);

  /// @brief デストラクタ
  ~VarHeap();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 保持している変数の数を返す．
  ymuint
  size() const;

  /// @brief 変数を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  Variable*
  var(ymuint pos) const;

  /// @brief 価値を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < size() )
  ymuint
  value(ymuint pos) const;

  /// @brief ヒープが空の時 true を返す．
  bool
  empty() const;

  /// @brief 変数を追加する．
  /// @param[in] var 追加する変数
  /// @param[in] value 価値
  void
  put(Variable* var,
      ymuint value);

  /// @brief 値が最小の要素を取り出す．
  /// そのノードはヒープから取り除かれる．
  Variable*
  get_min();

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  void
  print(ostream& s);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ型
  //////////////////////////////////////////////////////////////////////

  struct Node
  {
    Variable* mVar;
    ymuint32 mValue;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数を適当な位置まで沈める．
  /// @param[in] pos 対象の変数の位置
  void
  move_down(ymuint pos);

  /// @brief 変数を適当な位置まで浮かび上がらせる．
  /// @param[in] var 対象の変数の位置
  void
  move_up(ymuint pos);

  /// @brief 変数の比較を行う．
  /// @param[in] node1, node2 対象のノード
  /// @retval 負の値 node1 が node2 より前にある．
  /// @retval 0 node1 と node2 は同じ
  /// @retval 正の値 node1 が node2 より後ろにある．
  static
  int
  compare(const Node& node1,
	  const Node& node2);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // mHeap の実際の大きさ
  ymuint32 mHeapSize;

  // 変数のヒープ木
  Node* mHeap;

  // ヒープ木中にある変数の数
  ymuint32 mVarNum;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 確保されている変数の数を返す．
inline
ymuint
VarHeap::size() const
{
  return mVarNum;
}

// @brief 変数を返す．
// @param[in] pos 位置番号 ( 0 <= pos < size() )
inline
Variable*
VarHeap::var(ymuint pos) const
{
  return mHeap[pos].mVar;
}

// @brief 価値を返す．
// @param[in] pos 位置番号 ( 0 <= pos < size() )
inline
ymuint
VarHeap::value(ymuint pos) const
{
  return mHeap[pos].mValue;
}

// @brief ヒープが空の時 true を返す．
inline
bool
VarHeap::empty() const
{
  return mVarNum == 0;
}

// @brief 変数を追加する．
inline
void
VarHeap::put(Variable* var,
	     ymuint value)
{
  if ( mHeapSize <= mVarNum ) {
    ymuint old_size = mHeapSize;
    Node* old_array = mHeap;
    mHeapSize <<= 1;
    mHeap = new Node[mHeapSize];
    for (ymuint i = 0; i < old_size; ++ i) {
      mHeap[i] = old_array[i];
    }
    delete [] old_array;
  }
  assert_cond( mVarNum < mHeapSize, __FILE__, __LINE__);
  ymuint pos = mVarNum;
  ++ mVarNum;
  mHeap[pos].mVar = var;
  mHeap[pos].mValue = value;
  move_up(pos);
}

// @brief 値が最小の要素を取り出す．
// その変数はヒープから取り除かれる．
inline
Variable*
VarHeap::get_min()
{
  assert_cond( !empty(), __FILE__, __LINE__);
  Variable* var = mHeap[0].mVar;
  -- mVarNum;
  if ( mVarNum > 0 ) {
    mHeap[0] = mHeap[mVarNum];
    move_down(0);
  }
  return var;
}

// @brief 要素の比較を行う．
// @param[in] node1, node2 対象の要素
// @retval 負の値 node1 が node2 より前にある．
// @retval 0 node1 と node2 は同じ
// @retval 正の値 node1 が node2 より後ろにある．
inline
int
VarHeap::compare(const Node& node1,
		 const Node& node2)
{
  return node1.mValue - node2.mValue;
}

END_NAMESPACE_YM_IGF

#endif // VARHEAP_H
