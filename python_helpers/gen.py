import itertools
 
from itertools import combinations, chain
allsubsets = lambda n: list(chain(*[combinations(range(n), ni) for ni in range(n+1)]))
     
     
def powerset(seq):
    """
    Returns all the subsets of this set. This is a generator.
    """
    if len(seq) <= 1:
        yield seq
        yield []
    else:
        for item in powerset(seq[1:]):
            yield [seq[0]]+item
            yield item
           
def main():
	F = allsubsets(5)
	basic_code =  [list(y) for y in F]
	print(basic_code)
	
if __name__ == "__main__":
    main()
