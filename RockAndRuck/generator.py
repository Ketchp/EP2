import os
import sys
from random import seed, choice, choices, randrange, shuffle
from pathlib import Path
from functools import wraps

TEST_DIR = Path(__file__).parent / 'tests'


def complement(edges: set[tuple[int, int]], n) -> set[tuple[int, int]]:
    res = set()
    for a in range(n):
        for b in range(a+1, n):
            if (a, b) not in edges:
                res.add((a, b))
    return res


def write_graph(n: int, edges: set[tuple[int, int]], f_in):
    assert len(edges) <= 1_000_000
    f_in.write(f'{n} {len(edges)}\n')

    for a, b in edges:
        f_in.write(f'{a} {b}\n')


def write_queries(f_in, f_out, q: int, *edges: set[tuple[int, int]]):
    edges_lists = [list(c) for c in edges if len(c)]

    f_in.write(f'{q}\n')
    while q:
        c1 = choice(edges_lists)
        c2 = choice(edges_lists)

        answer = 'YES\n' if c1 is c2 else 'NO\n'

        s0, s1 = choice(c1)
        e0, e1 = choice(c2)

        f_in.write(f'{s0} {s1} {e0} {e1}\n')
        f_out.write(answer)
        q -= 1


def write_clique_queries(f_in, f_out, q: int, n: int):
    nodes = list(range(n))

    f_in.write(f'{q}\n')
    while q:
        a, b, c, d = choices(nodes, k=4)

        f_in.write(f'{a} {b} {c} {d}\n')
        f_out.write('YES\n')
        q -= 1


def path_graph(n: int, start: int = 0) -> set[tuple[int, int]]:
    return {(start + i, start + i + 1) for i in range(n-1)}


