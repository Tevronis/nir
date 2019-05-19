# coding=utf-8
import networkx as nx
import matplotlib.pyplot as plt


def save_graph(nxg, graph):
    nxg.clear()
    n = len(graph)

    for i in range(n):
        for j in range(n):
            if graph[i][j] == 1:
                nxg.add_edge(i, j)

    nx.draw_circular(nxg, node_color='#A0CBE2', node_size=5, width=2, with_labels=True, edge_color='m', font_size=20)


def read_graph(s):

    n = (ord(s[0]) - 63)
    print(s)

    result = [[0 for i in range(n)] for j in range(n)]

    row = 0
    col = 1
    for i in range(1, len(s)):
        a = ord(s[i]) - 63
        for ibit in range(5, -1, -1):
            bit = (a >> ibit) & 1
            result[row][col] = bit
            result[col][row] = bit
            row += 1
            if row == col:
                row = 0
                col += 1
            if col == n:
                return result


def main():
    g = nx.Graph()
    s = input('Enter graph6: ')
    gr = read_graph(s)
    save_graph(g, gr)
    plt.show()
    # plt.savefig("anedge.pdf")


main()
