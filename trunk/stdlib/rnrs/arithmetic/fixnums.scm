
(library (rnrs arithmetic fixnums (6))

  (export fixnum?
          fixnum-width
          least-fixnum
          greatest-fixnum
          fx=?
          fx<?
          fx>?
          fx<=?
          fx>=?
          fxzero?
          fxpositive?
          fxnegative?
          fxodd?
          fxeven?
          fxmax
          fxmin
          fx+
          fx*
          fx-
          fxdiv
          fxmod
          fxdiv-and-mod
          fxdiv0
          fxmod0
          fxdiv0-and-mod0
          fx+/carry
          fx-/carry
          fx*/carry
          fxnot
          fxand
          fxior
          fxxor
          fxif
          fxbit-count
          fxlength
          fxfirst-bit-set
          fxbit-set?
          fxcopy-bit
          fxbit-field
          fxcopy-bit-field
          fxarithmetic-shift
          fxarithmetic-shift-left
          fxarithmetic-shift-right
          fxrotate-bit-field
          fxreverse-bit-field)

  (import (core arithmetic)))