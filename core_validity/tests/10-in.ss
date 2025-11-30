( (def a  1.0) 
  (def b  2.0) 
  (def z  0)
  (if0 a 
    (block 
      (def y  (a + b))
      (if0 y
        (block 
          (def x a)
          (x = (x + y)))
          (x = y)))
    (z = 1))
  b)