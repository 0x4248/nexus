do i = 1 to 20
  out = ""
  if i // 3 = 0 then out = out || "Fizz"
  if i // 5 = 0 then out = out || "Buzz"
  if out = "" then out = i
  say out
end