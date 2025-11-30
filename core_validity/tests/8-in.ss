( (def a  1.0) 
  (def b  2.0) 
  (if0 a 
    (block 
      (def y 1)
      (y = (a + b))
      (if0 y
        (block 
          (def x 1)
          (x = (x + y)))
          (x = y)))
    (y = (a + b)))
  b)