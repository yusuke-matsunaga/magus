#ifndef BNETBLIFHANDLER_H
#define BNETBLIFHANDLER_H

/// @file BNetBlifHandler.h
/// @brief BNetBlifHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bnet.h"
#include "ym_cell/cell_nsdef.h"

#include "BlifHandler.h"


BEGIN_NAMESPACE_YM_BLIF

//////////////////////////////////////////////////////////////////////
/// @class BNetBlifHandler read_blif.cc
/// @brief BNetwork 用の BlifHandler
//////////////////////////////////////////////////////////////////////
class BNetBlifHandler :
  public BlifHandler
{
public:

  /// @brief コンストラクタ
  BNetBlifHandler();

  /// @brief デストラクタ
  virtual
  ~BNetBlifHandler();


public:

  /// @brief 対象のネットワークを設定する．
  /// @param[in] network 読み込む対象のネットワークを設定する．
  void
  set_network(BNetwork* network);


public:
  //////////////////////////////////////////////////////////////////////
  // BlifHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  init();

  /// @brief .model 文の処理
  /// @param[in] loc1 .model の位置情報
  /// @param[in] loc2 文字列の位置情報
  /// @param[in] name 文字列
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  model(const FileRegion& loc1,
	const FileRegion& loc2,
	const char* name);

  /// @brief .inputs 文中の文字列の処理
  /// @param[in] name_id 文字列のID番号
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  inputs_elem(ymuint32 name_id);

  /// @brief .outputs 文中の文字列の処理
  /// @param[in] name_id 文字列のID番号
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  outputs_elem(ymuint32 name_id);

  /// @brief .names 文の処理
  /// @param[in] name_id_array 各識別子のID番号の配列
  /// @param[in] nc キューブ数
  /// @param[in] cover_pat 入力カバーを表す文字列
  /// @param[in] opat 出力の極性
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  /// @note cover_pat は ni 個ごとに1行のパタンを表す．
  /// 各要素のとりうる値は '0', '1', '-' を表す．
  /// @note opat は '0' か '1' のどちらか
  virtual
  bool
  names(const vector<ymuint32>& name_id_array,
	ymuint32 nc,
	const char* cover_pat,
	char opat);

  /// @brief .gate 文の処理
  /// @param[in] cell セル
  /// @param[in] onode_id 出力ノードのID番号
  /// @param[in] inode_id_array 入力ノードのID番号の配列
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  gate(const Cell* cell,
       ymuint32 onode_id,
       const vector<ymuint32>& inode_id_array);

#if 0
  /// @brief .gate 文の開始
  /// @param[in] loc1 .gate の位置情報
  /// @param[in] loc2 セル名の位置情報
  /// @param[in] cell セル
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  gate_begin(const FileRegion& loc1,
	     const FileRegion& loc2,
	     const Cell* cell);

  /// @brief .gate 文中のピン割り当ての処理
  /// @param[in] loc1 ピン名の位置情報
  /// @param[in] pin ピン
  /// @param[in] name_id ノード名の ID番号
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  gate_assign(const FileRegion& loc1,
	      const CellPin* pin,
	      ymuint name_id);

  /// @brief .gate 文の終了
  virtual
  bool
  gate_end();
#endif

  /// @brief .latch 文の処理
  /// @param[in] name1_id 最初の識別子のID番号
  /// @param[in] name2_id 次の識別子のID番号
  /// @param[in] loc4 リセット値の位置情報
  /// @param[in] rval リセット時の値('0'/'1') 未定義なら ' '
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  latch(ymuint32 name1_id,
	ymuint32 name2_id,
	const FileRegion& loc4,
	char rval);

  /// @brief .end 文の処理
  /// @param[in] loc 位置情報
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  end(const FileRegion& loc);

  /// @brief 通常終了時の処理
  virtual
  void
  normal_exit();

  /// @brief エラー終了時の処理
  virtual
  void
  error_exit();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの取得 or 生成
  /// @param[in] name_id 名前ID
  BNode*
  get_node(ymuint32 name_id);

  /// @brief mNodeArray の拡大
  void
  resize(ymuint32 id);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の BNetwork
  BNetwork* mNetwork;

  // ネットワークを操作するクラス
  BNetManip* mManip;

  // ID番号をキーにした BNode を納めた配列
  vector<BNode*> mNodeArray;

};

END_NAMESPACE_YM_BLIF

#endif // BNETBLIFHANDLER_H
