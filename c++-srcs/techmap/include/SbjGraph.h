#ifndef SBJGRAPH_H
#define SBJGRAPH_H

/// @file SbjGraph.h
/// @brief SbjGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016, 2017, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "sbj_nsdef.h"
#include "ym/logic.h"


BEGIN_NAMESPACE_SBJ

class IOInfo;

//////////////////////////////////////////////////////////////////////
/// @class SbjGraph SbjGraph.h "SbjGraph.h"
/// @brief サブジェクトグラフを表すクラス
///
/// 入力ノード，出力ノード，論理ノードを分けてリストで持つ
/// ( @sa input_list(), output_list(), lnode_list() )．
/// また，すべてのノードに唯一な ID 番号を割り振っており，その ID 番号
/// からノードを取り出すこともできる( @sa node(int id) )．
/// 入力ノード，出力ノードは ID 番号とは別に入力 ID 番号，および出力
/// ID 番号を持っており，それらの ID 番号からノードを取り出すこともできる．
/// ( @sa input(int id), output(int id) )
/// 論理ノードを入力からのトポロジカル順に並んでいる．
///
/// ノードの DAG の外側にポート，DFF，ラッチを表す SbjPort, SbjDff, SbjLatch
/// を持つ．
///
/// @sa SbjEdge SbjNode, SbjPort, SbjDff, SbjLatch
//////////////////////////////////////////////////////////////////////
class SbjGraph
{
public:

  /// @brief コンストラクタ
  SbjGraph() = default;

  /// @brief コピーコンストラクタ
  SbjGraph(
    const SbjGraph& src ///< [in] コピー元
  );

  /// @brief 代入演算子
  SbjGraph&
  operator=(
    const SbjGraph& src ///< [in] コピー元
  );

  /// @brief デストラクタ
  ~SbjGraph();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 外部インターフェイス情報(ポート)の取得
  /// @{

  /// @brief モジュール名を得る．
  string
  name() const
  {
    return mName;
  }

  /// @brief ポート数を得る．
  SizeType
  port_num() const
  {
    return mPortArray.size();
  }

  /// @brief ポートを得る．
  const SbjPort*
  port(
    SizeType id ///< [in] ポートID ( 0 <= id < port_num() )
  ) const
  {
    ASSERT_COND( id >= 0 && id < port_num() );

    return mPortArray[id];
  }

  /// @brief ポートのリストを得る．
  const vector<SbjPort*>&
  port_list() const
  {
    return mPortArray;
  }

  /// @brief 入出力ノードに関連づけられたポートを得る．
  ///
  /// node がポートに関連付けられていない場合には nullptr を返す．
  const SbjPort*
  port(
    const SbjNode* node ///< [in] 入出力ノード
  ) const;

  /// @brief 入出力ノードのポートにおけるビット位置を得る．
  ///
  /// node がポートに関連付けられていない場合には -1 を返す．
  SizeType
  port_pos(
    const SbjNode* node ///< [in] 入出力ノード
  ) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 外部インターフェイス情報の設定
  /// @{

  /// @brief モジュール名を設定する．
  void
  set_name(
    const string& name ///< [in] 名前
  )
  {
    mName = name;
  }

  /// @brief ポートを追加する(1ビット版)．
  void
  add_port(
    const string& name, ///< [in] ポート名
    SbjNode* io_node    ///< [in] 対応付ける入出力ノード
  )
  {
    add_port(name, vector<SbjNode*>{io_node});
  }

