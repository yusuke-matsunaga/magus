//////////////////////////////////////////////////////////////////////
// Py%%TYPE_NAME%%: %%CTYPE_NAME%% を表す型
//////////////////////////////////////////////////////////////////////

/// @brief %%CTYPE_NAME%% を表すタイプオブジェクト
extern
PyTypeObject Py%%TYPE_NAME%%_Type;

/// @brief Py%%TYPE_NAME%% の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が Py%%TYPE_NAME%%_Type だった．
/// @retval false obj が他の型だった．
inline
bool
Py%%TYPE_NAME%%_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &Py%%TYPE_NAME%%_Type;
}

/// @brief %%CTYPE_NAME%% から PyObject を作る．
/// @param[in] val %%CTYPE_NAME%% の値
/// @return val を表す PyObject
extern
PyObject*
Py%%TYPE_NAME%%_From%%TYPE_NAME%%(%%CTYPE_NAME%% val);

/// @brief 文字列から PyObject を作る．
/// @param[in] str 文字列
/// @return PyObject
extern
PyObject*
Py%%TYPE_NAME%%_FromString(const char* str);

/// @brief PyObject から %%CTYPE_NAME%% の値を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return %%CTYPE_NAME%% を返す．
/// @note 変換が失敗したら TypeError を送出し，%%DEFAULT_VAL%% を返す．
extern
%%CTYPE_NAME%%
Py%%TYPE_NAME%%_As%%TYPE_NAME%%(PyObject* py_obj);
%%%PART_A%%%
