#ifndef YM_DOTLIB_DOTLIBMGR_H
#define YM_DOTLIB_DOTLIBMGR_H

/// @file libym_dotlib/DotlibMgr.h
/// @brief DotlibMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_dotlib/dotlib_nsdef.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_DOTLIB

class DotlibMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class DotlibMgr DotlibMgr.h "ym_dotlib/DotlibMgr.h"
/// @brief DotlibNode を管理するためのクラス
/// 内容は DotlibParser によってセットされる．
//////////////////////////////////////////////////////////////////////
class DotlibMgr
{
  friend class DotlibParser;

public:

  /// @brief コンストラクタ
  DotlibMgr();

  /// @brief デストラクタ
  ~DotlibMgr();


public:

  /// @brief 初期化する．
  /// @note 以前，生成されたオブジェクトは破壊される．
  void
  clear();

  /// @brief 根のノードを返す．
  const DotlibNode*
  root_node() const;

  /// @brief 使用メモリ量の一覧を出力する．
  /// @param[in] s 出力先のストリーム
  void
  show_stats(ostream& s) const;


public:
  //////////////////////////////////////////////////////////////////////
  // パース木から情報を取り出す便利関数
  // 実はこのクラスの関数である必要はない．
  //////////////////////////////////////////////////////////////////////

  /// @brief 1つの文字列からなる value_list から文字列を取り出す．
  /// @note 仮定が外れたらアボートする．
  static
  ShString
  get_string(const DotlibNode* value_list);

  /// @brief セルを表すノードから情報を取り出す．
  /// @param[in] cell セルを表すパース木のノード
  /// @param[out] name セル名を格納する変数
  /// @param[out] area 面積を格納する変数
  /// @param[out] pin_list ピングループのリスト
  /// @param[out] bus_list バスグループのリスト
  /// @param[out] bundle_list バンドルグループのリスト
  /// @param[out] その他の属性のリスト
  /// @retval true 正しく読み込めた．
  /// @retval false エラーが起こった．
  /// @note エラーは MsgMgr に出力する．
  static
  bool
  get_cell_info(const DotlibNode* cell,
		ShString& name,
		double& area,
		list<const DotlibNode*>& pin_list,
		list<const DotlibNode*>& bus_list,
		list<const DotlibNode*>& bundle_list,
		list<const DotlibNode*>& other_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の処理を行うクラス
  DotlibMgrImpl* mImpl;

};

END_NAMESPACE_YM_DOTLIB

#endif // YM_DOTLIB_DOTLIBMGR_H