  /// @brief ポートを追加する(ベクタ版)．
  void
  add_port(
    const string& name,          ///< [in] ポート名
    const vector<SbjNode*>& body ///< [in] 対応付ける入出力ノードのベクタ
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ノード関連の情報の取得
  /// @{

  /// @brief ノード数を返す．
  SizeType
  node_num() const
  {
    return mNodeArray.size();
  }

  /// @brief ID 番号によるノードの取得
  /// @return ID 番号が id のノードを返す．
  const SbjNode*
  node(
    SizeType id ///< [in] ID 番号 ( 0 <= id < node_num() )
  ) const
  {
    ASSERT_COND( id >= 0 && id < node_num() );

    return mNodeArray[id];
  }

  /// @brief 入力ノード数の取得
  /// @return 入力ノード数を返す．
  SizeType
  input_num() const
  {
    return mInputArray.size();
  }

  /// @brief 入力 ID 番号による入力ノードの取得
  /// @return 入力 ID 番号が id のノードを返す．
  const SbjNode*
  input(
    SizeType id ///< [in] 入力 ID 番号 ( 0 <= id < input_num() )
  ) const
  {
    ASSERT_COND( id >= 0 && id < input_num() );

    return mInputArray[id];
  }

  /// @brief 入力ノードのリストを得る．
  const vector<const SbjNode*>&
  input_list() const
  {
    return mInputArray;
  }

  /// @brief 出力のノード数を得る．
  SizeType
  output_num() const
  {
    return mOutputArray.size();
  }

  /// @brief 出力 ID 番号による出力ノードの取得
  /// @return 出力 ID 番号が id のノードを返す．
  const SbjNode*
  output(
    SizeType id ///< [in] 出力 ID 番号 ( 0 <= id < output_num() )
  ) const
  {
    ASSERT_COND( id >= 0 && id < output_num() );

    return mOutputArray[id];
  }

  /// @brief 出力ノードのリストを得る．
  const vector<const SbjNode*>&
  output_list() const
  {
    return mOutputArray;
  }

  /// @brief 論理ノード数を得る．
  SizeType
  logic_num() const
  {
    return mLogicList.size();
  }

  /// @brief 論理ノードを得る．
  const SbjNode*
  logic(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < logic_num() )
  ) const
  {
    ASSERT_COND( pos >= 0 && pos < logic_num() );

    return mLogicList[pos];
  }

  /// @brief 論理ノードのリストを得る．
  const vector<const SbjNode*>&
  logic_list() const
  {
    return mLogicList;
  }

  /// @brief 段数を求める．
  ///
  /// 副作用として各 SbjNode のレベルが設定される．
  SizeType
  level() const
  {
    return mLevel;
  }

  /// @brief 各ノードの minimum depth を求める．
  /// @return 出力の最大深さを返す．
  SizeType
  get_min_depth(
    SizeType k,                   ///< [in] LUT の最大入力数
    vector<SizeType>& depth_array ///< [out] 各ノードの深さを収める配列
  ) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name DFF の情報の取得
  /// @{

  /// @brief DFFノード数を得る．
  SizeType
  dff_num() const
  {
    return mDffList.size();
  }

  /// @brief DFFノードを得る．
  const SbjDff*
  dff(
    SizeType id ///< [in] DFF番号 ( 0 <= id < dff_num() )
  ) const
  {
    ASSERT_COND( id >= 0 && id < dff_num() );

    return mDffList[id];
  }

  /// @brief DFFノードのリストを得る．
  const vector<const SbjDff*>&
  dff_list() const
  {
    return mDffList;
  }

  /// @brief node に関連付けられている DFF を得る．
  ///
  /// node が DFF に関連付けられていない場合には nullptr を返す．
  const SbjDff*
  dff(
    const SbjNode* node ///< [in] 対象のノード
  ) const;

  /// @brief node がDFFの入力だった時に true を返す．
  bool
  is_dff_input(
    const SbjNode* node ///< [in] 対象のノード
  ) const;

  /// @brief node がDFFの出力だった時に true を返す．
  bool
  is_dff_output(
    const SbjNode* node ///< [in] 対象のノード
  ) const;

  /// @brief node がDFFのクロック端子だった時に true を返す．
  bool
  is_dff_clock(
    const SbjNode* node ///< [in] 対象のノード
  ) const;

  /// @brief node がDFFのクリア端子だった時に true を返す．
  bool
  is_dff_clear(
    const SbjNode* node ///< [in] 対象のノード
  ) const;

  /// @brief node がDFFのセット端子だった時に true を返す．
  bool
  is_dff_preset(
    const SbjNode* node ///< [in] 対象のノード
  ) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ラッチの情報の取得
  /// @{

