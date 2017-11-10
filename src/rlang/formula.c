#include "rlang.h"


SEXP r_f_rhs(SEXP f) {
  if (r_kind(f) != LANGSXP)
    r_abort("`x` must be a formula");

  switch (r_length(f)) {
  case 2: return r_node_cadr(f);
  case 3: return CADDR(f);
  default: r_abort("Invalid formula");
  }
}
SEXP r_f_lhs(SEXP f) {
  if (r_kind(f) != LANGSXP)
    r_abort("`x` must be a formula");

  switch (r_length(f)) {
  case 2: return r_null;
  case 3: return r_node_cadr(f);
  default: r_abort("Invalid formula");
  }
}
SEXP r_f_env(SEXP f) {
  return r_get_attr(f, r_sym(".Environment"));
}

bool r_f_has_env(SEXP f) {
  return r_is_environment(r_f_env(f));
}

bool is_formulaish(SEXP x, int scoped, int lhs) {
  if (r_kind(x) != LANGSXP)
    return false;

  SEXP head = r_node_car(x);
  if (head != r_sym("~") && head != r_sym(":="))
    return false;

  if (scoped >= 0) {
    int has_env = r_kind(r_f_env(x)) == ENVSXP;
    if (scoped != has_env)
      return false;
  }

  if (lhs >= 0) {
    int has_lhs = r_length(x) > 2;
    if (lhs != has_lhs)
      return false;
  }

  return true;
}

bool is_formula(SEXP x) {
  if (!is_formulaish(x, -1, -1))
    return false;

  return r_node_car(x) == r_sym("~");
}


// Export

int lgl_optional(SEXP lgl) {
  if (lgl == r_null)
    return -1;
  else
    return r_as_bool(lgl);
}

SEXP rlang_is_formulaish(SEXP x, SEXP scoped, SEXP lhs) {
  int scoped_int = lgl_optional(scoped);
  int lhs_int = lgl_optional(lhs);

  bool out = is_formulaish(x, scoped_int, lhs_int);
  return Rf_ScalarLogical(out);
}