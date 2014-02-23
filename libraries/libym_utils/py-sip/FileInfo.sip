
/// @file FileInfo.sip
/// @brief FinleInfo の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

class FileInfo {

%TypeHeaderCode
#include "utils/FileInfo.h"
using nsYm::FileInfo;
%End

public:

  /// @brief 空のコンストラクタ
  /// @note 無効な ID で初期化される．
  FileInfo();

  /// @brief ファイル名を指定したコンストラクタ
  /// @param[in] filename ファイル名
  FileInfo(const char* filename);

  /// @brief ファイル名とインクルード元の親ファイルの情報
  /// @param[in] filename ファイル名
  /// @param[in] parent_loc インクルード元の親ファイルの情報
  FileInfo(const char* filename,
	   const FileLoc& parent_loc);

  /// @brief 有効なデータを持っているときに true を返す．
  bool
  is_valid() const;

  /// @brief ID番号を得る．
  /// @note ほとんど等価比較演算子のための関数
  unsigned int
  id() const;

  /// @brief ファイル名を返す．
  const char*
  filename() const;
  %MethodCode
  sipRes = sipCpp->filename().c_str();
  %End

  /// @brief インクルード元のファイル位置を返す．
  /// @note インクルードされていないファイルの場合には無効なデータが返される．
  FileLoc
  parent_loc() const;

  /// @brief インクルード元のファイル位置の情報をすべてリストに積む．
  /// @param[out] loc_list ファイルの位置情報のリスト
  /// @note トップレベルのファイルが先頭になる．
  SIP_PYLIST
  parent_loc_list() const;
  %MethodCode
  using std::list;
  list<FileLoc> loc_list;
  sipCpp->parent_loc_list(loc_list);
  ymuint n = loc_list.size();
  sipRes = PyList_New(n);
  ymuint i = 0;
  for (list<FileLoc>::iterator p = loc_list.begin();
       p != loc_list.end(); ++ p, ++ i) {
    FileLoc& fl = *p;
    PyObject* obj = sipConvertFromNewType(&fl, sipType_FileLoc, NULL);
    PyList_SetItem(sipRes, i, obj);
  }
  %End

};

/// @relates FileInfo
/// @brief 等価比較演算子
/// @param[in] left, right オペランド
bool
operator==(FileInfo left,
	   FileInfo right);
