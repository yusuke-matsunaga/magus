﻿#ifndef MAGMGR_H
#define MAGMGR_H

/// @file MagMgr.h
/// @brief MagMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "magus.h"
#include "ym/clib.h"
#include "ym/ClibCellLibrary.h"
#include "ym/ItvlMgr.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class MagMgr MagMgr.h "MagMgr.h"
/// @ingroup MagusGroup
/// @brief ネットワークを管理するクラス
//////////////////////////////////////////////////////////////////////
class MagMgr
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  MagMgr();

  /// @brief デストラクタ
  ~MagMgr();


public:

  /// @brief カレントセルライブラリの取得
  ClibCellLibrary&
  cur_cell_library();

  /// @brief 新たな BnNetwork を作成して登録する．
  /// @param[in] name 名前
  /// @param[in] err_out エラー出力
  /// @return 作成したネットハンドル
  /// @note 同名のネットワークが既に存在していた場合にはエラーとなる．
  /// @note また，名前が不適切な場合にもエラーとなる．
  /// @note エラーが起きた場合には nullptr を返す．
  NetHandle*
  new_bnethandle(const string& name,
		 ostream* err_out = nullptr);

  /// @brief 新たな MvNetwork を作成して登録する．
  /// @param[in] name 名前
  /// @param[in] err_out エラー出力
  /// @return 作成したネットハンドル
  /// @note 同名のネットワークが既に存在していた場合にはエラーとなる．
  /// @note また，名前が不適切な場合にもエラーとなる．
  /// @note エラーが起きた場合には nullptr を返す．
  NetHandle*
  new_mvnhandle(const string& name,
		ostream* err_out = nullptr);

  /// @brief ネットワークの検索
  /// @param[in] name 名前
  /// @param[in] err_out エラー出力
  /// @return name という名のネットワークを返す．
  /// @note 名前が不適切な場合やその名前のネットワークが存在しない場合
  /// にはエラーとなり，nullptr を返す．
  NetHandle*
  find_nethandle(const string& name,
		 ostream* err_out = nullptr);

  /// @brief ネットワークの削除
  /// @param[in] name 名前
  /// @param[in] err_out エラー出力
  /// @return name という名のネットワークが存在したら削除して true を返す．
  /// @note 名前が不適切な場合や存在しない場合にはエラーとなり，false を返す．
  bool
  delete_nethandle(const string& name,
		   ostream* err_out = nullptr);

  /// @brief カレントネットワークの変更
  /// @param[in] name ネットワーク名
  /// @param[in] err_out エラー出力
  /// @return 正常終了したら true を返す．
  /// @note name という名のネットワークをカレントネットワークに設定する．
  /// @note 名前が不適切な場合や存在しない場合にはエラーとなり，false を返す．
  bool
  change_cur_nethandle(const string& name,
		       ostream* err_out = nullptr);

  /// @brief 操作対象のネットワークを新たに指定する．
  /// @param[in] name ネットワーク名
  /// @param[in] err_out エラー出力
  /// @return 正常終了したら true を返す．
  /// @note 名前が不適切な場合や存在しない場合にはエラーとなり，false を返す．
  /// @note 古いネットワークはスタックにつまれる．
  bool
  push_cur_nethandle(const string& name,
		     ostream* err_out = nullptr);

  /// @brief スタックにつまれたネットワークの復帰
  /// @param[in] err_out エラー出力
  /// @return 正常終了したら true を返す．
  /// @note スタックが空の場合にはエラーとなり，false を返す．
  bool
  pop_cur_nethandle(ostream* err_out = nullptr);

  /// @brief カレントネットワークの取得
  /// @note この関数はエラーとなることはない．
  NetHandle*
  cur_nethandle() const;

  /// @brief ネットワーク名のリストの作成
  /// @return 作成されたネットワーク名を収めるリストを返す．
  vector<string>
  get_network_names() const;

  /// @brief テンポラリ名の生成
  /// @note temporary name は '@' + 数字
  string
  new_tmpname();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワークの登録
  /// @param[in] handle 登録するネットワークハンドル
  /// @param[in] err_out エラー出力
  /// @return 実行結果
  /// @note 同名のネットワークが既に存在していた場合や名前が不適切な
  /// 場合にはエラーとなり，false を返す．
  bool
  reg_nethandle(NetHandle* handle,
		ostream* err_out = nullptr);

  /// @brief ネットワークを検索する．
  /// @param[in] name 名前
  NetHandle*
  _find_nethandle(const string& name);

  /// @brief ネットワークを削除する．
  /// @param[in] net_handle 削除するネットワークハンドル
  void
  _delete_nethandle(NetHandle* net_handle);

  /// @brief 名前が適切かどうか調べる関数
  /// @param[in] name 調べる名前
  /// @param[in] err_out エラー出力
  /// @return name が ^@?[a-zA-Z_]*$ で表される正規表現なら true を返す．
  static
  bool
  check_name(const string& name,
	     ostream* err_out);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  /// @brief NetHandle のハッシュ表のための構造体
  struct Cell
  {
    // NetHandle
    NetHandle* mHandle;

    // 次の要素を指すリンク
    Cell* mLink;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セルライブラリ
  ClibCellLibrary mCellLibrary;

  // 名前をキーにして NetHandle を格納するハッシュ表
  unordered_map<string, NetHandle*> mNetHandleMap;

  // 操作対象のネットワーク構造体(カレントネットワーク)
  NetHandle* mCurNet;

  // 古い操作対象のネットワーク構造体を入れておくスタック
  vector<NetHandle*> mNetStack;

  // 使用中の temporary name を管理するためのネットワーク
  ItvlMgr mTempNameSet;


private:
  //////////////////////////////////////////////////////////////////////
  // 定数メンバ
  //////////////////////////////////////////////////////////////////////

  /// @brief カレントネットワークを常に表す文字列．
  static
  string kCurNetwork;

};

END_NAMESPACE_MAGUS

#endif // MAGMGR_H
