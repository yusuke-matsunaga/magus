#ifndef CUT_H
#define CUT_H

/// @file Cut.h
/// @brief Cut のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2016, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap_nsdef.h"
#include "SbjGraph.h"
#include "ym/TvFunc.h"


BEGIN_NAMESPACE_LUTMAP

//////////////////////////////////////////////////////////////////////
/// @class Cut Cut.h "Cut.h"
/// @brief カットを表すクラス
///
/// カットは1つの根のノードと1つ以上の葉のノードを持つ．
/// 正確にはもっといろいろと条件があるが，それはカットを切り出す
/// 処理で考慮しているものとしてこのクラスでは純粋に根のノードと
/// 葉のノード(の配列)のみを持つ．
///
/// さらに葉のノードのポインタ配列を Cut 自身のメモリ領域として確保
/// するようにしているので Cut オブジェクトの生成は特殊な方法を用いている．
/// その為，普通には Cut オブジェクトを生成できないようにコンストラクタを
/// private にしている．Cut オブジェクトの生成は CutMgr が行う．
///
/// また，Cut のリストを内部のリンクポインタで実装しているので
/// CutList および CutListIterator を friend class にしている．
//////////////////////////////////////////////////////////////////////
class Cut
{
  friend class CutMgr;
  friend class CutList;
  friend class CutListIterator;

private:

  /// @brief コンストラクタ
  /// @param[in] root カットの根のノード
  /// @param[in] ni カットの入力数
  /// @param[in] inputs カットの入力のノードの配列
  Cut(const SbjNode* root,
      int ni,
      const SbjNode* inputs[]);

  /// @brief デストラクタ
  ~Cut();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 根のノードを得る．
  const SbjNode*
  root() const;

  /// @brief 入力のサイズを得る．
  int
  input_num() const;

  /// @brief pos 番目の入力を得る．
  const SbjNode*
  input(int pos) const;

  /// @brief 論理シミュレーションを行う．
  /// @param[in] vals 葉のノードの値
  /// @return 値のノードの値を返す．
  ///
  /// vals[i] が input(i) の葉の値に対応する．
  /// 値は64ビットのビットベクタで表す．
  ymuint64
  eval(const vector<ymuint64>& vals) const;

  /// @brief 論理関数を表す真理値表を得る．
  /// @param[in] inv 出力を反転する時 true にするフラグ
  TvFunc
  make_tv(bool inv) const;

  /// @brief 論理関数を表す真理値表を得る．
  /// @param[in] oinv 出力を反転する時 true にするフラグ
  /// @param[in] iinv 入力を反転極性の配列
  TvFunc
  make_tv(bool oinv,
	  const vector<bool>& iinv) const;

  /// @brief デバッグ用の表示ルーティン
  /// @param[in] s 出力先のストリーム
  ///
  /// 根のノード番号と葉のノード番号を1行で表示する．
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 根のノード
  const SbjNode* mRoot;

  // 次のカットを指すポインタ
  Cut* mLink;

  // 入力数
  int mNi;

  // 入力のノード配列
  const SbjNode* mInputs[1];

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] root カットの根のノード
// @param[in] ni カットの入力数
// @param[in] inputs カットの入力のノードの配列
inline
Cut::Cut(const SbjNode* root,
	 int ni,
	 const SbjNode* inputs[]) :
  mRoot(root),
  mLink(nullptr),
  mNi(ni)
{
  for ( int i = 0; i < ni; ++ i ) {
    mInputs[i] = inputs[i];
  }
}

// @brief デストラクタ
inline
Cut::~Cut()
{
}

// 根のノードを得る．
inline
const SbjNode*
Cut::root() const
{
  return mRoot;
}

// 入力のサイズを得る．
inline
int
Cut::input_num() const
{
  return mNi;
}

// pos 番目の入力を得る．
inline
const SbjNode*
Cut::input(int pos) const
{
  return mInputs[pos];
}

END_NAMESPACE_LUTMAP

#endif // CUT_H
