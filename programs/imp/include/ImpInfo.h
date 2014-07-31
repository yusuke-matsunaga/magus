#ifndef IMPINFO_H
#define IMPINFO_H

/// @file ImpInfo.h
/// @brief ImpInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/bdn.h"
#include "ImpVal.h"


BEGIN_NAMESPACE_YM_NETWORKS

class ImpMgr;

//////////////////////////////////////////////////////////////////////
/// @class ImpInfo ImpInfo.h "ImpInfo.h"
/// @brief 含意情報を表すクラス
//////////////////////////////////////////////////////////////////////
class ImpInfo
{
public:

  /// @brief 空のコンストラクタ
  ImpInfo();

  /// @brief コピーコンストラクタ
  ImpInfo(const ImpInfo& src);

  /// @brief デストラクタ
  ~ImpInfo();


public:
  //////////////////////////////////////////////////////////////////////
  // 値を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief サイズを得る．
  ymuint
  size() const;

  /// @brief 含意情報のリストを取り出す．
  /// @param[in] src_id 含意元のノード番号
  /// @param[in] src_val 含意元の値 ( 0 or 1 )
  const vector<ImpVal>&
  get(ymuint src_id,
      ymuint src_val) const;

  /// @brief 含意の総数を得る．
  ymuint
  imp_num(const ImpMgr& imp_mgr) const;

  /// @brief 実際に保持している含意の総数を返す．
  ymuint
  imp_num() const;

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  void
  print(ostream& s) const;

  /// @brief 統計情報を出力する．
  void
  print_stats(ostream& s) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 値を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief サイズを設定する．
  /// @param[in] max_id ID番号の最大値
  void
  set_size(ymuint max_id);

  /// @brief 内容をセットする．
  void
  set(vector<vector<ImpVal> >& imp_list_array);

  /// @brief 内容をセットする．
  void
  set(vector<vector<ImpVal> >& imp_list_array,
      const ImpInfo& src);

  /// @brief 推移的閉包を求める．
  void
  make_closure();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // mArray のサイズ
  ymuint32 mArraySize;

  // ImpList の配列
  vector<ImpVal>* mArray;

};

/// @brief 定数ノードの検査を行う．
void
check_const(ImpMgr& imp_mgr,
	    const ImpInfo& imp_info);

/// @brief 検証する．
void
verify(const ImpMgr& imp_mgr,
       const ImpInfo& imp_info);

END_NAMESPACE_YM_NETWORKS

#endif // IMPINFO_H
