\ Model first
\ LP format - for model browsing. Use MPS format to capture full model detail.
Minimize
  2 x0 + 5 x1 + 7 x2 + 3.5 x3
Subject To
 first_constraint: x0 - 4 x1 + x2 >= 10
 second_constraint: 2 x1 + 7 x2 - 5 x3 = 8
 third_constraint: 2 x0 + 9 x1 - 9 x2 + 5 x3 <= 17
Bounds
 x0 <= 23
 0.4 <= x1 <= 0.9
 x2 <= 10.9
 x3 <= 10
End
