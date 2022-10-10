import randomvar
import numpy as np

# This builds the answer from scratch
def multi_apply_v1(Fs, V) :
    return lambda n : np.array([[f(val) for f in Fs] for val in V(n)])

# This is maybe more elegant, since it uses our random variable tools,
# highlighting mathematical aspects.
def multi_apply_v2(Fs, V) :
    return randomvar.apply(lambda x : [f(x) for f in Fs], V)

U = randomvar.uniform()
V = randomvar.apply_array(lambda x : (10*x).astype(int), U)

K = multi_apply_v1([lambda x : x, lambda x : x**2, lambda x : 2*x+1, lambda x : 7*x], V)
randomvar.display(K, 'K')
print()
K = multi_apply_v2([lambda x : x, lambda x : x**2, lambda x : 2*x+1, lambda x : 7*x], V)
randomvar.display(K, 'K')