def binary_tree_graph(n: int, start: int = 0) -> set[tuple[int, int]]:
    return {(start + (i-1) // 2, start + i) for i in range(1, n)}


def clique_graph(n: int, start: int = 0) -> set[tuple[int, int]]:
    return {(start + a, start + b) for a in range(n) for b in range(a+1, n)}


def random_graph(n: int, m: int, start: int = 0) -> set[tuple[int, int]]:
    if m < n-1:
        raise ValueError('Can not be component.')

    possible_edges = n * (n-1) // 2
    if m > possible_edges:
        raise ValueError('Too many edges.')

    if n < 2:
        return set()

    edges = {(start, start + 1)}
    for b in range(2, n):
        a = randrange(b)
        edges.add((start + a, start + b))

    cl_edges = list(clique_graph(n, start))
    shuffle(cl_edges)

    for edge in cl_edges:
        if len(edges) == m:
            break
        edges.add(edge)

    return edges


def test_path(n: int, q: int, f_in, f_out):
    path = path_graph(n)

    path_c = complement(path, n)

    write_graph(n, path_c, f_in)
    write_queries(f_in, f_out, q, path)


def test_binary_tree(n: int, q: int, f_in, f_out):
    tree = binary_tree_graph(n)

    tree_c = complement(tree, n)
    write_graph(n, tree_c, f_in)
    write_queries(f_in, f_out, q, tree)


# complement of clique is graph without edges
def test_clique(n: int, q: int, f_in, f_out):
    write_graph(n, set(), f_in)
    write_clique_queries(f_in, f_out, q, n)


def test_clique_and_pairs(n: int, pairs: int, q: int, f_in, f_out):
    if 2 * pairs > n:
        raise ValueError

    graph = {(i, i+1) for i in range(0, 2*pairs, 2)}  # + clique on edges [2*pairs, n)
    graph_c = {(p, c) for p in range(0, 2*pairs) for c in range(p // 2 * 2 + 2, n)}
    write_graph(n, graph_c, f_in)

    clique_size = n - 2 * pairs
    if clique_size < 1000:
        clique_comp = clique_graph(n - 2 * pairs, start=2 * pairs)
    else:
        clique_comp = {(i, i+1) for i in choices(list(range(2 * pairs, n)), k=900_000)}
    write_queries(f_in, f_out, q,
                  *({edge} for edge in graph),
                  clique_comp)


def disconnected_x_graphs(graph_f, nl: list[int], q: int, f_in, f_out):
    c = len(nl)
    n = sum(nl)

    clique_starts = [sum(nl[:i]) for i in range(c + 1)]
    graph = [graph_f(size, start) for size, start in zip(nl, clique_starts[:c])]

    write_graph(n, complement({edge for edges in graph for edge in edges}, n), f_in)
    write_queries(f_in, f_out, q, *graph)


def test_disconnected_cliques(nl: list[int], q: int, f_in, f_out):
    n = sum(nl)

    # number of edges in graph complement
    assert n * (n-1) // 2 - sum(nc * (nc - 1) // 2 for nc in nl) <= 1_000_000
    disconnected_x_graphs(clique_graph, nl, q, f_in, f_out)


def test_disconnected_binary_trees(nl: list[int], q: int, f_in, f_out):
    n = sum(nl)

    # number of edges in graph complement
    assert n * (n-1) // 2 - sum(nc - 1 for nc in nl) <= 1_000_000
    disconnected_x_graphs(binary_tree_graph, nl, q, f_in, f_out)


def test_disconnected_paths(nl: list[int], q: int, f_in, f_out):
    n = sum(nl)

    # number of edges in graph complement
    assert n * (n-1) // 2 - sum(nc - 1 for nc in nl) <= 1_000_000
    disconnected_x_graphs(path_graph, nl, q, f_in, f_out)


def test_random_component(n: int, m: int, q: int, f_in, f_out):
    graph = random_graph(n, m)
    write_graph(n, complement(graph, n), f_in)
    write_queries(f_in, f_out, q, graph)


def test_more_random_components(nl: list[int], ml: list[int], q: int, f_in, f_out):
    n = sum(nl)
    start_acc = [sum(nl[:i]) for i in range(len(nl))]
    graphs = [random_graph(n, m, s) for n, m, s in zip(nl, ml, start_acc)]

    write_graph(n, complement({e for edges in graphs for e in edges}, n), f_in)
    write_queries(f_in, f_out, q, *graphs)


def test_wrapper(test_fn, *args, **kwargs):
    @wraps(test_fn)
    def inner(f_in=None, f_out=None):
        if f_in is None:
            f_in = sys.stdout
        if f_out is None:
            f_out = sys.stderr

        test_fn(*args, **kwargs, f_in=f_in, f_out=f_out)

        if f_in.seekable():
            f_in.seek(0)
        if f_out.seekable():
            f_out.seek(0)
    return inner


# tests are named after what complement of graph looks like:
#   - path
#   - binary tree
#   - clique
#   - clique and disconnected pairs
#   - disconnected cliques
#   - disconnected binary_trees
#   - disconnected paths
#   - one random component n = 1400, m = 800_000
#   - more random components

WRAPPED_TESTS = [
    test_wrapper(test_path, n=5, q=100),
    test_wrapper(test_path, n=50, q=1_000),
    test_wrapper(test_path, n=1414, q=700_000),
    test_wrapper(test_binary_tree, n=15, q=200),
    test_wrapper(test_binary_tree, n=50, q=500_000),
    test_wrapper(test_binary_tree, n=1414, q=100_000),
    test_wrapper(test_clique, n=15, q=200),
    test_wrapper(test_clique, n=50, q=500_000),
    test_wrapper(test_clique, n=1414, q=100_000),
    test_wrapper(test_clique, n=100_000, q=100_000),
    test_wrapper(test_clique, n=1_000_000, q=100_000),
    test_wrapper(test_clique_and_pairs, n=10, pairs=2, q=20),
    test_wrapper(test_clique_and_pairs, n=20, pairs=10, q=100_000),
    test_wrapper(test_clique_and_pairs, n=50, pairs=10, q=100_000),
    test_wrapper(test_clique_and_pairs, n=1414, pairs=1, q=100_000),
    test_wrapper(test_clique_and_pairs, n=1414, pairs=50, q=100_000),
    test_wrapper(test_clique_and_pairs, n=1414, pairs=1414//2, q=100_000),
    test_wrapper(test_clique_and_pairs, n=249_999, pairs=2, q=100_000),
    test_wrapper(test_disconnected_cliques, nl=[5, 4, 3, 2, 1], q=100_000),
    test_wrapper(test_disconnected_cliques, nl=list(range(1, 54)), q=100_000),
    test_wrapper(test_disconnected_cliques, nl=[145 for _ in range(10)], q=100_000),
    test_wrapper(test_disconnected_cliques, nl=[14 for _ in range(100)], q=100_000),
    test_wrapper(test_disconnected_binary_trees, nl=[700 for _ in range(2)], q=100_000),
    test_wrapper(test_disconnected_binary_trees, nl=[101 for _ in range(14)], q=100_000),
    test_wrapper(test_disconnected_binary_trees, nl=[14 for _ in range(101)], q=100_000),
    test_wrapper(test_disconnected_paths, nl=[700 for _ in range(2)], q=100_000),
    test_wrapper(test_disconnected_paths, nl=[101 for _ in range(14)], q=100_000),
    test_wrapper(test_disconnected_paths, nl=[14 for _ in range(101)], q=100_000),
    test_wrapper(test_random_component, n=100, m=150, q=100_000),
    test_wrapper(test_random_component, n=100, m=500, q=100_000),
    test_wrapper(test_random_component, n=100, m=1_200, q=100_000),
    test_wrapper(test_random_component, n=100, m=4_000, q=100_000),
    test_wrapper(test_random_component, n=1400, m=2_000, q=100_000),
    test_wrapper(test_random_component, n=1400, m=10_000, q=100_000),
    test_wrapper(test_random_component, n=1400, m=100_000, q=100_000),
    test_wrapper(test_random_component, n=1400, m=800_000, q=100_000),
    *(test_wrapper(test_more_random_components,
                   nl=[30 + 10 * i for i in range(c)],
                   ml=[70 + 30 * i for i in range(c)],
                   q=100_000) for c in (12, 14)),
    *(test_wrapper(test_more_random_components,
                   nl=[100 for _ in range(c)],
                   ml=[2000 + 100 * i for i in range(c)],
                   q=100_000) for c in (12, 14)),
    *(test_wrapper(test_more_random_components,
                   nl=[5 + i // 20 for i in range(c)],
                   ml=[(5 + i // 20) * (4 + i // 20) // 3 for i in range(c)],
                   q=100_000) for c in (130, 140)),
    *(test_wrapper(test_more_random_components,
                   nl=[1400 // c for _ in range(c)],
                   ml=[9000 for _ in range(c)],
                   q=100_000) for c in (2, 3)),
    test_wrapper(test_more_random_components,
                 nl=[2**i for i in range(2, 10)],
                 ml=[2**(i-1) * (2**i - 1) for i in range(2, 10)],
                 q=100_000),
    test_wrapper(test_more_random_components,
                 nl=[2, 4, 4, 4, 1000],
                 ml=[1, 5, 5, 5, 30000],
                 q=100_000),
]


def generate_in_out_files(generate_out: bool = False):
    for idx, test in enumerate(WRAPPED_TESTS, start=3):
        f_in_name = TEST_DIR / f'{idx:02}.in'
        f_out_name = TEST_DIR / f'{idx:02}.out' if generate_out else os.devnull

        with open(f_in_name, 'w') as f_in:
            with open(f_out_name, 'w') as f_out:
                test(f_in, f_out)
        break


if __name__ == '__main__':
    seed(0)
    generate_in_out_files(generate_out=True)
