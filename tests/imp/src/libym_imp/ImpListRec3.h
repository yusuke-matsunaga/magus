#ifndef IMPLISTREC2_H
#define IMPLISTREC2_H

/// @file ImpListRec3.h
/// @brief ImpListRec3 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ImpRec.h"
#include "ImpVal.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class ImpListRec ImpListRec.h "ImpListRec.h"
/// @brief 結果を imp_list の配列に格納する ImpRec
//////////////////////////////////////////////////////////////////////
class ImpListRec3 :
  public ImpRec
{
public:

  /// @brief コンストラクタ
  /// @param[in] imp_list_array 含意結果を格納するリストの配列
  ImpListRec3(vector<vector<ImpVal> >& imp_list_array);

  /// @brief デストラクタ
  virtual
  ~ImpListRec3();


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

  // 含意結果を格納するリストの配列
  vector<vector<ImpVal> >& mImpListArray;

};

END_NAMESPACE_YM_NETWORKS

#endif // IMPLISTREC2_H
