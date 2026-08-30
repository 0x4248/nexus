/* fibonacci */

do i = 1 to 10
    say "fib(" i ") = " fib(i)
end
exit

fib: procedure
  parse arg n
  if n <= 1 then return n
  return fib(n - 1) + fib(n - 2)