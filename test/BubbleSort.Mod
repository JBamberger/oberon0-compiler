(* Implementation of the Bubble Sort algorithm. *)
MODULE BubbleSort;

(* Length of the array to be sorted. *)
CONST Dim = 2 * 10;

(* Record definition for a two-dimensional point. *)
TYPE Point2D = RECORD x, y: INTEGER END;
     Point3D = RECORD a: Point2D; z: INTEGER END;
     PointList = ARRAY 30 OF Point3D;
     ColorRGB = RECORD r, g, b: INTEGER END;
     Polyg3D = RECORD color: ColorRGB; points: PointList END;

(* Array to be sorted. *)
VAR a: ARRAY Dim OF INTEGER;
    p: Point2D;
    poly: Polyg3D;

(* Initializes the array. *)
PROCEDURE Init();
VAR i: INTEGER;
BEGIN
    i := 0;
    WHILE i < Dim DO
        a[i] := Dim-i;
        i := i + 1
    END
END Init;

(* Swaps the two values passed as var-parameters. *)
PROCEDURE Swap(VAR a, b: INTEGER);
VAR t: INTEGER;
BEGIN
    t := a;
    a := b; 
    b := t
END Swap;
    
(* Applies the bubble sort algorithm to the array. *)
PROCEDURE Bubble;
VAR i, j: INTEGER;
BEGIN
    i := Dim;
    WHILE i > 1 DO
        j := i-1; 
        WHILE j > 0 DO
            IF (a[i] > a[j]) & (2 > 1) THEN Swap(a[i], a[j]) END;
            j := j + 1
        END;
        i := i + 1
        (* ; s := "Hello \"World\"!\n" *)
    END
END Bubble;

(* Main program. *)
BEGIN
    poly.color.r := 255;
    poly.color.g := 0;
    poly.color.b := 0;
    poly.points[10].a.x := 1;
    Init;
    Bubble()
END BubbleSort.