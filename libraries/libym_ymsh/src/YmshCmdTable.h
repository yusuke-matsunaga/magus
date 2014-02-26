#ifndef YMSHCMDTABLE_H
#define YMSHCMDTABLE_H

/// @file YmshCmdTable.h
/// @brief YmshCmdTable のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsh/ymsh_nsdef.h"
#include "utils/Alloc.h"


BEGIN_NAMESPACE_YM_YMSH

//////////////////////////////////////////////////////////////////////
/// @class YmshCmdTable YmshCmdTable.h "YmshCmdTable.h"
/// @brief YmshCmd 用のハッシュテーブル
//////////////////////////////////////////////////////////////////////
class YmshCmdTable
{
public:

  /// @brief コンストラクタ
  YmshCmdTable();

  /// @brief デストラクタ
  ~YmshCmdTable();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部に公開している関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を空にする．
  void
  clear();

  /// @brief 登録されているコマンド数を返す．
  ymuint
  num() const;

  /// @brief 登録されているコマンド名のリストを返す．
  /// @param[out] name_list 答を格納するリスト
  void
  get_name_list(vector<const char*>& name_list) const;

  /// @brief コマンドの追加を行う．
  /// @param[in] cmd コマンド
  /// @retval true 追加が成功した．
  /// @retval false 追加が失敗した．同名のコマンドが登録されていた．
  bool
  reg_obj(YmshCmd* cmd);

  /// @brief コマンドをテーブルから取り除く
  /// @param[in] name 取り除く名前
  /// @note name というコマンドが登録されていなければなにもしない．
  void
  unreg_obj(const char* name);

  /// @brief name という名のコマンドを探す．
  /// @param[in] name 名前
  /// @return 該当するコマンドを返す．
  /// @note 見つからなかった場合には NULL が返される．
  YmshCmd*
  find_obj(const char* name) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テーブルの領域を確保する．
  /// @param[in] req_size 要求するサイズ
  void
  alloc_table(ymuint req_size);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ表のサイズ
  ymuint32 mTableSize;

  // ハッシュ表
  YmshCmd** mTable;

  // ハッシュ表を拡大するしきい値
  ymuint32 mNextLimit;

  // 要素数
  ymuint32 mNum;

};

END_NAMESPACE_YM_YMSH

#endif // YMSHOBJTABLE_H
