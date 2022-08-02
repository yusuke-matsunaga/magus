#ifndef FRAIGMGR_H
#define FRAIGMGR_H

/// @file FraigMgr.h
/// @brief FraigMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "fraig_nsdef.h"
#include "FraigHandle.h"
#include "ym/bnet.h"
#include "ym/Expr.h"
#include "ym/Bdd.h"
#include "ym/SatBool3.h"
#include "ym/SatSolverType.h"


BEGIN_NAMESPACE_FRAIG

class FraigMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class FraigMgr FraigMgr.h "ym/FraigMgr.h"
/// @brief Functionary Reduced AND-INVERTOR Graph を管理するクラス
//////////////////////////////////////////////////////////////////////
class FraigMgr
{
public:

  /// @brief コンストラクタ
  FraigMgr(
    SizeType sig_size,               ///< [in] シグネチャのサイズ
    const SatSolverType& solver_type ///< [in] SAT-solver の種類を表すオブジェクト
    = SatSolverType{}
  );


  /// @brief デストラクタ
  ~FraigMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノード数を得る．
  SizeType
  input_num() const;

  /// @brief 入力ノードを取り出す．
  FraigNode*
  input_node(
    SizeType pos ///< [in] 入力番号 ( 0 <= pos < input_num() )
  ) const;

  /// @brief ノード数を得る．
  SizeType
  node_num() const;

  /// @brief ノードを取り出す．
  ///
  /// ANDノードの他に入力ノードも含まれる．
  FraigNode*
  node(
    SizeType pos ///< [in] ノード番号 ( 0 <= pos < node_num() )
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 構造(FraigNode)を作成するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数0関数をつくる．
  FraigHandle
  make_zero()
  {
    return FraigHandle::zero();
  }

  /// @brief 定数1関数をつくる．
  FraigHandle
  make_one()
  {
    return FraigHandle::one();
  }

  /// @brief 外部入力を作る．
  FraigHandle
  make_input();

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

  /// @brief 2つのノードの AND を作る．
  FraigHandle
  make_and(
    FraigHandle edge1, ///< [in] 入力1のハンドル
    FraigHandle edge2  ///< [in] 入力2のハンドル
  );

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

  /// @brief 2つのノードの NAND を作る．
  FraigHandle
  make_nand(
    FraigHandle edge1, ///< [in] 入力1のハンドル
    FraigHandle edge2  ///< [in] 入力2のハンドル
  )
  {
    return ~make_and(edge1, edge2);
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

  /// @brief 2つのノードの OR を作る．
  FraigHandle
  make_or(
    FraigHandle edge1, ///< [in] 入力1のハンドル
    FraigHandle edge2  ///< [in] 入力2のハンドル
  )
  {
    return ~make_and(~edge1, ~edge2);
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

  /// @brief 2つのノードの NOR を作る．
  FraigHandle
  make_nor(
    FraigHandle edge1, ///< [in] 入力1のハンドル
    FraigHandle edge2  ///< [in] 入力2のハンドル
  )
  {
    return make_and(~edge1, ~edge2);
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

  /// @brief 2つのノードの XOR を作る．
  FraigHandle
  make_xor(
    FraigHandle edge1, ///< [in] 入力1のハンドル
    FraigHandle edge2  ///< [in] 入力2のハンドル
  )
  {
    FraigHandle tmp1 = make_and( edge1, ~edge2);
    FraigHandle tmp2 = make_and(~edge1,  edge2);
    return make_or(tmp1, tmp2);
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

  /// @brief 2つのノードの XNOR を作る．
  FraigHandle
  make_xnor(
    FraigHandle edge1, ///< [in] 入力1のハンドル
    FraigHandle edge2  ///< [in] 入力2のハンドル
  )
  {
    FraigHandle tmp1 = make_and( edge1, ~edge2);
    FraigHandle tmp2 = make_and(~edge1,  edge2);
    return make_nor(tmp1, tmp2);
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

  /// @brief 論理式に対応するノード(木)を作る．
  FraigHandle
  make_expr(
    const Expr& expr,                 ///< [in] 対象の論理式
    const vector<FraigHandle>& inputs ///< [in] 入力のハンドルのリスト
  );

  /// @brief 真理値表に対応するノード(木)を作る．
  FraigHandle
  make_tv(
    const TvFunc& func,               ///< [in] 対象の真理値表
    const vector<FraigHandle>& inputs ///< [in] 入力のハンドルのリスト
  );

  /// @brief BDDに対応するノード(木)を作る．
  FraigHandle
  make_bdd(
    const Bdd& func,                  ///< [in] 対象のBDD
    const vector<FraigHandle>& inputs ///< [in] 入力のハンドルのリスト
  );

  /// @brief MUXに対応するノード(木)を作る．
  FraigHandle
  make_mux(
    FraigHandle cedge, ///< [in] 選択信号
    FraigHandle edge0, ///< [in] 0の入力
    FraigHandle edge1  ///< [in] 1の入力
  );

  /// @brief コファクターを計算する．
  FraigHandle
  make_cofactor(
    FraigHandle edge, ///< [in] 対象のハンドル
    int input_id,     ///< [in] 入力番号
    bool inv          ///< [in] 反転フラグ
  );

  /// @brief BnNetwork をインポートする．
  /// @return 出力のハンドルのリストを返す．
  vector<FraigHandle>
  import_subnetwork(
    const BnNetwork& network,                ///< [in] インポートするネットワーク
    const vector<FraigHandle>& input_handles ///< [in] 入力に接続するハンドルのリスト
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 検証用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 2つのハンドルが等価かどうか調べる．
  SatBool3
  check_equiv(
    FraigHandle aig1, ///< [in] 入力1のハンドル
    FraigHandle aig2  ///< [in] 入力2のハンドル
  );

  /// @brief ログレベルを設定する．
  void
  set_loglevel(
    SizeType level ///< [in] ログレベル
  );

  /// @brief ログ出力用ストリームを設定する．
  void
  set_logstream(
    ostream* out ///< [in] ストリーム
  );

  /// @brief 内部の統計情報を出力する．
  void
  dump_stats(
    ostream& s ///< [in] 出力ストリーム
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

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

  /// @brief make_tv() の下請け関数
  FraigHandle
  _make_tv(
    const TvFunc& func,                ///< [in] 対象の真理値表
    const vector<FraigHandle>& inputs, ///< [in] 入力のハンドルのリスト
    SizeType pos                       ///< [in] 次に処理する変数
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実装クラス
  unique_ptr<FraigMgrImpl> mRep;

};

END_NAMESPACE_FRAIG

#endif // FRAIGMGR_H
