( (def x 1.0) 
  (def y 1.0) 
  (if0 
      (x == y)
      
      (block 
       (def z 1)
       (y = (x + z)))
       
       (x = y)) 
    x)