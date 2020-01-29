MODULE Test;
VAR a,b,c,d,e,f,g, end: INTEGER; (*ARRAY 5 OF INTEGER;*)
BEGIN
        (* Compile-time evaluation*)
        b := 5;
        c := 10;
        d := 4;
        e := 14;
        f := 13;
        g := -7;
        end := 999999999;
        a := 11;        (* 11 *)
        a := 5 + 5;     (* 10 *)
        a := 10 - 1;    (*  9 *)
        a := 4 * 2;     (*  8 *)
        a := 14 DIV 2;  (*  7 *)
        a := 13 MOD 7;  (*  6 *)
        a := (-7) MOD 6;(*  5 *)
        (* a := 14 DIV 0; throws at compile-time *)
        (* a := 14 MOD 0; throws at compile-time *)
        (* Runtime evaluation *)
        a := b + 5;     (* 10 *)
        a := c - 1;     (*  9 *)
        a := d * 2;     (*  8 *)
        a := e DIV 2;   (*  7 *)
        a := f MOD 7;   (*  6 *)
        a := g MOD 6;    (*  5 *)
        (* a := e DIV 0; * throws at run-time *)
        (* a := e MOD 0; * throws at run-time *)
        end := 999999999
END Test.