  /// @brief ラッチノード数を得る．
  SizeType
  latch_num() const
  {
    return mLatchList.size();
  }

  /// @brief ラッチノードを得る．
  const SbjLatch*
  latch(
    SizeType id ///< [in] ラッチ番号 ( 0 <= id < latch_num() )
  ) const
  {
    ASSERT_COND( id >= 0 && id < latch_num() );

    return mLatchList[id];
  }

  /// @brief ラッチノードのリストを得る．
  const vector<const SbjLatch*>&
  latch_list() const
  {
    return mLatchList;
  }

  /// @brief node に関連付けられているラッチを返す．
  ///
  /// 関連付けられていない場合には nullptr を返す．
  const SbjLatch*
  latch(
    const SbjNode* node ///< [in] 対象のノード
  ) const;

  /// @brief node がラッチの入力だった場合に true を返す．
  bool
  is_latch_input(
    const SbjNode* node ///< [in] 対象のノード
  ) const;

  /// @brief node がラッチの出力だった場合に true を返す．
  bool
  is_latch_output(
    const SbjNode* node ///< [in] 対象のノード
  ) const;

  /// @brief node がラッチのイネーブル端子だった場合に true を返す．
  bool
  is_latch_enable(
    const SbjNode* node ///< [in] 対象のノード
  ) const;

  /// @brief node がラッチのクリア端子だった場合に true を返す．
  bool
  is_latch_clear(
    const SbjNode* node ///< [in] 対象のノード
  ) const;

  /// @brief node がラッチのセット端子だった場合に true を返す．
  bool
  is_latch_preset(
    const SbjNode* node ///< [in] 対象のノード
  ) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ノードの生成／変更
  /// @{

  /// @brief 入力ノードを作る．
  /// @return 作成したノードを返す．
  SbjNode*
  new_input(
    bool bipol //< [in] 両極性が利用可能な時 true にするフラグ
  );

  /// @brief 出力ノードを作る．
  /// @return 作成したノードを返す．
  SbjNode*
  new_output(
    SbjHandle ihandle ///< [in] 入力ハンドル
  );

  /// @brief 論理式から論理ノード(の木)を作る．
  /// @return 作成したノードのハンドルを返す．
  ///
  /// 入力が定数の時も考慮している．
  SbjHandle
  new_expr(
    const Expr& expr,                     ///< [in] 論理式
    const vector<SbjHandle>& ihandle_list ///< [in] 入力ハンドルのリスト
  );

  /// @brief ANDノードを作る．
  /// @return 作成したノードのハンドルを返す．
  ///
  /// ihandle1/ihandle2 が定数の時も考慮している．
  SbjHandle
  new_and(
    SbjHandle ihandle1, ///< [in] 1番めの入力ハンドル
    SbjHandle ihandle2  ///< [in] 2番めの入力ハンドル
  );

  /// @brief ANDノードを作る．
  /// @return 作成したノードのハンドルを返す．
  ///
  /// 入力が定数の時も考慮している．
  SbjHandle
  new_and(
    const vector<SbjHandle>& ihandle_list ///< [in] 入力ハンドルのリスト
  );

  /// @brief ORノードを作る．
  /// @return 作成したノードのハンドルを返す．
  ///
  /// ihandle1/ihandle2 が定数の時も考慮している．
  SbjHandle
  new_or(
    SbjHandle ihandle1, ///< [in] 1番めの入力ハンドル
    SbjHandle ihandle2  ///< [in] 2番めの入力ハンドル
  );

  /// @brief ORノードを作る．
  /// @return 作成したノードのハンドルを返す．
  ///
  /// 入力が定数の時も考慮している．
  SbjHandle
  new_or(
    const vector<SbjHandle>& ihandle_list ///< [in] 入力ハンドルのリスト
  );

  /// @brief XORノードを作る．
  /// @return 作成したノードのハンドルを返す．
  ///
  /// ihandle1/ihandle2 が定数の時も考慮している．
  SbjHandle
  new_xor(
    SbjHandle ihandle1, ///< [in] 1番めの入力ハンドル
    SbjHandle ihandle2  ///< [in] 2番めの入力ハンドル
  );

