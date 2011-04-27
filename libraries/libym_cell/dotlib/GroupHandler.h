#ifndef LIBYM_CELL_DOTLIB_GROUPHANDLER_H
#define LIBYM_CELL_DOTLIB_GROUPHANDLER_H

/// @file libym_cell/dotlib/GroupHandler.h
/// @brief GroupHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class GroupHandler GroupHandler.h "GroupHandler.h"
/// @brief group statement 用のハンドラ
//////////////////////////////////////////////////////////////////////
class GroupHandler :
  public DotlibHandler
{
public:

  /// @brief 親を持たないハンドラ用のコンストラクタ
  /// @param[in] parser パーサー
  /// @param[in] ptmgr パース木を管理するオブジェクト
  GroupHandler(DotlibParser& parser,
	       PtMgr& ptmgr);

  /// @brief 親を持つハンドラ用のコンストラクタ
  /// @param[in] parent 親のハンドラ
  GroupHandler(GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~GroupHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // DotlibHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値を読み込む．
  /// @param[in] attr_token 属性名を表すトークン
  /// @param[in] attr_loc ファイル上の位置
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_attr(const ShString& attr_name,
	    const FileRegion& attr_loc);


public:
  //////////////////////////////////////////////////////////////////////
  // GroupHandler の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハンドラの登録を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] handler 対応付けるハンドラ
  /// @note エラーが起きたら false を返す．
  bool
  reg_handler(const char* attr_name,
	      DotlibHandler* handler);

  /// @brief ハンドラの登録を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] handler 対応付けるハンドラ
  /// @note エラーが起きたら false を返す．
  bool
  reg_handler(const ShString& attr_name,
	      DotlibHandler* handler);

  /// @brief ハンドラを取り出す．
  /// @param[in] attr_name 属性名
  /// @note なければ NULL を返す．
  DotlibHandler*
  find_handler(const ShString& attr_name);


public:
  //////////////////////////////////////////////////////////////////////
  // 外部から用いられる GroupHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief simple attribute を設定する．
  /// @param[in] attr_name 属性名
  /// @param[in] value 値
  /// @return 設定が失敗したら false を返す．
  /// @note デフォルトの実装はエラーとなる．
  virtual
  bool
  add_simple_attr(const ShString& attr_name,
		  const PtValue* value);

  /// @brief complex attribute を設定する．
  /// @param[in] attr_name 属性名
  /// @param[in] value_list 値のリスト
  /// @return 設定が失敗したら false を返す．
  /// @note デフォルトの実装はエラーとなる．
  virtual
  bool
  add_complex_attr(const ShString& attr_name,
		   const vector<const PtValue*>& value_list);

  /// @brief セルを追加する．
  /// @param[in] cell セル
  /// @note library の時のみ有効
  virtual
  bool
  add_cell(PtCell* cell);

  /// @brief leakage_power を追加する．
  virtual
  bool
  add_leakage_power(PtLeakagePower* lp);

  /// @brief ピンを追加する．
  /// @param[in] pin ピン
  /// @note cell/bus/bundle の時のみ有効
  virtual
  bool
  add_pin(PtPin* pin);

  /// @brief バスを追加する．
  /// @param[in] bus バス
  /// @note cell の時のみ有効
  virtual
  bool
  add_bus(PtBus* bus);

  /// @brief バンドルを追加する．
  /// @param[in] bundle バンドル
  /// @note cell の時のみ有効
  virtual
  bool
  add_bundle(PtBundle* bundle);

  /// @brief タイミングを追加する．
  /// @param[in] timing タイミング条件
  /// @note pin の時のみ有効
  virtual
  bool
  add_timing(PtTiming* timing);

  /// @brief テーブルを追加する．
  /// @param[in] attr_name 属性名
  /// @param[in] table テーブル
  /// @note
  virtual
  bool
  add_table(const ShString& attr_name,
	    PtTable* table);

  /// @brief index_x 属性をセットする．
  /// @param[in] attr_name 属性名
  /// @param[in] value_list 値のリスト
  virtual
  bool
  add_index_x(const ShString& attr_name,
	      const vector<const PtValue*>& value_list);

  /// @brief values 属性をセットする．
  /// @param[in] attr_name 属性名
  /// @param[in] value_list 値のリスト
  virtual
  bool
  add_values(const vector<const PtValue*>& value_list);

  /// @brief domain グループをセットする．
  /// @param[in] domain ドメイン
  virtual
  bool
  add_domain(PtDomain* domain);


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる GroupHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief group statement の最初に呼ばれる関数
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value_list 値を表すトークンのリスト
  virtual
  bool
  begin_group(const ShString& attr_name,
	      const FileRegion& attr_loc,
	      const vector<const PtValue*>& value_list) = 0;

  /// @brief group statement の最後に呼ばれる関数
  virtual
  bool
  end_group() = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハンドラの連想配列
  hash_map<ShString, DotlibHandler*> mHandlerMap;

};


//////////////////////////////////////////////////////////////////////
/// @class GenGroupHandler GroupHandler.h "GroupHandler.h"
/// @brief 汎用の GroupHandler
//////////////////////////////////////////////////////////////////////
class GenGroupHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のハンドラ
  GenGroupHandler(GroupHandler* parent);

  /// @brief デストラクタ
  ~GenGroupHandler();


protected:
  //////////////////////////////////////////////////////////////////////
  // GroupHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief group statement の最初に呼ばれる関数
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value_list 値を表すトークンのリスト
  virtual
  bool
  begin_group(const ShString& attr_name,
	      const FileRegion& attr_loc,
	      const vector<const PtValue*>& value_list);

  /// @brief group statement の最後に呼ばれる関数
  virtual
  bool
  end_group();

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_GROUPHANDLER_H
