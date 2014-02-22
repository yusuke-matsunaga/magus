#ifndef ISCAS89NETWORKHANDLER_H
#define ISCAS89NETWORKHANDLER_H

/// @file Iscas89NetworkHandler.h
/// @brief Iscas89NetworkHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Iscas89Handler.h"


BEGIN_NAMESPACE_YM_ISCAS89

class Iscas89Network;

//////////////////////////////////////////////////////////////////////
/// @class Iscas89NetworkHandler Iscas89NetworkHandler.h
/// @brief Iscas89Network 用の Iscas89Handler
//////////////////////////////////////////////////////////////////////
class Iscas89NetworkHandler :
  public Iscas89Handler
{
public:

  /// @brief コンストラクタ
  Iscas89NetworkHandler();

  /// @brief デストラクタ
  virtual
  ~Iscas89NetworkHandler();


public:

  /// @brief 読み込む対象のネットワークを設定する．
  void
  set_network(Iscas89Network* network);

  /// @brief 初期化
  virtual
  bool
  init();

  /// @brief INPUT 文を読み込む．
  /// @param[in] loc ファイル位置
  /// @param[in] name_id 入力ピン名の ID 番号
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  read_input(const FileRegion& loc,
	     ymuint32 name_id);

  /// @brief OUTPUT 文を読み込む．
  /// @param[in] loc ファイル位置
  /// @param[in] name_id 出力ピン名の ID 番号
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  read_output(const FileRegion& loc,
	      ymuint32 name_id);

  /// @brief ゲート文を読み込む．
  /// @param[in] loc ファイル位置
  /// @param[in] type ゲートの型
  /// @param[in] oname_id 出力名の ID 番号
  /// @param[in] iname_list 入力名のリスト
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  read_gate(const FileRegion& loc,
	    tIscas89GateType type,
	    ymuint32 oname_id,
	    const vector<ymuint32>& iname_list);

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
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のネットワーク
  Iscas89Network* mNetwork;

};

END_NAMESPACE_YM_ISCAS89

#endif // ISCAS89NETWORKHANDLER_H
