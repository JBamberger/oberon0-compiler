MODULE Test;
TYPE A3      = ARRAY 3 OF INTEGER;
     Point2D = RECORD x, y: INTEGER END;
VAR a,b,c,d,e,f,g, end: INTEGER; (*ARRAY 5 OF INTEGER;*)
    x: ARRAY 5 OF INTEGER;
    y: ARRAY 5 OF A3;
    z: ARRAY 5 OF ARRAY 2 OF A3;
    q: Point2D;
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
        end := 999999999;
        x[0] := 10;
        x[1] := 9;
        x[2] := 8;
        x[3] := 7;
        x[4] := 6;
        a := x[0];
        a := x[1];
        a := x[2];
        a := x[3];
        a := x[4];
        end := 999999999;
        y[0][0] := 42;
        a := y[0][0];
        end := 999999999;
        z[1][0][0] := 211;
        a := z[1][0][0];
        z[1][0][1] := 212;
        a := z[1][0][1];
        z[1][1][1] := 222;
        a := z[1][1][1];
        z[1][0][2] := 213;
        a := z[1][0][2];
        end := 999999999;
        q.x := 5;
        a   := q.x;
        q.y := 7;
        a   := q.y;
        end := 999999999
END Test.