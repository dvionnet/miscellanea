# The Y Combinator is defined as
# Y = λf.(λx.f(xx))(λx.f(xx))
#
# it's possible to implement simple recursion with it:
#
# Yg = (λf.(λx.f(xx))(λx.f(xx)))g
#    = (λx.g(xx))(λx.g(xx))
#    = (λy.g(yy))(λx.g(xx))
#    = g(λx.g(xx))(λx.g(xx))
#    = g(Yg)
#    = g(g(Yg))

# the Y combinator
y = fn f -> (&(&1.(&1))).(&(f.(fn a -> (&1.(&1)).(a) end))) end
# factorial and fibonacci functions
fac = &(fn 0 -> 0; 1 -> 1; n -> n*&1.(n-1) end)
fib = &(fn 0 -> 0; 1 -> 1; n -> &1.(n-1) + &1.(n-2) end)
IO.puts "fac(100) = #{(y.(fac)).(100)}"
IO.puts "fib(20) = #{(y.(fib)).(20)}"
