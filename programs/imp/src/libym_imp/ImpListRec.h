#ifndef IMPLISTREC_H
#define IMPLISTREC_H

/// @file ImpListRec.h
/// @brief ImpListRec のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ImpRec.h"
#include "ImpVal.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class ImpListRec ImpListRec.h "ImpListRec.h"
/// @brief 結果を imp_list に格納する ImpRec
//////////////////////////////////////////////////////////////////////
class ImpListRec :
  public ImpRec
{
public:

  /// @brief コンストラクタ
  /// @param[in] imp_list 含意結果を格納するリスト
  ImpListRec(vector<ImpVal>& imp_list);

  /// @brief デストラクタ
  virtual
  ~ImpListRec();


public:

  /// @brief 含意結果を記録する仮想関数
  /// @param[in] src_node 含意元のノード
  /// @param[in] src_val 含意元の値
  /// @param[in] dst_node 含意先のノード
  /// @param[in[ dst_val 含意先の値
  virtual
  void
  record(ImpNode* src_node,
	 ymuint src_val,
	 ImpNode* dst_node,
	 ymuint dst_val);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 含意結果を格納するリスト
  vector<ImpVal>& mImpList;

};

END_NAMESPACE_YM_NETWORKS

#endif // IMPLISTREC_H
