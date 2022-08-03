#ifndef FRAIGENC_H
#define FRAIGENC_H

/// @file FraigEnc.h
/// @brief FraigEnc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "FraigMgr.h"
#include "ym/BnNode.h"
#include "ym/Expr.h"
#include "ym/TvFunc.h"
#include "ym/Bdd.h"


BEGIN_NAMESPACE_FRAIG

//////////////////////////////////////////////////////////////////////
/// @class FraigEnc FraigEnc.h "FraigEnc.h"
/// @brief BnNetwork の論理関数を表す AIG を作るクラス
//////////////////////////////////////////////////////////////////////
class FraigEnc
{
public:

  /// @brief コンストラクタ
  FraigEnc(
    FraigMgr& mgr ///< [in] FraigMgr
  ) : mMgr{mgr}
  {
  }

  /// @brief デストラクタ
  ~FraigEnc() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワークの出力の論理関数 を AIG に変換する．
  vector<FraigHandle>
  operator()(
    const BnNetwork& network,         ///< [in] ネットワーク
    const vector<FraigHandle>& inputs ///< [in] 入力のハンドルのリスト
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief BnNode の出力の論理関数を AIG に変換する．
  FraigHandle
  node2aig(
    const BnNode& node ///< [in] ノード
  );

  /// @brief Expr を AIG に変換する．
  FraigHandle
  expr2aig(
    const Expr& expr,                 ///< [in] 論理式
    const vector<FraigHandle>& inputs ///< [in] 入力のハンドル
  );

  /// @brief TvFunc を AIG に変換する．
  FraigHandle
  tv2aig(
    const TvFunc& func,               ///< [in] 関数
    SizeType pos,                     ///< [in] 次の入力位置
    const vector<FraigHandle>& inputs ///< [in] 入力のハンドル
  );

  /// @brief BDDをAIGに変換する．
  FraigHandle
  bdd2aig(
    const Bdd& bdd,                   ///< [in] BDD
    const vector<FraigHandle>& inputs ///< [in] 入力のハンドル
  );

  /// @brief 定数0関数をつくる．
  FraigHandle
  make_zero()
  {
    return mMgr.make_zero();
  }

  /// @brief 定数1関数をつくる．
  FraigHandle
  make_one()
  {
    return mMgr.make_one();
  }

  /// @brief バッファを作る．
  FraigHandle
  make_buff(
    FraigHandle edge1 ///< [in] 入力のハンドル
  )
  {
    return edge1;
  }

  /// @brief NOTを作る．
  FraigHandle
  make_not(
    FraigHandle edge1 ///< [in] 入力のハンドル
  )
  {
    return ~edge1;
  }

  /// @brief 複数のノードの AND を作る．
  FraigHandle
  make_and(
    const vector<FraigHandle>& edge_list ///< [in] 入力のハンドルのリスト
  )
  {
    SizeType n = edge_list.size();
    ASSERT_COND( n > 0 );
    return _make_and(edge_list, 0, n, false);
  }

  /// @brief 複数のノードの NAND を作る．
  FraigHandle
  make_nand(
    const vector<FraigHandle>& edge_list ///< [in] 入力のハンドルのリスト
  )
  {
    SizeType n = edge_list.size();
    ASSERT_COND( n > 0 );
    return ~_make_and(edge_list, 0, n, false);
  }

  /// @brief 複数のノードの OR を作る．
  FraigHandle
  make_or(
    const vector<FraigHandle>& edge_list ///< [in] 入力のハンドルのリスト
  )
  {
    SizeType n = edge_list.size();
    ASSERT_COND( n > 0 );
    return ~_make_and(edge_list, 0, n, true);
  }

  /// @brief 複数のノードの NOR を作る．
  FraigHandle
  make_nor(
    const vector<FraigHandle>& edge_list ///< [in] 入力のハンドルのリスト
  )
  {
    SizeType n = edge_list.size();
    ASSERT_COND( n > 0 );
    return _make_and(edge_list, 0, n, true);
  }

  /// @brief 複数のノードの XOR を作る．
  FraigHandle
  make_xor(
    const vector<FraigHandle>& edge_list ///< [in] 入力のハンドルのリスト
  )
  {
    SizeType n = edge_list.size();
    ASSERT_COND( n > 0 );
    return _make_xor(edge_list, 0, n);
  }

  /// @brief 複数のノードの XNOR を作る．
  FraigHandle
  make_xnor(
    const vector<FraigHandle>& edge_list ///< [in] 入力のハンドルのリスト
  )
  {
    SizeType n = edge_list.size();
    ASSERT_COND( n > 0 );
    return ~_make_xor(edge_list, 0, n);
  }

  /// @brief MUXに対応するノード(木)を作る．
  FraigHandle
  make_mux(
    FraigHandle cedge, ///< [in] 選択信号
    FraigHandle edge0, ///< [in] 0の入力
    FraigHandle edge1  ///< [in] 1の入力
  );

  /// @brief make_and() の下請け関数
  ///
  /// edge_list[start_pos] から edge_list[end_pos - 1] までの
  /// ハンドルの AND を取る．
  /// なので常に end_pos > start_pos が成り立つと仮定する．
  FraigHandle
  _make_and(
    const vector<FraigHandle>& edge_list, ///< [in] 入力のハンドルのリスト
    SizeType start_pos,                   ///< [in] 開始位置
    SizeType end_pos,                     ///< [in] 終了位置
    bool iinv                             ///< [in] 入力の反転フラグ
  );

  /// @brief make_xor() の下請け関数
  ///
  /// edge_list[start_pos] から edge_list[end_pos - 1] までの
  /// ハンドルの XOR を取る．
  /// なので常に end_pos > start_pos が成り立つと仮定する．
  FraigHandle
  _make_xor(
    const vector<FraigHandle>& edge_list, ///< [in] 入力のハンドルのリスト
    SizeType start_pos,			  ///< [in] 開始位置
    SizeType end_pos			  ///< [in] 終了位置
  );

  /// @brief 2つのノードの AND を作る．
  FraigHandle
  make_and(
    FraigHandle edge1, ///< [in] 入力1のハンドル
    FraigHandle edge2  ///< [in] 入力2のハンドル
  );

  /// @brief 2つのノードの OR を作る．
  FraigHandle
  make_or(
    FraigHandle edge1, ///< [in] 入力1のハンドル
    FraigHandle edge2  ///< [in] 入力2のハンドル
  )
  {
    return ~make_and(~edge1, ~edge2);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // FraigMgr
  FraigMgr& mMgr;

  // TvFuncをキーにしてAIGを格納する辞書
  unordered_map<TvFunc, FraigHandle> mTvMap;

};

END_NAMESPACE_FRAIG

#endif // FRAIGENC_H