  /// @brief XORノードを作る．
  /// @return 作成したノードのハンドルを返す．
  ///
  /// 入力が定数の時も考慮している．
  SbjHandle
  new_xor(
    const vector<SbjHandle>& ihandle_list ///< [in] 入力ハンドルのリスト
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name DFF の生成を行う関数
  /// @{

  /// @brief DFFノードを作る．
  /// @return 作成したノードを返す．
  SbjDff*
  new_dff(
    SbjNode* input,           ///< [in] データ入力
    SbjNode* output,          ///< [in] データ出力
    SbjNode* clock,           ///< [in] クロック端子
    SbjNode* clear = nullptr, ///< [in] リセット端子
    SbjNode* preset = nullptr ///< [in] セット端子
  );

  /// @brief ラッチノードを作る．
  /// @return 作成したノードを返す．
  SbjLatch*
  new_latch(
    SbjNode* input,           ///< [in] データ入力
    SbjNode* output,	      ///< [in] データ出力
    SbjNode* enable,	      ///< [in] イネーブル端子
    SbjNode* clear = nullptr, ///< [in] リセット端子
    SbjNode* preset = nullptr ///< [in] セット端子
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name その他の関数
  /// @{

  /// @brief 空にする．
  void
  clear();

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 複製する．
  ///
  /// nodemap はsrcのノード番号をキーにして新しいノードを保持する．
  void
  _copy(
    const SbjGraph& src,      ///< [in] コピー元のオブジェクト
    vector<SbjNode*>& nodemap ///< [in] ノード間の対応付けを表す配列
  );

  /// @brief new_and の下請け関数
  SbjHandle
  _new_and_tree(
    const vector<SbjHandle>& ihandle_list, ///< [in] 入力ハンドルのリスト
    SizeType start,                        ///< [in] 開始位置
    SizeType end                           ///< [in] 終了位置
  );

  /// @brief new_or の下請け関数
  SbjHandle
  _new_or_tree(
    const vector<SbjHandle>& ihandle_list, ///< [in] 入力ハンドルのリスト
    SizeType start,			   ///< [in] 開始位置
    SizeType end                           ///< [in] 終了位置
  );

  /// @brief new_xor の下請け関数
  SbjHandle
  _new_xor_tree(
    const vector<SbjHandle>& ihandle_list, ///< [in] 入力ハンドルのリスト
    SizeType start,			   ///< [in] 開始位置
    SizeType end                           ///< [in] 終了位置
  );

  /// @brief 新しい論理ノードを作る．
  SbjNode*
  _new_logic_node(
    SbjNodeType type,   ///< [in] ノードのタイプ
    SbjHandle ihandle1, ///< [in] 1番めのファンインのハンドル
    SbjHandle ihandle2  ///< [in] 2番めのファンインのハンドル
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  string mName;

  // ポートの配列
  vector<SbjPort*> mPortArray;

  // ID 番号をキーにしたノードの配列
  // すべてのノードが格納される．
  vector<SbjNode*> mNodeArray;

  // 入力番号をキーにした入力ノードの配列
  // 穴はあいていない．
  vector<const SbjNode*> mInputArray;

  // 入力番号をキーにしたIO情報の配列
  vector<IOInfo*> mInputInfoArray;

  // 出力番号をキーにした出力ノードの配列
  // 穴はあいていない．
  vector<const SbjNode*> mOutputArray;

  // 出力番号をキーにしたIO情報の配列
  vector<IOInfo*> mOutputInfoArray;

  // 論理ノードのリスト
  vector<const SbjNode*> mLogicList;

  // DFFノードのリスト
  vector<const SbjDff*> mDffList;

  // ラッチノードのリスト
  vector<const SbjLatch*> mLatchList;

  // 最大レベル
  SizeType mLevel{0};

};

END_NAMESPACE_SBJ

#endif // SBJGRAPH_H
