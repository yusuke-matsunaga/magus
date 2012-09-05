#ifndef BLIFHANDLER_H
#define BLIFHANDLER_H

/// @file BlifHandler.h
/// @brief BlifHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/blif_nsdef.h"
#include "ym_cell/cell_nsdef.h"
#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM_NETWORKS_BLIF

class BlifParser;

//////////////////////////////////////////////////////////////////////
/// @class BlifHandler BlifHandler.h "BlifHandler.h"
/// @ingroup BlifGroup
/// @brief blif パーサーのイベントハンドラの基底クラス
/// 純粋仮想関数を持っているので，このクラスを継承し，仮想関数を上書き
/// する必要がある．
/// @sa BlifParser
//////////////////////////////////////////////////////////////////////
class BlifHandler
{
  friend class BlifParser;

protected:

  /// @brief コンストラクタ
  BlifHandler();

  /// @brief デストラクタ
  virtual
  ~BlifHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスは必要に応じて下記の仮想関数を上書きすること
  // デフォルトの実装はなにもしない．
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  init() = 0;

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
	const char* name) = 0;

  /// @brief .inputs 文中の文字列の処理
  /// @param[in] name_id 文字列のID番号
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  inputs_elem(ymuint32 name_id) = 0;

  /// @brief .outputs 文中の文字列の処理
  /// @param[in] name_id 文字列のID番号
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  outputs_elem(ymuint32 name_id) = 0;

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
	char opat) = 0;

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
       const vector<ymuint32>& inode_id_array) = 0;

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
	char rval) = 0;

  /// @brief .end 文の処理
  /// @param[in] loc 位置情報
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  end(const FileRegion& loc) = 0;

  /// @brief 通常終了時の処理
  virtual
  void
  normal_exit() = 0;

  /// @brief エラー終了時の処理
  virtual
  void
  error_exit() = 0;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが利用することのできる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号から文字列を得る．
  const char*
  id2str(ymuint32 id);

  /// @brief ID番号からそれに関連した位置情報を得る．
  const FileRegion&
  id2loc(ymuint32 id);

  /// @brief ID番号からそれに関連した位置情報その2を得る．
  const FileRegion&
  id2def_loc(ymuint32 id);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パーサー本体
  BlifParser* mParser;

};

END_NAMESPACE_YM_NETWORKS_BLIF

#endif // YM_BLIF_BLIFHANDLER_H
