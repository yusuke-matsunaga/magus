#ifndef CUT_H
#define CUT_H

/// @file Cut.h
/// @brief Cut のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2016, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "lutmap.h"
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
  Cut(
    const SbjNode* root,    ///< [in] カットの根のノード
    SizeType ni,            ///< [in] カットの入力数
    const SbjNode* inputs[] ///< [in] カットの入力のノードの配列
  ) : mRoot{root},
      mLink{nullptr},
      mNi{ni}
  {
    for ( SizeType i = 0; i < ni; ++ i ) {
      mInputs[i] = inputs[i];
    }
  }

  /// @brief デストラクタ
  ~Cut() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 根のノードを得る．
  const SbjNode*
  root() const
  {
    return mRoot;
  }

  /// @brief 入力のサイズを得る．
  SizeType
  input_num() const
  {
    return mNi;
  }

  /// @brief pos 番目の入力を得る．
  const SbjNode*
  input(
    SizeType pos
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < input_num() );
    return mInputs[pos];
  }

  /// @brief 論理シミュレーションを行う．
  /// @return 値のノードの値を返す．
  ///
  /// vals[i] が input(i) の葉の値に対応する．
  /// 値は64ビットのビットベクタで表す．
  ymuint64
  eval(
    const vector<ymuint64>& vals ///< [in] 葉のノードの値
  ) const;

  /// @brief 論理関数を表す真理値表を得る．
  TvFunc
  make_tv(
    bool inv ///< [in] 出力を反転する時 true にするフラグ
  ) const;

  /// @brief 論理関数を表す真理値表を得る．
  TvFunc
  make_tv(
    bool oinv,               ///< [in] 出力を反転する時 true にするフラグ
    const vector<bool>& iinv ///< [in] 入力の反転極性の配列
  ) const;

  /// @brief デバッグ用の表示ルーティン
  ///
  /// 根のノード番号と葉のノード番号を1行で表示する．
  void
  print(
    ostream& s ///< [in] 出力先のストリーム
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 根のノード
  const SbjNode* mRoot;

  // 次のカットを指すポインタ
  Cut* mLink;

  // 入力数
  SizeType mNi;

  // 入力のノード配列
  const SbjNode* mInputs[1];

};

END_NAMESPACE_LUTMAP

#endif // CUT_H
